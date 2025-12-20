// DlgManualMeasure.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgManualMeasure.h"
#include "afxdialogex.h"
#include "LogFileSave.h"

const long EPSINON = 0.0000001;

// DlgManualMeasure 对话框

IMPLEMENT_DYNAMIC(CDlgManualMeasure, CDialog)

CDlgManualMeasure::CDlgManualMeasure(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualMeasure::IDD, pParent)
	, m_nRadDistanceMode(0)
	, m_bPixelValueMode(FALSE)
	, m_nCameraIndex(0)
{
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_strInputValue = _T("0.005");
	m_strCalibValue = _T("0.0");
	m_strDistance = _T("0.0");
	m_strXDistance = _T("0.0");
	m_strYDistance = _T("0.0");
	m_strActualDistance = _T("0.0");
	m_strActualXDistance = _T("0.0");
	m_strActualYDistance = _T("0.0");
	m_pVisionASM = NULL;

	m_nPrevCameraIndex = m_nCameraIndex;

	m_StatusBar = NULL;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_bStatusBarFlag = TRUE;

	m_pVAPrevMainWnd = NULL;

	m_pInteractiveContainer = NULL;
	m_pStaticContainer = NULL;


}

CDlgManualMeasure::~CDlgManualMeasure()
{

	if (m_StatusBar != NULL)
	{
		delete m_StatusBar;
		m_StatusBar = NULL;
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

	m_pInteractiveContainer = m_pGuiDisplay.GetInteractiveContainer();
	m_pStaticContainer = m_pGuiDisplay.GetStaticContainer();
	m_pInteractiveContainer->Clear();
	m_pStaticContainer->Clear();
}

void CDlgManualMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONTINU_GRAB, m_btnContinueGrab);
	DDX_Control(pDX, IDC_BUTTON_LOAD_IMAGE, m_btnLoadImage);
	DDX_Control(pDX, IDC_BUTTON_SAVE_IMAGE, m_btnSaveImage);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE, m_btnExecute);
	DDX_Radio(pDX, IDC_RADIO_POINT_TO_POINT, m_nRadDistanceMode);
	DDX_Text(pDX, IDC_EDIT_INPUT_VALUE, m_strInputValue);
	DDX_Text(pDX, IDC_EDIT_CALIB_VALUE, m_strCalibValue);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_strDistance);
	DDX_Radio(pDX, IDC_RADIO_INPUT_VALUE, m_bPixelValueMode);
	DDX_Control(pDX, IDC_COMBO_INPUT_OR_RESULT_IMAGE, m_comboDisplayImage);
	DDX_Text(pDX, IDC_EDIT_ACTUAL_DIS, m_strActualDistance);
	DDX_Radio(pDX, IDC_RADIO_POSITION0, m_nCameraIndex);
	DDX_Text(pDX, IDC_EDIT_XDISTANCE, m_strXDistance);
	DDX_Text(pDX, IDC_EDIT_YDISTANCE, m_strYDistance);
	DDX_Text(pDX, IDC_EDIT_ACTUAL_XDIS, m_strActualXDistance);
	DDX_Text(pDX, IDC_EDIT_ACTUAL_YDIS, m_strActualYDistance);
}


BEGIN_MESSAGE_MAP(CDlgManualMeasure, CDialog)
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SV_DISPLAY_MOUSEMOVE,&CDlgManualMeasure::OnGuiDisplayMouseMove)
	ON_BN_CLICKED(IDC_BUTTON_CONTINU_GRAB, &CDlgManualMeasure::OnBnClickedButtonContinuGrab)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CDlgManualMeasure::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGE, &CDlgManualMeasure::OnBnClickedButtonSaveImage)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CDlgManualMeasure::OnBnClickedButtonExecute)
	ON_EN_SETFOCUS(IDC_EDIT_INPUT_VALUE, &CDlgManualMeasure::OnEnSetfocusEditInputValue)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_OR_RESULT_IMAGE, &CDlgManualMeasure::OnCbnSelchangeComboInputOrResultImage)
	ON_BN_CLICKED(IDC_RADIO_POINT_TO_POINT, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_POINT_TO_SEG, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_POINT_TO_LINE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_POINT_TO_CIRCLE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_POINT_TO_ELLIPSE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_LINE_TO_CIRCLE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_LINE_TO_ELLIPSE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_TO_CIRCLE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_SEG_TO_SEG, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_SEG_TO_LINE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_SEG_TO_CIRCLE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_SEG_TO_ELLIPSE, &CDlgManualMeasure::OnBnClickedMeasureRadioChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION0, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION1, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION2, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION3, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION4, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION5, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION6, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_POSITION7, &CDlgManualMeasure::OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CALIB, &CDlgManualMeasure::OnBnClickedRadioInputValue)
	ON_BN_CLICKED(IDC_RADIO_INPUT_VALUE, &CDlgManualMeasure::OnBnClickedRadioInputValue)
END_MESSAGE_MAP()


// DlgManualMeasure 消息处理程序



BOOL CDlgManualMeasure::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateDialogLanguage();

	ASSERT(m_pVisionASM != NULL);
	int nCamNum = m_pVisionASM->GetPosNum();


	
	m_comboDisplayImage.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_INPUT_IMAGE));
	m_comboDisplayImage.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RESULT_IMAGE));

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	
	m_pGuiDisplay.Create(IDC_STATIC_DISPLAY_IMAGE, this);
	m_pGuiDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pGuiDisplay.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pGuiDisplay.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pGuiDisplay.SetLanguage(1);
		break;
	}
	m_pGuiDisplay.RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DBLCLICK);


	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[3] = {20, 400,  -1};
	m_StatusBar->SetParts(3, nPartWidth);
	m_StatusBar->ShowWindow(SW_SHOW);
	
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	

	BeginWaitCursor();
	// 调整相机选择按钮
	int i = 0;
	for (i=0; i<8; i++)
	{
		if (i < nCamNum)
		{
			GetDlgItem(IDC_RADIO_POSITION0+i)->ShowWindow(SW_SHOW);
			//if (m_pVisionASM->IsCameraFirstPosIndex(i))
			//{
			//	GetDlgItem(IDC_RADIO_POSITION0+i)->EnableWindow(TRUE);
			//}
			//else
			//{
			//	GetDlgItem(IDC_RADIO_POSITION0+i)->EnableWindow(FALSE);
			//}
		}
		else
		{
			GetDlgItem(IDC_RADIO_POSITION0+i)->ShowWindow(SW_HIDE);
		}
	}

	//设置按钮
	m_btnContinueGrab.SetThemeHelper(&m_themeHelper);
	m_btnLoadImage.SetThemeHelper(&m_themeHelper);
	m_btnSaveImage.SetThemeHelper(&m_themeHelper);
	m_btnExecute.SetThemeHelper(&m_themeHelper);

	m_comboDisplayImage.SetCurSel(0);

	if (m_pVisionASM != NULL)
	{
		// 如果没有相机，禁用“采集图像”按钮
		GetDlgItem(IDC_BUTTON_CONTINU_GRAB)->EnableWindow(m_pVisionASM->IsPosCameraOnline(m_nCameraIndex));

		if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
		{
			if (m_pVisionASM->FreezePosCamera(m_nCameraIndex))
			{
				SetDlgItemText(IDC_BUTTON_CONTINU_GRAB, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
			}
			else
			{
				SetDlgItemText(IDC_BUTTON_CONTINU_GRAB, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
			}
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_CONTINU_GRAB, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
		}
	}

	ShowControl();
	GetCalibratedInfoToMeasure();
	UpdateData(FALSE);
	EndWaitCursor();
	return TRUE;
}

void CDlgManualMeasure::UpdateStatusBar()
{
	m_StatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_StatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_StatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}



BOOL CDlgManualMeasure::DestroyWindow()
{

	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->FreezeAllCameras();
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}
    CString strLog;
    strLog.Format(_T("退出[手动测量界面]\n"));
    g_opLogFile.SaveLogText(strLog,3);

	return CDialog::DestroyWindow();

}



LRESULT CDlgManualMeasure::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);

		}
		break;
	case WM_CAMERA_ARRIVAL:
		{
			if (m_nCameraIndex == (int)wParam)
			{
				UpdateBtnDisplay();

				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
		}
		break;
	case WM_CAMERA_REMOVAL:
		{
			if (m_nCameraIndex == (int)wParam)
			{
				scGuiDisplay* pDisplay = m_pVisionASM->GetCameraDisplay((int)wParam);
				if ((pDisplay != NULL) && ::IsWindow(pDisplay->GetSafeHwnd()))
				{
					pDisplay->ClearScreen();
				}
				UpdateBtnDisplay();

				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgManualMeasure::OnBnClickedButtonContinuGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nCameraIndex))
		{
			m_pVisionASM->SetPosDisplay(m_nCameraIndex, &m_pGuiDisplay);
			m_comboDisplayImage.SetCurSel(0);
			if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
			{
				if (m_pVisionASM->FreezePosCamera(m_nCameraIndex))
				{
					m_ImageInput.Release();
					cpImage* pImageTemp = (cpImage*)m_pGuiDisplay.GetImageCP();
					if (pImageTemp != NULL)
					{
						pImageTemp->Clone(m_ImageInput);	
					}
					SetDlgItemText(IDC_BUTTON_CONTINU_GRAB, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
					m_comboDisplayImage.EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(TRUE);

					int nWidth = 752;
					int nHeight = 480;

					if (pImageTemp != NULL)
					{
						nWidth = pImageTemp->Width();
						nHeight = pImageTemp->Height();
					}

					UpdateGuiDisplay();
//					InitMeasureElems(m_pGuiDisplay.GetImageCP()->Width(),m_pGuiDisplay.GetImageCP()->Height());
					InitMeasureElems(nWidth, nHeight);
					UpdateDisplay();
					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_STOPPED);	//_T("采集已停止")
				}
				else
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB);	//_T("停止采集失败");
				}
			}
			else
			{


				if (m_pVisionASM->GrabPosCamera(m_nCameraIndex))
				{
					SetDlgItemText(IDC_BUTTON_CONTINU_GRAB, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
					m_comboDisplayImage.EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(FALSE);
					
					UpdateGuiDisplay();
					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING);	//_T("正在连续采集")
					m_strStatusBarInfo2 = _T("");
				}
				else
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_FAILED);	//_T("采集失败")
				}
			}		
		}
		else
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_CONNECTION_ERROR);	//_T("相机连接错误")
		}
	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_VISION_ALIGN_TOOL);
	}

	UpdateStatusBar();
}

void CDlgManualMeasure::OnBnClickedButtonLoadImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_FILE) + _T("|*.bmp||");
	CFileDialog cFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (IDOK == cFileDlg.DoModal())
	{
		scDIB cDib;
		CString strPath = cFileDlg.GetPathName();
		cDib.Init(strPath);
		cDib.GetImage(m_ImageInput);

		scGuiDisplay mGuiTmp;
		mGuiTmp.SetImage(m_ImageInput);

		InitMeasureElems(mGuiTmp.GetImageCP()->Width(),mGuiTmp.GetImageCP()->Height());

		m_comboDisplayImage.SetCurSel(0);
		UpdateDisplay();

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1.Format(_T("加载图像"));
		m_strStatusBarInfo2 = strPath;
		UpdateStatusBar();
	}

}


void CDlgManualMeasure::OnBnClickedButtonSaveImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE, ".bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"位图文件（*.bmp）| *.bmp||");

	// 选择要显示的图像（BMP）文件
	if(dlg.DoModal() == IDOK)
	{
		// 向显示控件中加载图像文件
		m_pGuiDisplay.SaveImage(LPCTSTR(dlg.GetPathName()));
	}


}


void CDlgManualMeasure::OnBnClickedButtonExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_ImageInput.IsValid())
	{
		//m_StatusBar->SetText("图像数据为空！",2,0);
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T(""));
		m_strStatusBarInfo2.Format(_T("图像数据为空！"));
		UpdateStatusBar();
		return;
	}

	double dDistance = 0.0;
	sc2Vector	Vector1;
	sc2Vector	Vector2;

	BOOL		bSuccess;
	scTimer		timer;

	switch(m_nRadDistanceMode)
	{
	case ePointToPointDistance:
		{
			bSuccess = sfDistancePoint2Point(m_GuiCross1.GetCenter(),m_GuiCross2.GetCenter(),dDistance);
			if (bSuccess)
			{
				double dXDistance = abs(m_GuiCross1.GetCenter().GetX() - m_GuiCross2.GetCenter().GetX());
				double dYDistance = abs(m_GuiCross1.GetCenter().GetY() - m_GuiCross2.GetCenter().GetY());
				UpdateResult(dDistance,dXDistance,dYDistance);
			}
		}
		break;
	case ePointToLineSegDistance:
		{
			bSuccess = sfDistancePoint2Segment(m_GuiCross1.GetCenter(),m_GuiLineSeg2.GetLineSeg(),dDistance,Vector1);
			if (bSuccess)
			{
				double dXDistance = abs(m_GuiCross1.GetCenter().GetX() - Vector1.GetX());
				double dYDistance = abs(m_GuiCross1.GetCenter().GetY() - Vector1.GetY());
				UpdateResult(dDistance,Vector1,dXDistance,dYDistance);
			}
		}
		break;
	case ePointToLineDistance:
		{
			bSuccess = sfDistancePoint2Line(m_GuiCross1.GetCenter(),m_GuiLine2.GetLine(),dDistance,Vector1);
			if (bSuccess)
			{
				double dXDistance = abs(m_GuiCross1.GetCenter().GetX() - Vector1.GetX());
				double dYDistance = abs(m_GuiCross1.GetCenter().GetY() - Vector1.GetY());
				UpdateResult(dDistance,Vector1,dXDistance,dYDistance);
			}
		}
		break;
	case ePointToCircleDistance:
		{
			bSuccess = sfDistancePoint2Circle(m_GuiCross1.GetCenter(),m_GuiCircle2.GetCircle(),dDistance,Vector1);
			if (bSuccess)
			{
				double dXDistance = abs(m_GuiCross1.GetCenter().GetX() - Vector1.GetX());
				double dYDistance = abs(m_GuiCross1.GetCenter().GetY() - Vector1.GetY());
				UpdateResult(dDistance,Vector1,dXDistance,dYDistance);
			}
		}
		break;
	case ePointToEllipseDistance:
		{
			bSuccess = sfDistancePoint2Ellipse(m_GuiCross1.GetCenter(),m_GuiEllipse.GetEllipse(),dDistance,Vector1);
			if (bSuccess)
			{
				double dXDistance = abs(m_GuiCross1.GetCenter().GetX() - Vector1.GetX());
				double dYDistance = abs(m_GuiCross1.GetCenter().GetY() - Vector1.GetY());
				UpdateResult(dDistance,Vector1,dXDistance,dYDistance);
			}
		}
		break;
	case eLineToCircleDistance:
		{
			bSuccess = sfDistanceLine2Circle(m_GuiLine1.GetLine(),m_GuiCircle2.GetCircle(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eLineToEllipseDistance:
		{
			bSuccess = sfDistanceLine2Ellipse(m_GuiLine1.GetLine(),m_GuiEllipse.GetEllipse(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eCircleToCircleDistance:
		{
			bSuccess = sfDistanceCircle2Circle(m_GuiCircle1.GetCircle(),m_GuiCircle2.GetCircle(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eLineSegToLineSegDistance:
		{
			bSuccess = sfDistanceSegment2Segment(m_GuiLineSeg1.GetLineSeg(),m_GuiLineSeg2.GetLineSeg(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eLineSegToLineDistance:
		{
			bSuccess = sfDistanceSegment2Line(m_GuiLineSeg1.GetLineSeg(),m_GuiLine2.GetLine(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eLineSegToCircleDistance:
		{
			bSuccess = sfDistanceSegment2Circle(m_GuiLineSeg1.GetLineSeg(),m_GuiCircle2.GetCircle(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;
	case eLineSegToEllipseDistance:
		{
			bSuccess = sfDistanceSegment2Ellipse(m_GuiLineSeg1.GetLineSeg(),m_GuiEllipse.GetEllipse(),dDistance,Vector1,Vector2);
			if (bSuccess)
			{
				double dXDistance = abs(Vector1.GetX() - Vector2.GetX());
				double dYDistance = abs(Vector1.GetY() - Vector2.GetY());
				UpdateResult(dDistance,Vector1,Vector2,dXDistance,dYDistance);
			}
		}
		break;

	default:
		break;
	
	}
	double time = timer.GetTimeMilli(true);
	CString strtime;
	strtime.Format("执行耗时：%.2f ms",time);

	if (!bSuccess)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T(""));
		m_strStatusBarInfo2.Format(_T("执行失败！"));
		UpdateStatusBar();
		return;
	}
	else
	{
		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = strtime;
		m_strStatusBarInfo2.Format(_T("执行成功！"));
		UpdateStatusBar();
	}

	m_comboDisplayImage.SetCurSel(1);
	UpdateDisplay();
}


void CDlgManualMeasure::OnEnSetfocusEditInputValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INPUT_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INPUT_VALUE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INPUT_VALUE, strOutput);
		}

	}
}


void CDlgManualMeasure::OnCbnSelchangeComboInputOrResultImage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDisplay();
}


void CDlgManualMeasure::UpdateDialogLanguage()
{
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

		if(_T("IDD_DLG_MANUAL_MEASURE") == strDlgID)
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


void CDlgManualMeasure::UpdateBtnDisplay()
{
	if (!m_pVisionASM->IsPosCameraOnline(m_nCameraIndex))
	{
		GetDlgItem(IDC_BUTTON_CONTINU_GRAB)->EnableWindow(FALSE);
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CONTINU_GRAB)->EnableWindow(TRUE);
	}

	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		GetDlgItem(IDC_BUTTON_CONTINU_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
		GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
		GetDlgItem(IDC_BUTTON_SAVE_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LOAD_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(TRUE);
	}
}

void CDlgManualMeasure::UpdateGuiDisplay()
{
	UpdateData(TRUE);

	m_pGuiDisplay.ClearScreen();
	int nComboDisplayType = m_comboDisplayImage.GetCurSel();
	switch(nComboDisplayType)
	{
	case 0:
		{
			m_pGuiDisplay.SetImage(m_ImageInput,TRUE);
		}
		break;
	default:
		break;
	}
}



void CDlgManualMeasure::SetVisionASMRef(vcBaseVisionAlign *pVisionASM)
{
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);

	}

	m_pVisionASM = pVisionASM;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplay.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplay.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);

	}
}

void CDlgManualMeasure::InitMeasureElems(long nWidthNew,long nHeightNew)
{
	//30万相机
	int nWidth = 752;
	int nHeight = 480;

	int nZoomX = nWidthNew/nWidth;
	int nZoomY = nHeightNew/nHeight;

	if (!nZoomX)
	{
		nZoomX = 1;
	}

	if (!nZoomY)
	{
		nZoomY =1;
	}

			
	m_GuiCross2.SetLineColor(RGB(255,0,255));
	m_GuiLineSeg2.SetLineColor(RGB(255,0,255));
	m_GuiLine2.SetLineColor(RGB(255,0,255));
	m_GuiCircle2.SetLineColor(RGB(255,0,255));
	m_GuiEllipse.SetLineColor(RGB(255,0,255));

	m_GuiStaticLineSeg.SetLineColor(RGB(255,255,0));
	m_GuiStaticCircle1.SetLineColor(RGB(255,255,0));
	m_GuiStaticCircle2.SetLineColor(RGB(255,255,0));		

	m_GuiCross1.SetCenterXYWidthHeight(100*nZoomX,200*nZoomY,64*nZoomX,64*nZoomX);
	m_GuiCross2.SetCenterXYWidthHeight(300*nZoomX,100*nZoomY,64*nZoomX,64*nZoomX);
	m_GuiLineSeg1.SetStartXYEndXY(100*nZoomX,200*nZoomY,100*nZoomX,300*nZoomY);
	m_GuiLineSeg2.SetStartXYEndXY(300*nZoomX,100*nZoomY,300*nZoomX,300*nZoomY);
	m_GuiLine1.SetXYRotation(300*nZoomX,200*nZoomY,sk1_PI);
	m_GuiLine2.SetXYRotation(200*nZoomX,100*nZoomY,sk1_PI);
	m_GuiCircle1.SetCenterRadius(250*nZoomX,150*nZoomY,50*nZoomX);
	m_GuiCircle1.SetCenterCrossExtent(sc2Vector(32*nZoomX,32*nZoomX));
	m_GuiCircle2.SetCenterRadius(350*nZoomX,250*nZoomY,50*nZoomX);
	m_GuiCircle2.SetCenterCrossExtent(sc2Vector(32*nZoomX,32*nZoomX));
	m_GuiEllipse.SetCenterXYRadiusXYRotation(300*nZoomX,100*nZoomY,50*nZoomX,100*nZoomY,0);
	m_GuiEllipse.SetCenterCrossExtent(sc2Vector(32*nZoomX,32*nZoomY));

}


void CDlgManualMeasure::UpdateMeasureElems(void)
{
	m_pGuiDisplay.ClearScreen();
	m_pGuiDisplay.RemoveImage();

	m_pInteractiveContainer = m_pGuiDisplay.GetInteractiveContainer();
	m_pStaticContainer = m_pGuiDisplay.GetStaticContainer();
	m_pInteractiveContainer->Clear();
	m_pStaticContainer->Clear();

	m_pGuiDisplay.SetImage(m_ImageInput);
	UpdateData(TRUE);
	switch (m_nRadDistanceMode)
	{
	case ePointToPointDistance:
		{
			m_GuiCross1.SetInteractive(TRUE);
			m_GuiCross2.SetInteractive(TRUE);
			m_GuiCross1.SetVisible(m_ImageInput.IsValid());	
			m_GuiCross2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiCross1,"Cross1");
			m_pInteractiveContainer->AddItem(&m_GuiCross2,"Cross2");
		}
		break;
	case ePointToLineSegDistance:
		{
			m_GuiCross1.SetInteractive(TRUE);	
			m_GuiLineSeg2.SetInteractive(TRUE);
			m_GuiCross1.SetVisible(m_ImageInput.IsValid());	
			m_GuiLineSeg2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiCross1,"Cross1");
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg2,"LineSeg2");
		}
		break;
	case ePointToLineDistance:
		{
			m_GuiCross1.SetInteractive(TRUE);	
			m_GuiLine2.SetInteractive(TRUE);		
			m_GuiCross1.SetVisible(m_ImageInput.IsValid());	
			m_GuiLine2.SetVisible(m_ImageInput.IsValid());		
			m_pInteractiveContainer->AddItem(&m_GuiCross1,"Cross1");
			m_pInteractiveContainer->AddItem(&m_GuiLine2,"Line2");
		}
		break;

	case ePointToCircleDistance:
		{
			m_GuiCross1.SetInteractive(TRUE);
			m_GuiCircle2.SetInteractive(TRUE);
			m_GuiCross1.SetVisible(m_ImageInput.IsValid());	
			m_GuiCircle2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiCross1,"Cross1");
			m_pInteractiveContainer->AddItem(&m_GuiCircle2,"Circle2");
		}
		break;

	case ePointToEllipseDistance:
		{
			m_GuiCross1.SetInteractive(TRUE);
			m_GuiEllipse.SetInteractive(TRUE);
			m_GuiCross1.SetVisible(m_ImageInput.IsValid());
			m_GuiEllipse.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiCross1,"Cross1");
			m_pInteractiveContainer->AddItem(&m_GuiEllipse,"Ellipse");
		}
		break;

	case eLineToCircleDistance:
		{
			m_GuiLine1.SetInteractive(TRUE);
			m_GuiCircle2.SetInteractive(TRUE);
			m_GuiLine1.SetVisible(m_ImageInput.IsValid());	
			m_GuiCircle2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLine1,"Line1");
			m_pInteractiveContainer->AddItem(&m_GuiCircle2,"Circle2");
		}
		break;

	case eLineToEllipseDistance:
		{
			m_GuiLine1.SetInteractive(TRUE);
			m_GuiEllipse.SetInteractive(TRUE);
			m_GuiLine1.SetVisible(m_ImageInput.IsValid());
			m_GuiEllipse.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLine1,"Line1");
			m_pInteractiveContainer->AddItem(&m_GuiEllipse,"Ellipse");
		}
		break;

	case eCircleToCircleDistance:
		{
			m_GuiCircle1.SetInteractive(TRUE);
			m_GuiCircle2.SetInteractive(TRUE);
			m_GuiCircle1.SetVisible(m_ImageInput.IsValid());
			m_GuiCircle2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiCircle1,"Circle1");
			m_pInteractiveContainer->AddItem(&m_GuiCircle2,"Circle2");
		}
		break;

	case eLineSegToLineSegDistance:
		{
			m_GuiLineSeg1.SetInteractive(TRUE);
			m_GuiLineSeg2.SetInteractive(TRUE);
			m_GuiLineSeg1.SetVisible(m_ImageInput.IsValid());
			m_GuiLineSeg2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg1,"LineSeg1");
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg2,"LineSeg2");
		}
		break;

	case eLineSegToLineDistance:
		{
			m_GuiLineSeg1.SetInteractive(TRUE);
			m_GuiLine2.SetInteractive(TRUE);
			m_GuiLineSeg1.SetVisible(m_ImageInput.IsValid());
			m_GuiLine2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg1,"LineSeg1");
			m_pInteractiveContainer->AddItem(&m_GuiLine2,"Line2");
		}
		break;

	case eLineSegToCircleDistance:
		{
			m_GuiLineSeg1.SetInteractive(TRUE);
			m_GuiCircle2.SetInteractive(TRUE);
			m_GuiLineSeg1.SetVisible(m_ImageInput.IsValid());
			m_GuiCircle2.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg1,"LineSeg1");
			m_pInteractiveContainer->AddItem(&m_GuiCircle2,"Circle2");
		}
		break;

	case eLineSegToEllipseDistance:
		{
			m_GuiLineSeg1.SetInteractive(TRUE);
			m_GuiEllipse.SetInteractive(TRUE);
			m_GuiLineSeg1.SetVisible(m_ImageInput.IsValid());
			m_GuiEllipse.SetVisible(m_ImageInput.IsValid());
			m_pInteractiveContainer->AddItem(&m_GuiLineSeg1,"LineSeg1");
			m_pInteractiveContainer->AddItem(&m_GuiEllipse,"Ellipse");
		}
		break;

	default:
		break;
	}
	m_pGuiDisplay.SetInteractiveContainer(m_pInteractiveContainer);
	m_pGuiDisplay.SetStaticContainer(m_pStaticContainer);
	m_pGuiDisplay.MyInvalidate();

}

void CDlgManualMeasure::UpdateDisplay(void)
{
	UpdateMeasureElems();
	int nModelImg = m_comboDisplayImage.GetCurSel();
	switch(nModelImg)
	{
	case 1:
		{
			m_GuiStaticLineSeg.SetVisible(m_ImageInput.IsValid() && m_bResult);	
			m_pStaticContainer->AddItem(&m_GuiStaticLineSeg,"Result");

			m_GuiStaticCircle1.SetVisible(m_ImageInput.IsValid() && m_bResult);	
			m_pStaticContainer->AddItem(&m_GuiStaticCircle1,"ResultCircle1");

			m_GuiStaticCircle2.SetVisible(m_ImageInput.IsValid() && m_bResult);	
			m_pStaticContainer->AddItem(&m_GuiStaticCircle2,"ResultCircle2");

			switch(m_nRadDistanceMode)
			{
			case ePointToPointDistance:
				{
					m_GuiCross1.SetInteractive(FALSE);
					m_GuiCross2.SetInteractive(FALSE);
				}
				break;
			case ePointToLineSegDistance:
				{
					m_GuiCross1.SetInteractive(FALSE);
					m_GuiLineSeg2.SetInteractive(FALSE);
				}
				break;
			case ePointToLineDistance:
				{
					m_GuiCross1.SetInteractive(FALSE);
					m_GuiLine2.SetInteractive(FALSE);
				}
				break;

			case ePointToCircleDistance:
				{
					m_GuiCross1.SetInteractive(FALSE);
					m_GuiCircle2.SetInteractive(FALSE);
				}
				break;

			case ePointToEllipseDistance:
				{
					m_GuiCross1.SetInteractive(FALSE);
					m_GuiEllipse.SetInteractive(FALSE);
				}
				break;
			case eLineToCircleDistance:
				{
					m_GuiLine1.SetInteractive(FALSE);
					m_GuiCircle2.SetInteractive(FALSE);
				}
				break;
			case eLineToEllipseDistance:
				{
					m_GuiLine1.SetInteractive(FALSE);
					m_GuiEllipse.SetInteractive(FALSE);
				}
				break;
			case eCircleToCircleDistance:
				{
					m_GuiCircle1.SetInteractive(FALSE);
					m_GuiCircle2.SetInteractive(FALSE);
				}
				break;
			case eLineSegToLineSegDistance:
				{
					m_GuiLineSeg1.SetInteractive(FALSE);
					m_GuiLineSeg2.SetInteractive(FALSE);
				}
				break;
			case eLineSegToLineDistance:
				{
					m_GuiLineSeg1.SetInteractive(FALSE);
					m_GuiLine2.SetInteractive(FALSE);
				}
				break;
			case eLineSegToCircleDistance:
				{
					m_GuiLineSeg1.SetInteractive(FALSE);
					m_GuiCircle2.SetInteractive(FALSE);
				}
				break;
			case eLineSegToEllipseDistance:
				{
					m_GuiLineSeg1.SetInteractive(FALSE);
					m_GuiEllipse.SetInteractive(FALSE);
				}
				break;
			default:
				break;

			}
			
		}
		break;

	default:
		break;
	}

	m_pGuiDisplay.SetInteractiveContainer(m_pInteractiveContainer);
	m_pGuiDisplay.SetStaticContainer(m_pStaticContainer);
	m_pGuiDisplay.MyInvalidate();

}

void CDlgManualMeasure::UpdateResult(double dDistance, double dXDistance,double dYDistance)
{
	if ((dDistance<-EPSINON) || (dDistance>EPSINON))
	{
		//两点不重合
		if (m_bPixelValueMode)
		{
			m_strActualDistance.Format("%.3f",_tstof(m_strCalibValue)*dDistance);
			m_strActualXDistance.Format("%.3f",_tstof(m_strCalibValue)*dXDistance);
			m_strActualYDistance.Format("%.3f",_tstof(m_strCalibValue)*dYDistance);

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);
		}
		else
		{
			m_strActualDistance.Format("%.3f",dDistance*_tstof(m_strInputValue));
			m_strActualXDistance.Format("%.3f",dXDistance*_tstof(m_strInputValue));
			m_strActualYDistance.Format("%.3f",dYDistance*_tstof(m_strInputValue));

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);

		}
		m_GuiStaticLineSeg.SetStartXYEndXY(m_GuiCross1.GetCenter(),m_GuiCross2.GetCenter());

		m_GuiStaticCircle1.SetCenterRadius(m_GuiCross1.GetCenter(),2);
		m_GuiStaticCircle2.SetCenterRadius(m_GuiCross2.GetCenter(),2);
		m_bResult = TRUE;
	}
	else
	{
		//两点重合
		m_strActualDistance.Format("0.0");
		m_strActualXDistance.Format("0.0");
		m_strActualYDistance.Format("0.0");
		m_strDistance.Format("0.0");
		m_strXDistance.Format("0.0");
		m_strYDistance.Format("0.0");
		m_bResult = FALSE;
	}
	UpdateData(FALSE);
}

void CDlgManualMeasure::UpdateResult(double dDistance,sc2Vector Vector1, double dXDistance,double dYDistance)
{
	int nTmp = 0;
	if ((dDistance<-EPSINON) || (dDistance>EPSINON))
	{
		//点与另一图形相离
		if (m_bPixelValueMode)
		{
			m_strActualDistance.Format("%.3f",dDistance*_tstof(m_strCalibValue));
			m_strActualXDistance.Format("%.3f",dXDistance*_tstof(m_strCalibValue));
			m_strActualYDistance.Format("%.3f",dYDistance*_tstof(m_strCalibValue));

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);
		}
		else
		{
			m_strActualDistance.Format("%.3f",dDistance*_tstof(m_strInputValue));
			m_strActualXDistance.Format("%.3f",dXDistance*_tstof(m_strInputValue));
			m_strActualYDistance.Format("%.3f",dYDistance*_tstof(m_strInputValue));

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);
		}

		m_GuiStaticLineSeg.SetStartXYEndXY(m_GuiCross1.GetCenter(),Vector1);

		m_GuiStaticCircle1.SetCenterRadius(m_GuiCross1.GetCenter(),2);
		m_GuiStaticCircle2.SetCenterRadius(Vector1,2);
		m_bResult = TRUE;
	}
	else
	{
		m_strActualDistance.Format("0.0");
		m_strActualXDistance.Format("0.0");
		m_strActualYDistance.Format("0.0");
		m_strDistance.Format("0.0");
		m_strXDistance.Format("0.0");
		m_strYDistance.Format("0.0");
		m_bResult = FALSE;
	}
	UpdateData(FALSE);
}

void CDlgManualMeasure::UpdateResult(double dDistance,sc2Vector Vector1,sc2Vector Vector2, double dXDistance,double dYDistance)
{
	if ((dDistance<-EPSINON) || (dDistance>EPSINON))
	{
		//两图形相离
		if (m_bPixelValueMode)
		{
			m_strActualDistance.Format("%.3f",dDistance*_tstof(m_strCalibValue));
			m_strActualXDistance.Format("%.3f",dXDistance*_tstof(m_strCalibValue));
			m_strActualYDistance.Format("%.3f",dYDistance*_tstof(m_strCalibValue));

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);
		}
		else
		{
			m_strActualDistance.Format("%.3f",_tstof(m_strInputValue)*dDistance);
			m_strActualXDistance.Format("%.3f",_tstof(m_strInputValue)*dXDistance);
			m_strActualYDistance.Format("%.3f",_tstof(m_strInputValue)*dYDistance);

			m_strDistance.Format("%.3f",dDistance);
			m_strXDistance.Format("%.3f",dXDistance);
			m_strYDistance.Format("%.3f",dYDistance);
		}

		m_GuiStaticLineSeg.SetStartXYEndXY(Vector1,Vector2);
		m_GuiStaticCircle1.SetCenterRadius(Vector1,2);
		m_GuiStaticCircle2.SetCenterRadius(Vector2,2);
		m_bResult = TRUE;
	}
	else
	{
		//两图形相交
		m_strActualDistance.Format("0.0");
		m_strActualXDistance.Format("0.0");
		m_strActualYDistance.Format("0.0");
		m_strDistance.Format("0.0");
		m_strXDistance.Format("0.0");
		m_strYDistance.Format("0.0");
		m_bResult = FALSE;
	}
	UpdateData(FALSE);
}


LRESULT CDlgManualMeasure::OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam)
{
	CString	strStatus = " ";

	if (wParam == m_pGuiDisplay.GetID())
	{
		if (m_pGuiDisplay.GetImageCP() != NULL)
		{
			if (m_pGuiDisplay.GetImageCP()->IsValid())
			{
				long nWidth = m_pGuiDisplay.GetImageCP()->Width();
				long nHeight = m_pGuiDisplay.GetImageCP()->Height();

				//获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标，低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX,dImageY;
				m_pGuiDisplay.TransformPos(x,y,dImageX,dImageY);

				// 获取对应像素灰度值
				BYTE R,G,B;
				m_pGuiDisplay.GetImagePixelValue(LONG(dImageX),LONG(dImageY),R,G,B);
				if (dImageX<0 || dImageY<0 || dImageX>=nWidth || dImageY>=nHeight)
				{
					R = 0;
					G = 0;
					B = 0;
				}
				strStatus.Format("(%.0f,%.0f) (%d,%d,%d)",dImageX,dImageY,R,G,B);
			}
		}
	}

	m_strStatusBarInfo2 = strStatus;
	UpdateStatusBar();

	return 0;
}


void CDlgManualMeasure::OnBnClickedMeasureRadioChange()
{
	// TODO: 在此添加控件通知处理程序代码
	m_comboDisplayImage.SetCurSel(0);
	GetDlgItem(IDC_EDIT_DISTANCE)->SetWindowText(_T("0.0"));
	GetDlgItem(IDC_EDIT_ACTUAL_DIS)->SetWindowText(_T("0.0"));
	UpdateDisplay();
}


void CDlgManualMeasure::OnCameraChange()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nCameraIndex == m_nPrevCameraIndex)
	{
		return;
	}

	m_pVisionASM->FreezePosCamera(m_nPrevCameraIndex);
	m_pVisionASM->SetPosDisplay(m_nPrevCameraIndex, NULL);
	m_pGuiDisplay.ClearScreen();
	m_pGuiDisplay.RemoveImage();

	GetCalibratedInfoToMeasure();
	if (m_pVisionASM->IsPosCameraOnline(m_nCameraIndex) && !m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		m_pVisionASM->SetPosDisplay(m_nCameraIndex, &m_pGuiDisplay);
		m_pVisionASM->GrabPosCamera(m_nCameraIndex);
		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING);	//_T("正在连续采集")
		m_strStatusBarInfo2 = _T("");
		UpdateStatusBar();
	}
    CString strLog;
    strLog.Format(_T("修改[位置%d]为[位置%d]\n"),m_nPrevCameraIndex,m_nCameraIndex);
    g_opLogFile.SaveLogText(strLog,4);
	m_nPrevCameraIndex = m_nCameraIndex;

	UpdateBtnDisplay();
}

void CDlgManualMeasure::GetCalibratedInfoToMeasure()
{
	if(m_pVisionASM != NULL)
	{
		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pVisionASM->GetTotalCalibData();
		if (pTotalCalibData != NULL)
		{
			// 获取当前产品对应的标定数据
			CCalibData* pCalibData;
			pCalibData = pTotalCalibData->GetItemPtr(pTotalCalibData->GetCurCalibIndex());
			if (pCalibData != NULL)
			{
				// 获取当前位置对应的标定结果
				CCalibratedInfo* pCalibratedInfo;
				if(m_nCameraIndex>=0 && m_nCameraIndex<pCalibData->m_vpCalibratedInfo.size())
				{
					pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(m_nCameraIndex);

					if(pCalibratedInfo != NULL)
					{
						double h[9];
						memset(h, 0.0, sizeof(double)*9);
						h[0] = 1.0;
						h[4] = 1.0;
						h[8] = 1.0;
						pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(m_nCameraIndex);
						pCalibratedInfo->GetPlatformTransferH(h);
						m_strCalibValue.Format(_T("%.6f "), h[0]);
						UpdateData(FALSE);
					}
				}
			}
		}
	}
}

void CDlgManualMeasure::OnBnClickedRadioInputValue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ShowControl();
}

void CDlgManualMeasure::ShowControl()
{
	GetDlgItem(IDC_STATIC_CALIB_RESULT)->ShowWindow(m_bPixelValueMode);
	GetDlgItem(IDC_EDIT_CALIB_VALUE)->ShowWindow(m_bPixelValueMode);
	GetDlgItem(IDC_STATIC_CALIB_UNIT)->ShowWindow(m_bPixelValueMode);
	GetDlgItem(IDC_STATIC_INPUT_UNIT)->ShowWindow(!m_bPixelValueMode);
	GetDlgItem(IDC_EDIT_INPUT_VALUE)->ShowWindow(!m_bPixelValueMode);
	GetDlgItem(IDC_STATIC_INPUT_VALUE)->ShowWindow(!m_bPixelValueMode);
}