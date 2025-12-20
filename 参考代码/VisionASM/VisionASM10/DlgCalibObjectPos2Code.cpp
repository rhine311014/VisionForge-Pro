// DlgCalibObjectPos2Code.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibObjectPos2Code.h"
#include "afxdialogex.h"
#include "DlgCalibTargetDmCodeParam.h"


// CDlgCalibObjectPos2Code 对话框

IMPLEMENT_DYNAMIC(CDlgCalibObjectPos2Code, CDialog)

CDlgCalibObjectPos2Code::CDlgCalibObjectPos2Code(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibObjectPos2Code::IDD, pParent)
{
	m_pVisionASM = NULL;
	m_pVAPrevMainWnd = NULL;
	
	m_vpGuiDisplay.clear();
	m_vpGuiInteractiveContainer.clear();
	m_vpPrevGuiDisplays.clear();

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;

	// 状态栏相关
	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	// 通信交互
	m_nEvent = 0;
	m_bExitThread = FALSE;
	m_pThreadReceiveData = NULL;	
	m_hEventReceiveThreadExit = NULL;

	// 相机采集
	m_bIsGrabbing = FALSE;

	// 二维码定位
//	m_bIsDmCodeSearch = FALSE;


}

CDlgCalibObjectPos2Code::~CDlgCalibObjectPos2Code()
{
	for (int i=0; i<m_pVisionASM->GetPosNum(); i++)
	{
		m_pVisionASM->RemoveGuiCalibrateGraphicsFromDisplay(i,m_vpGuiDisplay.at(i));
	}


	for (int i=0; i<m_vpGuiDisplay.size(); i++)
	{
		if (m_vpGuiDisplay.at(i))
		{
			delete m_vpGuiDisplay.at(i);
			m_vpGuiDisplay.at(i) = NULL;
		}
	}


	m_pVisionASM = NULL;
}

void CDlgCalibObjectPos2Code::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NOTE_AXIS, m_labelNote);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibObjectPos2Code, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_POS1_CAMERA_SHUTTER, &CDlgCalibObjectPos2Code::OnBnClickedButtonPos1CameraShutter)
	ON_BN_CLICKED(IDC_BUTTON_POS2_CAMERA_SHUTTER, &CDlgCalibObjectPos2Code::OnBnClickedButtonPos2CameraShutter)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT0_MOVE_POS0, &CDlgCalibObjectPos2Code::OnBnClickedBtnSearchObject0MovePos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS0, &CDlgCalibObjectPos2Code::OnBnClickedBtnSearchObject1MovePos0)
	ON_BN_CLICKED(IDC_BTN_SET_DMCODE_PARAM, &CDlgCalibObjectPos2Code::OnBnClickedBtnSetDmcodeParam)
	ON_BN_CLICKED(IDOK, &CDlgCalibObjectPos2Code::OnBnClickedOk)

	ON_BN_CLICKED(IDC_BTN_CALIB_OBJECT_POS, &CDlgCalibObjectPos2Code::OnBnClickedBtnCalibObjectPos)
END_MESSAGE_MAP()


// CDlgCalibObjectPos2Code 消息处理程序

void CDlgCalibObjectPos2Code::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
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

void CDlgCalibObjectPos2Code::SetSysInfoStringsRef(CStringArray *psaSysInfoStrings)
{
	m_psaSysInfoStrings = psaSysInfoStrings;
}

void CDlgCalibObjectPos2Code::EnableSysKeyboard(BOOL bEnable)
{
	m_bSysKeyboardEnabled = bEnable;
}


void CDlgCalibObjectPos2Code::InitDlgItem()
{
	m_vbCalibObjectPosSuccess.resize(m_pVisionASM->GetPosNum(), FALSE);

	int i = 0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay[i];
		m_vpGuiDisplay[i] = NULL;
	}
	m_vpGuiDisplay.clear();
	m_vpGuiInteractiveContainer.clear();


	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetPosNum(); i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));

		m_vpGuiDisplay.push_back(pGUIDisplay);
		m_vpGuiDisplay.at(i)->ShowWindow(SW_SHOW);

		m_pVisionASM->SetPosDisplay(i, pGUIDisplay);

		pGUIDisplay = NULL;
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
	m_labelNote.SetText(_T(""));


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


	for (int i=0;i<m_pVisionASM->GetPosNum();i++)
	{
		m_pVisionASM->AddGuiCalibrateGraphicsToDisplay(i,m_vpGuiDisplay.at(i));
	}

	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	int nCurProductIdx = pTotalProductData->GetCurProductIndex();

	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

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

LRESULT CDlgCalibObjectPos2Code::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

	case WM_UPDATE_CALIBRATE_TARGET_GRAPHIC:
		{

			m_pVisionASM->ShowGuiCalibrateGraphics((int)wParam, (BOOL)lParam);
			int nPosIndex = (int)wParam;
			m_vpGuiDisplay.at(nPosIndex)->Invalidate();
		}
		break;

	default:
		{

		}
		break;

	}

	return CDialog::WindowProc(message, wParam, lParam);
}


UINT CDlgCalibObjectPos2Code::ThreadReceiveData( LPVOID pParam )
{
 	CDlgCalibObjectPos2Code *pOwner = (CDlgCalibObjectPos2Code*)pParam;

	while (!pOwner->m_bExitThread)
	{
		switch(pOwner->m_nEvent)
		{
		case 1:
			{
				pOwner->m_nEvent = 0;
				pOwner->DmCodeSearch(0);
			}
			break;

		case 2:
			{
				pOwner->m_nEvent = 0;
				pOwner->DmCodeSearch(1);
			}
			break;

		default:
			{

			}
			break;
		}

		Sleep(100);
	}

	SetEvent(pOwner->m_hEventReceiveThreadExit);

	return 0;
}


void CDlgCalibObjectPos2Code::CameraGrab()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();

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

		m_bIsGrabbing = TRUE;
	}
}

BOOL CDlgCalibObjectPos2Code::DmCodeSearch(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return FALSE;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return FALSE;
	}

	if (FALSE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExDmCode(nPosIndex))
	{
		return FALSE;
	}

	return TRUE;
}


void CDlgCalibObjectPos2Code::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgCalibObjectPos2Code::UpdateDialogLanguage()
{
	//	return;
// 	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
// 	if (m_psaSysInfoStrings != NULL)
// 	{
// 		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
// 	}
// 
// 	vcXMLConfigurator xml;
// 	CString strXMLPath;
// 	CString strCurWorkDir;
// 	TCHAR szPath[MAX_PATH];
// 	GetModuleFileName(NULL, szPath, MAX_PATH);
// 	strCurWorkDir = szPath;
// 	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));
// 
// 	switch (lgidLanguage)
// 	{
// 	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
// 		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
// 		break;
// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
// 		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
// 		break;
// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
// 	default:
// 		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
// 		break;
// 	}
// 	xml.Load(strXMLPath);
// 	xml.FindElem(_T("ResourceBundle"));    
// 	xml.EnterElem();
// 
// 	while(xml.FindElem(_T("")))
// 	{
// 		CString strDlgID = xml.GetAttribute(_T("Key"));
// 
// 		if(_T("IDD_DLG_CALIB_TARGET_POS4_BOARD") == strDlgID)
// 		{
// 			int nID;
// 			CString strValue = xml.GetAttribute(_T("Value"));
// 			SetWindowText(strValue);
// 
// 			xml.EnterElem();	   
// 			while(xml.FindElem())
// 			{
// 				xml.GetAttribute(_T("Id"),  nID);
// 				strValue = xml.GetAttribute(_T("Value"));
// 
// 				if(GetDlgItem(nID) != NULL)
// 				{
// 					GetDlgItem(nID)->SetWindowText(strValue);
// 				}	  
// 			}
// 			xml.LeaveElem();
// 		}  
// 	}
// 	xml.LeaveElem();
// 	if(m_bIsDmCodeSearch)
// 	{
// 		switch (lgidLanguage)
// 		{
// 		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("Target position3"));
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("Target position4"));
// 			break;
// 		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置3"));
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置4"));
// 			break;
// 		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
// 		default:
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置3"));
// 			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置4"));
// 			break;
// 		}
// 	}
}

BOOL CDlgCalibObjectPos2Code::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitDlgItem();	

	CameraGrab();

	if (!m_pVisionASM->IsCommOpen())
	{
		m_pVisionASM->OpenComm();
	}

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	UpdateDialogLanguage();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCalibObjectPos2Code::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_bExitThread = TRUE;
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

void CDlgCalibObjectPos2Code::OnBnClickedButtonPos1CameraShutter()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgCalibObjectPos2Code::OnBnClickedButtonPos2CameraShutter()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgCalibObjectPos2Code::OnBnClickedBtnSearchObject0MovePos0()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 1;
}


void CDlgCalibObjectPos2Code::OnBnClickedBtnSearchObject1MovePos0()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 2;
}


void CDlgCalibObjectPos2Code::OnBnClickedBtnSetDmcodeParam()
{
	// TODO: 在此添加控件通知处理程序代码

	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	if (pTotalProductData->GetCount()<= 0)
	{
		return;
	}

// 	CString strLog;
// 	strLog.Format(_T("点击[二维码参数设置]进入[二维码参数设置界面]\n"));
// 	g_opLogFile.SaveLogText(strLog,2);

	CameraGrab();

	CDlgCalibTargetDmcodeParam dlg;
	dlg.SetVisionASMRef(m_pVisionASM);	
	dlg.SetIsBoardCalibrate(true);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlg.SetLogLevel(3);
	dlg.DoModal();

	CameraGrab();

}

void CDlgCalibObjectPos2Code::OnBnClickedBtnCalibObjectPos()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pVisionASM == NULL)
	{
		return;
	}

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();

	if (!m_bIsGrabbing)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("相机采集失败"));
		m_strStatusBarInfo1.Format(_T(""));

		UpdateStatusBar();

		return ;
	}

	if (NULL == m_pVisionASM->m_pAlignerTool)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("对位工具异常"));
		m_strStatusBarInfo1.Format(_T(""));

		UpdateStatusBar();

		return ;
	}


	for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
	{
		if (FALSE == m_pVisionASM->m_vDmCodeCalibTool.at(i).IsSearchValid())
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_OBJECT_AND_TARGET_POS);//.Format(_T("请获取对象、目标位置！"));

			UpdateStatusBar();
			return;
		}
	}


	int nExProductIndex = 0;

	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}



	std::vector<sc2Vector> vImagePos;
	std::vector<sc2Vector> vBoardPos;

 	CString strTempInfo2;
	int nPosIndex = 0;
	for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum(); nPosIndex++)
	{
		m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex).GetCornersImagePos(vImagePos);
		m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex).GetCornersLabelPos(vBoardPos);

		sc2Vector scImageCenter = sc2Vector(0, 0);

		scImageCenter.SetX( m_pVisionASM->GetPosDisplay(nPosIndex)->GetImageWidth() / 2.0 );
		scImageCenter.SetY( m_pVisionASM->GetPosDisplay(nPosIndex)->GetImageHeight() / 2.0 );

		if (TRUE == m_pVisionASM->m_pAlignerTool->ExecuteCalibrateDmCodeSingleCamEX(nPosIndex+m_pVisionASM->GetPosNum()*nExProductIndex,scImageCenter,vImagePos,vBoardPos))
		{
			m_vbCalibObjectPosSuccess.at(nPosIndex) = TRUE;
		}
		else
		{
			m_vbCalibObjectPosSuccess.at(nPosIndex) = FALSE;
		}
	}

	m_bStatusBarFlag = m_vbCalibObjectPosSuccess.at(0);
	for(nPosIndex =  0; nPosIndex < m_pVisionASM->GetPosNum(); nPosIndex++)
	{
		m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibObjectPosSuccess.at(nPosIndex);
		if (TRUE == m_vbCalibObjectPosSuccess.at(nPosIndex))
		{
			strTempInfo2.Format(_T("对象位置%d标定成功！"), nPosIndex + 1);
		}
		else
		{
			strTempInfo2.Format(_T("对象位置%d标定失败！"), nPosIndex + 1);
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

void CDlgCalibObjectPos2Code::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pVisionASM == NULL)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();
	}
	
	int nPosIndex = 0;
	BOOL bCalibSuccess = m_vbCalibObjectPosSuccess.at(nPosIndex);
	for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum(); nPosIndex++)
	{
		bCalibSuccess = bCalibSuccess && m_vbCalibObjectPosSuccess.at(nPosIndex);	
	}

	if (bCalibSuccess)
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
			m_pVisionASM->ExecuteCopyCalibrateData(1,nExProductIndex);
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

		for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
		{
			m_pVisionASM->m_vDmCodeCalibTool.at(i).SetSearchValid(FALSE);
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearSearchResult();
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearRelationResult();
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

	CDialog::OnOK();
}


