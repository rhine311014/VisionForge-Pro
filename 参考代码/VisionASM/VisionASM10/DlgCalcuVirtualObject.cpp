// DlgCalcuVirtualObject.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalcuVirtualObject.h"
#include "afxdialogex.h"


// CDlgCalcuVirtualObject 对话框

IMPLEMENT_DYNAMIC(CDlgCalcuVirtualObject, CDialog)

CDlgCalcuVirtualObject::CDlgCalcuVirtualObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalcuVirtualObject::IDD, pParent)
	, m_nRadioExProductIndex(0)
{
	m_nType = 0;
	m_pVAPrevMainWnd = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_vpGuiDisplay.clear();
	m_vpGuiVirtualObjectDisplay.clear();
	m_vpPrevGuiDisplays.clear();
	m_pVisionASM = NULL;

	m_psaSysInfoStrings = NULL;

	m_vpGuiCross.clear();

	m_bDisplayCross = FALSE;

	// 状态栏相关
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	m_vpGuiInteractiveContainer.clear();
	m_vpGuiVirtualObjectInteractiveContainer.clear();

	m_vMarkCorrdPos.clear();
	m_vdPreShutter.clear();
	m_vPrevGrabStatus.clear();
	m_vPrevTriggerMode.clear();

	m_bCalcuVirtualObjectSuccess = FALSE;
	m_bIsGrabbing = FALSE;

	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;

	m_nEvent = 0;
	m_nPreExProductIndex = 0;
}

CDlgCalcuVirtualObject::~CDlgCalcuVirtualObject()
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

	for (i=0; i<m_vpGuiVirtualObjectDisplay.size(); i++)
	{
		if (m_vpGuiVirtualObjectDisplay.at(i))
		{
			delete m_vpGuiVirtualObjectDisplay.at(i);
			m_vpGuiVirtualObjectDisplay.at(i) = NULL;
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

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos.at(i);
			m_vpGuiAlnObjectImageMarkPos.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnVirtualObjectImageMarkPos.size();i++)
	{
		if (m_vpGuiAlnVirtualObjectImageMarkPos.at(i) != NULL)
		{
			delete m_vpGuiAlnVirtualObjectImageMarkPos.at(i);
			m_vpGuiAlnVirtualObjectImageMarkPos.at(i) = NULL;
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


	for(i=0; i<m_vpGuiVirtualObjectText.size();i++)
	{
		if (m_vpGuiVirtualObjectText.at(i) != NULL)
		{
			delete m_vpGuiVirtualObjectText.at(i);
			m_vpGuiVirtualObjectText.at(i) = NULL;
		}
	}

	m_pVisionASM = NULL;
}

void CDlgCalcuVirtualObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SERIAL_GRAB, m_btnGrabImage);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS0, m_btnSearchObjectPos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS1, m_btnSearchObjectPos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0, m_btnSearchVirtualObjectPos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1, m_btnSearchVirtualObjectPos1);
	DDX_Control(pDX, IDC_BTN_CALCU_VIRTUAL_OBJECT, m_btnCalcuVirtualObject);
	DDX_Control(pDX, IDC_STATIC_NOTE, m_labelNote);
	DDX_Radio(pDX, IDC_RADIO_EX_PRODUCT_INDEX_0, m_nRadioExProductIndex);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalcuVirtualObject, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERIAL_GRAB, &CDlgCalcuVirtualObject::OnBnClickedBtnSerialGrab)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalcuVirtualObject::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgCalcuVirtualObject::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS0, &CDlgCalcuVirtualObject::OnBnClickedBtnSearchObjectPos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0, &CDlgCalcuVirtualObject::OnBnClickedBtnSearchVirtualObjectPos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS1, &CDlgCalcuVirtualObject::OnBnClickedBtnSearchObjectPos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1, &CDlgCalcuVirtualObject::OnBnClickedBtnSearchVirtualObjectPos1)
	ON_BN_CLICKED(IDC_BTN_CALCU_VIRTUAL_OBJECT, &CDlgCalcuVirtualObject::OnBnClickedBtnCalcuVirtualObject)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_EX_PRODUCT_INDEX_0, &CDlgCalcuVirtualObject::OnBnClickedRadioExProductIndex0)
	ON_BN_CLICKED(IDC_RADIO_EX_PRODUCT_INDEX_1, &CDlgCalcuVirtualObject::OnBnClickedRadioExProductIndex0)
	ON_BN_CLICKED(IDC_RADIO_EX_PRODUCT_INDEX_2, &CDlgCalcuVirtualObject::OnBnClickedRadioExProductIndex0)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_INDEX, &CDlgCalcuVirtualObject::OnCbnSelchangeComboExproductIndex)
END_MESSAGE_MAP()


// CDlgCalcuVirtualObject 消息处理程序


void CDlgCalcuVirtualObject::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
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

BOOL CDlgCalcuVirtualObject::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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

	UpdateData(FALSE);

	OnBnClickedBtnSerialGrab();

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	UpdateDialogLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCalcuVirtualObject::InitDlgItem()
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

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_LABEL_NOTE)/*_T("注:请确保对象、虚拟对象各拍照位置的平台θ轴位置一致！！！")*/);

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

	m_btnSearchObjectPos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchObjectPos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos1.SetThemeHelper(&m_themeHelper);
	m_btnCalcuVirtualObject.SetThemeHelper(&m_themeHelper);

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

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	 
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetPosNum() && i < 2; i++)
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

	for (i=0; i<m_pVisionASM->GetPosNum() && i < 2; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i+2, this,WS_VISIBLE);
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
		m_vpGuiVirtualObjectDisplay.push_back(pGUIDisplay);
		m_vpGuiVirtualObjectDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}

	scGuiStaticContainer *pGuiSC = NULL;
	scGuiCoordCross* pGuiCoordCross = NULL;
	scGuiText *pGuiText = NULL;
	// 对象
	CString strKey;
	CString strInfo;
	for(i = 0;  i<m_pVisionASM->GetPosNum() && i < 2; i++)
	{
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));
		m_vpGuiAlnObjectImageMarkPos.push_back(pGuiCoordCross);

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_OBJECT_POS)/*_T("对象位置%d")*/,i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiText.push_back(pGuiText);

		pGuiSC = m_vpGuiDisplay.at(i)->GetStaticContainer();

		strKey.Format(_T("ObjectPos%d"),i);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos.at(i), strKey);

		strKey.Format(_T("GuiText%d"),i);
		pGuiSC->AddItem(m_vpGuiText.at(i), strKey);

		m_vpGuiDisplay.at(i)->SetStaticContainer(pGuiSC);
	}


	// 虚拟对象
	for(i = 0;  i<m_pVisionASM->GetPosNum() && i < 2; i++)
	{
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));
		m_vpGuiAlnVirtualObjectImageMarkPos.push_back(pGuiCoordCross);

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_VIRTUAL_OBJECT_POS)/*_T("虚拟对象位置%d")*/,i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiVirtualObjectText.push_back(pGuiText);

		pGuiSC = m_vpGuiVirtualObjectDisplay.at(i)->GetStaticContainer();

		strKey.Format(_T("VirtualObjectPos%d"),i);
		pGuiSC->AddItem(m_vpGuiAlnVirtualObjectImageMarkPos.at(i), strKey);

		strKey.Format(_T("GuiVirtualObjectText%d"),i);
		pGuiSC->AddItem(m_vpGuiVirtualObjectText.at(i), strKey);

		m_vpGuiVirtualObjectDisplay.at(i)->SetStaticContainer(pGuiSC);
	}
	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
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

void CDlgCalcuVirtualObject::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}


void CDlgCalcuVirtualObject::OnDestroy()
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


LRESULT CDlgCalcuVirtualObject::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);

			int nPosIndex = (int)wParam;
			if (nPosIndex<m_vpGuiDisplay.size() && nPosIndex<m_vpGuiVirtualObjectDisplay.size())
			{
				if (m_vpGuiDisplay[nPosIndex]->GetImageCP() != NULL)
				{
					m_vpGuiVirtualObjectDisplay[nPosIndex]->SetImage(*(cpImage*)m_vpGuiDisplay[nPosIndex]->GetImageCP(),TRUE);
				}
				else
				{
					m_vpGuiVirtualObjectDisplay[nPosIndex]->RemoveImage(FALSE);
				}
			}

			
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



void CDlgCalcuVirtualObject::UpdateBtnLiveGrab()
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

void CDlgCalcuVirtualObject::UpdateShutterDisplay()
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




BOOL CDlgCalcuVirtualObject::SetGUI()
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
bool CDlgCalcuVirtualObject::UpdateGuiDisplay()
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

void CDlgCalcuVirtualObject::OnBnClickedBtnSerialGrab()
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
		g_opLogFile.SaveLogText(strLog,2);
		for (i=0; i<nCamNum && i<2; i++)
		{
			vbSucceed.push_back(m_pVisionASM->FreezePosCamera(i));
		}
		for (i=0; i<nCamNum && i<2; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			for (i=0; i<nCamNum && i<2; i++)	
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
		CString strLog;
		strLog.Format(_T("点击[停止采集]\n"));
		g_opLogFile.SaveLogText(strLog,2);

		for (i=0; i<nCamNum && i<2; i++)
		{
			vbSucceed.push_back(m_pVisionASM->GrabPosCamera(i));
		}
		for (i=0; i<nCamNum && i<2; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机采集失败，则恢复所有成功采集的相机为停止状态）
			for (i=0; i<nCamNum && i<2; i++)	
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


void CDlgCalcuVirtualObject::OnBnClickedBtnSearchObjectPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 1;
	CString strLog;
	strLog.Format(_T("点击[搜索对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject::OnBnClickedBtnSearchVirtualObjectPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 2;
	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject::OnBnClickedBtnSearchObjectPos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 3;
	CString strLog;
	strLog.Format(_T("点击[搜索对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject::OnBnClickedBtnSearchVirtualObjectPos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 4;
	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject::OnBnClickedBtnCalcuVirtualObject()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[计算虚拟对象位置]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	if (m_pVisionASM == NULL)
	{
		return;
	}
	UpdateData(TRUE);
	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	int nExProductIndex = 0;
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}

	if (TRUE == m_pVisionASM->CalculateObjectToVirtualMarkOffset(m_vObjectToVirtualOffsets, nExProductIndex))
	{
		m_bCalcuVirtualObjectSuccess = TRUE;
		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALCU_SUCCESS);//.Format(_T("计算成功"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();
	}
	else
	{
		m_bCalcuVirtualObjectSuccess = FALSE;
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALCU_FAILE);//.Format(_T("计算失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();
	};
}


void CDlgCalcuVirtualObject::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存退出]计算虚拟对象位置\n"));
	g_opLogFile.SaveLogText(strLog,1);

	if (m_bCalcuVirtualObjectSuccess)
	{
		if (m_pVisionASM != NULL 
			//&& TRUE == m_pVisionASM->SetAndSaveVirtualMarkOffset(m_vObjectToVirtualOffsets, m_nRadioExProductIndex))
			&& TRUE == m_pVisionASM->SetAndSaveVirtualMarkOffset(m_vObjectToVirtualOffsets, m_nPreExProductIndex))
		{
			m_bStatusBarFlag = TRUE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_SUCCESS);//.Format(_T("保存成功"));
			m_strStatusBarInfo2.Format(_T(""));
			UpdateStatusBar();
		}
		else
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_FAILE);//.Format(_T("保存失败"));
			m_strStatusBarInfo2.Format(_T(""));
			UpdateStatusBar();

			return;
		}
	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_FAILE);//.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();

		return;
	}


	CDialog::OnOK();
}


void CDlgCalcuVirtualObject::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[不保存退出]计算虚拟对象位置\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnCancel();
}



void CDlgCalcuVirtualObject::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnTimer(nIDEvent);
}



UINT CDlgCalcuVirtualObject::ThreadReceiveData( LPVOID pParam )
{
	CDlgCalcuVirtualObject *pOwner = (CDlgCalcuVirtualObject*)pParam;
	CString strReceived;
	while (!pOwner->m_bExitThread)
	{
		if (pOwner->m_nEvent == 1)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			pOwner->SearchObjectPos(nPosIndex);

		}		
		else if (pOwner->m_nEvent == 2)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			pOwner->SearchVirtualObjectPos(nPosIndex);
		}
		else if (pOwner->m_nEvent == 3)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 1;
			pOwner->SearchObjectPos(nPosIndex);
		}
		else if (pOwner->m_nEvent == 4)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 1;
			pOwner->SearchVirtualObjectPos(nPosIndex);
		}
		Sleep(10);
	}

	SetEvent(pOwner->m_hEventReceiveThreadExit);
	return 0;
}

void CDlgCalcuVirtualObject::SearchObjectPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	m_vpGuiAlnObjectImageMarkPos.at(nPosIndex)->SetVisible(FALSE);

	m_pVisionASM->SetDefaultObjectSearchMarkPos(TRUE);
	if (TRUE == m_pVisionASM->SingleCameraAlignerObjectSearchExFOBJ(nPosIndex))
	{
		m_mpObjectMarkImagePos = m_pVisionASM->GetObjectMarkImagePos();

		if (TRUE == m_mpObjectMarkImagePos.GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos = m_mpObjectMarkImagePos.GetMarkImagePos(nPosIndex);

			m_vpGuiAlnObjectImageMarkPos.at(nPosIndex)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
			m_vpGuiAlnObjectImageMarkPos.at(nPosIndex)->SetVisible(TRUE);
		}
	}
}

void CDlgCalcuVirtualObject::SearchVirtualObjectPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	m_vpGuiAlnVirtualObjectImageMarkPos.at(nPosIndex)->SetVisible(FALSE);
	m_pVisionASM->SetDefaultVirtualObjectSearchMarkPos(TRUE);
	if (TRUE == m_pVisionASM->SingleCameraAlignerVirtualObjectSearchExFVOJ(nPosIndex))
	{
		m_mpVirtualMarkImagePos = m_pVisionASM->GetVirtualObjectMarkImagePos();

		if (TRUE == m_mpVirtualMarkImagePos.GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos = m_mpVirtualMarkImagePos.GetMarkImagePos(nPosIndex);

			m_vpGuiAlnVirtualObjectImageMarkPos.at(nPosIndex)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
			m_vpGuiAlnVirtualObjectImageMarkPos.at(nPosIndex)->SetVisible(TRUE);
		}
	};
}

void CDlgCalcuVirtualObject::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_CALCU_VIRTUAL_OBJECT") == strDlgID)
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

void CDlgCalcuVirtualObject::OnBnClickedRadioExProductIndex0()
{
	// TODO: 在此添加控件通知处理程序代码m_nRadioExProductIndex
	int nRadioExProductIndexOld = m_nRadioExProductIndex;
	UpdateData(TRUE);
	CString strLog;
	if (nRadioExProductIndexOld != m_nRadioExProductIndex)
	{
		CString strVal[] = {_T("1"),_T("2"),_T("3")};
		strLog.Format(_T("修改[扩展产品序号]由%s到%s\n"), strVal[nRadioExProductIndexOld],strVal[m_nRadioExProductIndex]);
		g_opLogFile.SaveLogText(strLog,2);
	}
}


void CDlgCalcuVirtualObject::OnCbnSelchangeComboExproductIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurExProductIndex = m_comboExProductIndex.GetCurSel();
	CString strLog;
	if (nCurExProductIndex != m_nPreExProductIndex)
	{
		strLog.Format(_T("修改[扩展产品序号]由%d到%d\n"), m_nPreExProductIndex+1,nCurExProductIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
	}
	m_nPreExProductIndex = nCurExProductIndex;	//更新上一次扩展产品的序号，在下一次更改之前，Pre为当前扩展产品序号
}
