// CDlgCornerSearchCheckInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCornerSearchCheckInspectTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgCornerSearchCheckInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgCornerSearchCheckInspectTool, CDialog)

CDlgCornerSearchCheckInspectTool::CDlgCornerSearchCheckInspectTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCornerSearchCheckInspectTool::IDD, pParent)
	, m_nSearchToolTypeIndex(0)
	, m_nInspectTypeIndex(0)
	, m_bUseInspectTool(FALSE)
	, m_bInspectAfterCorner(FALSE)
{
	
	m_TabIndex=0;
	m_bEnableCorner=false;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;
	
}

CDlgCornerSearchCheckInspectTool::~CDlgCornerSearchCheckInspectTool()
{
}

void CDlgCornerSearchCheckInspectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_cTabCtl);
	DDX_Control(pDX, IDC_BTN_MARK_TRAIN, m_btnMarkTrain);
	DDX_Control(pDX, IDC_BTN_MARK_GETTRAINIMAGE, m_btnMarkGetTrainImage);
	DDX_CBIndex(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_nSearchToolTypeIndex);
	DDX_CBIndex(pDX, IDC_CMB_INSPECT_TYPE, m_nInspectTypeIndex);
	DDX_Control(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_cmbSearchToolType);
	DDX_Control(pDX, IDC_CMB_INSPECT_TYPE, m_cmbInspectType);
	DDX_Check(pDX, IDC_CHK_USEINSPECT, m_bUseInspectTool);
	DDX_Check(pDX, IDC_CHK_INSPECTAFTERCORNER, m_bInspectAfterCorner);
	DDX_Check(pDX, IDC_CHECK_ENABLE_CORNER, m_bEnableCorner);
}


BEGIN_MESSAGE_MAP(CDlgCornerSearchCheckInspectTool, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgCornerSearchCheckInspectTool::OnTcnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_CMB_SEARCHTOOL_TYPE, &CDlgCornerSearchCheckInspectTool::OnCbnSelchangeCmbSearchtoolType)
	ON_CBN_SELCHANGE(IDC_CMB_INSPECT_TYPE, &CDlgCornerSearchCheckInspectTool::OnCbnSelchangeCmbInspectType)
	ON_BN_CLICKED(IDC_BTN_MARK_GETTRAINIMAGE, &CDlgCornerSearchCheckInspectTool::OnBnClickedBtnMarkGettrainimage)
	ON_BN_CLICKED(IDC_BTN_MARK_TRAIN, &CDlgCornerSearchCheckInspectTool::OnBnClickedBtnMarkTrain)
	ON_BN_CLICKED(IDC_CHK_USEINSPECT, &CDlgCornerSearchCheckInspectTool::OnBnClickedChkUseinspect)
//	ON_BN_SETFOCUS(IDC_CHECK_ENABLE_CORNER, &CDlgCornerSearchCheckInspectTool::OnBnSetfocusCheckEnableCorner)
ON_BN_CLICKED(IDC_CHECK_ENABLE_CORNER, &CDlgCornerSearchCheckInspectTool::OnBnClickedCheckEnableCorner)
END_MESSAGE_MAP()


// CDlgCornerSearchCheckInspectTool 消息处理程序
void CDlgCornerSearchCheckInspectTool::RemoveImage()
{
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgCornerSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgQuickSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgPatternSearchTool.m_GuiPatternDisplay.RemoveImage();
}
void CDlgCornerSearchCheckInspectTool::SetGuiPatternDisplayImage(BOOL bCopy)
{
	
}

void CDlgCornerSearchCheckInspectTool::ClearScreen()
{
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgCornerSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgQuickSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgPatternSearchTool.m_GuiPatternDisplay.ClearScreen();
}
void CDlgCornerSearchCheckInspectTool::UpdateDialogLanguage()
{

	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_pDlgParent->m_strCurWorkDir;

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

		if(_T("IDD_DLG_CORNERSEARCH_CHECK_INSPECT_TOOL") == strDlgID)
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
void  CDlgCornerSearchCheckInspectTool::UpdateDlgItem()
{
	UpdateData(TRUE);

	m_TabIndex=m_cTabCtl.GetCurSel();

	switch(m_TabIndex)
	{
		case 0:
			GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(TRUE);
			if (m_nSearchToolTypeIndex==0)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
			}
			else if (m_nSearchToolTypeIndex==1)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(TRUE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
			}
			break;
		case 1:
			GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(FALSE);
			if (m_nInspectTypeIndex==0)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(TRUE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);

			}
			else if (m_nInspectTypeIndex==1)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(TRUE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
			}
			break;
		case 2:
			{
				m_DlgLineToLineDistanceTool1.ShowWindow(TRUE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);

				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(FALSE);

			}
			break;
		case 3:
			{
				m_DlgLineToLineDistanceTool2.ShowWindow(TRUE);
				m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);

				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(FALSE);
			}
			break;
		case 4:
			{
				m_DlgCircleToLineDistanceTool.ShowWindow(TRUE);
				m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
				m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgPatternSearchTool.ShowWindow(FALSE);

				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(FALSE);
			}
			break;
		default:
			m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
			m_DlgQuickSearchTool.ShowWindow(FALSE);
			m_DlgCornerSearchTool.ShowWindow(FALSE);
			m_DlgPatternSearchTool.ShowWindow(FALSE);
			m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);
			m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);
			m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_USEINSPECT)->ShowWindow(TRUE);
			break;

	}

	
	
}
void  CDlgCornerSearchCheckInspectTool::InitDlgItem()
{

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	m_cTabCtl.InsertItem(0,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CORNER_SEARCH_TOOL));
	m_cTabCtl.InsertItem(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CHECK_TOOL));
	m_cTabCtl.InsertItem(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_DISTANCE_INSPECT_TOOL1));
	m_cTabCtl.InsertItem(3,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_DISTANCE_INSPECT_TOOL2));
	m_cTabCtl.InsertItem(4,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_DISTANCE_INSPECT_TOOL3));
	m_cTabCtl.SetCurSel(0);

	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_PATTERN_CORNER_SEARCH_TOOL));
	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_QUICK_CORNER_SEARCH_TOOL));
	m_cmbSearchToolType.SetCurSel(0);
	m_cmbInspectType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_PATTERN_SEARCH_TOOL));
	m_cmbInspectType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_QUICK_SEARCH_TOOL));
	m_cmbInspectType.SetCurSel(0);

	CRect rect;
	m_cTabCtl.GetClientRect(rect);

	GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(FALSE);

	int nIntendY = 57;
	rect.top+=nIntendY;
	rect.left-=2;
	rect.right+=2;

	CRect rect2;
	rect2 = rect;
	rect2.top -=20;
	m_DlgLineToLineDistanceTool1.m_pDlgParent=m_pDlgParent;
	m_DlgLineToLineDistanceTool1.m_pDlgParent2 = this;

	m_DlgLineToLineDistanceTool1.Create(IDD_DLG_LINE_TO_LINE_DISTANCE_INSPECT,&m_cTabCtl);
	m_DlgLineToLineDistanceTool1.MoveWindow(rect2);
	m_DlgLineToLineDistanceTool1.ShowWindow(FALSE);

	m_DlgLineToLineDistanceTool2.m_pDlgParent=m_pDlgParent;
	m_DlgLineToLineDistanceTool2.m_pDlgParent2 = this;
	m_DlgLineToLineDistanceTool2.Create(IDD_DLG_LINE_TO_LINE_DISTANCE_INSPECT,&m_cTabCtl);
	m_DlgLineToLineDistanceTool2.MoveWindow(rect2);
	m_DlgLineToLineDistanceTool2.ShowWindow(FALSE);

	m_DlgCircleToLineDistanceTool.m_pDlgParent=m_pDlgParent;
	m_DlgCircleToLineDistanceTool.m_pDlgParent2 = this;
	m_DlgCircleToLineDistanceTool.Create(IDD_DLG_CIRCLE_TO_LINE_DISTANCE_INSPECT,&m_cTabCtl);
	m_DlgCircleToLineDistanceTool.MoveWindow(rect2);
	m_DlgCircleToLineDistanceTool.ShowWindow(FALSE);

	m_DlgCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgCornerSearchTool.Create(IDD_DLG_CORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgCornerSearchTool.MoveWindow(rect);
	m_DlgCornerSearchTool.ShowWindow(FALSE);

	m_DlgPatternCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgPatternCornerSearchTool.Create(IDD_DLG_PATTERNCORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgPatternCornerSearchTool.MoveWindow(rect);
	m_DlgPatternCornerSearchTool.ShowWindow(TRUE);


	m_DlgQuickSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgQuickSearchTool.Create(IDD_DLG_QUICKSEARCH_TOOL,&m_cTabCtl);
	m_DlgQuickSearchTool.MoveWindow(rect);
	m_DlgQuickSearchTool.ShowWindow(FALSE);

	m_DlgPatternSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgPatternSearchTool.Create(IDD_DLG_PATTERNSEARCH_TOOL,&m_cTabCtl);
	m_DlgPatternSearchTool.MoveWindow(rect);
	m_DlgPatternSearchTool.ShowWindow(FALSE);

	

	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	//(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);

	(m_DlgLineToLineDistanceTool1.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgLineToLineDistanceTool1.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);

	(m_DlgLineToLineDistanceTool2.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgLineToLineDistanceTool2.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);

	(m_DlgCircleToLineDistanceTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgCircleToLineDistanceTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);

//	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);

}
void CDlgCornerSearchCheckInspectTool::CheckDlgDataValidation()
{
	m_DlgPatternCornerSearchTool.CheckDlgDataValidation();
	m_DlgCornerSearchTool.CheckDlgDataValidation();
	m_DlgPatternSearchTool.CheckDlgDataValidation();
	m_DlgQuickSearchTool.CheckDlgDataValidation();
    m_DlgLineToLineDistanceTool1.CheckDlgDataValidation();
	m_DlgLineToLineDistanceTool2.CheckDlgDataValidation();
    m_DlgCircleToLineDistanceTool.CheckDlgDataValidation();
}
void CDlgCornerSearchCheckInspectTool::UpdateControlDisplay()
{

	m_DlgPatternSearchTool.UpdateControlDisplay();
	m_DlgQuickSearchTool.UpdateControlDisplay();
	m_DlgPatternCornerSearchTool.UpdateControlDisplay();
	m_DlgCornerSearchTool.UpdateControlDisplay();
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_INSPECT_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
		
	}
	else
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_INSPECT_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(TRUE);

		

	}
	
}

BOOL CDlgCornerSearchCheckInspectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnMarkGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnMarkTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	InitDlgItem();
	// TODO:  在此添加额外的初始化
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgCornerSearchCheckInspectTool::UpdateTrainData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		CSearchToolType tSearchToolType;
		CSearchToolType tInspectToolType;

		if (m_nSearchToolTypeIndex==0)
		{
			tSearchToolType=ePatternCornerSearchTool;
		}
		else if (m_nSearchToolTypeIndex==1)
		{
			tSearchToolType=eCornerSearchTool;
		}
		if (m_nInspectTypeIndex==0)
		{
			tInspectToolType=ePatternSearchTool;
		}
		else if (m_nInspectTypeIndex==1)
		{
			tInspectToolType=eQuickSearchTool;
		}
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetSubSearchToolType(tSearchToolType);
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetSubInspectToolType(tInspectToolType);
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck=m_bUseInspectTool;
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCorner = m_bEnableCorner;
	/*	if (!m_bUseInspectTool)
		{
			m_cmbInspectType.EnableWindow(FALSE);
			m_DlgPatternSearchTool.ShowWindow(FALSE);
			m_DlgQuickSearchTool.ShowWindow(FALSE);
		}
		else
		{
			m_cmbInspectType.EnableWindow(TRUE);
			m_DlgPatternSearchTool.ShowWindow(TRUE);
			m_DlgQuickSearchTool.ShowWindow(TRUE);
		}*/
 		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgPatternCornerSearchTool.m_nImageChannel;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_DlgPatternCornerSearchTool.m_nPatAccuracyMode);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_DlgPatternCornerSearchTool.m_nPatSpeedMode+1);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgPatternCornerSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
			/*	if (m_DlgPatternCornerSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgPatternCornerSearchTool.m_imageMask);
				}*/
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  = m_DlgPatternCornerSearchTool.m_dAngleLow;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh =m_DlgPatternCornerSearchTool.m_dAngleHigh;

				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgPatternCornerSearchTool.m_nLineOutLineNum1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgPatternCornerSearchTool.m_dLineContrastThre1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgPatternCornerSearchTool.m_bSortByScore1;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_DlgPatternCornerSearchTool.m_bEnableDualEdge;
                ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_1;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_DlgPatternCornerSearchTool.m_bShowCaliper_1;

				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] =m_DlgPatternCornerSearchTool.m_nLineOutLineNum2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgPatternCornerSearchTool.m_dLineContrastThre2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgPatternCornerSearchTool.m_bSortByScore2;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_DlgPatternCornerSearchTool.m_bEnableDualEdge2;
                ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_2;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_DlgPatternCornerSearchTool.m_bShowCaliper_2;
			}
			break;
		case 1:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgCornerSearchTool.m_nImageChannel;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_DlgCornerSearchTool.m_dGrau;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_DlgCornerSearchTool.m_lNoiseThre;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_DlgCornerSearchTool.m_lTotalMag;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgCornerSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
			/*	if (m_DlgCornerSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgCornerSearchTool.m_imageMask);
				}*/
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgCornerSearchTool.m_nLineOutLineNum1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgCornerSearchTool.m_dLineContrastThre1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgCornerSearchTool.m_bSortByScore1;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgCornerSearchTool.m_nLineCaliperNum1;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_DlgCornerSearchTool.m_bEnableDualEdge;
                ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_DlgCornerSearchTool.m_bEnableLineMask_1;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_DlgCornerSearchTool.m_bShowCaliper_1;

				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_DlgCornerSearchTool.m_nLineOutLineNum2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgCornerSearchTool.m_dLineContrastThre2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgCornerSearchTool.m_bSortByScore2;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgCornerSearchTool.m_nLineCaliperNum2;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_DlgCornerSearchTool.m_bEnableDualEdge2;
                 ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_DlgCornerSearchTool.m_bEnableLineMask_2;
				((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_DlgCornerSearchTool.m_bShowCaliper_2;

			}
			break;
		}
		switch(m_nInspectTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1 = (Accuracy)(1<<m_DlgPatternSearchTool.m_nPatAccuracyMode);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1 = (CoarseScale)(m_DlgPatternSearchTool.m_nPatSpeedMode+1);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1 = m_DlgPatternSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
			/*	if (m_DlgPatternSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage2(m_DlgPatternSearchTool.m_imageMask);
				}*/
				
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1  =  m_DlgPatternSearchTool.m_dAngleLow;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1 = m_DlgPatternSearchTool.m_dAngleHigh;

			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau1 = m_DlgQuickSearchTool.m_dGrau; 
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre1 = m_DlgQuickSearchTool.m_lNoiseThre;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag1 = m_DlgQuickSearchTool.m_lTotalMag;
				((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1 = m_DlgQuickSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
			/*	if (m_DlgQuickSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgQuickSearchTool.m_imageMask);
				}*/
			}
			break;
		}

		//获取线距检测工具的GUI 参数
		m_DlgLineToLineDistanceTool1.UpdateData(TRUE);
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[0] = m_DlgLineToLineDistanceTool1.m_bEnableLineDistanceTool;
		if (m_DlgLineToLineDistanceTool1.m_bEnableLineDistanceTool)
		{
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[0]  = m_DlgLineToLineDistanceTool1.m_nLineOutLineNum1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[0] = m_DlgLineToLineDistanceTool1.m_dLineContrastThre1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[0] = m_DlgLineToLineDistanceTool1.m_lLineFilterHalfWidth1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[0] = (EdgePolarity)m_DlgLineToLineDistanceTool1.m_nLinePolarityMode1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[0] = m_DlgLineToLineDistanceTool1.m_bSortByScore1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[0] = m_DlgLineToLineDistanceTool1.m_nLineCaliperNum1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_DlgLineToLineDistanceTool1.m_bEnableLineMask_1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[0] = m_DlgLineToLineDistanceTool1.m_bEnableDualEdge;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[0] = m_DlgLineToLineDistanceTool1.m_bShowCaliper_1;

			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[1] =m_DlgLineToLineDistanceTool1.m_nLineOutLineNum2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[1] = m_DlgLineToLineDistanceTool1.m_dLineContrastThre2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[1] = m_DlgLineToLineDistanceTool1.m_lLineFilterHalfWidth2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[1] = (EdgePolarity)m_DlgLineToLineDistanceTool1.m_nLinePolarityMode2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[1] = m_DlgLineToLineDistanceTool1.m_bSortByScore2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[1] = m_DlgLineToLineDistanceTool1.m_nLineCaliperNum2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_DlgLineToLineDistanceTool1.m_bEnableLineMask_2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[1] = m_DlgLineToLineDistanceTool1.m_bEnableDualEdge2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[1] = m_DlgLineToLineDistanceTool1.m_bShowCaliper_2;

		}
		
		m_DlgLineToLineDistanceTool2.UpdateData(TRUE);
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[1] = m_DlgLineToLineDistanceTool2.m_bEnableLineDistanceTool;
		if (m_DlgLineToLineDistanceTool2.m_bEnableLineDistanceTool)
		{
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[2]  = m_DlgLineToLineDistanceTool2.m_nLineOutLineNum1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[2] = m_DlgLineToLineDistanceTool2.m_dLineContrastThre1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[2] = m_DlgLineToLineDistanceTool2.m_lLineFilterHalfWidth1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[2] = (EdgePolarity)m_DlgLineToLineDistanceTool2.m_nLinePolarityMode1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[2] = m_DlgLineToLineDistanceTool2.m_bSortByScore1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[2] = m_DlgLineToLineDistanceTool2.m_nLineCaliperNum1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[2] = m_DlgLineToLineDistanceTool2.m_bEnableLineMask_1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[2] = m_DlgLineToLineDistanceTool2.m_bEnableDualEdge;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[2] = m_DlgLineToLineDistanceTool2.m_bShowCaliper_1;

			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[3] =m_DlgLineToLineDistanceTool2.m_nLineOutLineNum2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[3] = m_DlgLineToLineDistanceTool2.m_dLineContrastThre2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[3] = m_DlgLineToLineDistanceTool2.m_lLineFilterHalfWidth2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[3] = (EdgePolarity)m_DlgLineToLineDistanceTool2.m_nLinePolarityMode2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[3] = m_DlgLineToLineDistanceTool2.m_bSortByScore2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[3] = m_DlgLineToLineDistanceTool2.m_nLineCaliperNum2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[3] = m_DlgLineToLineDistanceTool2.m_bEnableLineMask_2;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[3] = m_DlgLineToLineDistanceTool2.m_bEnableDualEdge2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[3] = m_DlgLineToLineDistanceTool2.m_bShowCaliper_2;
		}

		m_DlgCircleToLineDistanceTool.UpdateData(TRUE);
		
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[2] = m_DlgCircleToLineDistanceTool.m_bEnableLineDistanceTool;
		((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[3] = m_DlgCircleToLineDistanceTool.m_bEnableCircleDistanceTool;//m_bEnableCircleDistanceTool;
		if (m_DlgCircleToLineDistanceTool.m_bEnableLineDistanceTool || m_DlgCircleToLineDistanceTool.m_bEnableCircleDistanceTool)														 
		{
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[4]  = m_DlgCircleToLineDistanceTool.m_nLineOutLineNum1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[4] = m_DlgCircleToLineDistanceTool.m_dLineContrastThre1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[4] = m_DlgCircleToLineDistanceTool.m_lLineFilterHalfWidth1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[4] = (EdgePolarity)m_DlgCircleToLineDistanceTool.m_nLinePolarityMode1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[4] = m_DlgCircleToLineDistanceTool.m_bSortByScore1;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[4] = m_DlgCircleToLineDistanceTool.m_bEnableDualEdge;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore2 = m_DlgCircleToLineDistanceTool.m_bSortByScore2;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[4] = m_DlgCircleToLineDistanceTool.m_nLineCaliperNum1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[4] = m_DlgCircleToLineDistanceTool.m_bEnableLineMask_1;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[4] = m_DlgCircleToLineDistanceTool.m_bShowCaliper_1;

			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum = m_DlgCircleToLineDistanceTool.m_nCircleOutLineNum;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre = m_DlgCircleToLineDistanceTool.m_dCircleContrastThre;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth = m_DlgCircleToLineDistanceTool.m_lCircleFilterHalfWidth;
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty = (EdgePolarity)m_DlgCircleToLineDistanceTool.m_nCirclePolarityMode;		
			((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCircleDualEdge = m_DlgCircleToLineDistanceTool.m_bEnableCircleDualEdge;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum = m_DlgCircleToLineDistanceTool.m_nCircleCaliperNum;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_DlgCircleToLineDistanceTool.m_nCircleInwardMode;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible = m_DlgCircleToLineDistanceTool.m_bShowCircleCaliper;
			((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask = m_DlgCircleToLineDistanceTool.m_bEnableCircleMask;
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_bEnableCorner=((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCorner;
		if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetSubSearchToolType()==ePatternCornerSearchTool)
		{
			m_nSearchToolTypeIndex=0;
		}
		else if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetSubSearchToolType()==eCornerSearchTool)
		{
			m_nSearchToolTypeIndex=1;
		}
		if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetInspectToolType()==ePatternSearchTool)
		{
			m_nInspectTypeIndex=0;
		}
		else if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetInspectToolType()==eQuickSearchTool)
		{
			m_nInspectTypeIndex=1;
		}
		m_bUseInspectTool=((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck;
		UpdateData(FALSE);
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.m_nImageChannel = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
				m_DlgPatternCornerSearchTool.m_nPatAccuracyMode = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
				m_DlgPatternCornerSearchTool.m_nPatSpeedMode = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
				m_DlgPatternCornerSearchTool.m_dAngleLow	    = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
				m_DlgPatternCornerSearchTool.m_dAngleHigh	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
				m_DlgPatternCornerSearchTool.m_bEnableMask = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

				m_DlgPatternCornerSearchTool.m_nLineOutLineNum1 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
				m_DlgPatternCornerSearchTool.m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
				m_DlgPatternCornerSearchTool.m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
				m_DlgPatternCornerSearchTool.m_bEnableDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];
                m_DlgPatternCornerSearchTool.m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0];
				m_DlgPatternCornerSearchTool.m_bShowCaliper_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];

				m_DlgPatternCornerSearchTool.m_nLineOutLineNum2 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgPatternCornerSearchTool.m_dLineContrastThre2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgPatternCornerSearchTool.m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgPatternCornerSearchTool.m_bEnableDualEdge2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];
                m_DlgPatternCornerSearchTool.m_bEnableLineMask_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1];
				m_DlgPatternCornerSearchTool.m_bShowCaliper_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];

				if (m_DlgPatternCornerSearchTool.m_bEnableMask)
				{
					m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgPatternCornerSearchTool.m_comboAccuracy.SetCurSel(m_DlgPatternCornerSearchTool.m_nPatAccuracyMode);
				m_DlgPatternCornerSearchTool.m_comboSpeed.SetCurSel(m_DlgPatternCornerSearchTool.m_nPatSpeedMode);
				m_DlgPatternCornerSearchTool.m_checkSortByScore1.SetCheck(m_DlgPatternCornerSearchTool.m_bSortByScore1);
				m_DlgPatternCornerSearchTool.m_checkSortByScore2.SetCheck(m_DlgPatternCornerSearchTool.m_bSortByScore2);
				m_DlgPatternCornerSearchTool.UpdateData(FALSE);

			}
			break;
		case 1:
			{
				m_DlgCornerSearchTool.m_nImageChannel= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
				m_DlgCornerSearchTool.m_dGrau= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
				m_DlgCornerSearchTool.m_lNoiseThre	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
				m_DlgCornerSearchTool.m_lTotalMag		= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
				m_DlgCornerSearchTool.m_bEnableMask	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

				m_DlgCornerSearchTool.m_nLineOutLineNum1 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
				m_DlgCornerSearchTool.m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
				m_DlgCornerSearchTool.m_nLinePolarityMode1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
				m_DlgCornerSearchTool.m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
				m_DlgCornerSearchTool.m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
				m_DlgCornerSearchTool.m_bEnableDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];
                m_DlgCornerSearchTool.m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0];
				m_DlgCornerSearchTool.m_bShowCaliper_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
															
				m_DlgCornerSearchTool.m_nLineOutLineNum2 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgCornerSearchTool.m_dLineContrastThre2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgCornerSearchTool.m_nLinePolarityMode2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgCornerSearchTool.m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				m_DlgCornerSearchTool.m_nLineCaliperNum2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgCornerSearchTool.m_bEnableDualEdge2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];
                m_DlgCornerSearchTool.m_bEnableLineMask_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1];
				m_DlgCornerSearchTool.m_bShowCaliper_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];

				if (m_DlgCornerSearchTool.m_bEnableMask)
				{
					m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgCornerSearchTool.m_checkSortByScore1.SetCheck(m_DlgCornerSearchTool.m_bSortByScore1);
				m_DlgCornerSearchTool.m_checkSortByScore2.SetCheck(m_DlgCornerSearchTool.m_bSortByScore2);

				m_DlgCornerSearchTool.UpdateData(FALSE);
			}
			break;
		}
		switch(m_nInspectTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.m_nPatAccuracyMode = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1/2;
				m_DlgPatternSearchTool.m_nPatSpeedMode = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1-1;
				m_DlgPatternSearchTool.m_dAngleLow	    = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1;
				m_DlgPatternSearchTool.m_dAngleHigh	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1;
				m_DlgPatternSearchTool.m_bEnableMask = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1;				
				if (m_DlgPatternSearchTool.m_bEnableMask)
				{
					m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgPatternSearchTool.m_comboAccuracy.SetCurSel(m_DlgPatternSearchTool.m_nPatAccuracyMode);
				m_DlgPatternSearchTool.m_comboSpeed.SetCurSel(m_DlgPatternSearchTool.m_nPatSpeedMode);
				m_DlgPatternSearchTool.UpdateData(FALSE);

			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.m_dGrau			= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau1;
				m_DlgQuickSearchTool.m_lNoiseThre	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre1;
				m_DlgQuickSearchTool.m_lTotalMag		= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag1;
				m_DlgQuickSearchTool.m_bEnableMask	= ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1;
				if (m_DlgQuickSearchTool.m_bEnableMask)
				{
					m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgQuickSearchTool.UpdateData(FALSE);
			}
			break;
		}

		m_DlgLineToLineDistanceTool1.m_bEnableLineDistanceTool = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[0];
		//if(((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[0])
		{
			m_DlgLineToLineDistanceTool1.m_nLineOutLineNum1 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[0];
			m_DlgLineToLineDistanceTool1.m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[0];
			m_DlgLineToLineDistanceTool1.m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[0];
			m_DlgLineToLineDistanceTool1.m_nLinePolarityMode1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[0];
			m_DlgLineToLineDistanceTool1.m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[0];
			m_DlgLineToLineDistanceTool1.m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[0];
			m_DlgLineToLineDistanceTool1.m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];
			m_DlgLineToLineDistanceTool1.m_bShowCaliper_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[0];

			m_DlgLineToLineDistanceTool1.m_bEnableDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[0];
			m_DlgLineToLineDistanceTool1.m_nLineOutLineNum2 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[1];
			m_DlgLineToLineDistanceTool1.m_dLineContrastThre2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[1];
			m_DlgLineToLineDistanceTool1.m_lLineFilterHalfWidth2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[1];
			m_DlgLineToLineDistanceTool1.m_nLinePolarityMode2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[1];
			m_DlgLineToLineDistanceTool1.m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[1];
			m_DlgLineToLineDistanceTool1.m_nLineCaliperNum2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[1];
			m_DlgLineToLineDistanceTool1.m_bEnableLineMask_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
			m_DlgLineToLineDistanceTool1.m_bEnableDualEdge2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[1];
			m_DlgLineToLineDistanceTool1.m_bShowCaliper_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[1];

		}
		m_DlgLineToLineDistanceTool1.UpdateData(FALSE);
		m_DlgLineToLineDistanceTool2.m_bEnableLineDistanceTool = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[1];
		//if(((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[1])
		{
			m_DlgLineToLineDistanceTool2.m_nLineOutLineNum1 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[2];
			m_DlgLineToLineDistanceTool2.m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[2];
			m_DlgLineToLineDistanceTool2.m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[2];
			m_DlgLineToLineDistanceTool2.m_nLinePolarityMode1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[2];
			m_DlgLineToLineDistanceTool2.m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[2];
			m_DlgLineToLineDistanceTool2.m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[2];
			m_DlgLineToLineDistanceTool2.m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[2];
			m_DlgLineToLineDistanceTool2.m_bEnableDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[2];
			m_DlgLineToLineDistanceTool2.m_bShowCaliper_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[2];

			m_DlgLineToLineDistanceTool2.m_nLineOutLineNum2 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[3];
			m_DlgLineToLineDistanceTool2.m_dLineContrastThre2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[3];
			m_DlgLineToLineDistanceTool2.m_lLineFilterHalfWidth2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[3];
			m_DlgLineToLineDistanceTool2.m_nLinePolarityMode2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[3];
			m_DlgLineToLineDistanceTool2.m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[3];
			m_DlgLineToLineDistanceTool2.m_nLineCaliperNum2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[3];
			m_DlgLineToLineDistanceTool2.m_bEnableLineMask_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[3];
			m_DlgLineToLineDistanceTool2.m_bEnableDualEdge2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[3];
			m_DlgLineToLineDistanceTool2.m_bShowCaliper_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[3];
		}
		m_DlgLineToLineDistanceTool2.UpdateData(FALSE);

		m_DlgCircleToLineDistanceTool.UpdateData(TRUE);
		m_DlgCircleToLineDistanceTool.m_bEnableLineDistanceTool =((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[2];
		m_DlgCircleToLineDistanceTool.m_bEnableCircleDistanceTool =((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[3];
		//if (m_DlgCircleToLineDistanceTool.m_bEnableLineDistanceTool)
		{
			m_DlgCircleToLineDistanceTool.m_nLineOutLineNum1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum1[4];
			m_DlgCircleToLineDistanceTool.m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre1[4] ;
			m_DlgCircleToLineDistanceTool.m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth1[4];
			m_DlgCircleToLineDistanceTool.m_nLinePolarityMode1 = (int)((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty1[4];
			m_DlgCircleToLineDistanceTool.m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore1[4];
			m_DlgCircleToLineDistanceTool.m_bEnableDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge1[4];
			m_DlgCircleToLineDistanceTool.m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore2;//	
			m_DlgCircleToLineDistanceTool.m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum1[4];
			m_DlgCircleToLineDistanceTool.m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[4] ;
			m_DlgCircleToLineDistanceTool.m_bShowCaliper_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bInspectCaliperVisible[4];
	
			m_DlgCircleToLineDistanceTool.m_nCircleOutLineNum = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum ;
			m_DlgCircleToLineDistanceTool.m_dCircleContrastThre = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre;
			m_DlgCircleToLineDistanceTool.m_lCircleFilterHalfWidth = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth;
			m_DlgCircleToLineDistanceTool.m_nCirclePolarityMode = (int)((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty;		
			m_DlgCircleToLineDistanceTool.m_bEnableCircleDualEdge = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCircleDualEdge;
			m_DlgCircleToLineDistanceTool.m_nCircleCaliperNum = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum ;
			m_DlgCircleToLineDistanceTool.m_nCircleInwardMode = (int)((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection;
			m_DlgCircleToLineDistanceTool.m_bShowCircleCaliper = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible;
			m_DlgCircleToLineDistanceTool.m_bEnableCircleMask = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask;
		}
		m_DlgCircleToLineDistanceTool.UpdateData(FALSE);

	}

}
void CDlgCornerSearchCheckInspectTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCheckAfterSearch=m_bInspectAfterCorner;
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgPatternCornerSearchTool.m_nSearchNum;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgPatternCornerSearchTool.m_dAcceptThre;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_DlgPatternCornerSearchTool.m_dConfusionThre;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bLine2ForceVerticalLine1 = m_DlgPatternCornerSearchTool.m_bLine2ForceVerticalLine1;
				
			}
				break;
		case 1:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgCornerSearchTool.m_nSearchNum;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgCornerSearchTool.m_dAcceptThre;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_DlgCornerSearchTool.m_dAngleLow;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_DlgCornerSearchTool.m_dAngleHigh;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bLine2ForceVerticalLine1 = m_DlgCornerSearchTool.m_bLine2ForceVerticalLine1;


			}
			break;
		}

		switch(m_nInspectTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1 = m_DlgPatternSearchTool.m_nSearchNum;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1 = m_DlgPatternSearchTool.m_dAcceptThre;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1 = m_DlgPatternSearchTool.m_dConfusionThre;

			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.UpdateData(TRUE);
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1 = m_DlgQuickSearchTool.m_nSearchNum;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1 = m_DlgQuickSearchTool.m_dAcceptThre;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow1 = m_DlgQuickSearchTool.m_dAngleLow;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh1 = m_DlgQuickSearchTool.m_dAngleHigh;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow1 = m_DlgQuickSearchTool.m_dSizeLow;
				((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh1 = m_DlgQuickSearchTool.m_dSizeHigh;

			}
			break;
		}

		m_DlgLineToLineDistanceTool1.UpdateData(TRUE);
		//if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[0])
		{
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[0] = m_DlgLineToLineDistanceTool1.m_dPixelRatio;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode[0] = m_DlgLineToLineDistanceTool1.m_nDistanceMode;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType[0] = m_DlgLineToLineDistanceTool1.m_nDistanceType;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0] = m_DlgLineToLineDistanceTool1.m_bEnableMax1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0] = m_DlgLineToLineDistanceTool1.m_bEnableMin1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0] = m_DlgLineToLineDistanceTool1.m_dDistanceMax1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0] = m_DlgLineToLineDistanceTool1.m_dDistanceMin1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[0] = m_DlgLineToLineDistanceTool1.m_bEanbleOKNG_Decision;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0] = m_DlgLineToLineDistanceTool1.m_strDistanceLabel;

			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[0] = m_DlgLineToLineDistanceTool1.m_bShowFitPoint1;
			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[1] =m_DlgLineToLineDistanceTool1.m_bShowFitPoint2;
		}
		
		m_DlgLineToLineDistanceTool2.UpdateData(TRUE);
		//if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[1])
		{
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[1] = m_DlgLineToLineDistanceTool2.m_dPixelRatio;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode[1] = m_DlgLineToLineDistanceTool2.m_nDistanceMode;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType[1] = m_DlgLineToLineDistanceTool2.m_nDistanceType;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1] = m_DlgLineToLineDistanceTool2.m_bEnableMax1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1] = m_DlgLineToLineDistanceTool2.m_bEnableMin1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1] = m_DlgLineToLineDistanceTool2.m_dDistanceMax1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1] = m_DlgLineToLineDistanceTool2.m_dDistanceMin1;

			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[1] = m_DlgLineToLineDistanceTool2.m_bEanbleOKNG_Decision;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1] = m_DlgLineToLineDistanceTool2.m_strDistanceLabel;

			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[2] = m_DlgLineToLineDistanceTool2.m_bShowFitPoint1;
			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[3] =m_DlgLineToLineDistanceTool2.m_bShowFitPoint2;
		}

		m_DlgCircleToLineDistanceTool.UpdateData(TRUE);
		//if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableInspect[1])
		{
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[2] = m_DlgCircleToLineDistanceTool.m_dPixelRatio;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2] = m_DlgCircleToLineDistanceTool.m_bEnableMax1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2] = m_DlgCircleToLineDistanceTool.m_dDistanceMax1;

			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2] = m_DlgCircleToLineDistanceTool.m_bEnableMin1;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2] = m_DlgCircleToLineDistanceTool.m_dDistanceMin1;

			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[2] = m_DlgCircleToLineDistanceTool.m_bEanbleOKNG_Decision;
			((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2] = m_DlgCircleToLineDistanceTool.m_strDistanceLabel;
			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[4] = m_DlgCircleToLineDistanceTool.m_bShowFitPoint1;
			((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitCirclePoint = m_DlgCircleToLineDistanceTool.m_bEnableShowFitCirclePoint;
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
	
		m_bInspectAfterCorner=((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCheckAfterSearch;
		if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck)
		{
			GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(FALSE);
		}
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.m_nSearchNum	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgPatternCornerSearchTool.m_dAcceptThre	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
				m_DlgPatternCornerSearchTool.m_dConfusionThre = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;
				m_DlgPatternCornerSearchTool.m_bShowFitPoint_1 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1;
				m_DlgPatternCornerSearchTool.m_bShowFitPoint_2 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2;
				m_DlgPatternCornerSearchTool.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgCornerSearchTool.m_nSearchNum	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgCornerSearchTool.m_dAcceptThre	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
				m_DlgCornerSearchTool.m_dAngleLow		= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
				m_DlgCornerSearchTool.m_dAngleHigh	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
                m_DlgCornerSearchTool.m_bShowFitPoint_1	= ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1;
                m_DlgCornerSearchTool.m_bShowFitPoint_2	= ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2;
				m_DlgCornerSearchTool.UpdateData(FALSE);
			}
			break;
		}
		switch(m_nInspectTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.m_nSearchNum	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1;
				m_DlgPatternSearchTool.m_dAcceptThre	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1;	
				m_DlgPatternSearchTool.m_dConfusionThre = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1;
				m_DlgPatternSearchTool.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.m_nSearchNum	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1;
				m_DlgQuickSearchTool.m_dAcceptThre	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1;
				m_DlgQuickSearchTool.m_dAngleLow		= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow1;
				m_DlgQuickSearchTool.m_dAngleHigh	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh1;
				m_DlgQuickSearchTool.m_dSizeLow		= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow1;
				m_DlgQuickSearchTool.m_dSizeHigh	= ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh1;

				m_DlgQuickSearchTool.UpdateData(FALSE);
			}
			break;
		}

		//if (m_DlgLineToLineDistanceTool1.m_bEnableLineDistanceTool)
		{
			m_DlgLineToLineDistanceTool1.m_dPixelRatio = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[0];
			m_DlgLineToLineDistanceTool1.m_nDistanceMode = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode[0];
			m_DlgLineToLineDistanceTool1.m_nDistanceType = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType[0];
			m_DlgLineToLineDistanceTool1.m_bEnableMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0];
			m_DlgLineToLineDistanceTool1.m_bEnableMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0];
			m_DlgLineToLineDistanceTool1.m_dDistanceMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0];
			m_DlgLineToLineDistanceTool1.m_dDistanceMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0];
			m_DlgLineToLineDistanceTool1.m_bEanbleOKNG_Decision = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[0];
			m_DlgLineToLineDistanceTool1.m_bShowFitPoint1 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[0];
			m_DlgLineToLineDistanceTool1.m_bShowFitPoint2 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[1];

			m_DlgLineToLineDistanceTool1.m_dInspectDistanceValue = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dInspectDistanceValue[0];
			m_DlgLineToLineDistanceTool1.m_strInspectInfo = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectResultInfo[0];
			m_DlgLineToLineDistanceTool1.m_strDistanceLabel = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0];

		}
		m_DlgLineToLineDistanceTool1.UpdateData(FALSE);

		//if (m_DlgLineToLineDistanceTool2.m_bEnableLineDistanceTool)
		{
			m_DlgLineToLineDistanceTool2.m_dPixelRatio = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[1];
			m_DlgLineToLineDistanceTool2.m_nDistanceMode = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode[1];
			m_DlgLineToLineDistanceTool2.m_nDistanceType = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType[1];
			m_DlgLineToLineDistanceTool2.m_bEnableMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1];
			m_DlgLineToLineDistanceTool2.m_bEnableMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1];
			m_DlgLineToLineDistanceTool2.m_dDistanceMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1];
			m_DlgLineToLineDistanceTool2.m_dDistanceMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1];
			m_DlgLineToLineDistanceTool2.m_bEanbleOKNG_Decision = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[1];

			m_DlgLineToLineDistanceTool2.m_bShowFitPoint1 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[2];
			m_DlgLineToLineDistanceTool2.m_bShowFitPoint2 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[3];
			m_DlgLineToLineDistanceTool2.m_dInspectDistanceValue = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dInspectDistanceValue[1];
			m_DlgLineToLineDistanceTool2.m_strInspectInfo = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectResultInfo[1];
			m_DlgLineToLineDistanceTool2.m_strDistanceLabel = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1];

		}
		m_DlgLineToLineDistanceTool2.UpdateData(FALSE);
		{
			m_DlgCircleToLineDistanceTool.m_dPixelRatio = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio[2];
			m_DlgCircleToLineDistanceTool.m_bEnableMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2];
			m_DlgCircleToLineDistanceTool.m_dDistanceMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2];

			m_DlgCircleToLineDistanceTool.m_bEnableMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2];
			m_DlgCircleToLineDistanceTool.m_dDistanceMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2];

			m_DlgCircleToLineDistanceTool.m_bEanbleOKNG_Decision = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableInspectOKNG[2];
			m_DlgCircleToLineDistanceTool.m_strDistanceLabel = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2];
			m_DlgCircleToLineDistanceTool.m_dInspectDistanceValue = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dInspectDistanceValue[2];
			m_DlgCircleToLineDistanceTool.m_strInspectInfo = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectResultInfo[2];
			m_DlgCircleToLineDistanceTool.m_bShowFitPoint1 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bInspectVisibleFitPoint[4];
			m_DlgCircleToLineDistanceTool.m_bEnableShowFitCirclePoint = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitCirclePoint;
		}

		m_DlgCircleToLineDistanceTool.UpdateData(FALSE);
		UpdateData(FALSE);
	}
}
void CDlgCornerSearchCheckInspectTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateDlgItem();
	*pResult = 0;
}


void CDlgCornerSearchCheckInspectTool::OnCbnSelchangeCmbSearchtoolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}


void CDlgCornerSearchCheckInspectTool::OnCbnSelchangeCmbInspectType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}


void CDlgCornerSearchCheckInspectTool::OnBnClickedBtnMarkGettrainimage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateTrainData(TRUE);

	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;

	switch(m_nSearchToolTypeIndex)
	{
	case 0:
		{
			if (m_DlgPatternCornerSearchTool.m_bEnableMask)
			{
				if ( (m_DlgPatternCornerSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternCornerSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternCornerSearchTool.m_imageMask.Release();
				}
			}
		}
		break;
	case 1:
		{
			if (m_DlgCornerSearchTool.m_bEnableMask)
			{
				if ( (m_DlgCornerSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgCornerSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgCornerSearchTool.m_imageMask.Release();
				}
			}

		}
		break;
	}
	switch(m_nInspectTypeIndex)
	{
	case 0:
		{
			if (m_DlgPatternSearchTool.m_bEnableMask)
			{
				if ( (m_DlgPatternSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternSearchTool.m_imageMask.Release();
				}
			}
		}
		break;
	case 1:
		{
			if (m_DlgQuickSearchTool.m_bEnableMask)
			{
				if ( (m_DlgQuickSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgQuickSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgQuickSearchTool.m_imageMask.Release();
				}
			}
		}
		break;
	}
	
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}


void CDlgCornerSearchCheckInspectTool::OnBnClickedBtnMarkTrain()
{
	// TODO: 在此添加控件通知处理程序代码

	{
		// TODO: Add your control notification handler code here
		if (!m_pDlgParent->m_pSearchTool)
		{
			return;
		}
		if (!m_pDlgParent->m_imageTrain.IsValid())
		{
			m_pDlgParent->m_bStatusBarFlag = FALSE;
			m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED);		//_T("训练失败");
			m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_TRAIN_IMAGE);	//_T("训练图像数据为空");
			m_pDlgParent->UpdateStatusBar();
			return;
		}

		CheckDlgDataValidation();
		UpdateTrainData(TRUE);

		m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);
		
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			if (m_DlgPatternCornerSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgPatternCornerSearchTool.m_imageMask);
			}
			break;
		case 1:
			if (m_DlgCornerSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgCornerSearchTool.m_imageMask);
			}
			break;

		}
		if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck)
		{
			switch(m_nInspectTypeIndex)
			{
			case 0:
				if (m_DlgPatternSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage2(m_DlgPatternSearchTool.m_imageMask);
				}
				break;
			case 1:
				if (m_DlgQuickSearchTool.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage2(m_DlgQuickSearchTool.m_imageMask);
				}
				break;
			}
		}
		

		if (m_pDlgParent->m_pSearchTool->Train())
		{
			cpImage trainImage;
			switch(m_nSearchToolTypeIndex)
			{
			case 0:
				m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
				m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
				m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			}
            trainImage.Release();
			//((CCornerCheckSearchTool*)m_pDlgParent->m_pSearchTool)->GetTrainResultImage2(trainImage);
			switch(m_nInspectTypeIndex)
			{
			case 0:
				((CCornerCheckSearchTool*)m_pDlgParent->m_pSearchTool)->GetTrainResultImage2(trainImage);
				m_DlgPatternSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				((CCornerCheckSearchTool*)m_pDlgParent->m_pSearchTool)->GetTrainResultImage2(trainImage);
				m_DlgQuickSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;

			}
			m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
			((CCornerCheckSearchTool*)m_pDlgParent->m_pSearchTool)->UpdateGuiDisplay();
			m_pDlgParent->UpdateGuiDisplay();
			m_pDlgParent->m_bTrainDataUpdated = TRUE;
			m_pDlgParent->m_bSearchDataUpdated = FALSE;
		}
		else
		{
			CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
			m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
			m_pDlgParent->m_bTrainDataUpdated = FALSE;
		}
	}
}


void CDlgCornerSearchCheckInspectTool::OnBnClickedChkUseinspect()
{
	// TODO: 在此添加控件通知处理程序代码
//	UpdateData();
	//((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck=m_bUseInspectTool;
	/*if (m_bUseInspectTool)
	{
	}*/
	UpdateData(TRUE);
	OnBnClickedBtnMarkGettrainimage();

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	if (((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheck)
	{
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(FALSE);
	}
}


//void CDlgCornerSearchCheckInspectTool::OnBnSetfocusCheckEnableCorner()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//
//}


void CDlgCornerSearchCheckInspectTool::OnBnClickedCheckEnableCorner()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
}
