// DlgDistorCorrImage.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgDistorCorrImage.h"
#include "afxdialogex.h"


// CDlgDistorCorrImage 对话框

IMPLEMENT_DYNAMIC(CDlgDistorCorrImage, CDialog)

CDlgDistorCorrImage::CDlgDistorCorrImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDistorCorrImage::IDD, pParent)
	, m_dTileX(1)
	, m_dTileY(1)
	, m_nThre(16)
	, m_nCurIndex(0)
	, m_bOutputOutpoints(FALSE)
	, m_bKillFocusEnabled(TRUE)
	//, m_StatusBar(NULL)
	, m_pGuiCrossArray(NULL)
	, m_pImageWarpTool(NULL)
	, m_nCurExProductIndex(0)
{
	m_nPosNum = MAXPOSNUM;
	m_vbCopyPos.resize(MAXPOSNUM, FALSE);
	m_bTargetObjectCamSeparate = 0;
	m_bCopyResult = false;
}

CDlgDistorCorrImage::~CDlgDistorCorrImage()
{
}

void CDlgDistorCorrImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(	pDX,	IDC_EDIT_TILE_X		, m_dTileX);
	DDX_Text(	pDX,	IDC_EDIT_TILE_Y		, m_dTileY);
	DDX_Text(	pDX,	IDC_EDIT_THRE		, m_nThre);
	DDX_Check(	pDX,	IDC_CHECK_OUTPUT_OUTPOINTS, m_bOutputOutpoints);
	DDX_Control(pDX,	IDC_COMBO_IMAGE_LIST, m_ComboImageList);
	DDX_Control(pDX,	IDC_COMBO_MODE_LIST	, m_ComboModeList);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
	DDX_CBIndex(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_nCurExProductIndex);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX1, m_checkPos1);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX2, m_checkPos2);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX3, m_checkPos3);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX4, m_checkPos4);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX5, m_checkPos5);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX6, m_checkPos6);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX7, m_checkPos7);
	DDX_Control(pDX, IDC_CHECK_COPY_INDEX8, m_checkPos8);
}


BEGIN_MESSAGE_MAP(CDlgDistorCorrImage, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SV_DISPLAY_MOUSEMOVE,OnGuiDisplayMouseMove)
	ON_EN_SETFOCUS(IDC_EDIT_THRE,		&CDlgDistorCorrImage::OnEnSetfocusEditThre)
	ON_EN_SETFOCUS(IDC_EDIT_TILE_X,		&CDlgDistorCorrImage::OnEnSetfocusEditTileX)
	ON_EN_SETFOCUS(IDC_EDIT_TILE_Y,		&CDlgDistorCorrImage::OnEnSetfocusEditTileY)

	ON_EN_KILLFOCUS(IDC_EDIT_TILE_X,	&CDlgDistorCorrImage::OnEnKillfocusEditTileX)
	ON_EN_KILLFOCUS(IDC_EDIT_TILE_Y,	&CDlgDistorCorrImage::OnEnKillfocusEditTileY)
	ON_EN_KILLFOCUS(IDC_EDIT_THRE,		&CDlgDistorCorrImage::OnEnKillfocusEditThre)

	ON_BN_CLICKED(IDC_OK,				&CDlgDistorCorrImage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL,			&CDlgDistorCorrImage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_GRAB_IMAGE,	&CDlgDistorCorrImage::OnBnClickedBtnGrabImage)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE,	&CDlgDistorCorrImage::OnBnClickedBtnLoadImage)
	ON_BN_CLICKED(IDC_BTN_CALIBRATE,	&CDlgDistorCorrImage::OnBnClickedBtnCalibrate)

	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_LIST, &CDlgDistorCorrImage::OnCbnSelchangeComboImageList)
	ON_BN_CLICKED(ID_BUTTON_IMAGE_WARP, &CDlgDistorCorrImage::OnBnClickedButtonImageWarp)
END_MESSAGE_MAP()


// CDlgDistorCorrImage 消息处理程序
BOOL CDlgDistorCorrImage::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_GuiDisplay.Create(IDC_STATIC_CH0_DISPLAY, this);
	m_GuiDisplay.RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DBLCLICK);
	if(m_pVisionASM != NULL)
	{
		m_pVisionASM->SetPosDisplay(m_nCurIndex, &m_GuiDisplay);
	}
	m_GuiDisplay.ShowWindow(SW_SHOW);

	m_vpCheckPos.push_back(&m_checkPos1);
	m_vpCheckPos.push_back(&m_checkPos2);
	m_vpCheckPos.push_back(&m_checkPos3);
	m_vpCheckPos.push_back(&m_checkPos4);
	m_vpCheckPos.push_back(&m_checkPos5);
	m_vpCheckPos.push_back(&m_checkPos6);
	m_vpCheckPos.push_back(&m_checkPos7);
	m_vpCheckPos.push_back(&m_checkPos8);

	int i = 0;
	for (i=0; i<MAXPOSNUM; i++)
	{
		if (i < m_nPosNum)
		{	
			m_vpCheckPos.at(i)->ShowWindow(SW_SHOW);
			if (m_bCopyResult)
			{
				if (!m_bTargetObjectCamSeparate)//勾选所有
				{
					m_vpCheckPos.at(i)->SetCheck(TRUE);
				}else
				{
					//勾选对象的
					if ( m_nCurIndex < m_nPosNum/2 && i < m_nPosNum/2)
					{
						m_vpCheckPos.at(i)->SetCheck(TRUE);
					}
					//勾选目标的
					if (m_nCurIndex >= m_nPosNum/2 && i >= m_nPosNum/2)
					{
						m_vpCheckPos.at(i)->SetCheck(TRUE);
					}
				}
			}
		}
		else
		{
			m_vpCheckPos.at(i)->ShowWindow(SW_HIDE);
		}
	}


	InitDlgItem();
	UpdateDialogLanguage();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDlgDistorCorrImage::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_DISTORCORR_IMAGE") == strDlgID)
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

void CDlgDistorCorrImage::InitDlgItem()
{
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[4] = {20, 150, 330, -1};
	m_pStatusBar->SetParts(4, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();



	m_ComboModeList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CHECKER_COLLECT));
	m_ComboModeList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_GRID_COLLECT));
	m_ComboModeList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CIRCLE_CENTRE_COLLECT));
	m_ComboModeList.SetCurSel(0);

	m_ComboImageList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_INPUT_IMAGE));
	m_ComboImageList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TRAIN_IMAGE));
	m_ComboImageList.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RESULT_IMAGE));
	m_ComboImageList.SetCurSel(0);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		for (int i=0; i<sysPlatformInfo.m_nMultiCalibExtensionMaxNum+1; i++)
		{
			CString strExProductIndex;
			strExProductIndex.Format(_T("%d"), i+1);
			m_comboExProductIndex.InsertString(i, strExProductIndex);
		}
		m_comboExProductIndex.SetCurSel(0);

		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EXPRODUCT_TIP)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EXPRODUCT_TIP)->ShowWindow(SW_HIDE);
	}
}

void CDlgDistorCorrImage::OnBnClickedBtnLoadImage()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"位图文件（*.bmp）| *.bmp||");*/
	CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_AND_JPEG_FILE);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	// 选择要显示的图像（BMP）文件
	if(dlg.DoModal() == IDOK)
	{
		/*scDIB dib;

		dib.Init(dlg.GetPathName());

		dib.GetImage(m_ImageInPut, 1);*/
		CString strPath = dlg.GetPathName();
		if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(strPath);
			cDib.GetImage(m_ImageInPut);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(strPath,m_ImageInPut);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(strPath,m_ImageInPut);
		}
		m_ComboImageList.SetCurSel(0);
		UpdateDisplay();
	}
}


void CDlgDistorCorrImage::OnBnClickedBtnCalibrate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_imgcorners.clear();
	m_objcorners.clear();
	m_ImageResult.Release();
	UpdateData(TRUE);
	scTimer timer;
	double time = 0;
	
	if (!m_ImageInPut.IsValid())
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_EMPTY_IMAGE),2,0); 
		
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_EMPTY_IMAGE);
		UpdateStatusBar();
		
		return;
	}
	
	if (m_dTileX <= 0 || m_dTileY <= 0 )
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_DATA_SET_ERROR),2,0); 
		
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_DATA_SET_ERROR);
		UpdateStatusBar();

		return;
	}

	long lWidth = m_ImageInPut.Width();
	long lHeight = m_ImageInPut.Height();
	m_ImagePoint = m_ImageInPut;


	scFiducialParam param;
	param.m_bFiducial = FALSE;
	param.m_bCenter = FALSE;

	if (m_pImageWarpTool != NULL)
	{
		delete m_pImageWarpTool;
		m_pImageWarpTool = NULL;
	}

	scCalibrateResult result(scCalibrateResult::eNonlinearProject);

	bool bSuccess =false;
	timer.Reset();
	int nCurSel = m_ComboModeList.GetCurSel();
	if (m_ImageInPut.PixelSize() != 1)// 若是彩色图像，则转换成灰度图像，因为二维码靶标基本都是黑白的，无需使用彩色
	{
		cpImage tmpImage;
		m_ImageInPut.GetGrayImage(tmpImage);
		if(0 == nCurSel)
		{
			bSuccess = sfCalibtareQuad(tmpImage,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,m_nThre,m_bOutputOutpoints,param);
		}
		else if(1 == nCurSel)
		{
			bSuccess = sfCalibtareGridLine(tmpImage,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
		}
		else if(2 == nCurSel)
		{
			bSuccess = sfCalibtareDotTarget(tmpImage,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
		}
	}
	else
	{
		if(0 == nCurSel)
		{
			bSuccess = sfCalibtareQuad(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,m_nThre,m_bOutputOutpoints,param);
		}
		else if(1 == nCurSel)
		{
			bSuccess = sfCalibtareGridLine(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
		}
		else if(2 == nCurSel)
		{
			bSuccess = sfCalibtareDotTarget(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
		}
	}

	//if(0 == nCurSel)
	//{
	//	bSuccess = sfCalibtareQuad(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,m_nThre,m_bOutputOutpoints,param);
	//}
	//else if(1 == nCurSel)
	//{
	//	bSuccess = sfCalibtareGridLine(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
	//}
	//else if(2 == nCurSel)
	//{
	//	bSuccess = sfCalibtareDotTarget(m_ImageInPut,m_dTileX,m_dTileY,result,m_imgcorners,m_objcorners,param);
	//}

	if (!bSuccess)
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,	IDS_CALIBRATE_FAILE),2,0); 
		
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = _T("");
		UpdateStatusBar();

		return;
	}
	m_pImageWarpTool = new scImageWarp(result, sc2Vector(lWidth,lHeight));

	m_pImageWarpTool->ExecuteProjectAndLensWarp(m_ImageInPut, m_ImageResult, false);

	time = timer.GetTimeMilli(TRUE);
	CString strtime;
	strtime.Format("校正耗时：%.2f ms",time);
	
	//m_StatusBar->SetIcon(0,m_hIcon1);
	//m_StatusBar->SetText(strtime,1,0); 
	//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,	IDS_CALIBRATE_SUCCESS),2,0); 
	
	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS);
	m_strStatusBarInfo2 = strtime;
	UpdateStatusBar();
	
	m_result = result;
	m_ComboImageList.SetCurSel(1);
	UpdateDisplay();
	UpdateResultDate();
	return;

}

void CDlgDistorCorrImage::UpdateResultDate()
{
	CString str;
	str.Format("%.4f",m_result.GetCoefficientXX());
	GetDlgItem(IDC_EDIT_XX)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientXY());
	GetDlgItem(IDC_EDIT_XY)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientXC());
	GetDlgItem(IDC_EDIT_XC)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientYX());
	GetDlgItem(IDC_EDIT_YX)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientYY());
	GetDlgItem(IDC_EDIT_YY)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientYC());
	GetDlgItem(IDC_EDIT_YC)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientDX());
	GetDlgItem(IDC_EDIT_DX)->SetWindowText(str);

	str.Format("%.4f",m_result.GetCoefficientDY());
	GetDlgItem(IDC_EDIT_DY)->SetWindowText(str);
	
	str.Format("%.5f",m_result.GetCoefficientK());
	GetDlgItem(IDC_EDIT_K)->SetWindowText(str);

	str.Format("%.4f",m_result.GetTranslationX());
	GetDlgItem(IDC_EDIT_TX)->SetWindowText(str);

	str.Format("%.4f",m_result.GetTranslationY());
	GetDlgItem(IDC_EDIT_TY)->SetWindowText(str);

	str.Format("%.4f",m_result.GetScalingX());
	GetDlgItem(IDC_EDIT_SX)->SetWindowText(str);

	str.Format("%.4f",m_result.GetScalingY());
	GetDlgItem(IDC_EDIT_SY)->SetWindowText(str);

	str.Format("%.4f",scDegree(m_result.GetRotate()).ToDouble());
	GetDlgItem(IDC_EDIT_ROTATE)->SetWindowText(str);

	str.Format("%.4f",scDegree(m_result.GetSkew()).ToDouble());
	GetDlgItem(IDC_EDIT_SKEW)->SetWindowText(str);

	str.Format("%.2f",m_result.GetRMS());
	GetDlgItem(IDC_EDIT_RMS)->SetWindowText(str);
}

void CDlgDistorCorrImage::OnEnSetfocusEditTileX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(this->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TILE_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TILE_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TILE_X, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditTileX();
	}	
}



void CDlgDistorCorrImage::OnEnSetfocusEditTileY()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(this->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TILE_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TILE_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TILE_Y, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditTileY();
	}
}

void CDlgDistorCorrImage::OnEnSetfocusEditThre()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(this->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_THRE, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditThre();
	}
}

void CDlgDistorCorrImage::OnEnKillfocusEditTileX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		CString str;
		GetDlgItemText(IDC_EDIT_TILE_X, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			str.Format(_T("%.4f"),m_dTileX);
			SetDlgItemText(IDC_EDIT_TILE_X, str);
		}
		else
		{
			str.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TILE_X, str);
			UpdateData(TRUE);
		}
	}
}


void CDlgDistorCorrImage::OnEnKillfocusEditTileY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		CString str;
		GetDlgItemText(IDC_EDIT_TILE_Y, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			str.Format(_T("%.4f"),m_dTileY);
			SetDlgItemText(IDC_EDIT_TILE_Y, str);
		}
		else
		{
			str.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TILE_Y, str);
			UpdateData(TRUE);
		}
	}
}


void CDlgDistorCorrImage::OnEnKillfocusEditThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		CString str;
		GetDlgItemText(IDC_EDIT_THRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			str.Format(_T("%d"),m_nThre);
			SetDlgItemText(IDC_EDIT_THRE, str);
		}
		else
		{
			str.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_THRE, str);
			UpdateData(TRUE);
		}
	}
}

BOOL CDlgDistorCorrImage::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgDistorCorrImage::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
			if ((i == 0) && (ch == '+'))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgDistorCorrImage::UpdateDisplay(void)
{
	m_GuiDisplay.ClearScreen();

	m_pInteractiveContainer = m_GuiDisplay.GetInteractiveContainer();
	m_pStaticContainer = m_GuiDisplay.GetStaticContainer();

	int nModeImg = m_ComboImageList.GetCurSel();

	switch (nModeImg)
	{
	case  0:
		m_GuiDisplay.SetImage(m_ImageInPut);

		break;
	case  1:
		m_GuiDisplay.SetImage(m_ImagePoint);
		if (m_pGuiCrossArray != NULL)
		{
			delete [] m_pGuiCrossArray;
			m_pGuiCrossArray = NULL;
		}

		if ( (m_imgcorners.size() > 0) && (m_objcorners.size() > 0) )
		{
			long nNum = m_imgcorners.size();

			m_pGuiCrossArray = new scGuiCross[nNum];

			long i=0;
			CString str;
			for (i=0; i<nNum; i++)
			{
				m_pGuiCrossArray[i].SetCenterXYWidthHeight(m_imgcorners[i].GetX(), m_imgcorners[i].GetY());
				m_pGuiCrossArray[i].SetVisible(TRUE);

				str.Format("Cross%d",i);
				m_pStaticContainer->AddItem(m_pGuiCrossArray+i, str);
			}
		}

		break;
	case  2:
		m_GuiDisplay.SetImage(m_ImageResult);
		break;
	default:
		break;
	}

	m_GuiDisplay.SetInteractiveContainer(m_pInteractiveContainer);
	m_GuiDisplay.SetStaticContainer(m_pStaticContainer);
	m_GuiDisplay.MyInvalidate();
}

LRESULT CDlgDistorCorrImage::OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam)
{
	CString strStatus = " ";

	if (wParam == m_GuiDisplay.GetID())
	{
		if (m_GuiDisplay.GetImageCP() != NULL)
		{
			if (m_GuiDisplay.GetImageCP()->IsValid())
			{
				long nWidth  = m_GuiDisplay.GetImageCP()->Width();
				long nHeight = m_GuiDisplay.GetImageCP()->Height();

				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_GuiDisplay.TransformPos(x,y,dImageX,dImageY);

				// 获取对应像素灰度值
				BYTE R,G,B;
				m_GuiDisplay.GetImagePixelValue(LONG(dImageX),LONG(dImageY),R,G,B);
				if(dImageX<0||dImageY<0||dImageY>=nHeight||dImageX>=nWidth)
				{
					R = 0;
					G = 0;
					B = 0;
				}

				strStatus.Format("(%.0f,%.0f)  (%d,%d,%d)", dImageX, dImageY, R, G, B);
			}
		}	
	}
	m_pStatusBar->SetText(strStatus,3,0);

	return 0 ;
}

int CDlgDistorCorrImage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//m_StatusBar = new CStatusBarCtrl;
	//RECT m_Rect;  
	//GetClientRect(&m_Rect);         //获取对话框的矩形区域
	//m_Rect.top=m_Rect.bottom-20;    //设置状态栏的矩形区域
	//m_StatusBar->Create(WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,IDC_STATUS_BAR);  

	//int nParts[4]= {20, 150, 330, -1};      //分割尺寸
	//m_StatusBar->SetParts(4, nParts);       //分割状态栏

	//m_hIcon1   =(HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,0);   
	//m_hIcon2   =(HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON,16,16,0);   

	//m_StatusBar->SetIcon(0,m_hIcon1);
	//m_StatusBar->SetText(" ",1,0); 
	//m_StatusBar->SetText(" ",2,0); 
	//m_StatusBar->SetText(" ",3,0); 
	//m_StatusBar->ShowWindow(SW_SHOW); 

	return 0;
}

void CDlgDistorCorrImage::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//if (m_StatusBar != NULL)
	//{
	//	delete m_StatusBar;
	//	m_StatusBar = NULL;
	//}
	if (m_pGuiCrossArray != NULL)
	{
		delete [] m_pGuiCrossArray;
		m_pGuiCrossArray = NULL;
	}
	if (m_pImageWarpTool != NULL)
	{
		delete m_pImageWarpTool;
		m_pImageWarpTool = NULL;
	}

	// 状态栏
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
		if (m_pVisionASM->IsPosCameraOnline(m_nCurIndex))
		{
			m_pVisionASM->FreezePosCamera(m_nCurIndex);
		}
		//m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
		m_pVisionASM->SetPosDisplay(m_nCurIndex, m_pPrevGuiDisplay);
	}
}

void CDlgDistorCorrImage::OnCbnSelchangeComboImageList()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDisplay();
}

void CDlgDistorCorrImage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(0.0 == m_result.GetRMS())
	{
		if(IDNO == AfxMessageBox(_T("未进行畸变校正，确定要保存数据？"),MB_YESNO))
		{
			return;
		}
	}
	UpdateData(TRUE);
	int i = 0;
	for (i=0; i<MAXPOSNUM && i<m_vpCheckPos.size(); i++)
	{
		if (m_vpCheckPos.at(i)->GetCheck() == BST_CHECKED)
		{
			m_vbCopyPos.at(i) = TRUE;
		}
		else
		{
			m_vbCopyPos.at(i) = FALSE;
		}
	}
	CDialog::OnOK();
}


void CDlgDistorCorrImage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CDlgDistorCorrImage::OnBnClickedBtnGrabImage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nCurIndex))
		{
			m_ComboImageList.SetCurSel(0);
			if (m_pVisionASM->IsPosCameraGrabbing(m_nCurIndex))
			{
				if (m_pVisionASM->FreezePosCamera(m_nCurIndex))
				{
					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
					//SetDlgItemText(IDC_BTN_GRAB_IMAGE,_T("采集图像"));
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_CALIBRATE)->EnableWindow(TRUE);
					GetDlgItem(IDC_OK)->EnableWindow(TRUE);
					cpImage* pImageTemp = (cpImage*)m_GuiDisplay.GetImageCP();
					if (pImageTemp != NULL)
					{
						pImageTemp->Clone(m_ImageInPut);	
					}
					UpdateDisplay();
					//m_StatusBar->SetIcon(0,m_hIcon1);
					//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,	IDS_SB_GRAB_STOPPED),2,0); 

					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_STOPPED);
					m_strStatusBarInfo2 = _T("");
					UpdateStatusBar();
				}
				else
				{
					//m_StatusBar->SetIcon(0,m_hIcon2);
					//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,	IDS_MB_FAIL_TO_STOP_GRAB),2,0);

					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_STOP_GRAB);
					m_strStatusBarInfo2 = _T("");
					UpdateStatusBar();
				}
			}
			else
			{

				if (m_pVisionASM->GrabPosCamera(m_nCurIndex))
				{
					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
					//SetDlgItemText(IDC_BTN_GRAB_IMAGE,_T("停止采集"));
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_CALIBRATE)->EnableWindow(FALSE);
					GetDlgItem(IDC_OK)->EnableWindow(FALSE);
					//m_StatusBar->SetIcon(0,m_hIcon1);
					//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING),2,0); 
					UpdateDisplay();
					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING);
					m_strStatusBarInfo2 = _T("");
					UpdateStatusBar();
				}
				else
				{
					//m_StatusBar->SetIcon(0,m_hIcon2);
					//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_START_GRAB),2,0); 

					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_START_GRAB);
					m_strStatusBarInfo2 = _T("");
					UpdateStatusBar();
				}
			}		
		}
		else
		{
			//m_StatusBar->SetIcon(0,m_hIcon2);
			//m_StatusBar->SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_CONNECTION_ERROR),2,0); 

			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_CONNECTION_ERROR);
			m_strStatusBarInfo2 = _T("");
			UpdateStatusBar();
		}
	}
}

void CDlgDistorCorrImage::SetVisionASMRef(vcBaseVisionAlign *pVisionASM, int nPositionIdx)
{
	m_pVisionASM = pVisionASM;
	// 设置新视觉对位工具的主窗口
	if (m_pVisionASM != NULL)
	{
		m_pPrevGuiDisplay = m_pVisionASM->GetPosDisplay(nPositionIdx);
		m_pVisionASM->SetPosDisplay(nPositionIdx, &m_GuiDisplay);
		m_nCurIndex = nPositionIdx;
	}
}

void CDlgDistorCorrImage::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgDistorCorrImage::OnBnClickedButtonImageWarp()
{
	if (!m_ImageInPut.IsValid())
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText("图像数据为空!",2,0); 

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_EMPTY_IMAGE);
		UpdateStatusBar();

		return;
	}

	if (m_pImageWarpTool == NULL)
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText("未找到标定结果!",2,0); 

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = _T("未找到标定结果");
		UpdateStatusBar();

		return;
	}

	long lWidth  = (long)floor(m_pImageWarpTool->GetImageSize().GetX() + 0.5);
	long lHeight = (long)floor(m_pImageWarpTool->GetImageSize().GetY() + 0.5);
	if ( (m_ImageInPut.Width() != lWidth) || (m_ImageInPut.Height() != lHeight) )
	{
		//m_StatusBar->SetIcon(0,m_hIcon2);
		//m_StatusBar->SetText("图像尺寸与标定结果不符!",2,0); 

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = _T("图像尺寸与标定结果不符");
		UpdateStatusBar();

		return;
	}

	scCalibrateResult::CalibrateMode mode = m_pImageWarpTool->GetCalibrateResult().GetCalibrateMode();
	if (mode != scCalibrateResult::eNonlinearProject)	// 非线性标定
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);
		m_strStatusBarInfo2 = _T("标定模式不等于非线性标定");
		UpdateStatusBar();

		return;
	}

	scTimer timer;
	double time = 0;

	timer.Reset();
	m_pImageWarpTool->ExecuteProjectAndLensWarp(m_ImageInPut, m_ImageResult, false);
	time = timer.GetTimeMilli(TRUE);

	CString strtime;
	strtime.Format("校正耗时：%.2f ms",time);

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("校正成功");;
	m_strStatusBarInfo2 = strtime;
	UpdateStatusBar();

	m_ComboImageList.SetCurSel(2);
	UpdateDisplay();

	return;
}