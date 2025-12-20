// DlgCalcuVirtualObject4.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalcuVirtualObject4.h"
#include "afxdialogex.h"


// CDlgCalcuVirtualObject4 对话框

IMPLEMENT_DYNAMIC(CDlgCalcuVirtualObject4, CDialog)

CDlgCalcuVirtualObject4::CDlgCalcuVirtualObject4(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalcuVirtualObject4::IDD, pParent)
{
	m_pVAPrevMainWnd = NULL;
	m_pVisionASM = NULL;
	
	m_psaSysInfoStrings = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;
	m_nEvent = 0;

	m_bIsGrabbing = FALSE;
	m_vpPrevGuiDisplays.clear();
	m_vpGuiDisplay.clear();
	m_vpGuiVirtualObjectDisplay.clear();
	m_vpGuiInteractiveContainer.clear();
	m_vpGuiVirtualObjectInteractiveContainer.clear();
//	m_vpGuiCross.clear();
	m_vpGuiText.clear();
	m_vpGuiVirtualObjectText.clear();

	m_bCalcuVirtualObjectSuccess = FALSE;
	m_nPreExProductIndex = 0;
}

CDlgCalcuVirtualObject4::~CDlgCalcuVirtualObject4()
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
	m_vpGuiDisplay.clear();


	for (i=0; i<m_vpGuiVirtualObjectDisplay.size(); i++)
	{
		if (m_vpGuiVirtualObjectDisplay.at(i))
		{
			delete m_vpGuiVirtualObjectDisplay.at(i);
			m_vpGuiVirtualObjectDisplay.at(i) = NULL;
		}
	}
	m_vpGuiVirtualObjectDisplay.clear();


	for(i=0; i<m_vpGuiText.size();i++)
	{
		if (m_vpGuiText.at(i) != NULL)
		{
			delete m_vpGuiText.at(i);
			m_vpGuiText.at(i) = NULL;
		}
	}
	m_vpGuiText.clear();


	for(i=0; i<m_vpGuiVirtualObjectText.size();i++)
	{
		if (m_vpGuiVirtualObjectText.at(i) != NULL)
		{
			delete m_vpGuiVirtualObjectText.at(i);
			m_vpGuiVirtualObjectText.at(i) = NULL;
		}
	}
	m_vpGuiVirtualObjectText.clear();


	for(i=0; i< m_vpGuiAlnObjectImageMarkPos.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos.at(i);
			m_vpGuiAlnObjectImageMarkPos.at(i) = NULL;
		}
	}
	m_vpGuiAlnObjectImageMarkPos.clear();


	for(i=0; i< m_vpGuiAlnVirtualObjectImageMarkPos.size();i++)
	{
		if (m_vpGuiAlnVirtualObjectImageMarkPos.at(i) != NULL)
		{
			delete m_vpGuiAlnVirtualObjectImageMarkPos.at(i);
			m_vpGuiAlnVirtualObjectImageMarkPos.at(i) = NULL;
		}
	}
	m_vpGuiAlnVirtualObjectImageMarkPos.clear();


	m_pVisionASM = NULL;
}

void CDlgCalcuVirtualObject4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NOTE, m_labelNote);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);

	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS0, m_btnSearchObjectPos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS1, m_btnSearchObjectPos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0, m_btnSearchVirtualObjectPos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1, m_btnSearchVirtualObjectPos1);

	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS2, m_btnSearchObjectPos2);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT_POS3, m_btnSearchObjectPos3);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS2, m_btnSearchVirtualObjectPos2);
	DDX_Control(pDX, IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS3, m_btnSearchVirtualObjectPos3);

	DDX_Control(pDX, IDC_BTN_CALCU_VIRTUAL_OBJECT, m_btnCalcuVirtualObject);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalcuVirtualObject4, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS0, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS1, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS2, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos2)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT_POS3, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos3)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS2, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos2)
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS3, &CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos3)
	ON_BN_CLICKED(IDC_BTN_CALCU_VIRTUAL_OBJECT, &CDlgCalcuVirtualObject4::OnBnClickedBtnCalcuVirtualObject)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CDlgCalcuVirtualObject4::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalcuVirtualObject4::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_INDEX, &CDlgCalcuVirtualObject4::OnCbnSelchangeComboExproductIndex)
END_MESSAGE_MAP()


// CDlgCalcuVirtualObject4 消息处理程序

BOOL CDlgCalcuVirtualObject4::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitDlgItem();	

	UpdateData(FALSE);

	SerialCamGrab();

	UpdateDialogLanguage();

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCalcuVirtualObject4::OnDestroy()
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

UINT CDlgCalcuVirtualObject4::ThreadReceiveData( LPVOID pParam )
{
	CDlgCalcuVirtualObject4 *pOwner = (CDlgCalcuVirtualObject4*)pParam;
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
		else if (pOwner->m_nEvent == 5)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 2;
			pOwner->SearchObjectPos(nPosIndex);
		}
		else if (pOwner->m_nEvent == 6)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 2;
			pOwner->SearchVirtualObjectPos(nPosIndex);

		}
		else if (pOwner->m_nEvent == 7)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 3;
			pOwner->SearchObjectPos(nPosIndex);

		}
		else if (pOwner->m_nEvent == 8)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 3;
			pOwner->SearchVirtualObjectPos(nPosIndex);
		}


		Sleep(10);
	}

	SetEvent(pOwner->m_hEventReceiveThreadExit);

	return 0;
}

void CDlgCalcuVirtualObject4::SearchObjectPos(int nPosIndex)
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

	m_pVisionASM->SetDefaultObjectSearchMarkPos(TRUE, 11);
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

void CDlgCalcuVirtualObject4::SearchVirtualObjectPos(int nPosIndex)
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
	}
}

void CDlgCalcuVirtualObject4::UpdateDialogLanguage()
{

}

void CDlgCalcuVirtualObject4::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
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

void CDlgCalcuVirtualObject4::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgCalcuVirtualObject4::InitDlgItem()
{
	if (!m_pVisionASM->IsCommOpen())
	{
		m_pVisionASM->OpenComm();
	}

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

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_LABEL_NOTE)/*_T("注:请确保对象、虚拟对象各拍照位置的平台θ轴位置一致！！！")*/);


	m_btnOK.SetThemeHelper(&m_themeHelper);	
	m_btnCancel.SetThemeHelper(&m_themeHelper);

	m_btnSearchObjectPos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchObjectPos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos1.SetThemeHelper(&m_themeHelper);
	m_btnCalcuVirtualObject.SetThemeHelper(&m_themeHelper);

	m_btnSearchObjectPos2.SetThemeHelper(&m_themeHelper);
	m_btnSearchObjectPos3.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos2.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObjectPos3.SetThemeHelper(&m_themeHelper);

	m_btnCalcuVirtualObject.SetThemeHelper(&m_themeHelper);


	int i = 0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay[i];
		m_vpGuiDisplay[i] = NULL;
	}
	m_vpGuiDisplay.clear();
	m_vpGuiInteractiveContainer.clear();


	// 对象
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetPosNum() && i < 4; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));

		m_pVisionASM->SetPosDisplay(i, pGUIDisplay);
		m_vpGuiDisplay.push_back(pGUIDisplay);
		m_vpGuiDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;

	}


	// 虚拟对象
	for (i=0; i<m_pVisionASM->GetPosNum() && i < 4; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i+4, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));

		m_vpGuiVirtualObjectDisplay.push_back(pGUIDisplay);
		m_vpGuiVirtualObjectDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}


	scGuiStaticContainer *pGuiSC = NULL;
	scGuiCoordCross* pGuiCoordCross = NULL;
	scGuiText *pGuiText = NULL;
	CString strKey;
	CString strInfo;

	// 对象
	for(i = 0;  i<m_pVisionASM->GetPosNum() && i < 4; i++)
	{
		// 十字GUI
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));
		m_vpGuiAlnObjectImageMarkPos.push_back(pGuiCoordCross);

		// 文字GUI
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_OBJECT_POS)/*_T("对象位置%d")*/,i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiText.push_back(pGuiText);


		pGuiSC = m_vpGuiDisplay.at(i)->GetStaticContainer();
		// 十字GUI
		strKey.Format(_T("ObjectPos%d"),i);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos.at(i), strKey);
		// 文字GUI
		strKey.Format(_T("GuiText%d"),i);
		pGuiSC->AddItem(m_vpGuiText.at(i), strKey);
		m_vpGuiDisplay.at(i)->SetStaticContainer(pGuiSC);
	}


	// 虚拟对象
	for(i = 0;  i<m_pVisionASM->GetPosNum() && i < 4; i++)
	{
		// 十字GUI
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));
		m_vpGuiAlnVirtualObjectImageMarkPos.push_back(pGuiCoordCross);

		// 文字GUI
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_VIRTUAL_OBJECT_POS)/*_T("虚拟对象位置%d")*/,i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiVirtualObjectText.push_back(pGuiText);

		pGuiSC = m_vpGuiVirtualObjectDisplay.at(i)->GetStaticContainer();
		// 十字GUI
		strKey.Format(_T("VirtualObjectPos%d"),i);
		pGuiSC->AddItem(m_vpGuiAlnVirtualObjectImageMarkPos.at(i), strKey);
		// 文字GUI
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

}

void CDlgCalcuVirtualObject4::SerialCamGrab()
{
	int i=0;
	BOOL bSucceed = TRUE;
	std::vector<BOOL> vbSucceed;
	int nCamNum = m_pVisionASM->GetPosNum(); 
	

	if (m_bIsGrabbing)
	{
		for (i=0; i<nCamNum && i<4; i++)
		{
			vbSucceed.push_back(m_pVisionASM->FreezePosCamera(i));
		}

		for (i=0; i<nCamNum && i<4; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}

		if (!bSucceed)
		{
			CString strInfo, strNote;

			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			for (i=0; i<nCamNum && i<4; i++)	
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
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			UpdateStatusBar();

			return;
		}

		m_bIsGrabbing = FALSE;
	}
	else
	{
		for (i=0; i<nCamNum && i<4; i++)
		{
			vbSucceed.push_back(m_pVisionASM->GrabPosCamera(i));
		}

		for (i=0; i<nCamNum && i<4; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}

		if (!bSucceed)
		{
			CString strInfo, strNote;

			// roll back（如果一个相机采集失败，则恢复所有成功采集的相机为停止状态）
			for (i=0; i<nCamNum && i<4; i++)	
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
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			UpdateStatusBar();

			return;
		}

		m_bIsGrabbing = TRUE;
	}

}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 1;

	CString strLog;
	strLog.Format(_T("点击[搜索对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}

void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 2;

	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 3;

	CString strLog;
	strLog.Format(_T("点击[搜索对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 4;

	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 5;
	CString strLog;
	strLog.Format(_T("点击[搜索对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}

void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 6;
	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置1]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchObjectPos3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 7;
	CString strLog;
	strLog.Format(_T("点击[搜索对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnSearchVirtualObjectPos3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 8;

	CString strLog;
	strLog.Format(_T("点击[搜索虚拟对象位置2]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgCalcuVirtualObject4::OnBnClickedBtnCalcuVirtualObject()
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
	if (TRUE == m_pVisionASM->CalculateObjectToVirtualMarkOffset(m_vObjectToVirtualOffsets,nExProductIndex))
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

void CDlgCalcuVirtualObject4::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存退出]计算虚拟对象位置\n"));
	g_opLogFile.SaveLogText(strLog,1);

	if (m_bCalcuVirtualObjectSuccess)
	{
		if (m_pVisionASM != NULL && TRUE == m_pVisionASM->SetAndSaveVirtualMarkOffset(m_vObjectToVirtualOffsets))
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



LRESULT CDlgCalcuVirtualObject4::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);

			int nPosIndex = (int)wParam;
			if (m_vpGuiDisplay[nPosIndex]->GetImageCP() != NULL)
			{
				m_vpGuiVirtualObjectDisplay[nPosIndex]->SetImage(*(cpImage*)m_vpGuiDisplay[nPosIndex]->GetImageCP(),TRUE);
			}
			else
			{
				m_vpGuiVirtualObjectDisplay[nPosIndex]->RemoveImage(FALSE);
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





void CDlgCalcuVirtualObject4::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[不保存退出]计算虚拟对象位置\n"));
	g_opLogFile.SaveLogText(strLog,2);
	CDialog::OnCancel();
}


void CDlgCalcuVirtualObject4::OnCbnSelchangeComboExproductIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurExProductIndex = m_comboExProductIndex.GetCurSel();
	CString strLog;
	if (nCurExProductIndex != m_nPreExProductIndex)
	{
		strLog.Format(_T("修改[扩展产品序号]由%s到%s\n"), m_nPreExProductIndex+1,nCurExProductIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
	}
}
