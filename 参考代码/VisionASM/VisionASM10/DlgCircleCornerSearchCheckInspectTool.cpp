// CDlgCircleCornerSearchCheckInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCircleCornerSearchCheckInspectTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgCircleCornerSearchCheckInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgCircleCornerSearchCheckInspectTool, CDialog)

CDlgCircleCornerSearchCheckInspectTool::CDlgCircleCornerSearchCheckInspectTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCircleCornerSearchCheckInspectTool::IDD, pParent)
	, m_nSearchToolTypeIndex(0)
	, m_nCircleTypeIndex(0)
{
	
	m_TabIndex=0;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;
	
}

CDlgCircleCornerSearchCheckInspectTool::~CDlgCircleCornerSearchCheckInspectTool()
{
}

void CDlgCircleCornerSearchCheckInspectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_cTabCtl);
	DDX_Control(pDX, IDC_BTN_MARK_TRAIN, m_btnMarkTrain);
	DDX_Control(pDX, IDC_BTN_MARK_GETTRAINIMAGE, m_btnMarkGetTrainImage);
	DDX_CBIndex(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_nSearchToolTypeIndex);
	DDX_CBIndex(pDX, IDC_CMB_INSPECT_TYPE, m_nCircleTypeIndex);
	DDX_Control(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_cmbSearchToolType);
	DDX_Control(pDX, IDC_CMB_INSPECT_TYPE, m_cmbInspectType);
}


BEGIN_MESSAGE_MAP(CDlgCircleCornerSearchCheckInspectTool, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgCircleCornerSearchCheckInspectTool::OnTcnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_CMB_SEARCHTOOL_TYPE, &CDlgCircleCornerSearchCheckInspectTool::OnCbnSelchangeCmbSearchtoolType)
	ON_CBN_SELCHANGE(IDC_CMB_INSPECT_TYPE, &CDlgCircleCornerSearchCheckInspectTool::OnCbnSelchangeCmbInspectType)
	ON_BN_CLICKED(IDC_BTN_MARK_GETTRAINIMAGE, &CDlgCircleCornerSearchCheckInspectTool::OnBnClickedBtnMarkGettrainimage)
	ON_BN_CLICKED(IDC_BTN_MARK_TRAIN, &CDlgCircleCornerSearchCheckInspectTool::OnBnClickedBtnMarkTrain)
END_MESSAGE_MAP()


// CDlgCircleCornerSearchCheckInspectTool 消息处理程序
void CDlgCircleCornerSearchCheckInspectTool::RemoveImage()
{
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgCornerSearchTool.m_GuiPatternDisplay.RemoveImage();
}
void CDlgCircleCornerSearchCheckInspectTool::SetGuiPatternDisplayImage(BOOL bCopy)
{
	
}

void CDlgCircleCornerSearchCheckInspectTool::ClearScreen()
{
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgCornerSearchTool.m_GuiPatternDisplay.ClearScreen();
	
}
void CDlgCircleCornerSearchCheckInspectTool::UpdateDialogLanguage()
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
void  CDlgCircleCornerSearchCheckInspectTool::UpdateDlgItem()
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
			
			if (m_nSearchToolTypeIndex==0)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);
			}
			else if (m_nSearchToolTypeIndex==1)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(TRUE);
				m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);
			}
			break;
		case 1:
			GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);	 
			GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(TRUE);
			//设置工具选择为False 只用圆
			GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(FALSE);
			if (m_nCircleTypeIndex==0)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgCircleMeasureInspectTool.ShowWindow(TRUE);
				m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);
			}
			else if (m_nCircleTypeIndex==1)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgCircleDoubleMeasureInspectTool.ShowWindow(TRUE);
				m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);
			}
			else if (m_nCircleTypeIndex==2)
			{
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);
				m_DlgObroundMeasureInspectTool.ShowWindow(TRUE);
			}

			break;
		default:
			m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
			m_DlgCornerSearchTool.ShowWindow(FALSE);
			m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);
			m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);
			m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);
			
			break;

	}

	
	
}
void  CDlgCircleCornerSearchCheckInspectTool::InitDlgItem()
{

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	m_cTabCtl.InsertItem(0,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CORNER_SEARCH_TOOL));
	m_cTabCtl.InsertItem(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CHECK_TOOL));
	m_cTabCtl.SetCurSel(0);

	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_PATTERN_CORNER_SEARCH_TOOL));
	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_QUICK_CORNER_SEARCH_TOOL));
	m_cmbSearchToolType.SetCurSel(0);
	m_cmbInspectType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_TOOL));
	m_cmbInspectType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_CIRCLE_DOUBLE_MEASURE_INSPECT_TOOL));
	m_cmbInspectType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_STR_OBROUND_MEASURE_INSPECT_TOOL));
	m_cmbInspectType.SetCurSel(0);

	CRect rect;
	m_cTabCtl.GetClientRect(rect);

	GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_INSPECT)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_INSPECT_TYPE)->ShowWindow(TRUE);
	//GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(FALSE);

	int nIntendY = 47;
	rect.top+=nIntendY;
	rect.left-=2;
	rect.right+=2;

	CRect rect2;
	rect2 = rect;
	rect2.top -=20;
	// 几何定位
	m_DlgCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgCornerSearchTool.Create(IDD_DLG_CORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgCornerSearchTool.MoveWindow(rect);
	m_DlgCornerSearchTool.ShowWindow(FALSE);

	// 区域定位
	m_DlgPatternCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgPatternCornerSearchTool.Create(IDD_DLG_PATTERNCORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgPatternCornerSearchTool.MoveWindow(rect);
	m_DlgPatternCornerSearchTool.ShowWindow(TRUE);

	// 1圆
	m_DlgCircleMeasureInspectTool.m_pDlgParent  = m_pDlgParent;
	m_DlgCircleMeasureInspectTool.m_pDlgParent2 = this;
	m_DlgCircleMeasureInspectTool.Create(IDD_DLG_CIRCLE_MEASURE_INSPECT_TOOL,&m_cTabCtl);
	m_DlgCircleMeasureInspectTool.MoveWindow(rect2);
	m_DlgCircleMeasureInspectTool.ShowWindow(FALSE);

	// 2圆
	m_DlgCircleDoubleMeasureInspectTool.m_pDlgParent  = m_pDlgParent;
	m_DlgCircleDoubleMeasureInspectTool.m_pDlgParent2 = this;
	m_DlgCircleDoubleMeasureInspectTool.Create(IDD_DLG_CIRCLE_DOUBLE_MEASURE_INSPECT_TOOL,&m_cTabCtl);
	m_DlgCircleDoubleMeasureInspectTool.MoveWindow(rect2);
	m_DlgCircleDoubleMeasureInspectTool.ShowWindow(FALSE);

	// 长圆
	m_DlgObroundMeasureInspectTool.m_pDlgParent  = m_pDlgParent;
	m_DlgObroundMeasureInspectTool.m_pDlgParent2 = this;
	m_DlgObroundMeasureInspectTool.Create(IDD_DLG_OBROUND_MEASURE_INSPECT_TOOL,&m_cTabCtl);
	m_DlgObroundMeasureInspectTool.MoveWindow(rect2);
	m_DlgObroundMeasureInspectTool.ShowWindow(FALSE);

	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);

}
void CDlgCircleCornerSearchCheckInspectTool::CheckDlgDataValidation()
{
	m_DlgPatternCornerSearchTool.CheckDlgDataValidation();
	m_DlgCornerSearchTool.CheckDlgDataValidation();
	m_DlgCircleMeasureInspectTool.CheckDlgDataValidation();

}
void CDlgCircleCornerSearchCheckInspectTool::UpdateControlDisplay()
{
	m_DlgPatternCornerSearchTool.UpdateControlDisplay();
	m_DlgCornerSearchTool.UpdateControlDisplay();
	m_DlgCircleMeasureInspectTool.UpdateControlDisplay();
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_INSPECT_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_INSPECT_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(TRUE);
	}
	
}

BOOL CDlgCircleCornerSearchCheckInspectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnMarkGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnMarkTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	InitDlgItem();
	return TRUE;  
}
void CDlgCircleCornerSearchCheckInspectTool::UpdateTrainData(bool bValidate)
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
		if (m_nSearchToolTypeIndex==0)
		{
			tSearchToolType = ePatternCornerSearchTool;
		}
		else if (m_nSearchToolTypeIndex==1)
		{
			tSearchToolType = eCornerSearchTool;
		}

		((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetSubSearchToolType(tSearchToolType);
		((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetSubCircleToolType(m_nCircleTypeIndex + 1);

		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_DlgPatternCornerSearchTool.m_nPatAccuracyMode);
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_DlgPatternCornerSearchTool.m_nPatSpeedMode+1);
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgPatternCornerSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  = m_DlgPatternCornerSearchTool.m_dAngleLow;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh =m_DlgPatternCornerSearchTool.m_dAngleHigh;

				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgPatternCornerSearchTool.m_nLineOutLineNum1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgPatternCornerSearchTool.m_dLineContrastThre1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgPatternCornerSearchTool.m_bSortByScore1;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_DlgPatternCornerSearchTool.m_bEnableDualEdge;
                ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_1;

				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] =m_DlgPatternCornerSearchTool.m_nLineOutLineNum2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgPatternCornerSearchTool.m_dLineContrastThre2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgPatternCornerSearchTool.m_bSortByScore2;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_DlgPatternCornerSearchTool.m_bEnableDualEdge2;
                ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_2;
			}
			break;
		case 1:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_DlgCornerSearchTool.m_dGrau;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_DlgCornerSearchTool.m_lNoiseThre;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_DlgCornerSearchTool.m_lTotalMag;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgCornerSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;

				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgCornerSearchTool.m_nLineOutLineNum1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgCornerSearchTool.m_dLineContrastThre1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgCornerSearchTool.m_bSortByScore1;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgCornerSearchTool.m_nLineCaliperNum1;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_DlgCornerSearchTool.m_bEnableDualEdge;
                ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_DlgCornerSearchTool.m_bEnableLineMask_1;

				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_DlgCornerSearchTool.m_nLineOutLineNum2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgCornerSearchTool.m_dLineContrastThre2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgCornerSearchTool.m_bSortByScore2;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgCornerSearchTool.m_nLineCaliperNum2;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_DlgCornerSearchTool.m_bEnableDualEdge2;
                 ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_DlgCornerSearchTool.m_bEnableLineMask_2;


			}
			break;
		}
		
		//m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		//m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

		//m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		//m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

		m_DlgCircleMeasureInspectTool.UpdateData();
		switch(m_nCircleTypeIndex )
		{
		case 0://1.个圆
			{
				// Circle
				// Train GUI
				m_DlgCircleMeasureInspectTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[0] = m_DlgCircleMeasureInspectTool.m_nCircleCaliperNum;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[0] = (CaliperSearchDirection)m_DlgCircleMeasureInspectTool.m_nCircleInwardMode;

				// Train Data
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[0]   = m_DlgCircleMeasureInspectTool.m_nCircleOutLineNum ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[0] = m_DlgCircleMeasureInspectTool.m_dCircleContrastThre ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[0] = m_DlgCircleMeasureInspectTool.m_lCircleFilterHalfWidth;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[0] = (EdgePolarity)m_DlgCircleMeasureInspectTool.m_nCirclePolarityMode ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[0] = m_DlgCircleMeasureInspectTool.m_bCircleSortByScore;

			}
			break;
		case 1://2.个圆
			{
				m_DlgCircleDoubleMeasureInspectTool.UpdateData(TRUE);
				m_DlgCircleDoubleMeasureInspectTool.UpdateCircleTrainData(true);
				//int nCircleIndex = m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndex;
				//((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[nCircleIndex] = m_DlgCircleDoubleMeasureInspectTool.m_nCircleCaliperNum;
				//((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[nCircleIndex] = (CaliperSearchDirection)m_DlgCircleDoubleMeasureInspectTool.m_nCircleInwardMode;

				//// Train Data
				//((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[nCircleIndex]   = m_DlgCircleDoubleMeasureInspectTool.m_nCircleOutLineNum ;
				//((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[nCircleIndex] = m_DlgCircleDoubleMeasureInspectTool.m_dCircleContrastThre ;
				//((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[nCircleIndex] = m_DlgCircleDoubleMeasureInspectTool.m_lCircleFilterHalfWidth;
				//((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[nCircleIndex] = (EdgePolarity)m_DlgCircleDoubleMeasureInspectTool.m_nCirclePolarityMode ;
				//((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[nCircleIndex] = m_DlgCircleDoubleMeasureInspectTool.m_bCircleSortByScore;
				for (int it=0;it<2;it++)
				{
					((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[it] = m_DlgCircleDoubleMeasureInspectTool.m_nCircleCaliperNumTmp[it];
					((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[it] = (CaliperSearchDirection)m_DlgCircleDoubleMeasureInspectTool.m_nCircleInwardModeTmp[it];

					// Train Data
					((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[it]   = m_DlgCircleDoubleMeasureInspectTool.m_nCircleOutLineNumTmp[it];
					((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[it] = m_DlgCircleDoubleMeasureInspectTool.m_dCircleContrastThreTmp[it];
					((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[it] = m_DlgCircleDoubleMeasureInspectTool.m_lCircleFilterHalfWidthTmp[it];
					((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[it] = (EdgePolarity)m_DlgCircleDoubleMeasureInspectTool.m_nCirclePolarityModeTmp[it];
					((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[it] = m_DlgCircleDoubleMeasureInspectTool.m_bCircleSortByScoreTmp[it];

				}

			}
			break;
		case 2://3.长圆
			{
				// Obround
				// Train GUI
				m_DlgObroundMeasureInspectTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nObroundCaliperNum = m_DlgObroundMeasureInspectTool.m_nObroundCaliperNum;
				((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_ObroundCaliperSearchDirection = (CaliperSearchDirection)m_DlgObroundMeasureInspectTool.m_nObroundInwardMode;

				// Train Data
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nObroundOutLineNum   = m_DlgObroundMeasureInspectTool.m_nObroundOutLineNum ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dObroundContrastThre = m_DlgObroundMeasureInspectTool.m_dObroundContrastThre ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lObroundFilterHalfWidth = m_DlgObroundMeasureInspectTool.m_lObroundFilterHalfWidth;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeObroundProperty = (EdgePolarity)m_DlgObroundMeasureInspectTool.m_nObroundPolarityMode ;
				((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bObroundSortByScore = m_DlgObroundMeasureInspectTool.m_bObroundSortByScore;
			}
			break;
		}


		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		if (((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetSubSearchToolType()==ePatternCornerSearchTool)
		{
			m_nSearchToolTypeIndex=0;
		}
		else if (((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetSubSearchToolType()==eCornerSearchTool)
		{
			m_nSearchToolTypeIndex=1;
		}
		m_nCircleTypeIndex =  ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetCircleToolType() - 1;

		UpdateData(FALSE);
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.m_nPatAccuracyMode = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
				m_DlgPatternCornerSearchTool.m_nPatSpeedMode = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
				m_DlgPatternCornerSearchTool.m_dAngleLow	    = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
				m_DlgPatternCornerSearchTool.m_dAngleHigh	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
				m_DlgPatternCornerSearchTool.m_bEnableMask = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

				m_DlgPatternCornerSearchTool.m_nLineOutLineNum1 =  ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
				m_DlgPatternCornerSearchTool.m_dLineContrastThre1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
				m_DlgPatternCornerSearchTool.m_bSortByScore1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum1 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
				m_DlgPatternCornerSearchTool.m_bEnableDualEdge = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];
                m_DlgPatternCornerSearchTool.m_bEnableLineMask_1 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0];

				m_DlgPatternCornerSearchTool.m_nLineOutLineNum2 =  ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgPatternCornerSearchTool.m_dLineContrastThre2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgPatternCornerSearchTool.m_bSortByScore2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum2 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgPatternCornerSearchTool.m_bEnableDualEdge2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];
                m_DlgPatternCornerSearchTool.m_bEnableLineMask_2 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1];

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
				m_DlgCornerSearchTool.m_dGrau= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
				m_DlgCornerSearchTool.m_lNoiseThre	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
				m_DlgCornerSearchTool.m_lTotalMag		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
				m_DlgCornerSearchTool.m_bEnableMask	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

				m_DlgCornerSearchTool.m_nLineOutLineNum1 =  ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
				m_DlgCornerSearchTool.m_dLineContrastThre1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
				m_DlgCornerSearchTool.m_nLinePolarityMode1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
				m_DlgCornerSearchTool.m_bSortByScore1 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
				m_DlgCornerSearchTool.m_nLineCaliperNum1 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
				m_DlgCornerSearchTool.m_bEnableDualEdge = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];
                m_DlgCornerSearchTool.m_bEnableLineMask_1 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0];

				m_DlgCornerSearchTool.m_nLineOutLineNum2 =  ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgCornerSearchTool.m_dLineContrastThre2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgCornerSearchTool.m_nLinePolarityMode2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgCornerSearchTool.m_bSortByScore2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				m_DlgCornerSearchTool.m_nLineCaliperNum2 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgCornerSearchTool.m_bEnableDualEdge2 = ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];
                m_DlgCornerSearchTool.m_bEnableLineMask_2 = ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1];

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
		switch(m_nCircleTypeIndex)
		{
		case 0://1.个圆
			{
				// Circle
				// Train GUI
				m_DlgCircleMeasureInspectTool.m_nCircleCaliperNum		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[0];
				m_DlgCircleMeasureInspectTool.m_nCircleInwardMode		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[0];

				// Train Data
				m_DlgCircleMeasureInspectTool.m_nCircleOutLineNum		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[0];
				m_DlgCircleMeasureInspectTool.m_dCircleContrastThre		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[0];
				m_DlgCircleMeasureInspectTool.m_lCircleFilterHalfWidth	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[0];
				m_DlgCircleMeasureInspectTool.m_nCirclePolarityMode		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[0];
				m_DlgCircleMeasureInspectTool.m_bCircleSortByScore		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[0];
				m_DlgCircleMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		case 1://2.个圆
			{
				//int nCircleIndex = m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndex;
				//m_DlgCircleDoubleMeasureInspectTool.m_nCircleCaliperNum		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[nCircleIndex];
				//m_DlgCircleDoubleMeasureInspectTool.m_nCircleInwardMode		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[nCircleIndex];

				//// Train Data
				//m_DlgCircleDoubleMeasureInspectTool.m_nCircleOutLineNum		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[nCircleIndex];
				//m_DlgCircleDoubleMeasureInspectTool.m_dCircleContrastThre		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[nCircleIndex];
				//m_DlgCircleDoubleMeasureInspectTool.m_lCircleFilterHalfWidth	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[nCircleIndex];
				//m_DlgCircleDoubleMeasureInspectTool.m_nCirclePolarityMode		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[nCircleIndex];
				//m_DlgCircleDoubleMeasureInspectTool.m_bCircleSortByScore		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[nCircleIndex];
				//
				m_DlgCircleDoubleMeasureInspectTool.UpdateCircleTrainData(false);
				
				m_DlgCircleDoubleMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		case 2://3.长圆
			{
				// Obround
				// Train GUI
				m_DlgObroundMeasureInspectTool.m_nObroundCaliperNum		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nObroundCaliperNum;
				m_DlgObroundMeasureInspectTool.m_nObroundInwardMode		= ((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_ObroundCaliperSearchDirection;

				// Train Data
				m_DlgObroundMeasureInspectTool.m_nObroundOutLineNum		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nObroundOutLineNum;
				m_DlgObroundMeasureInspectTool.m_dObroundContrastThre		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dObroundContrastThre;
				m_DlgObroundMeasureInspectTool.m_lObroundFilterHalfWidth	= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lObroundFilterHalfWidth;
				m_DlgObroundMeasureInspectTool.m_nObroundPolarityMode		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeObroundProperty;
				m_DlgObroundMeasureInspectTool.m_bObroundSortByScore		= ((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bObroundSortByScore;
				m_DlgCircleMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		}

		m_DlgCircleMeasureInspectTool.UpdateData(FALSE);
	}

}
void CDlgCircleCornerSearchCheckInspectTool::UpdateSearchData(bool bValidate)
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
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgPatternCornerSearchTool.m_nSearchNum;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgPatternCornerSearchTool.m_dAcceptThre;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_DlgPatternCornerSearchTool.m_dConfusionThre;
				
			}
				break;
		case 1:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgCornerSearchTool.m_nSearchNum;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgCornerSearchTool.m_dAcceptThre;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_DlgCornerSearchTool.m_dAngleLow;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_DlgCornerSearchTool.m_dAngleHigh;

			}
			break;
		}

		switch(m_nCircleTypeIndex)
		{
		case 0://1.个圆
			{
				m_DlgCircleMeasureInspectTool.UpdateData(TRUE);
				((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[0] = m_DlgCircleMeasureInspectTool.m_bShowCircleFitPoint;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck    = m_DlgCircleMeasureInspectTool.m_bEnableSizeCheck ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool = m_DlgCircleMeasureInspectTool.m_bSizeCheckUseToTool ;				
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool = m_DlgCircleMeasureInspectTool.m_bDistCheckUseToTool ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck = m_DlgCircleMeasureInspectTool.m_bEnableDistCheck;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio      = m_DlgCircleMeasureInspectTool.m_dPixelRatio;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0]      = m_DlgCircleMeasureInspectTool.m_bEnableDispLineSeg;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1]      = m_DlgCircleMeasureInspectTool.m_bEnableDispLineSeg2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0]   = m_DlgCircleMeasureInspectTool.m_strSizetLabel1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0]   = m_DlgCircleMeasureInspectTool.m_bEnableMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0] = m_DlgCircleMeasureInspectTool.m_dDistanceMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0]   = m_DlgCircleMeasureInspectTool.m_bEnableMinCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0] = m_DlgCircleMeasureInspectTool.m_dDistanceMinCircleRadius1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0]   = m_DlgCircleMeasureInspectTool.m_bEnableCircleRadius1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0] = m_DlgCircleMeasureInspectTool.m_dDistanceCircleRadiusBase1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0]   = m_DlgCircleMeasureInspectTool.m_dDistanceCircleRadiusFlu1;


				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1]   = m_DlgCircleMeasureInspectTool.m_strDistLabel2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1]   = m_DlgCircleMeasureInspectTool.m_bEnableMax1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1] = m_DlgCircleMeasureInspectTool.m_dDistanceMax1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1]	 = m_DlgCircleMeasureInspectTool.m_bEnableMin1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1] = m_DlgCircleMeasureInspectTool.m_dDistanceMin1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1]   = m_DlgCircleMeasureInspectTool.m_bEnableDistance1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1] = m_DlgCircleMeasureInspectTool.m_dDistanceBase1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1]   = m_DlgCircleMeasureInspectTool.m_dDistanceFlu1;



				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2]   = m_DlgCircleMeasureInspectTool.m_strDistLabel3;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2]   = m_DlgCircleMeasureInspectTool.m_bEnableMax2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2] = m_DlgCircleMeasureInspectTool.m_dDistanceMax2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2]   = m_DlgCircleMeasureInspectTool.m_bEnableMin2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2] = m_DlgCircleMeasureInspectTool.m_dDistanceMin2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2]   = m_DlgCircleMeasureInspectTool.m_bEnableDistance2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2] = m_DlgCircleMeasureInspectTool.m_dDistanceBase2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2]   = m_DlgCircleMeasureInspectTool.m_dDistanceFlu2;



				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool = m_DlgCircleMeasureInspectTool.m_bBlobInspectUseToTool ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio = m_DlgCircleMeasureInspectTool.m_dAreaScaleRatio ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect = m_DlgCircleMeasureInspectTool.m_bEnableBlobInspect ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre = m_DlgCircleMeasureInspectTool.m_nBinaryThre ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea = m_DlgCircleMeasureInspectTool.m_nMinAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea = m_DlgCircleMeasureInspectTool.m_nMaxAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea = m_DlgCircleMeasureInspectTool.m_dMaxDefectArea ;
				int nDefectType  = m_DlgCircleMeasureInspectTool.m_comboDefectType.GetCurSel();
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType = nDefectType + 1;
				m_DlgCircleMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);
			}
			break;
		case 1://2.个圆
			{
				m_DlgCircleDoubleMeasureInspectTool.UpdateData(TRUE);
				int nCircleIndex = m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndex;
				//int nCircleMeasureIndex = m_DlgCircleDoubleMeasureInspectTool.m_nCircleMeasureIndex;
				((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[nCircleIndex] = m_DlgCircleDoubleMeasureInspectTool.m_bShowCircleFitPoint;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool   = m_DlgCircleDoubleMeasureInspectTool.m_bSizeCheckUseToTool;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck = m_DlgCircleDoubleMeasureInspectTool.m_bEnableSizeCheck;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck = m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistCheck;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio      = m_DlgCircleDoubleMeasureInspectTool.m_dPixelRatio;

				// 距离模式
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType      = m_DlgCircleDoubleMeasureInspectTool.m_nDistanceType;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0]      = m_DlgCircleDoubleMeasureInspectTool.m_bEnableDispLineSeg;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1]      = m_DlgCircleDoubleMeasureInspectTool.m_bEnableDispLineSeg2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0]   = m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius1;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1]   = m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2]   = m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel3;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius3;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase3;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu3;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius3;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius3;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius3;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius3;



				m_DlgCircleDoubleMeasureInspectTool.UpdateCircleSearchData(TRUE);
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool   = m_DlgCircleDoubleMeasureInspectTool.m_bDistCheckUseToTool;
				for (int it =0;it<3;it++)
				{
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[3+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_strDistLabel1Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[3+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistance1Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[3+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceBase1Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[3+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceFlu1Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[3+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMax1Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[3+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMax1Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[3+2*it]	 = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMin1Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[3+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMin1Tmp[it];

					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[4+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_strDistLabel2Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[4+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistance2Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[4+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceBase2Tmp[it];
					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[4+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceFlu2Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[4+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMax2Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[4+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMax2Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[4+2*it]   = m_DlgCircleDoubleMeasureInspectTool.m_bEnableMin2Tmp[it];
// 					((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[4+2*it] = m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMin2Tmp[it];


				}



				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool = m_DlgCircleDoubleMeasureInspectTool.m_bBlobInspectUseToTool ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio = m_DlgCircleDoubleMeasureInspectTool.m_dAreaScaleRatio ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect = m_DlgCircleDoubleMeasureInspectTool.m_bEnableBlobInspect ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre = m_DlgCircleDoubleMeasureInspectTool.m_nBinaryThre ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea = m_DlgCircleDoubleMeasureInspectTool.m_nMinAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea = m_DlgCircleDoubleMeasureInspectTool.m_nMaxAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea = m_DlgCircleDoubleMeasureInspectTool.m_dMaxDefectArea ;
				int nDefectType  = m_DlgCircleDoubleMeasureInspectTool.m_comboDefectType.GetCurSel();
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType = nDefectType + 1;
				m_DlgCircleDoubleMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);
			}
			break;
		case 2://3.长圆
			{
				m_DlgObroundMeasureInspectTool.UpdateData(TRUE);
				//((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[0] = m_DlgObroundMeasureInspectTool.m_bShowCircleFitPoint;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck    = m_DlgObroundMeasureInspectTool.m_bEnableSizeCheck ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool = m_DlgObroundMeasureInspectTool.m_bSizeCheckUseToTool ;				
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool = m_DlgObroundMeasureInspectTool.m_bDistCheckUseToTool ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck = m_DlgObroundMeasureInspectTool.m_bEnableDistCheck;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio      = m_DlgObroundMeasureInspectTool.m_dPixelRatio;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0]      = m_DlgObroundMeasureInspectTool.m_bEnableDispLineSeg;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1]      = m_DlgObroundMeasureInspectTool.m_bEnableDispLineSeg2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0]   = m_DlgObroundMeasureInspectTool.m_strSizeLabel1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0]   = m_DlgObroundMeasureInspectTool.m_bEnableCircleRadius1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0] = m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusBase1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0] = m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusFlu1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0]   = m_DlgObroundMeasureInspectTool.m_bEnableMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0] = m_DlgObroundMeasureInspectTool.m_dDistanceMaxCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0]   = m_DlgObroundMeasureInspectTool.m_bEnableMinCircleRadius1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0] = m_DlgObroundMeasureInspectTool.m_dDistanceMinCircleRadius1;


				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1]   = m_DlgObroundMeasureInspectTool.m_strSizeLabel2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1]   = m_DlgObroundMeasureInspectTool.m_bEnableCircleRadius2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1] = m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusBase2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1] = m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusFlu2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1]   = m_DlgObroundMeasureInspectTool.m_bEnableMaxCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1] = m_DlgObroundMeasureInspectTool.m_dDistanceMaxCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1]   = m_DlgObroundMeasureInspectTool.m_bEnableMinCircleRadius2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1] = m_DlgObroundMeasureInspectTool.m_dDistanceMinCircleRadius2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2]   = m_DlgObroundMeasureInspectTool.m_strDistLabel1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2]   = m_DlgObroundMeasureInspectTool.m_bEnableDistance1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2] = m_DlgObroundMeasureInspectTool.m_dDistanceBase1;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2] = m_DlgObroundMeasureInspectTool.m_dDistanceFlu1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2]   = m_DlgObroundMeasureInspectTool.m_bEnableMax1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2] = m_DlgObroundMeasureInspectTool.m_dDistanceMax1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2]	 = m_DlgObroundMeasureInspectTool.m_bEnableMin1;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2] = m_DlgObroundMeasureInspectTool.m_dDistanceMin1;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[3]   = m_DlgObroundMeasureInspectTool.m_strDistLabel2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[3]   = m_DlgObroundMeasureInspectTool.m_bEnableDistance2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[3] = m_DlgObroundMeasureInspectTool.m_dDistanceBase2;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[3] = m_DlgObroundMeasureInspectTool.m_dDistanceFlu2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[3]   = m_DlgObroundMeasureInspectTool.m_bEnableMax2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[3] = m_DlgObroundMeasureInspectTool.m_dDistanceMax2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[3]   = m_DlgObroundMeasureInspectTool.m_bEnableMin2;
// 				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[3] = m_DlgObroundMeasureInspectTool.m_dDistanceMin2;

				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool = m_DlgObroundMeasureInspectTool.m_bBlobInspectUseToTool ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio = m_DlgObroundMeasureInspectTool.m_dAreaScaleRatio ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect = m_DlgObroundMeasureInspectTool.m_bEnableBlobInspect ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre = m_DlgObroundMeasureInspectTool.m_nBinaryThre ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea = m_DlgObroundMeasureInspectTool.m_nMinAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea = m_DlgObroundMeasureInspectTool.m_nMaxAcceptArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea = m_DlgObroundMeasureInspectTool.m_dMaxDefectArea ;
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea = m_DlgObroundMeasureInspectTool.m_dMaxDefectArea ;
				int nDefectType  = m_DlgObroundMeasureInspectTool.m_comboDefectType.GetCurSel();
				((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType = nDefectType + 1;
				m_DlgObroundMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);
			}
			break;
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternCornerSearchTool.m_nSearchNum	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgPatternCornerSearchTool.m_dAcceptThre	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
				m_DlgPatternCornerSearchTool.m_dConfusionThre = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;
				m_DlgPatternCornerSearchTool.m_bShowFitPoint_1 = ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1;
				m_DlgPatternCornerSearchTool.m_bShowFitPoint_2 = ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2;
				m_DlgPatternCornerSearchTool.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgCornerSearchTool.m_nSearchNum	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgCornerSearchTool.m_dAcceptThre	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
				m_DlgCornerSearchTool.m_dAngleLow		= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
				m_DlgCornerSearchTool.m_dAngleHigh	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
                m_DlgCornerSearchTool.m_bShowFitPoint_1	= ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1;
                m_DlgCornerSearchTool.m_bShowFitPoint_2	= ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2;
				m_DlgCornerSearchTool.UpdateData(FALSE);
			}
			break;
		}

		switch(m_nCircleTypeIndex)
		{
		case 0://1.个圆
			{
				m_DlgCircleMeasureInspectTool.m_bShowCircleFitPoint = ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[0];

				m_DlgCircleMeasureInspectTool.m_bEnableSizeCheck = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck ;
				m_DlgCircleMeasureInspectTool.m_bSizeCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool ;
				m_DlgCircleMeasureInspectTool.m_bDistCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool ;
				m_DlgCircleMeasureInspectTool.m_bEnableDistCheck = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck ;
				m_DlgCircleMeasureInspectTool.m_dPixelRatio      = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio ;
				m_DlgCircleMeasureInspectTool.m_bEnableDispLineSeg      = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0] ;
				m_DlgCircleMeasureInspectTool.m_bEnableDispLineSeg2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1] ;

				m_DlgCircleMeasureInspectTool.m_strSizetLabel1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0] ;
// 				m_DlgCircleMeasureInspectTool.m_bEnableMaxCircleRadius1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0] ;
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMaxCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0];
// 				m_DlgCircleMeasureInspectTool.m_bEnableMinCircleRadius1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0]  ;
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0] ;
				m_DlgCircleMeasureInspectTool.m_bEnableCircleRadius1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0] ;
				m_DlgCircleMeasureInspectTool.m_dDistanceCircleRadiusBase1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0];
				m_DlgCircleMeasureInspectTool.m_dDistanceCircleRadiusFlu1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0]  ;



				m_DlgCircleMeasureInspectTool.m_strDistLabel2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1] ;
// 				m_DlgCircleMeasureInspectTool.m_bEnableMax1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1];
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMax1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1] ;
// 				m_DlgCircleMeasureInspectTool.m_bEnableMin1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1];
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMin1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1];
				m_DlgCircleMeasureInspectTool.m_bEnableDistance1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1] ;
				m_DlgCircleMeasureInspectTool.m_dDistanceBase1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1];
				m_DlgCircleMeasureInspectTool.m_dDistanceFlu1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1]  ;


				m_DlgCircleMeasureInspectTool.m_strDistLabel3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2] ;
// 				m_DlgCircleMeasureInspectTool.m_bEnableMax2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2];
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMax2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2] ;
// 				m_DlgCircleMeasureInspectTool.m_bEnableMin2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2];
// 				m_DlgCircleMeasureInspectTool.m_dDistanceMin2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2];
				m_DlgCircleMeasureInspectTool.m_bEnableDistance2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2] ;
				m_DlgCircleMeasureInspectTool.m_dDistanceBase2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2];
				m_DlgCircleMeasureInspectTool.m_dDistanceFlu2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2]  ;


				m_DlgCircleMeasureInspectTool.m_bBlobInspectUseToTool   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool;
				m_DlgCircleMeasureInspectTool.m_dAreaScaleRatio   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio;
				m_DlgCircleMeasureInspectTool.m_bEnableBlobInspect   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect;
				m_DlgCircleMeasureInspectTool.m_nBinaryThre   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre;
				m_DlgCircleMeasureInspectTool.m_nMinAcceptArea = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea ;
				m_DlgCircleMeasureInspectTool.m_nMaxAcceptArea   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea;
				m_DlgCircleMeasureInspectTool.m_dMaxDefectArea = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea;
				int nDefectType								= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType - 1;
				m_DlgCircleMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);

				m_DlgCircleMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		case 1://2.个圆
			{
				int nCircleIndex = m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndex;
				m_DlgCircleDoubleMeasureInspectTool.m_bShowCircleFitPoint = ((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[nCircleIndex];

				m_DlgCircleDoubleMeasureInspectTool.m_bSizeCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool ;
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableSizeCheck    = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck;
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistCheck    = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck;
				m_DlgCircleDoubleMeasureInspectTool.m_dPixelRatio         = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio  ;

				m_DlgCircleDoubleMeasureInspectTool.m_nDistanceType = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType;
				m_DlgCircleDoubleMeasureInspectTool.m_comboDistanceType.SetCurSel(m_DlgCircleDoubleMeasureInspectTool.m_nDistanceType);
				if (m_DlgCircleDoubleMeasureInspectTool.m_nDistanceType == 3)
				{
					m_DlgCircleDoubleMeasureInspectTool.OnCbnSelchangeComboDistancetype();
				}


				m_DlgCircleDoubleMeasureInspectTool.m_bEnableDispLineSeg  = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0] ;
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableDispLineSeg2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1] ;

				m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel1      = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0];
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius1=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0];

// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius1=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0];
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0];
// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0] ;
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0];

				m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1];
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius2=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1];
// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1] ;
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1];
// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1] ;
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1];

				m_DlgCircleDoubleMeasureInspectTool.m_strSizeLabel3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2];
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableCircleRadius3=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusBase3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2];
				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceCircleRadiusFlu3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2];
// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMaxCircleRadius3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2] ;
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMaxCircleRadius3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2];
// 				m_DlgCircleDoubleMeasureInspectTool.m_bEnableMinCircleRadius3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2]  ;
// 				m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMinCircleRadius3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2];

				//m_DlgCircleDoubleMeasureInspectTool.m_bDistCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool ;
				//m_DlgCircleDoubleMeasureInspectTool.m_strDistLabel1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[3] ;
				//m_DlgCircleDoubleMeasureInspectTool.m_bEnableMax1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[3] ;
				//m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMax1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[3];
				//m_DlgCircleDoubleMeasureInspectTool.m_bEnableMin1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[3];
				//m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMin1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[3];

				//m_DlgCircleDoubleMeasureInspectTool.m_strDistLabel2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[4] ;
				//m_DlgCircleDoubleMeasureInspectTool.m_bEnableMax2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[4] ;
				//m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMax2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[4];
				//m_DlgCircleDoubleMeasureInspectTool.m_bEnableMin2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[4] ;
				//m_DlgCircleDoubleMeasureInspectTool.m_dDistanceMin2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[4];
				m_DlgCircleDoubleMeasureInspectTool.UpdateCircleSearchData(false);

				m_DlgCircleDoubleMeasureInspectTool.m_bBlobInspectUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool ;
				m_DlgCircleDoubleMeasureInspectTool.m_dAreaScaleRatio = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio ;
				m_DlgCircleDoubleMeasureInspectTool.m_bEnableBlobInspect = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect ;
				m_DlgCircleDoubleMeasureInspectTool.m_nBinaryThre = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre ;
				m_DlgCircleDoubleMeasureInspectTool.m_nMinAcceptArea = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea ;
				m_DlgCircleDoubleMeasureInspectTool.m_nMaxAcceptArea = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea ;
				m_DlgCircleDoubleMeasureInspectTool.m_dMaxDefectArea = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea ;
				int nDefectType								= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType - 1;
				m_DlgCircleDoubleMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);

				m_DlgCircleDoubleMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		case 2://3.长圆
			{
				m_DlgObroundMeasureInspectTool.m_bEnableSizeCheck  = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSizeCheck;
				m_DlgObroundMeasureInspectTool.m_bSizeCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSizeCheckUseToTool;				
				m_DlgObroundMeasureInspectTool.m_bDistCheckUseToTool = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool;
				m_DlgObroundMeasureInspectTool.m_bEnableDistCheck = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck ;
				m_DlgObroundMeasureInspectTool.m_dPixelRatio = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio ;
				m_DlgObroundMeasureInspectTool.m_bEnableDispLineSeg = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[0] ;
				m_DlgObroundMeasureInspectTool.m_bEnableDispLineSeg2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[1];

				m_DlgObroundMeasureInspectTool.m_strSizeLabel1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0] ;
				m_DlgObroundMeasureInspectTool.m_bEnableCircleRadius1=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[0];
				m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusBase1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[0];
				m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusFlu1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[0];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMaxCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0];
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMaxCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0];
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0];


				m_DlgObroundMeasureInspectTool.m_strSizeLabel2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1] ;
				m_DlgObroundMeasureInspectTool.m_bEnableCircleRadius2=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[1];
				m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusBase2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[1];
				m_DlgObroundMeasureInspectTool.m_dDistanceCircleRadiusFlu2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[1];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMaxCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1] ;
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMaxCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMinCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1] ;
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMinCircleRadius2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1];

				m_DlgObroundMeasureInspectTool.m_strDistLabel1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2] ;
				m_DlgObroundMeasureInspectTool.m_bEnableDistance1=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[2];
				m_DlgObroundMeasureInspectTool.m_dDistanceBase1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[2];
				m_DlgObroundMeasureInspectTool.m_dDistanceFlu1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[2];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMax1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2] ;
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMax1= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMin1= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2];
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMin1= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2];

				m_DlgObroundMeasureInspectTool.m_strDistLabel2= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[3];
				m_DlgObroundMeasureInspectTool.m_bEnableDistance2=((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[3];
				m_DlgObroundMeasureInspectTool.m_dDistanceBase2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[3];
				m_DlgObroundMeasureInspectTool.m_dDistanceFlu2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[3];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMax2= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[3] ;
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMax2= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[3];
// 				m_DlgObroundMeasureInspectTool.m_bEnableMin2= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[3];
// 				m_DlgObroundMeasureInspectTool.m_dDistanceMin2= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[3];

				m_DlgObroundMeasureInspectTool.m_bBlobInspectUseToTool= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobInspectUseToTool;
				m_DlgObroundMeasureInspectTool.m_dAreaScaleRatio= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAreaScaleRatio ;
				m_DlgObroundMeasureInspectTool.m_bEnableBlobInspect= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEanbleBlobInspect;
				m_DlgObroundMeasureInspectTool.m_nBinaryThre= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBinaryThre  ;
				m_DlgObroundMeasureInspectTool.m_nMinAcceptArea= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMinAcceptArea  ;
				m_DlgObroundMeasureInspectTool.m_nMaxAcceptArea= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nMaxAcceptArea   ;
				m_DlgObroundMeasureInspectTool.m_dMaxDefectArea= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea ;
				m_DlgObroundMeasureInspectTool.m_dMaxDefectArea= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dMaxDefectArea ;
				int nDefectType								= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDefectType - 1;
				m_DlgObroundMeasureInspectTool.m_comboDefectType.SetCurSel(nDefectType);
				m_DlgObroundMeasureInspectTool.UpdateData(FALSE);
			}
			break;
		}

		UpdateData(FALSE);
	}
}
void CDlgCircleCornerSearchCheckInspectTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateDlgItem();
	*pResult = 0;
}


void CDlgCircleCornerSearchCheckInspectTool::OnCbnSelchangeCmbSearchtoolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}


void CDlgCircleCornerSearchCheckInspectTool::OnCbnSelchangeCmbInspectType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}


void CDlgCircleCornerSearchCheckInspectTool::OnBnClickedBtnMarkGettrainimage()
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
	
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}


void CDlgCircleCornerSearchCheckInspectTool::OnBnClickedBtnMarkTrain()
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
		

		if (m_pDlgParent->m_pSearchTool->Train())
		{
			cpImage trainImage;
			m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
			switch(m_nSearchToolTypeIndex)
			{
			case 0:
				m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				//m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				//m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			}
            trainImage.Release();

			m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
			m_pDlgParent->m_bTrainDataUpdated = TRUE;
			m_pDlgParent->m_bSearchDataUpdated = FALSE;

			m_pDlgParent->UpdateGuiDisplay();


		}
		else
		{
			m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED));
			m_pDlgParent->m_bTrainDataUpdated = FALSE;
		}
	}
}


//将搜索的结果更新到界面上
void CDlgCircleCornerSearchCheckInspectTool::UpdateSearchResult()
{
	switch(m_nCircleTypeIndex)
	{
	case  0:
		{
				CSearchResult searchResult;
				m_pDlgParent->m_pSearchTool->GetResult(0, searchResult);
				if(m_DlgCircleMeasureInspectTool.m_bEnableSizeCheck)
				{
					if(searchResult.m_CircleCornerDistance.m_vSizeMM.size() > 0)
					{
						if(searchResult.m_CircleCornerDistance.m_nSizeControlResult == -1)
						{
							m_DlgCircleMeasureInspectTool.m_strCurSizeResult = _T("NG");
						}else
						{
							m_DlgCircleMeasureInspectTool.m_strCurSizeResult = _T("OK");
						}
						m_DlgCircleMeasureInspectTool.m_dCurRadius = searchResult.m_CircleCornerDistance.m_vSizeMM[0];
					}
				}
				if(m_DlgCircleMeasureInspectTool.m_bEnableDistCheck)
				{
					if(searchResult.m_CircleCornerDistance.m_vDistMM.size() > 1)
					{
						if(searchResult.m_CircleCornerDistance.m_nDistControlResult == -1)
						{
							m_DlgCircleMeasureInspectTool.m_strCurDistResult = _T("NG");
						}else
						{
							m_DlgCircleMeasureInspectTool.m_strCurDistResult = _T("OK");
						}
						m_DlgCircleMeasureInspectTool.m_dCurDistance1 = searchResult.m_CircleCornerDistance.m_vDistMM[0];
						m_DlgCircleMeasureInspectTool.m_dCurDistance2 = searchResult.m_CircleCornerDistance.m_vDistMM[1];
					}
				}

				if(m_DlgCircleMeasureInspectTool.m_bEnableBlobInspect)
				{
					if(searchResult.m_CircleCornerDistance.m_nBlobResult == -1)
					{
						m_DlgCircleMeasureInspectTool.m_strCurInspectResult = _T("NG");
					}else
					{
						m_DlgCircleMeasureInspectTool.m_strCurInspectResult = _T("OK");
					}
					if(searchResult.m_CircleCornerDistance.m_vBlobArea.size() > 0)
					{
						m_DlgCircleMeasureInspectTool.m_dCurBlobArea = searchResult.m_CircleCornerDistance.m_vBlobArea[0];
					}
					
				}

				if(m_DlgCircleMeasureInspectTool.m_bEnableBlobInspect
					||m_DlgCircleMeasureInspectTool.m_bEnableDistCheck
					||m_DlgCircleMeasureInspectTool.m_bEnableSizeCheck)
				{
					m_DlgCircleMeasureInspectTool.UpdateData(FALSE);
				}
		}
		break;
	case  1:
		{
			CSearchResult searchResult;
			m_pDlgParent->m_pSearchTool->GetResult(0, searchResult);
			if(m_DlgCircleDoubleMeasureInspectTool.m_bEnableSizeCheck)
			{
					if(searchResult.m_CircleCornerDistance.m_vSizeMM.size() > 2)
					{
						if(searchResult.m_CircleCornerDistance.m_nSizeControlResult == -1)
						{
							m_DlgCircleDoubleMeasureInspectTool.m_strCurSizeResult = _T("NG");
						}else
						{
							m_DlgCircleDoubleMeasureInspectTool.m_strCurSizeResult = _T("OK");
						}

						m_DlgCircleDoubleMeasureInspectTool.m_dCurRadius = searchResult.m_CircleCornerDistance.m_vSizeMM[0];
						m_DlgCircleDoubleMeasureInspectTool.m_dCurRadius2 = searchResult.m_CircleCornerDistance.m_vSizeMM[1];
						m_DlgCircleDoubleMeasureInspectTool.m_dCurRadius3 = searchResult.m_CircleCornerDistance.m_vSizeMM[2];
					}
			}

			if(m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistCheck)
			{
					if(searchResult.m_CircleCornerDistance.m_vDistMM.size() > 1)
					{
						if(searchResult.m_CircleCornerDistance.m_nDistControlResult == -1)
						{
							m_DlgCircleDoubleMeasureInspectTool.m_strCurDistResult = _T("NG");
						}else
						{
							m_DlgCircleDoubleMeasureInspectTool.m_strCurDistResult = _T("OK");
						}

						if (1+2*m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndexDistance <searchResult.m_CircleCornerDistance.m_vDistMM.size() && m_DlgCircleDoubleMeasureInspectTool.m_nDistanceType==3)
						{
							m_DlgCircleDoubleMeasureInspectTool.m_dCurDistance1 = searchResult.m_CircleCornerDistance.m_vDistMM[0+2*m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndexDistance];
							m_DlgCircleDoubleMeasureInspectTool.m_dCurDistance2 = searchResult.m_CircleCornerDistance.m_vDistMM[1+2*m_DlgCircleDoubleMeasureInspectTool.m_nCircleIndexDistance];
						}
						else
						{
							if (searchResult.m_CircleCornerDistance.m_vDistMM.size()>=2)
							{
								m_DlgCircleDoubleMeasureInspectTool.m_dCurDistance1 = searchResult.m_CircleCornerDistance.m_vDistMM[0];
								m_DlgCircleDoubleMeasureInspectTool.m_dCurDistance2 = searchResult.m_CircleCornerDistance.m_vDistMM[1];
							}
						}

					}
			}

			if(m_DlgCircleDoubleMeasureInspectTool.m_bEnableBlobInspect)
			{
				if(searchResult.m_CircleCornerDistance.m_nBlobResult == -1)
				{
					m_DlgCircleDoubleMeasureInspectTool.m_strCurInspectResult = _T("NG");
				}else
				{
					m_DlgCircleDoubleMeasureInspectTool.m_strCurInspectResult = _T("OK");
				}
				if(m_DlgCircleDoubleMeasureInspectTool.m_dCurBlobArea = searchResult.m_CircleCornerDistance.m_vBlobArea.size() > 0)
				{
					m_DlgCircleDoubleMeasureInspectTool.m_dCurBlobArea = searchResult.m_CircleCornerDistance.m_vBlobArea[0];
				}
			}

			if(m_DlgCircleDoubleMeasureInspectTool.m_bEnableBlobInspect
				||m_DlgCircleDoubleMeasureInspectTool.m_bEnableDistCheck
				||m_DlgCircleDoubleMeasureInspectTool.m_bEnableSizeCheck )
			{
				m_DlgCircleDoubleMeasureInspectTool.UpdateData(FALSE);
			}
		}
		break;
	case  2:
		{
			CSearchResult searchResult;
			m_pDlgParent->m_pSearchTool->GetResult(0, searchResult);
			if(m_DlgObroundMeasureInspectTool.m_bEnableSizeCheck)
			{
				if(searchResult.m_CircleCornerDistance.m_vSizeMM.size() > 1)
				{
					if(searchResult.m_CircleCornerDistance.m_nSizeControlResult == -1)
					{
						m_DlgObroundMeasureInspectTool.m_strCurSizeResult = _T("NG");
					}else
					{
						m_DlgObroundMeasureInspectTool.m_strCurSizeResult = _T("OK");
					}
					m_DlgObroundMeasureInspectTool.m_dCurRadius = searchResult.m_CircleCornerDistance.m_vSizeMM[0];
					m_DlgObroundMeasureInspectTool.m_dCurRadius2 = searchResult.m_CircleCornerDistance.m_vSizeMM[1];
				}
			}
			if(m_DlgObroundMeasureInspectTool.m_bEnableDistCheck)
			{
				if(searchResult.m_CircleCornerDistance.m_vDistMM.size() > 1)
				{
					if(searchResult.m_CircleCornerDistance.m_nDistControlResult == -1)
					{
						m_DlgObroundMeasureInspectTool.m_strCurDistResult = _T("NG");
					}else
					{
						m_DlgObroundMeasureInspectTool.m_strCurDistResult = _T("OK");
					}
					m_DlgObroundMeasureInspectTool.m_dCurDistance1 = searchResult.m_CircleCornerDistance.m_vDistMM[0];
					m_DlgObroundMeasureInspectTool.m_dCurDistance2 = searchResult.m_CircleCornerDistance.m_vDistMM[1];
				}
			}

			if(m_DlgObroundMeasureInspectTool.m_bEnableBlobInspect)
			{
				if(searchResult.m_CircleCornerDistance.m_nBlobResult == -1)
				{
					m_DlgObroundMeasureInspectTool.m_strCurInspectResult = _T("NG");
				}else
				{
					m_DlgObroundMeasureInspectTool.m_strCurInspectResult = _T("OK");
				}
				if(searchResult.m_CircleCornerDistance.m_vBlobArea.size() > 0)
				{
					m_DlgObroundMeasureInspectTool.m_dCurBlobArea = searchResult.m_CircleCornerDistance.m_vBlobArea[0];
				}

			}

			if(m_DlgObroundMeasureInspectTool.m_bEnableBlobInspect
				||m_DlgObroundMeasureInspectTool.m_bEnableDistCheck
				||m_DlgObroundMeasureInspectTool.m_bEnableSizeCheck)
			{
				m_DlgObroundMeasureInspectTool.UpdateData(FALSE);
			}
		}
		break;
	}
}


BOOL CDlgCircleCornerSearchCheckInspectTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
