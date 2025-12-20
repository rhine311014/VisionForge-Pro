// DlgAlignOffsetByAreaSet.cpp : 实现文件
// DlgAlignOffsetByAreaSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAlignOffsetByAreaSet.h"
#include "afxdialogex.h"
#include "DlgVisionRunAlignInfo.h"


// CDlgAlignOffsetByAreaSet 对话框

IMPLEMENT_DYNAMIC(CDlgAlignOffsetByAreaSet, CDialogEx)

	CDlgAlignOffsetByAreaSet::CDlgAlignOffsetByAreaSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlignOffsetByAreaSet::IDD, pParent)
	, m_nEditRowNum(1)
	, m_nEditColNum(1)
	, m_dEditStartX(0)
	, m_dEditStartY(0)
	, m_dEditWidth(300)
	, m_dEditHeight(300)
	, m_bChkAlignOffsetByAreaEnable(FALSE)
	, m_nEditLineWidth(1)
	, m_nEditLightLineWidth(1)
	, m_nRdSearchType(0)
{
	m_pVisionASM = NULL;
	m_pVAPrevMainWnd = NULL;
	m_bIsGrabbing = false;
	m_bKeyboardEnable = false;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_pParentWnd = NULL;

	m_nCurPositionIdx = 0;
}

CDlgAlignOffsetByAreaSet::~CDlgAlignOffsetByAreaSet()
{
}

void CDlgAlignOffsetByAreaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROW_NUM, m_nEditRowNum);
	DDX_Text(pDX, IDC_EDIT_COL_NUM, m_nEditColNum);
	DDX_Text(pDX, IDC_EDIT_START_X, m_dEditStartX);
	DDX_Text(pDX, IDC_EDIT_START_Y, m_dEditStartY);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_dEditWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dEditHeight);
	DDX_Control(pDX, IDC_BTN_NORMAL_COLOR, m_btnNormalColor);
	DDX_Control(pDX, IDC_BTN_LIGHT_COLOR, m_btnLightColor);
	DDX_Check(pDX, IDC_CHK_ALIGNOFFSET_BY_AERA_ENABLE, m_bChkAlignOffsetByAreaEnable);
	DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_nEditLineWidth);
	DDX_Text(pDX, IDC_EDIT_LIGHT_LINE_WIDTH, m_nEditLightLineWidth);
	DDX_Radio(pDX, IDC_RD_OBJECT_SEARCH, m_nRdSearchType);
}


BEGIN_MESSAGE_MAP(CDlgAlignOffsetByAreaSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAlignOffsetByAreaSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAlignOffsetByAreaSet::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTART, &CDlgAlignOffsetByAreaSet::OnDisplayDragStart)
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTOP, &CDlgAlignOffsetByAreaSet::OnDisplayDragStop)
	ON_EN_CHANGE(IDC_EDIT_ROW_NUM, &CDlgAlignOffsetByAreaSet::OnEnChangeEditRowNum)
	ON_EN_CHANGE(IDC_EDIT_COL_NUM, &CDlgAlignOffsetByAreaSet::OnEnChangeEditColNum)
	ON_BN_CLICKED(IDC_BTN_NORMAL_COLOR, &CDlgAlignOffsetByAreaSet::OnBnClickedBtnNormalColor)
	ON_BN_CLICKED(IDC_BTN_LIGHT_COLOR, &CDlgAlignOffsetByAreaSet::OnBnClickedBtnLightColor)
	ON_EN_CHANGE(IDC_EDIT_LINE_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnChangeEditLineWidth)
	ON_EN_CHANGE(IDC_EDIT_LIGHT_LINE_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnChangeEditLightLineWidth)
	ON_EN_CHANGE(IDC_EDIT_START_X, &CDlgAlignOffsetByAreaSet::OnEnChangeEditStartX)
	ON_EN_CHANGE(IDC_EDIT_START_Y, &CDlgAlignOffsetByAreaSet::OnEnChangeEditStartY)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CDlgAlignOffsetByAreaSet::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnChangeEditWidth)
	ON_EN_SETFOCUS(IDC_EDIT_ROW_NUM, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_COL_NUM, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditColNum)
	ON_EN_SETFOCUS(IDC_EDIT_START_X, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditStartX)
	ON_EN_SETFOCUS(IDC_EDIT_START_Y, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditStartY)
	ON_EN_SETFOCUS(IDC_EDIT_HEIGHT, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditHeight)
	ON_EN_SETFOCUS(IDC_EDIT_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditWidth)
	ON_EN_SETFOCUS(IDC_EDIT_LINE_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditLineWidth)
	ON_EN_SETFOCUS(IDC_EDIT_LIGHT_LINE_WIDTH, &CDlgAlignOffsetByAreaSet::OnEnSetfocusEditLightLineWidth)
	ON_BN_CLICKED(IDC_BTN_GRAB_IMAGE, &CDlgAlignOffsetByAreaSet::OnBnClickedBtnGrabImage)
END_MESSAGE_MAP()


// CDlgAlignOffsetByAreaSet 消息处理程序


void CDlgAlignOffsetByAreaSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pVisionASM)
	{
		UpdateDlgData(true);

		int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();

		if (!m_pVisionASM->SaveProductDataInfo(nProductIndex))
		{
			MessageBox(_T("保存分区域补偿参数失败"));
		}

		if(!m_pVisionASM->m_pSystempOptionConfig->SaveSystemOption())
		{
			MessageBox(_T("保存分区域补偿显示参数失败"));
		}
	}

	CDialogEx::OnOK();
}


void CDlgAlignOffsetByAreaSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgAlignOffsetByAreaSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDlgItem();

	OnCamGrab();


	UpdateDlgData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAlignOffsetByAreaSet::InitDlgItem()
{
	// 创建状态栏
	InitStatusUI();
	// 初始化显示display
	InitDisplayUI();
}


void CDlgAlignOffsetByAreaSet::InitStatusUI()
{
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
}

void CDlgAlignOffsetByAreaSet::InitDisplayUI()
{
	// 创建Guidisplay
	scGuiDisplay *pGUIDisplay = NULL;
	int iLanguageType = GetLanguageType();
	CleanDisplayUI();

	pGUIDisplay = new scGuiDisplay;
	pGUIDisplay->Create(IDC_ST_DISPLAY, this,WS_VISIBLE);
	pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));	
	pGUIDisplay->SetLanguage(iLanguageType);
	pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DRAGSTART|SV_DISPLAY_DRAGSTOP);

	m_pVisionASM->SetPosDisplay(0, pGUIDisplay);

	m_vpGuiDisplay.push_back(pGUIDisplay);	
	m_vpGuiDisplay[0]->ShowWindow(SW_SHOW);


	// 添加控件
	scGuiStaticContainer *pGuiSC = NULL;
	scGuiInteractiveContainer* pGuiIC = NULL;

	pGuiSC = m_vpGuiDisplay[0]->GetStaticContainer();
	pGuiIC = m_vpGuiDisplay[0]->GetInteractiveContainer();

	// 添加搜索区域
	m_GuiSearchRect.SetInteractive(TRUE);
	m_GuiSearchRect.SetRect(scRect(0, 0, 100, 100));
	m_GuiSearchRect.SetLineColor(RGB(0, 255, 0));
	m_GuiSearchRect.SetLineWidth(1);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetTipTextEnabled(TRUE);
	m_GuiSearchRect.SetVisible(TRUE);
	m_GuiSearchRect.SetTipText(_T("CDlgAlignOffsetByAreaSet_SearchRect"));
	pGuiIC->AddItem(&m_GuiSearchRect, m_GuiSearchRect.GetTipText());

	// 添加分区域矩形
	CString strTmp;
	for (int i = 0; i < MAX_AREA_RECT_NUM; i++)
	{
		m_GuiAreaRect[i].SetRect(scRect(0, 0, 10, 10));
		m_GuiAreaRect[i].SetLineColor(RGB(0, 0, 255));
		m_GuiAreaRect[i].SetLineWidth(1);
		m_GuiAreaRect[i].SetLabelVisible(TRUE);
		strTmp.Format(_T("区域%d"), i+1);
		m_GuiAreaRect[i].SetLabel(strTmp);
		m_GuiAreaRect[i].SetTipTextEnabled(TRUE);
		strTmp.Format(_T("CDlgAlignOffsetByAreaSet_AreahRect%d"), i);
		m_GuiAreaRect[i].SetTipText(strTmp);
		pGuiSC->AddItem(&m_GuiAreaRect[i], m_GuiAreaRect[i].GetTipText());
	}

	m_vpGuiDisplay[0]->SetStaticContainer(pGuiSC);
	m_vpGuiDisplay[0]->SetInteractiveContainer(pGuiIC);

	UpdateSearchRectToGui();
	UpdateAreaRectShow();
}


void CDlgAlignOffsetByAreaSet::CleanDisplayUI()
{
	if (m_vpGuiDisplay.size() > 0 && m_vpGuiDisplay[0])
	{
		scGuiStaticContainer *pGuiSC = NULL;
		scGuiInteractiveContainer* pGuiIC = NULL;

		pGuiSC = m_vpGuiDisplay[0]->GetStaticContainer();
		pGuiIC = m_vpGuiDisplay[0]->GetInteractiveContainer();
		for (int i = 0; i < MAX_AREA_RECT_NUM; i++)
		{
			pGuiSC->RemoveItem(m_GuiAreaRect[i].GetTipText());
		}

		pGuiIC->RemoveItem(m_GuiSearchRect.GetTipText());

		m_vpGuiDisplay[0]->SetStaticContainer(pGuiSC);
		m_vpGuiDisplay[0]->SetInteractiveContainer(pGuiIC);

		m_vpGuiDisplay[0]->DestroyWindow();
		delete m_vpGuiDisplay[0];
		m_vpGuiDisplay[0] = NULL;
	}


}

void CDlgAlignOffsetByAreaSet::UpdateAreaRectShow()
{
	UpdateData(TRUE);
	if (m_nEditColNum == 0 || m_nEditRowNum == 0)
	{
		return;
	}

	for (int i = 0; i < MAX_AREA_RECT_NUM; i++)
	{
		m_GuiAreaRect[i].SetVisible(FALSE);
	}

	double dAreaWidth = m_dEditWidth / m_nEditColNum;
	double dAreaHeight = m_dEditHeight / m_nEditRowNum;

	for (int iRow = 0; iRow < m_nEditRowNum; iRow++)
	{
		for (int iCol = 0; iCol < m_nEditColNum; iCol++)
		{
			int nGuiIndex = iRow * m_nEditColNum + iCol;
			double dStartX = iCol * dAreaWidth + m_dEditStartX;
			double dStartY = iRow * dAreaHeight + m_dEditStartY;

			m_GuiAreaRect[nGuiIndex].SetRect(scRect(dStartX, dStartY, dAreaWidth, dAreaHeight));
			m_GuiAreaRect[nGuiIndex].SetVisible(TRUE);
		}

	}

	m_vpGuiDisplay[0]->MyInvalidate();
}

void CDlgAlignOffsetByAreaSet::UpdateSearchRectToGui()
{
	UpdateData(TRUE);
	m_GuiSearchRect.SetRect(scRect(m_dEditStartX, m_dEditStartY, m_dEditWidth, m_dEditHeight));

	UpdateAreaRectShow();
}


void CDlgAlignOffsetByAreaSet::UpdateSearchRectFromGui()
{
	m_GuiSearchRect.GetXYWidthHeight(m_dEditStartX, m_dEditStartY, m_dEditWidth, m_dEditHeight);
	UpdateData(FALSE);

	UpdateAreaRectShow();
}

void CDlgAlignOffsetByAreaSet::UpdateGuiStyle()
{
	//m_GuiSearchRect.SetLineWidth(m_nEditLineWidth);
	//m_GuiSearchRect.SetLineColor(m_clrNormal);

	for (int i = 0; i < MAX_AREA_RECT_NUM; i++)
	{
		m_GuiAreaRect[i].SetLineWidth(m_nEditLineWidth);
		m_GuiAreaRect[i].SetLineColor(m_clrNormal);
	}
}

void CDlgAlignOffsetByAreaSet::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
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

void CDlgAlignOffsetByAreaSet::UpdateDlgData(bool bSaveAndValidate)
{
	if (!m_pVisionASM)
	{
		return;
	}

	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
		CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
		if (!pProduct)
		{
			return;
		}

		AlignOffsetByAera alignOffsetByArea = pProduct->m_pAlignerParam->GetAlignOffsetByArea();
		AlignOffsetByAreaGui alignOffsetByAreaGui = m_pVisionASM->m_pSystempOptionConfig->m_AlignOffsetAreaGui;


		alignOffsetByArea.m_bEnable = m_bChkAlignOffsetByAreaEnable;
		alignOffsetByArea.m_nAreaRow = m_nEditRowNum;
		alignOffsetByArea.m_nAreaCol = m_nEditColNum;
		alignOffsetByArea.m_nSearchType = m_nRdSearchType;
		alignOffsetByArea.m_dSearchStartX = m_dEditStartX;
		alignOffsetByArea.m_dSearchStartY = m_dEditStartY;
		alignOffsetByArea.m_dSearchWidth = m_dEditWidth;
		alignOffsetByArea.m_dSearchHeight = m_dEditHeight;

		alignOffsetByAreaGui.m_nLineWidth = m_nEditLineWidth;
		alignOffsetByAreaGui.m_nLightLineWidth = m_nEditLightLineWidth;
		alignOffsetByAreaGui.m_clrNormal = m_clrNormal;
		alignOffsetByAreaGui.m_clrLight = m_clrLight;



		pProduct->m_pAlignerParam->SetAlignOffsetByArea(alignOffsetByArea);
		m_pVisionASM->m_pSystempOptionConfig->m_AlignOffsetAreaGui = alignOffsetByAreaGui;
	}
	else
	{
		int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
		CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
		if (!pProduct)
		{
			return;
		}

		AlignOffsetByAera alignOffsetByArea = pProduct->m_pAlignerParam->GetAlignOffsetByArea();

		AlignOffsetByAreaGui alignOffsetByAreaGui = m_pVisionASM->m_pSystempOptionConfig->m_AlignOffsetAreaGui;


		m_bChkAlignOffsetByAreaEnable = alignOffsetByArea.m_bEnable;
		m_nEditRowNum = alignOffsetByArea.m_nAreaRow;
		m_nEditColNum = alignOffsetByArea.m_nAreaCol;
		m_dEditStartX = alignOffsetByArea.m_dSearchStartX;
		m_dEditStartY = alignOffsetByArea.m_dSearchStartY;
		m_dEditWidth = alignOffsetByArea.m_dSearchWidth;
		m_dEditHeight = alignOffsetByArea.m_dSearchHeight;
		m_nRdSearchType = alignOffsetByArea.m_nSearchType;

		m_nEditLineWidth = alignOffsetByAreaGui.m_nLineWidth;
		m_nEditLightLineWidth = alignOffsetByAreaGui.m_nLightLineWidth;
		m_clrNormal = alignOffsetByAreaGui.m_clrNormal;
		m_clrLight = alignOffsetByAreaGui.m_clrLight;

		UpdateData(FALSE);

		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_IN, m_clrNormal);
		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_FOCUS, m_clrNormal);
		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_OUT, m_clrNormal);

		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_IN, m_clrLight);
		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_FOCUS, m_clrLight);
		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_OUT, m_clrLight);

		UpdateSearchRectToGui();
		UpdateAreaRectShow();
		UpdateGuiStyle();
	}
}

void CDlgAlignOffsetByAreaSet::OnCamGrab(bool bStart /*= true*/)
{
	int i=0;
	BOOL bSucceed = TRUE;
	int nCamNum = m_pVisionASM->GetPosNum(); 
	std::vector<BOOL> vbSucceed;
	if (m_bIsGrabbing)
	{
		for (i=0; i<nCamNum && i<1; i++)
		{
			vbSucceed.push_back(m_pVisionASM->FreezePosCamera(i));
		}
		for (i=0; i<nCamNum && i<1; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			for (i=0; i<nCamNum && i<1; i++)	
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

		m_bIsGrabbing = TRUE;
	}
}


void CDlgAlignOffsetByAreaSet::RestoreStatus()
{
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
	m_pVisionASM = NULL;
}

void CDlgAlignOffsetByAreaSet::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}


void CDlgAlignOffsetByAreaSet::UpdateDialogLanguage()
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
		strXMLPath = strCurWorkDir + _T("\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle"));    
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_DLG_ALIGN_OFFSET_BY_AREA_SET") == strDlgID)
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

int CDlgAlignOffsetByAreaSet::GetLanguageType()
{
	int iLanguageType = 0;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			iLanguageType = 0;
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			iLanguageType = 1;
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			iLanguageType = 1;
			break;
		}
	}
	return iLanguageType;
}

void CDlgAlignOffsetByAreaSet::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CleanDisplayUI();

	RestoreStatus();

	if (m_pStatusBar)
	{
		m_pStatusBar->DestroyWindow();
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}

}


LRESULT CDlgAlignOffsetByAreaSet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);
		}
		break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


LRESULT CDlgAlignOffsetByAreaSet::OnDisplayDragStart(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


LRESULT CDlgAlignOffsetByAreaSet::OnDisplayDragStop(WPARAM wParam, LPARAM lParam)
{
	if (m_vpGuiDisplay.size() > 0 && m_vpGuiDisplay[0])
	{
		if (m_GuiSearchRect.IsSelected())
		{
			UpdateSearchRectFromGui();
			UpdateAreaRectShow();
		}
	}

	return 0;
}

void CDlgAlignOffsetByAreaSet::OnEnChangeEditRowNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	int nPreRowNum = m_nEditRowNum;
	UpdateData(TRUE);
	if (m_nEditRowNum == 0)
	{
		m_nEditRowNum = nPreRowNum;
		UpdateData(FALSE);
	}

	if (m_nEditRowNum * m_nEditColNum > 25)
	{
		MessageBox(_T("不支持区域数超过25"));
		m_nEditRowNum = nPreRowNum;
		UpdateData(FALSE);
	}

	UpdateAreaRectShow();
}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditColNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	int nPreColNum = m_nEditColNum;
	UpdateData(TRUE);
	if (m_nEditColNum == 0)
	{
		m_nEditColNum = nPreColNum;
		UpdateData(FALSE);
	}

	if (m_nEditRowNum * m_nEditColNum > 25)
	{
		MessageBox(_T("不支持区域数超过25"));
		m_nEditColNum = nPreColNum;
		UpdateData(FALSE);
	}

	UpdateAreaRectShow();
}


void CDlgAlignOffsetByAreaSet::OnBnClickedBtnNormalColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_clrNormal = dlg.GetColor();

		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_IN, m_clrNormal);
		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_FOCUS, m_clrNormal);
		m_btnNormalColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_OUT, m_clrNormal);

		UpdateGuiStyle();
	}
}


void CDlgAlignOffsetByAreaSet::OnBnClickedBtnLightColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_clrLight = dlg.GetColor();
		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_IN, m_clrLight);
		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_FOCUS, m_clrLight);
		m_btnLightColor.SetColor(CXPStyleButtonST::BTNST_COLOR_BK_OUT, m_clrLight);

		UpdateGuiStyle();
	}
}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditLineWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	int nLineWidth = m_nEditLineWidth;
	UpdateData(TRUE);
	if (m_nEditLineWidth == 0)
	{
		m_nEditLineWidth = nLineWidth;
		UpdateData(FALSE);
	}

	UpdateGuiStyle();

}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditLightLineWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	int nLineWidth = m_nEditLightLineWidth;
	UpdateData(TRUE);
	if (m_nEditLightLineWidth == 0)
	{
		m_nEditLightLineWidth = nLineWidth;
		UpdateData(FALSE);
	}

}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditStartX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateSearchRectToGui();
}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditStartY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateSearchRectToGui();

}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditHeight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	double dVal = m_dEditHeight;
	UpdateData(TRUE);
	if (m_dEditHeight <=10)
	{
		m_dEditHeight = dVal;
		UpdateData(FALSE);
	}
	UpdateSearchRectToGui();

}


void CDlgAlignOffsetByAreaSet::OnEnChangeEditWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	double dVal = m_dEditWidth;
	UpdateData(TRUE);
	if (m_dEditWidth <= 10)
	{
		m_dEditWidth = dVal;
		UpdateData(FALSE);
	}

	UpdateSearchRectToGui();
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditRowNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ROW_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ROW_NUM, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ROW_NUM, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditColNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COL_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COL_NUM, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COL_NUM, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditStartX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_START_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_START_X, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_START_X, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditStartY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_START_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_START_Y, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_START_Y, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditHeight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_HEIGHT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_HEIGHT, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_HEIGHT, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WIDTH, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WIDTH, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LINE_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LINE_WIDTH, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LINE_WIDTH, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnEnSetfocusEditLightLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKeyboardEnable)
	{
		::PostMessage(m_pParentWnd->m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LIGHT_LINE_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LIGHT_LINE_WIDTH, strInput);

		if (TRUE == m_KeyBoard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LIGHT_LINE_WIDTH, strOutput);
		}
	}
}


void CDlgAlignOffsetByAreaSet::OnBnClickedBtnGrabImage()
{
	// TODO: 在此添加控件通知处理程序代码

	m_strStatusBarInfo2 = _T("");
	if (m_pVisionASM != NULL)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
		{
			if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
			{

				if (m_pVisionASM->FreezePosCamera(m_nCurPositionIdx))
				{
					cpImage* pImageTemp = (cpImage*)m_guiDisplay.GetImageCP();
					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")

					//UpdateGuiDisplay();
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

				if (m_pVisionASM->GrabPosCamera(m_nCurPositionIdx))
				{
					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")

					//UpdateGuiDisplay();
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

	CString strSelText = _T("");

	UpdateStatusBar();
}
