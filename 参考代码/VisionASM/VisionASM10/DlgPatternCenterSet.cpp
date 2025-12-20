// DlgPatternCenterSet.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgPatternCenterSet.h"
#include "svXMLConfigurator.h"

#include "svBlob.h"
#include "svBlobSceneDescription.h"
#include "svHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternCenterSet dialog


CDlgPatternCenterSet::CDlgPatternCenterSet(cpImage& imageModel, double dMarkX, double dMarkY, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternCenterSet::IDD, pParent)
	, m_nMarkPosIndex(0)
{
	//{{AFX_DATA_INIT(CDlgPatternCenterSet)
	m_strStepLength = _T("");
	//}}AFX_DATA_INIT
	m_pGuiInteractiveContainer = NULL;
	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_nMarkNum = 1;
	if (imageModel.IsValid())
	{
		m_imageModel = imageModel;
		m_dMarkX = dMarkX;
		m_dMarkY = dMarkY;
	}
	else
	{
		m_imageModel.Release();
		m_dMarkX = 0.0;
		m_dMarkY = 0.0;
	}

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_nMoveMode = 0;
	m_boIsMultipleModelPoint = FALSE;
	m_vecCurrentRoiMarkXY.clear();
	m_vecCurrentModelMarkXY.clear();
	m_vpGuiMarkCross.clear();
	m_vpGuiMarkText.clear();
}

CDlgPatternCenterSet::CDlgPatternCenterSet(cpImage& imageModel, std::vector<sc2Vector> vecRoiMarkXY, 
	cpImage& imageTrain, scRect rcTrain, bool boIsMultipleModelPoint,CWnd* pParent /*= NULL*/)
	: CDialog(CDlgPatternCenterSet::IDD, pParent)
	, m_nMarkPosIndex(0)
{
	//{{AFX_DATA_INIT(CDlgPatternCenterSet)
	m_strStepLength = _T("");
	//}}AFX_DATA_INIT
	m_pGuiInteractiveContainer	= NULL;
	m_pStatusBar				= NULL;
	m_bStatusBarFlag			= TRUE;
	m_hIconSucceed				= NULL;
	m_hIconFailed				= NULL;
	m_nMarkNum					= vecRoiMarkXY.size();	//zzc
	m_rcTrain					= rcTrain;				//zzc
	if (imageModel.IsValid())
	{
		m_imageModel			= imageModel;
		m_vecCurrentRoiMarkXY	= vecRoiMarkXY;			//zzc
	}
	else
	{
		m_imageModel.Release();
		m_vecCurrentRoiMarkXY.clear();
	}

	if (imageTrain.IsValid())	//zzc
	{
		m_imageTrain = imageTrain;
		for (int i = 0; i < m_nMarkNum; i++)
		{
			sc2Vector modelMarkXY(m_vecCurrentRoiMarkXY.at(i).GetX() + m_rcTrain.GetUL().GetX(), 
				m_vecCurrentRoiMarkXY.at(i).GetY() + m_rcTrain.GetUL().GetY());
			m_vecCurrentModelMarkXY.push_back(modelMarkXY);
		}
	}
	else
	{
		m_imageTrain.Release();
		m_vecCurrentModelMarkXY.clear();
	}

	m_psaSysInfoStrings			= NULL;
	m_bSysKeyboardEnabled		= FALSE;
	m_nMoveMode					= 0;
	m_boIsMultipleModelPoint	= boIsMultipleModelPoint;
}

void CDlgPatternCenterSet::InitDlgItem()
{
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
	
	UpdateStatusBar();

	// 初始化界面控件
// 	m_comboBlobMode.ResetContent();
// 	m_comboBlobMode.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BLACK_MARK_WIGHT_BKGRD)/*_T("黑Mark白背景")*/);
// 	m_comboBlobMode.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_WIGHT_MARK_BLACK_BKGRD)/*_T("白Mark黑背景")*/);
// 	m_comboBlobMode.SetCurSel(0);

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	m_GuiDisplay.Create(IDC_STATIC_DISPLAY, this);
	m_GuiDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiDisplay.SetLanguage(0);
		m_comboMarkTypeSel.AddString(_T("Select in ROI"));	
		m_comboMarkTypeSel.AddString(_T("Select in Full Img"));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiDisplay.SetLanguage(1);
		m_comboMarkTypeSel.AddString(_T("在ROI上选取"));	
		m_comboMarkTypeSel.AddString(_T("在全图上选取"));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiDisplay.SetLanguage(1);
		m_comboMarkTypeSel.AddString(_T("在ROI上选取"));	
		m_comboMarkTypeSel.AddString(_T("在全图上选取"));
		break;
	}
//	m_GuiDisplay.SetBackgroundColor(RGB(0,0,0));
	m_GuiDisplay.RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DRAGSTOP);
	m_pGuiInteractiveContainer = m_GuiDisplay.GetInteractiveContainer();

	if (m_boIsMultipleModelPoint)
	{			
 		CString strTempInfo = _T("");
		for (int i = 0; i < m_nMarkNum; i++)
		{
			scGuiCross* pscGuiMarkCross = new scGuiCross;
			pscGuiMarkCross->SetCenterXYWidthHeight(m_vecCurrentRoiMarkXY[i].GetX(), m_vecCurrentRoiMarkXY[i].GetY(), 1000, 1000);
			pscGuiMarkCross->SetTipTextEnabled(true);
			//pscGuiMarkCross->SetTipTextEnabled(false);
			strTempInfo.Format(_T("标记点%d"), i+1);
			pscGuiMarkCross->SetTipText(strTempInfo);

			scGuiText* pscGuiMarkText = new scGuiText;
			sc2Vector markTextPos(m_vecCurrentRoiMarkXY[i].GetX() + 10, m_vecCurrentRoiMarkXY[i].GetY() + 10);	//在中心点的右上方
			pscGuiMarkText->SetXYRotationText(markTextPos, scRadian(0), strTempInfo);


			if (i == 0)
			{
				pscGuiMarkCross->SetVisible(TRUE);
				pscGuiMarkText->SetVisible(TRUE);
			} 
			else
			{
				pscGuiMarkCross->SetVisible(FALSE);
				pscGuiMarkText->SetVisible(FALSE);
			}

			m_vpGuiMarkCross.push_back(pscGuiMarkCross);
			m_vpGuiMarkText.push_back(pscGuiMarkText);
			strTempInfo.Format(_T("MarkCross%d"), i);
			m_pGuiInteractiveContainer->AddItem(m_vpGuiMarkCross[i], strTempInfo);
			strTempInfo.Format(_T("MarkText%d"), i);
			m_pGuiInteractiveContainer->AddItem(m_vpGuiMarkText[i], strTempInfo);
		}
		m_GuiDisplay.SetInteractiveContainer(m_pGuiInteractiveContainer);

		CString strInfo;
		strInfo.Format(_T("(%.2f, %.2f)"), m_vecCurrentRoiMarkXY[0].GetX(), m_vecCurrentRoiMarkXY[0].GetY());	//zzc
		GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);

		m_vecMarkXY = m_vecCurrentRoiMarkXY; //默认是ROI
	}
	else
	{
		if (!m_vecCurrentRoiMarkXY.empty())
		{
			m_dMarkX = m_vecCurrentRoiMarkXY[0].GetX();
			m_dMarkY = m_vecCurrentRoiMarkXY[0].GetY();
		}
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		m_GuiMarkCross.SetVisible(TRUE);
		m_pGuiInteractiveContainer->AddItem(&m_GuiMarkCross, _T("MarkCross"));
		m_GuiDisplay.SetInteractiveContainer(m_pGuiInteractiveContainer);

		CString strInfo;
		strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
		GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	}

	m_comboMarkTypeSel.SetCurSel(0);
	m_nMarkType = 0;

	CString strInfo;
	m_dStepLength = 0.05;
	strInfo.Format(_T("%f"),m_dStepLength);
	GetDlgItem(IDC_EDIT_STEP_LENGTH)->SetWindowText(strInfo);

	strInfo.Format(_T("%d"), m_nMarkNum);
	GetDlgItem(IDC_EDIT_MARK_NUM)->SetWindowText(strInfo);
	UpdateRadioButtonShow();
	m_GuiDisplay.SetImage(m_imageModel);

// 	strInfo.Format(_T("%.2f"),m_dMarkX);
// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkY);
// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);
	
	m_btnMoveUp.SetThemeHelper(&m_themeHelper);
	m_btnMoveUp.SetIcon(IDI_ICON_MOVE_UP_32);
	m_btnMoveDown.SetThemeHelper(&m_themeHelper);
	m_btnMoveDown.SetIcon(IDI_ICON_MOVE_DOWN_32);
	m_btnMoveLeft.SetThemeHelper(&m_themeHelper);
	m_btnMoveLeft.SetIcon(IDI_ICON_MOVE_LEFT_32);
	m_btnMoveRight.SetThemeHelper(&m_themeHelper);
	m_btnMoveRight.SetIcon(IDI_ICON_MOVE_RIGHT_32);
	
//	m_btnCenterAbstract.SetThemeHelper(&m_themeHelper);
// 	m_btnCenterAbstract.SetIcon(IDI_ICON_EXTRACT_POINT_40);
// 	m_btnCenterAbstract.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
}
void CDlgPatternCenterSet::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}
void CDlgPatternCenterSet::GetPatternCenter( double& dMarkX, double& dMarkY )
{
	if (m_nMarkType == 0)
	{
		dMarkX = m_dMarkX;
		dMarkY = m_dMarkY;
	} 
	else
	{
		dMarkX = m_dMarkX - m_rcTrain.GetUL().GetX();
		dMarkY = m_dMarkY - m_rcTrain.GetUL().GetY();
	}
}

void CDlgPatternCenterSet::GetPatternCenter(std::vector<sc2Vector>& vecMarkXY, int &nMarkType)
{
	if (m_nMarkType == 0)	//ROI点需要还原成全图绝对位置，，内部无需计算
	{
		vecMarkXY = m_vecMarkXY;
		nMarkType = m_nMarkType;
	}
	else	//全图绝对位置，外部有处理，内部要预计算
	{
		for (int n = 0; n < m_nMarkNum; n++)
		{
			m_vecMarkXY[n].SetX(m_vecMarkXY[n].GetX()-m_rcTrain.GetUL().GetX());
			m_vecMarkXY[n].SetY(m_vecMarkXY[n].GetY()-m_rcTrain.GetUL().GetY());
		}

		vecMarkXY = m_vecMarkXY;
		nMarkType = m_nMarkType;
	}

}

void CDlgPatternCenterSet::UpdateRadioButtonShow()	//zzc
{
	UpdateData(TRUE);

	if (m_nMarkNum < 1)
	{
		return;
	}

	CButton* radioBtnMarkPos1 = (CButton*)GetDlgItem(IDC_RADIO_INDEX_1);
	CButton* radioBtnMarkPos2 = (CButton*)GetDlgItem(IDC_RADIO_INDEX_2);
	CButton* radioBtnMarkPos3 = (CButton*)GetDlgItem(IDC_RADIO_INDEX_3);
	CButton* radioBtnMarkPos4 = (CButton*)GetDlgItem(IDC_RADIO_INDEX_4);

	switch(m_nMarkNum)
	{
	case 1:
		{
			radioBtnMarkPos1->ShowWindow(SW_SHOW);
			radioBtnMarkPos2->ShowWindow(SW_HIDE);
			radioBtnMarkPos3->ShowWindow(SW_HIDE);
			radioBtnMarkPos4->ShowWindow(SW_HIDE);
			radioBtnMarkPos1->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SHOW_ALL)->ShowWindow(SW_HIDE);
		}
		break;
	case 2:
		{
			radioBtnMarkPos1->ShowWindow(SW_SHOW);
			radioBtnMarkPos2->ShowWindow(SW_SHOW);
			radioBtnMarkPos3->ShowWindow(SW_HIDE);
			radioBtnMarkPos4->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SHOW_ALL)->ShowWindow(SW_SHOW);
		}
		break;
	case 3:
		{
			radioBtnMarkPos1->ShowWindow(SW_SHOW);
			radioBtnMarkPos2->ShowWindow(SW_SHOW);
			radioBtnMarkPos3->ShowWindow(SW_SHOW);
			radioBtnMarkPos4->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SHOW_ALL)->ShowWindow(SW_SHOW);
		}
		break;
	case 4:
		{
			radioBtnMarkPos1->ShowWindow(SW_SHOW);
			radioBtnMarkPos2->ShowWindow(SW_SHOW);
			radioBtnMarkPos3->ShowWindow(SW_SHOW);
			radioBtnMarkPos4->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SHOW_ALL)->ShowWindow(SW_SHOW);
		}
		break;
	default:
		break;
	}
}

BOOL CDlgPatternCenterSet::CheckMoveStepValid()
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
	return TRUE;
}

void CDlgPatternCenterSet::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_PATTERN_CENTER_SET") == strDlgID)
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

void CDlgPatternCenterSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatternCenterSet)
//	DDX_Control(pDX, IDC_COMBO_BLOB_MODE, m_comboBlobMode);
	DDX_Text(pDX, IDC_EDIT_STEP_LENGTH, m_strStepLength);
//	DDX_Control(pDX, IDC_BTN_ABSTRACT, m_btnCenterAbstract);
	DDX_Control(pDX, IDC_BTN_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btnMoveRight);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Radio(pDX, IDC_RADIO_MODE0, m_nMoveMode);
	DDV_MinMaxInt(pDX, m_nMoveMode, 0, 1);
	DDX_Control(pDX, IDC_COMBO_MARK_TYPE_SEL, m_comboMarkTypeSel);
	DDX_Radio(pDX, IDC_RADIO_INDEX_1, m_nMarkPosIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPatternCenterSet, CDialog)
	//{{AFX_MSG_MAP(CDlgPatternCenterSet)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTOP, OnGuiDisplayDrapStop)
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTART, OnGuiDisplayDrapStart)
	ON_EN_SETFOCUS(IDC_EDIT_STEP_LENGTH, OnSetfocusEditStepLength)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgPatternCenterSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPatternCenterSet::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_MARK_TYPE_SEL, &CDlgPatternCenterSet::OnCbnSelchangeComboMarkTypeSel)
	ON_BN_CLICKED(IDC_RADIO_INDEX_1, &CDlgPatternCenterSet::OnBnClickedRadioIndex1)
	ON_BN_CLICKED(IDC_RADIO_INDEX_2, &CDlgPatternCenterSet::OnBnClickedRadioIndex1)
	ON_BN_CLICKED(IDC_RADIO_INDEX_3, &CDlgPatternCenterSet::OnBnClickedRadioIndex1)
	ON_BN_CLICKED(IDC_RADIO_INDEX_4, &CDlgPatternCenterSet::OnBnClickedRadioIndex1)
	ON_BN_CLICKED(IDC_BTN_SHOW_ALL, &CDlgPatternCenterSet::OnBnClickedBtnShowAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternCenterSet message handlers

BOOL CDlgPatternCenterSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateDialogLanguage();
	InitDlgItem();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
// 
// void CDlgPatternCenterSet::OnBtnAbstract() 
// {
// 	std::vector<long>histogram;
// 	histogram.insert(histogram.begin(),256,0);
// 	HStatus HValue = sfHistogram(m_imageModel, histogram);
// 	if (HValue != 0)  
// 	{
// 		m_bStatusBarFlag = FALSE;
// 		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_CREATE_HISTOGRAM);	//_T("创建直方图失败！");
// 		UpdateStatusBar();
// 		return ;
// 	}
// 	scHistoStats ImgStat(histogram);
// 	double dScore;
// 	int iThre = ImgStat.GetDynamicThreshold(dScore);
// 	
// 	if (iThre < 20) iThre = 20;
// 	if (iThre > 235) iThre = 235;
// 	
// 	scBlobTool   m_BlobTool;
// 	m_BlobTool.ConnectivityType(scBlobSceneDescription::eWholeImageGreyScale);
// 	m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::eNone);
// 	m_BlobTool.EnableComputeInertia(false);
// 	m_BlobTool.EnableComputeContour(false);
// 	m_BlobTool.ConnectivityMinPels(10);
// 	m_BlobTool.ClearMask();
// 	
// 	int softness = 32;
// 	int iCurSel = m_comboBlobMode.GetCurSel();
// 	if (iCurSel == 0)
// 		m_BlobTool.SetSegmentationSoftThresh(iThre-15, iThre +15, softness, true);
// 	else
// 		m_BlobTool.SetSegmentationSoftThresh(iThre-15, iThre +15, softness, false);
// 	scRect roi(1,1,m_imageModel.Width()-2, m_imageModel.Height()-2);
// 	BOOL bIsSuccessed = m_BlobTool.Execute(m_imageModel, &roi);
// 	if (bIsSuccessed)
// 	{
// 		scBlobSceneDescription* pBSD;
// 		pBSD = m_BlobTool.GetBSD();
// 		pBSD->clearFilters();
// 		pBSD->filter(eArea, 10, 307200, false);
// 		pBSD->clearSort();
// 		pBSD->sort(eArea, eDescending);
// 		int iBlobNum = pBSD->numBlobs(true);
// 		if (iBlobNum > 0)
// 		{
// 			const scBlob* pBlob = pBSD->getBlob(1, true);
// 			m_dMarkX = pBlob->centerMassX;
// 			m_dMarkY = pBlob->centerMassY;
// 			m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY);
// 			m_GuiDisplay.MyInvalidate();
// 			
// 			CString strInfo;
// 			strInfo.Format(_T("%.2f"),m_dMarkX);
// 			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 			strInfo.Format(_T("%.2f"),m_dMarkY);
// 			GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);
// 			
// 			m_bStatusBarFlag = TRUE;
// 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_ABSTRACT_CENTER_SUCCEED);	//_T("自动提取Mark中心点成功！");
// 			UpdateStatusBar();
// 		}
// 		else
// 		{
// 			m_bStatusBarFlag = FALSE;
// 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CANNOT_ABSTRACT_CENTER_AUTOMATICALLY);	//_T("无法准确提取Mark所在区域的位置信息！");
// 			UpdateStatusBar();			
// 			return ;
// 		}
// 	}
// 	else
// 	{
// 		m_bStatusBarFlag = FALSE;
// 		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_ABSTRACT_CENTER_SUCCEED);	//_T("自动提取Mark中心点失败！");
// 		UpdateStatusBar();
// 		return ;
// 	}
// }

void CDlgPatternCenterSet::OnBtnUp() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	CString strLog;
	strLog.Format(_T("点击[向上]按钮\n"));
	g_opLogFile.SaveLogText(strLog, 7);
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY -= m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(0,-m_dStepLength,m_nMarkPosIndex);
	} 
	else
	{
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY -= m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(0,-m_dStepLength,0);
	}
	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkX);
// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkY);
// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnBtnDown() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	CString strLog;
	strLog.Format(_T("点击[向下]按钮\n"));
	g_opLogFile.SaveLogText(strLog, 7);
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY += m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));
		MarkPosUpdate(0,m_dStepLength,m_nMarkPosIndex);
	} 
	else
	{
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY += m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		MarkPosUpdate(0,m_dStepLength,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;	
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkX);
// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkY);
// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnBtnLeft() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	CString strLog;
	strLog.Format(_T("点击[向左]按钮\n"));
	g_opLogFile.SaveLogText(strLog, 7);

	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX -= m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(-m_dStepLength,0,m_nMarkPosIndex);
	} 
	else
	{	
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX -= m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(-m_dStepLength,0,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkX);
// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkY);
// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnBtnRight() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	CString strLog;
	strLog.Format(_T("点击[向右]按钮\n"));
	g_opLogFile.SaveLogText(strLog, 7);

	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX += m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(m_dStepLength,0,m_nMarkPosIndex);
	} 
	else
	{	
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX += m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(m_dStepLength,0,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkX);
// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
// 	strInfo.Format(_T("%.2f"),m_dMarkY);
// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnUp() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY -= m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(0,-m_dStepLength,m_nMarkPosIndex);
	} 
	else
	{
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY -= m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		MarkPosUpdate(0,-m_dStepLength,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkX);
	// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkY);
	// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnDown() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY += m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(0,m_dStepLength,m_nMarkPosIndex);
	} 
	else
	{
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkY += m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(0,m_dStepLength,0);
	}
	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;	
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkX);
	// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkY);
	// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnLeft() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX -= m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(-m_dStepLength,0,m_nMarkPosIndex);
	} 
	else
	{	
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX -= m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(-m_dStepLength,0,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkX);
	// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkY);
	// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

void CDlgPatternCenterSet::OnRight() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_boIsMultipleModelPoint)
	{
		m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX += m_dStepLength;
		m_vpGuiMarkCross[m_nMarkPosIndex]->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
		sc2Vector markXYResult(m_dMarkX, m_dMarkY);
		m_vecMarkXY[m_nMarkPosIndex] = markXYResult;
		sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
		m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));

		MarkPosUpdate(m_dStepLength,0,m_nMarkPosIndex);
	} 
	else
	{	
		m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);
		m_dMarkX += m_dStepLength;
		m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);

		MarkPosUpdate(m_dStepLength,0,0);
	}

	m_GuiDisplay.MyInvalidate();
	
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkX);
	// 	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	// 	strInfo.Format(_T("%.2f"),m_dMarkY);
	// 	GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);	
}

LRESULT CDlgPatternCenterSet::OnGuiDisplayDrapStop( WPARAM wParam, LPARAM lParam )
{
	if (m_boIsMultipleModelPoint)
	{
		std::vector<scGuiCross*>::iterator it = m_vpGuiMarkCross.begin();
		for (int i = 0;it != m_vpGuiMarkCross.end(); it++, i++)
		{
			if ((*it)->IsSelected())
			{
				m_nMarkPosIndex = i;
				for (int j = 0; j < m_nMarkNum; j++)
				{
					if (j == i)
					{	
						CButton* radioBtnMarkPosIndex = (CButton*)GetDlgItem(IDC_RADIO_INDEX_1 + j);
						radioBtnMarkPosIndex->SetCheck(TRUE);
					}
					else
					{
						CButton* radioBtnMarkPosIndex = (CButton*)GetDlgItem(IDC_RADIO_INDEX_1 + j);
						radioBtnMarkPosIndex->SetCheck(FALSE);
					}
				}
				(*it)->GetCenterXY(m_dMarkX, m_dMarkY);
				CString strInfo;
				strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
				GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
				sc2Vector markXYResult(m_dMarkX, m_dMarkY);
				m_vecMarkXY[i] = markXYResult;
				sc2Vector markTextPos(m_dMarkX + 10, m_dMarkY + 10);
				m_vpGuiMarkText[m_nMarkPosIndex]->SetXYRotation(markTextPos, scRadian(0));
				
				if(m_nMarkType == 0)
				{
					//更新全图位置
					double dX = m_dMarkX - m_vecCurrentRoiMarkXY[m_nMarkPosIndex].GetX();
					double dY = m_dMarkY - m_vecCurrentRoiMarkXY[m_nMarkPosIndex].GetY();
					double dModelMarkX = dX + m_vecCurrentModelMarkXY[m_nMarkPosIndex].GetX();
					double dModelMarkY = dY + m_vecCurrentModelMarkXY[m_nMarkPosIndex].GetY();
					m_vecCurrentModelMarkXY[m_nMarkPosIndex].SetX(dModelMarkX);
					m_vecCurrentModelMarkXY[m_nMarkPosIndex].SetY(dModelMarkY);
					//记录Roi当前位置
					m_vecCurrentRoiMarkXY[m_nMarkPosIndex].SetX(m_dMarkX);
					m_vecCurrentRoiMarkXY[m_nMarkPosIndex].SetY(m_dMarkY);
				}
				else if(m_nMarkType == 1)
				{
					//记录Roi当前位置
					double dX = m_dMarkX - m_vecCurrentModelMarkXY[m_nMarkPosIndex].GetX();
					double dY = m_dMarkY - m_vecCurrentModelMarkXY[m_nMarkPosIndex].GetY();
					double dModelMarkX = dX + m_vecCurrentRoiMarkXY[m_nMarkPosIndex].GetX();
					double dModelMarkY = dY + m_vecCurrentRoiMarkXY[m_nMarkPosIndex].GetY();
					m_vecCurrentRoiMarkXY[m_nMarkPosIndex].SetX(dModelMarkX);
					m_vecCurrentRoiMarkXY[m_nMarkPosIndex].SetY(dModelMarkY);
					//更新全图位置
					m_vecCurrentModelMarkXY[m_nMarkPosIndex].SetX(m_dMarkX);
					m_vecCurrentModelMarkXY[m_nMarkPosIndex].SetY(m_dMarkY);
				}
				
				SetTimer(5, 200, 0);
			}
		}
	} 
	else
	{
		if (m_GuiMarkCross.IsSelected())
		{
			m_GuiMarkCross.GetCenterXY(m_dMarkX, m_dMarkY);

			CString strInfo;
			strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);

			if(m_vecCurrentRoiMarkXY.size()>=1&&m_vecCurrentModelMarkXY.size()>=1)
			{
				if(m_nMarkType == 0)
				{
					//更新全图位置
					double dX = m_dMarkX - m_vecCurrentRoiMarkXY[0].GetX();
					double dY = m_dMarkY - m_vecCurrentRoiMarkXY[0].GetY();
					double dModelMarkX = dX + m_vecCurrentModelMarkXY[0].GetX();
					double dModelMarkY = dY + m_vecCurrentModelMarkXY[0].GetY();
					m_vecCurrentModelMarkXY[0].SetX(dModelMarkX);
					m_vecCurrentModelMarkXY[0].SetY(dModelMarkY);
					//记录Roi当前位置
					m_vecCurrentRoiMarkXY[0].SetX(m_dMarkX);
					m_vecCurrentRoiMarkXY[0].SetY(m_dMarkY);
				}
				else if(m_nMarkType == 1)
				{
					//记录Roi当前位置
					double dX = m_dMarkX - m_vecCurrentModelMarkXY[0].GetX();
					double dY = m_dMarkY - m_vecCurrentModelMarkXY[0].GetY();
					double dModelMarkX = dX + m_vecCurrentRoiMarkXY[0].GetX();
					double dModelMarkY = dY + m_vecCurrentRoiMarkXY[0].GetY();
					m_vecCurrentRoiMarkXY[0].SetX(dModelMarkX);
					m_vecCurrentRoiMarkXY[0].SetY(dModelMarkY);
					//更新全图位置
					m_vecCurrentModelMarkXY[0].SetX(m_dMarkX);
					m_vecCurrentModelMarkXY[0].SetY(m_dMarkY);
				}
			}	
			// 		strInfo.Format(_T("%.2f"),m_dMarkX);
			// 		GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
			// 		strInfo.Format(_T("%.2f"),m_dMarkY);
			// 		GetDlgItem(IDC_STATIC_MARK_Y)->SetWindowText(strInfo);
		}
	}
	return 0;
}

LRESULT CDlgPatternCenterSet::OnGuiDisplayDrapStart(WPARAM wParam, LPARAM lParam)
{
	if (m_boIsMultipleModelPoint)
	{
		std::vector<scGuiCross*>::iterator it = m_vpGuiMarkCross.begin();
		for (;it != m_vpGuiMarkCross.end(); it++)
		{
			(*it)->SetTipTextEnabled(FALSE);
		}
	} 
	return 0;
}

void CDlgPatternCenterSet::OnSetfocusEditStepLength() 
{
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
			CString strLog;
			strLog.Format(_T("修改[步长]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,7);
		}
	}
}

BOOL CDlgPatternCenterSet::DestroyWindow() 
{
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
	for(int i = 0;i < m_vpGuiMarkCross.size();i++)
	{
		if(m_vpGuiMarkCross[i] != NULL)
		{
			delete m_vpGuiMarkCross[i];
			m_vpGuiMarkCross[i] = NULL;
		}
	}
	m_vpGuiMarkCross.clear();

	for(int i = 0;i < m_vpGuiMarkText.size();i++)
	{
		if(m_vpGuiMarkText[i] != NULL)
		{
			delete m_vpGuiMarkText[i];
			m_vpGuiMarkText[i] = NULL;
		}
	}
	m_vpGuiMarkText.clear();

	return CDialog::DestroyWindow();
}

void CDlgPatternCenterSet::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
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
	else if (nIDEvent == 5)
	{
		if (m_boIsMultipleModelPoint)
		{
			std::vector<scGuiCross*>::iterator it = m_vpGuiMarkCross.begin();
			for (; it != m_vpGuiMarkCross.end(); it++)
			{
				if ((*it)->IsDragging())
				{
					(*it)->SetTipTextEnabled(FALSE);
				}
				else
				{
					(*it)->SetTipTextEnabled(TRUE);
				}
			}
		}
		
		KillTimer(5);
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgPatternCenterSet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 如果处于单动模式，则不再响应按钮按下和释放动作
	UpdateData(TRUE);
	if (m_nMoveMode == 1)
	{
		return CDialog::PreTranslateMessage(pMsg);
	}

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


void CDlgPatternCenterSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
	CString strLog;
	strLog.Format(_T("点击[确定]退出\n"));
	g_opLogFile.SaveLogText(strLog, 6);
}


void CDlgPatternCenterSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
	//CDialog::OnOK();
	CString strLog;
	strLog.Format(_T("点击[取消]退出\n"));
	g_opLogFile.SaveLogText(strLog, 6);
}


void CDlgPatternCenterSet::OnCbnSelchangeComboMarkTypeSel()	//zzc
{
	if (m_comboMarkTypeSel.GetCurSel() == 0)	//ROI
	{
		m_nMarkType = 0;
		if (m_boIsMultipleModelPoint)
		{
			for (int i = 0; i < m_nMarkNum; i++)
			{
				m_vpGuiMarkCross[i]->SetCenterXYWidthHeight(m_vecCurrentRoiMarkXY[i].GetX(), m_vecCurrentRoiMarkXY[i].GetY(), 1000, 1000);
				sc2Vector markTextPos(m_vecCurrentRoiMarkXY[i].GetX() + 10, m_vecCurrentRoiMarkXY[i].GetY() + 10);
				m_vpGuiMarkText[i]->SetXYRotation(markTextPos, scRadian(0));
				if ( i == 0)
				{
					m_vpGuiMarkCross[i]->SetVisible(TRUE);
					m_vpGuiMarkText[i]->SetVisible(TRUE);
				}
				else
				{
					m_vpGuiMarkCross[i]->SetVisible(FALSE);
					m_vpGuiMarkText[i]->SetVisible(FALSE);
				}
			}

			m_vecMarkXY = m_vecCurrentRoiMarkXY;	//切成ROI，当前为RIO下坐标
			CString strInfo;
			strInfo.Format(_T("(%.2f, %.2f)"),m_vecCurrentRoiMarkXY[0].GetX(), m_vecCurrentRoiMarkXY[0].GetY()); //默认第一个点信息
			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
			OnBnClickedRadioIndex1();
		} 
		else
		{
			if (!m_vecCurrentRoiMarkXY.empty())
			{
				m_dMarkX = m_vecCurrentRoiMarkXY[0].GetX();
				m_dMarkY = m_vecCurrentRoiMarkXY[0].GetY();
			}
			m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
			CString strInfo;
			strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
		}
		m_GuiDisplay.SetImage(m_imageModel);
	} 
	else	//全图
	{
		m_nMarkType = 1;
		if (m_boIsMultipleModelPoint)
		{
			for (int i = 0; i < m_nMarkNum; i++)
			{
				m_vpGuiMarkCross[i]->SetCenterXYWidthHeight(m_vecCurrentModelMarkXY[i].GetX(), m_vecCurrentModelMarkXY[i].GetY(), 1000, 1000);
				sc2Vector markTextPos(m_vecCurrentModelMarkXY[i].GetX() + 10, m_vecCurrentModelMarkXY[i].GetY() + 10);
				m_vpGuiMarkText[i]->SetXYRotation(markTextPos, scRadian(0));
				if ( i == 0)
				{
					m_vpGuiMarkCross[i]->SetVisible(TRUE);
					m_vpGuiMarkText[i]->SetVisible(TRUE);
				}
				else
				{
					m_vpGuiMarkCross[i]->SetVisible(FALSE);
					m_vpGuiMarkCross[i]->SetVisible(FALSE);
				}
			}
			m_vecMarkXY = m_vecCurrentModelMarkXY;
			CString strInfo;
			strInfo.Format(_T("(%.2f, %.2f)"), m_vecCurrentModelMarkXY[0].GetX(), m_vecCurrentModelMarkXY[0].GetY());
			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
			OnBnClickedRadioIndex1();
		} 
		else
		{
			if (!m_vecCurrentModelMarkXY.empty())
			{
				m_dMarkX = m_vecCurrentModelMarkXY[0].GetX();
				m_dMarkY = m_vecCurrentModelMarkXY[0].GetY();
			}
			m_GuiMarkCross.SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,1000,1000);
			CString strInfo;
			strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
			GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
		}
		m_GuiDisplay.SetImage(m_imageTrain);
	}
}

void CDlgPatternCenterSet::OnBnClickedRadioIndex1()
{
	UpdateData(TRUE);

	switch(m_nMarkPosIndex)
	{
	case 0:
		{
			m_vpGuiMarkCross[0]->SetVisible(TRUE);
			m_vpGuiMarkCross[0]->SetSelected(TRUE);			
			m_vpGuiMarkText[0]->SetVisible(TRUE);
			if (m_nMarkNum > 1)
			{
				m_vpGuiMarkCross[1]->SetVisible(FALSE);
				m_vpGuiMarkCross[1]->SetSelected(FALSE);
				m_vpGuiMarkText[1]->SetVisible(FALSE);
			}
		}
		break;
	case 1:
		{
			m_vpGuiMarkCross[0]->SetVisible(FALSE);
			m_vpGuiMarkCross[0]->SetSelected(FALSE);
			m_vpGuiMarkCross[1]->SetVisible(TRUE);
			m_vpGuiMarkCross[1]->SetSelected(TRUE);
			m_vpGuiMarkText[0]->SetVisible(FALSE);
			m_vpGuiMarkText[0]->SetSelected(FALSE);
			m_vpGuiMarkText[1]->SetVisible(TRUE);
			m_vpGuiMarkText[1]->SetSelected(TRUE);
		}
		break;
	default:
		break;
	}

	m_vpGuiMarkCross[m_nMarkPosIndex]->GetCenterXY(m_dMarkX, m_dMarkY);
	CString strInfo;
	strInfo.Format(_T("(%.2f, %.2f)"),m_dMarkX, m_dMarkY);
	GetDlgItem(IDC_STATIC_MARK_X)->SetWindowText(strInfo);
	m_GuiDisplay.MyInvalidate();
	UpdateData(FALSE);
}


void CDlgPatternCenterSet::OnBnClickedBtnShowAll()
{

	for (int i = 0; i < m_nMarkNum; i++)
	{
		m_vpGuiMarkCross[i]->SetLineStyle((scGuiGraphic::StyleFlags)i);
		m_vpGuiMarkCross[i]->SetVisible(TRUE);
		m_vpGuiMarkText[i]->SetVisible(TRUE);
	}
	m_GuiDisplay.MyInvalidate();
}

void CDlgPatternCenterSet::MarkPosUpdate(double dMarkX,double dMarkY,int nPosIndex)
{
	if(nPosIndex<0||m_vecCurrentRoiMarkXY.size()<=nPosIndex)
	{
		return;
	}
	if(nPosIndex<0||m_vecCurrentModelMarkXY.size()<=nPosIndex)
	{
		return;
	}
	//Roi更新坐标
	double dRoiX = dMarkX + m_vecCurrentRoiMarkXY[nPosIndex].GetX();
	double dRoiY = dMarkY + m_vecCurrentRoiMarkXY[nPosIndex].GetY();
	m_vecCurrentRoiMarkXY[nPosIndex].SetX(dRoiX);
	m_vecCurrentRoiMarkXY[nPosIndex].SetY(dRoiY);
	//全图更新坐标
	double dModelX = dMarkX + m_vecCurrentModelMarkXY[nPosIndex].GetX();
	double dModelY = dMarkY + m_vecCurrentModelMarkXY[nPosIndex].GetY();
	m_vecCurrentModelMarkXY[nPosIndex].SetX(dModelX);
	m_vecCurrentModelMarkXY[nPosIndex].SetY(dModelY);
	
}