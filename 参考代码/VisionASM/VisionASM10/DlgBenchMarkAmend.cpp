// DlgBenchMarkAmend.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgBenchMarkAmend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBenchMarkAmend dialog


CDlgBenchMarkAmend::CDlgBenchMarkAmend(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBenchMarkAmend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBenchMarkAmend)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGuiDisplays = NULL;
	m_pGuiCross = NULL;
	m_ppGuiInteractiveContainer = NULL;
	m_pVisionASM = NULL;
	m_bIsGrabbing = FALSE;
	m_pStatusBar = FALSE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_bStatusBarFlag = TRUE;
	m_psaSysInfoStrings = NULL;
	m_pVAPrevMainWnd = NULL;
}


void CDlgBenchMarkAmend::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
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
	
	m_pVisionASM = pVisionASM;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplays.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplays.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, m_pGuiDisplays);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}
}

void CDlgBenchMarkAmend::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_BENCHMARK") == strDlgID)
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

void CDlgBenchMarkAmend::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgBenchMarkAmend::MoveDlgItem()
{
	if (m_pVisionASM != NULL)
	{
		int i = 0, j = 0;
		int nPatternNum = m_pVisionASM->GetPosNum();
		for (i=0; i<6; i++)
		{
			if (i < nPatternNum)
			{
				GetDlgItem(IDC_STATIC_CH0_DISPLAY+i)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_STATIC_CH0_DISPLAY+i)->ShowWindow(SW_HIDE);
			}
		}

		CRect rcView;
		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
		ScreenToClient(rcView);
		int nRow = 0, nCol = 0;
// 		int nDisplayX = 4, nDisplayY = 4;
		int iStartX = 0, iStartY = 0, iWidth = 0, iHeight = 0;
		if ((nPatternNum > 0) && (nPatternNum <= 2))
		{
			nRow = 1;
			nCol = nPatternNum/nRow;
		}
		else if ((nPatternNum >2) && (nPatternNum<=6))
		{
			nRow = 2;
			nCol = nPatternNum/nRow + nPatternNum%nRow;
		}
		iWidth = (rcView.Width()/* - (nCol-1) * nDisplayX*/) / nCol;
		iHeight = (rcView.Height()/* - (nRow-1) * nDisplayY*/) / nRow;

		for (j=0; j<nRow; j++)
		{
			for (i=0; i<nCol; i++)
			{
				if (j*nCol+i < nPatternNum)
				{
					iStartX = iWidth * i + /*i*nDisplayX + */rcView.left;
					iStartY = iHeight * j + /*j*nDisplayY + */rcView.top;
					GetDlgItem(IDC_STATIC_CH0_DISPLAY+j*nCol+i)->MoveWindow(iStartX, iStartY, iWidth, iHeight);
					if (m_pGuiDisplays != NULL)
					{
						m_pGuiDisplays[j*nCol+i].MoveWindow(iStartX, iStartY, iWidth, iHeight);
					}
				}
				else
				{
					break;
				}
			}
		}
	}
}

void CDlgBenchMarkAmend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBenchMarkAmend)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SERIAL_GRAB, m_btnGrabImage);
	DDX_Control(pDX, IDC_BTN_RESET_MARK, m_btnResetMark);
	DDX_Control(pDX, IDC_BTN_BENCH_FIXTURE, m_btnAmendBenchMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBenchMarkAmend, CDialog)
	//{{AFX_MSG_MAP(CDlgBenchMarkAmend)
	ON_BN_CLICKED(IDC_BTN_SERIAL_GRAB, OnBtnSerialGrab)
	ON_BN_CLICKED(IDC_BTN_BENCH_FIXTURE, OnBtnBenchFixture)
	ON_BN_CLICKED(IDC_BTN_RESET_MARK, OnBtnResetMark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBenchMarkAmend message handlers

LRESULT CDlgBenchMarkAmend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
			UpdateStatusBar();
		}
	case WM_CAMERA_REMOVAL:
		{
			scGuiDisplay* pDisplay = m_pVisionASM->GetCameraDisplay((int)wParam);
			if ((pDisplay != NULL) && ::IsWindow(pDisplay->GetSafeHwnd()))
			{
				pDisplay->ClearScreen();
			}
		}
		break;
	default:
		break;
	}
		
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgBenchMarkAmend::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[3] = {20, 300, -1};
	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	UpdateStatusBar();
	UpdateDialogLanguage();

	// 根据相机数调整显示界面
	MoveDlgItem();

	// 创建各相机采图显示用的GUIDisplay
	int i=0;
	if (m_pVisionASM != NULL)
	{
		int nCamNum = m_pVisionASM->GetPosNum();
		if ((nCamNum >= 1) && (nCamNum <= 6))	// 便于扩展
		{
			m_pGuiDisplays = new scGuiDisplay[nCamNum];
			m_pGuiCross = new scGuiCross[nCamNum];
			m_ppGuiInteractiveContainer = new scGuiInteractiveContainer*[nCamNum];
			for (i=0; i<nCamNum; i++)
			{
				m_pGuiDisplays[i].Create(IDC_STATIC_CH0_DISPLAY+i, this);
			     m_pGuiDisplays[i].SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
			//	m_pGuiDisplays[i].SetBackgroundColor(RGB(0,0,0));;
				m_pVisionASM->SetPosDisplay(i, &m_pGuiDisplays[i]);
				m_pGuiCross[i].SetVisible(FALSE);
				m_ppGuiInteractiveContainer[i] = m_pGuiDisplays[i].GetInteractiveContainer();
				m_pGuiCross[i].SetCenterExtent(sc2Vector(0, 0));
				m_ppGuiInteractiveContainer[i]->AddItem(&m_pGuiCross[i], _T("MarkPoint"));
				m_pGuiDisplays[i].SetInteractiveContainer(m_ppGuiInteractiveContainer[i]);
			}
		}
		else
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_DISPLAY_CTRL_NOT_CREATE);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_NUM_NOT_SUPPORT);
			UpdateStatusBar();
		}

		// 从相机抓取一帧图像，并显示当前产品的基准修正位置
		int nCurProductIdx = 0;
		CCoordPos pos;
		CProductData product;
		nCurProductIdx = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
		product = m_pVisionASM->GetTotalProuductData()->GetItem(nCurProductIdx);
		
		for (i=0; i<nCamNum; i++)
		{
			if (m_pVisionASM->SnapPosCamera(i))
			{
				pos = product.m_pAlignerParam->m_pBenchTargetMarkFixInfo->GetMarkImageCoordPos(i);
				m_ppGuiInteractiveContainer[i] = m_pGuiDisplays[i].GetInteractiveContainer();
				m_pGuiCross[i].SetCenterExtent(sc2Vector(pos.GetPosX(), pos.GetPosY()));
				m_ppGuiInteractiveContainer[i]->AddItem(&m_pGuiCross[i], _T("MarkPoint"));
				m_pGuiDisplays[i].SetInteractiveContainer(m_ppGuiInteractiveContainer[i]);	
			}
		}
	}
	
	// 设置按钮
	m_btnGrabImage.SetThemeHelper(&m_themeHelper);
// 	m_btnGrabImage.SetIcon(IDI_ICON_GRAB_40);
// 	m_btnGrabImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnResetMark.SetThemeHelper(&m_themeHelper);
// 	m_btnResetMark.SetIcon(IDI_ICON_RESET_POINT_POS_40);
// 	m_btnResetMark.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnAmendBenchMark.SetThemeHelper(&m_themeHelper);
// 	m_btnAmendBenchMark.SetIcon(IDI_ICON_BENCHFIXTURE_40);
// 	m_btnAmendBenchMark.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnOK.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBenchMarkAmend::OnBtnSerialGrab() 
{
	CString strLog;
	strLog.Format(_T("点击[采集]\n"));
	g_opLogFile.SaveLogText(strLog,2);

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
					strInfo.Format(_T("CH%d、"), i);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
										strNote.Left(strNote.GetLength()-2), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			UpdateStatusBar();
			return;
		}

		GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));
		GetDlgItem(IDC_BTN_RESET_MARK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BENCH_FIXTURE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
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
					strInfo.Format(_T("CH%d、"), i);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
										strNote.Left(strNote.GetLength()-2), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			UpdateStatusBar();
			return;
		}

		GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_FAILED));
		GetDlgItem(IDC_BTN_RESET_MARK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BENCH_FIXTURE)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		m_bIsGrabbing = TRUE;
	}
}

void CDlgBenchMarkAmend::OnBtnResetMark() 
{
	CString strLog;
	strLog.Format(_T("点击[重置标记点]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	if (m_bIsGrabbing)
	{
		return;
	}

	int nCamNum = m_pVisionASM->GetPosNum();
	LONG nWidth = 0, nHeight = 0;

	for (int i=0; i<nCamNum; i++)
	{
		nWidth = m_pGuiDisplays[i].GetImageWidth();
		nHeight = m_pGuiDisplays[i].GetImageHeight();
		m_ppGuiInteractiveContainer[i] = m_pGuiDisplays[i].GetInteractiveContainer();
		m_pGuiCross[i].SetVisible(TRUE);
		m_pGuiCross[i].SetCenterExtent(sc2Vector(nWidth/2, nHeight/2));
		m_ppGuiInteractiveContainer[i]->AddItem(&m_pGuiCross[i], _T("MarkPoint"));
		m_pGuiDisplays[i].SetInteractiveContainer(m_ppGuiInteractiveContainer[i]);		
	}
}

void CDlgBenchMarkAmend::OnBtnBenchFixture() 
{
	CString strLog;
	strLog.Format(_T("点击[设置基准目标]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	std::vector<CCoordPos> vPtImageBench;
	CCoordPos pos;
	double dX = 0.0, dY = 0.0;
	for (int i=0; i<m_pVisionASM->GetPosNum(); i++)
	{
		m_pGuiCross[i].GetCenterXY(dX, dY);
		pos.m_dPosX = dX;
		pos.m_dPosY = dY;
		vPtImageBench.push_back(pos);
	}
	if (m_pVisionASM->ExecuteBenchMarkFixed(vPtImageBench))
	{
		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_BENCH_MARK_AMEND_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_btnOK.EnableWindow(TRUE);
	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_BENCH_MARK_AMEND_FAILED);
		m_strStatusBarInfo2 = _T("");
	}
	UpdateStatusBar();
}

BOOL CDlgBenchMarkAmend::DestroyWindow() 
{
	if (m_pVisionASM != NULL)
	{
		if (m_bIsGrabbing)
		{
			m_pVisionASM->FreezeAllCameras();
		}
		
		if (m_pGuiDisplays != NULL)
		{
			for (int i=0; i<m_pVisionASM->GetPosNum(); i++)
			{
				m_pVisionASM->SetPosDisplay(i, NULL);
			}
			m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
			
			delete[] m_pGuiDisplays;
			m_pGuiDisplays = NULL;
		}

		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplays.at(i));
		}
	}

	if (m_pGuiCross != NULL)
	{
		delete[] m_pGuiCross;
		m_pGuiCross = NULL;
	}
	if (m_ppGuiInteractiveContainer != NULL)
	{
		delete[] m_ppGuiInteractiveContainer;
		m_ppGuiInteractiveContainer = NULL;
	}

	return CDialog::DestroyWindow();
}



void CDlgBenchMarkAmend::OnOK() 
{
	// TODO: Add extra validation here
	CString strLog;
	strLog.Format(_T("[确定]退出基准目标设置\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnOK();
}

void CDlgBenchMarkAmend::OnCancel() 
{
	// TODO: Add extra cleanup here
	CString strLog;
	strLog.Format(_T("[取消]退出基准目标设置\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnCancel();
}
