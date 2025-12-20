// DlgLineToLineDistanceTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLineToLineDistanceTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// CDlgLineToLineDistanceTool 对话框

IMPLEMENT_DYNAMIC(CDlgLineToLineDistanceTool, CDialogEx)

CDlgLineToLineDistanceTool::CDlgLineToLineDistanceTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLineToLineDistanceTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_bEnableLineMask_1(FALSE)
	, m_bSortByScore2(TRUE)
	, m_bEnableLineMask_2(FALSE)
	, m_bShowFitPoint1(FALSE)
	, m_bShowFitPoint2(FALSE)
	, m_bEnableMax1(FALSE)
	, m_dDistanceMax1(0.1)
	, m_bEnableMin1(FALSE)
	, m_dDistanceMin1(0.0)
	, m_nDistanceMode(0)
	, m_nDistanceType(0)
	, m_bEnableLineDistanceTool(FALSE)
	, m_dInspectDistanceValue(0)
	, m_strInspectInfo(_T(""))
	, m_bEanbleOKNG_Decision(FALSE)
	, m_bEnableDualEdge(FALSE)
	, m_bEnableDualEdge2(FALSE)
	, m_strDistanceLabel(_T(""))
	, m_bShowCaliper_1(FALSE)
	, m_bShowCaliper_2(FALSE)
{
	m_nLineCaliperNum1 = 20;
	m_nLineCaliperNum2 = 20;

	m_dLineContrastThre1 = 10.0;
	m_dLineContrastThre2 = 10.0;

	m_lLineFilterHalfWidth1 = 1;
	m_lLineFilterHalfWidth2 = 1;

	m_nLineOutLineNum1 = 10;
	m_nLineOutLineNum2 = 10;
	m_dPixelRatio	   = 0.01;

	m_nLinePolarityMode1 = (int)eLightToDark;
	m_nLinePolarityMode2 = (int)eLightToDark;
	m_pDlgParent = NULL;
	m_pDlgParent2 = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgLineToLineDistanceTool::~CDlgLineToLineDistanceTool()
{
	m_pDlgParent = NULL;
	m_pDlgParent2 = NULL;
	m_psaSysInfoStrings = NULL;
}

void CDlgLineToLineDistanceTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	/*DDX_Text(pDX, IDC_EDT_LINE_HALF_WIDTH_VALUE, m_lLineFilterHalfWidth1);*/
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);

	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_2, m_nLineOutLineNum2);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_2, m_dLineContrastThre2);
	//DDX_Text(pDX, IDC_EDT_LINE_HALF_WIDTH_VALUE2, m_lLineFilterHalfWidth2);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_2, m_nLinePolarityMode2);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_2, m_bEnableLineMask_2);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_2, m_nLineCaliperNum2);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint1);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_2, m_bShowFitPoint2);
	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);
	DDX_Radio(pDX, IDC_RAD_L1_TO_L2_MODE, m_nDistanceMode);
	DDX_Radio(pDX, IDC_RAD_DISTANCE_OPTION, m_nDistanceType);
	DDX_Check(pDX, IDC_CHECK_ENABLE_DISTANCE_INSPECT, m_bEnableLineDistanceTool);
	DDX_Text(pDX, IDC_EDT_DISTANCE_1, m_dInspectDistanceValue);
	DDX_Text(pDX, IDC_EDT_DISTANCE_2, m_strInspectInfo);
	DDX_Check(pDX, IDC_CHK_ENABLE_OKNG_DECISION, m_bEanbleOKNG_Decision);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE3, m_bEnableDualEdge2);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL, m_strDistanceLabel);
	DDV_MaxChars(pDX, m_strDistanceLabel, 5);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_2, m_bShowCaliper_2);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_2, m_lLineFilterHalfWidth2);

}


BEGIN_MESSAGE_MAP(CDlgLineToLineDistanceTool, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DISTANCE_INSPECT, &CDlgLineToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgLineToLineDistanceTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgLineToLineDistanceTool::OnBnClickedCheckShowFitpoint2)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgLineToLineDistanceTool::OnBnClickedBtnGetTrainImage)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_HALF_WIDTH_VALUE, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineHalfWidthValue)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_2, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_2, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_2, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineContrastThre2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_HALF_WIDTH_VALUE2, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineHalfWidthValue2)
	ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtPixelRatio)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtDistanceMax1)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, &CDlgLineToLineDistanceTool::OnEnSetfocusEdtDistanceMin1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgLineToLineDistanceTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgLineToLineDistanceTool::OnBnClickedCheckLineMask2)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL, &CDlgLineToLineDistanceTool::OnEnSetfocusEditInspectdisLabel)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgLineToLineDistanceTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgLineToLineDistanceTool::OnBnClickedChkShowLineCaliper2)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, &CDlgLineToLineDistanceTool::OnEnChangeEdtLineCalipernum1)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_2, &CDlgLineToLineDistanceTool::OnEnChangeEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)
END_MESSAGE_MAP()


// CDlgLineToLineDistanceTool 消息处理程序


BOOL CDlgLineToLineDistanceTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
// 	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
// 	if (m_psaSysInfoStrings != NULL)
// 	{
// 		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
// 	}

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	
}


BOOL CDlgLineToLineDistanceTool::PreTranslateMessage(MSG* pMsg)
{

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgLineToLineDistanceTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_LINE_TO_LINE_DISTANCE_INSPECT") == strDlgID)
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

void CDlgLineToLineDistanceTool::UpdateControlDisplay()
{
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
	}
}

void CDlgLineToLineDistanceTool::UpdateTrainData(bool bValidate)
{

	//if (!m_pDlgParent->m_pSearchTool)
	//{
	//	return;
	//}

	//m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	//m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	//if (bValidate)	// 从界面获取训练参数
	//{
	//	UpdateData(TRUE);

	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
	//	((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
	//	//((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;

	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_nLineOutLineNum2;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_dLineContrastThre2;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_lLineFilterHalfWidth2;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_nLinePolarityMode2;
	//	((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_bSortByScore2;
	//	((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
	//	//((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
	//	m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	//	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	//	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	//	m_pDlgParent->m_bTrainDataUpdated = TRUE;
	//	m_pDlgParent->m_bSearchDataUpdated = FALSE;
	//}
	//else // 设置界面值
	//{

	//	m_nLineOutLineNum1 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
	//	m_dLineContrastThre1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
	//	m_lLineFilterHalfWidth1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
	//	m_nLinePolarityMode1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
	//	m_bSortByScore1 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
	//	m_nLineCaliperNum1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
	//	//m_bEnableLineMask_1 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];

	//	m_nLineOutLineNum2 =  ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
	//	m_dLineContrastThre2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
	//	m_lLineFilterHalfWidth2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
	//	m_nLinePolarityMode2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
	//	m_bSortByScore2 = ((CCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
	//	m_nLineCaliperNum2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
	//	//m_bEnableLineMask_2 = ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];

	//	UpdateData(FALSE);
	//}

}

void CDlgLineToLineDistanceTool::UpdateSearchData(bool bValidate)
{

//	if (!m_pDlgParent->m_pSearchTool)
//	{
//		return;
//	}
//
//	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
//	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
//
//	if (bValidate) // 从界面获取搜索参数
//	{
//		UpdateData(TRUE);
//
//		((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1 = m_bShowFitPoint1;
//		((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2 = m_bShowFitPoint2;
//
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio = m_dPixelRatio;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode = m_nDistanceMode;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType = m_nDistanceType;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax = m_bEnableMax1;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax = m_dDistanceMax1;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin = m_bEnableMin1;
//// 		((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin = m_dDistanceMin1;
//
//		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
//		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
//		m_pDlgParent->m_bSearchDataUpdated = TRUE;
//
//	}
//	else // 设置界面值
//	{
//		m_bShowFitPoint1 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1;
//		m_bShowFitPoint2 = ((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2;
//
//// 		m_dPixelRatio = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio;
//// 		m_nDistanceMode = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceMode;
//// 		m_nDistanceType = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nDistanceType;
//// 		m_bEnableMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax;
//// 		m_dDistanceMax1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax;
//// 		m_bEnableMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin;
//// 		m_dDistanceMin1 = ((CCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin;
//		UpdateData(FALSE);
//	}

}

void CDlgLineToLineDistanceTool::CheckDlgDataValidation()
{

	UpdateData(TRUE);

	if (m_nLineCaliperNum1 < 2)
	{
		m_nLineCaliperNum1 = 3;
	}
	if (m_nLineCaliperNum1 > 100)
	{
		m_nLineCaliperNum1 = 100;
	}

	if ((m_nLineOutLineNum1 < 0) || (m_nLineOutLineNum1 > (m_nLineCaliperNum1 - 2)))
	{
		m_nLineOutLineNum1 = 0;
	}
	if ((m_dLineContrastThre1 < 0.0) || (m_dLineContrastThre1 > 255.0))
	{
		m_dLineContrastThre1 = 10;
	}
	if (m_lLineFilterHalfWidth1 < 1)
	{
		m_lLineFilterHalfWidth1 = 1;
	}

	if (m_nLineCaliperNum2 < 2)
	{
		m_nLineCaliperNum2 = 3;
	}
	if (m_nLineCaliperNum2 > 100)
	{
		m_nLineCaliperNum2 = 100;
	}

	if ((m_nLineOutLineNum2 < 0) || (m_nLineOutLineNum2 > (m_nLineCaliperNum2 - 2)))
	{
		m_nLineOutLineNum2 = 0;
	}
	if ((m_dLineContrastThre2 < 0.0) || (m_dLineContrastThre2 > 255.0))
	{
		m_dLineContrastThre2 = 10;
	}
	if (m_lLineFilterHalfWidth2 < 1)
	{
		m_lLineFilterHalfWidth2 = 1;
	}

	UpdateData(FALSE);

}

void CDlgLineToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect()
{
	UpdateData(TRUE);
	m_pDlgParent2->OnBnClickedBtnMarkGettrainimage();
}


void CDlgLineToLineDistanceTool::OnBnClickedCheckShowFitpoint1()
{
	UpdateData(TRUE);
	m_pDlgParent2->UpdateSearchData(true);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

	//m_pDlgParent2->UpdateSearchData(true);
}


void CDlgLineToLineDistanceTool::OnBnClickedCheckShowFitpoint2()
{
	UpdateData(TRUE);
	m_pDlgParent2->UpdateSearchData(true);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgLineToLineDistanceTool::OnBnClickedBtnGetTrainImage()
{

}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineCalipernum1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strInput);
		
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineContrastThre1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strOutput);
		}
	}

}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineHalfWidthValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_HALF_WIDTH_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_HALF_WIDTH_VALUE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_HALF_WIDTH_VALUE, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineCalipernum2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineContrastThre2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_2, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtLineHalfWidthValue2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_HALF_WIDTH_VALUE2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_HALF_WIDTH_VALUE2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_HALF_WIDTH_VALUE2, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtPixelRatio()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_PIXEL_RATIO)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_PIXEL_RATIO, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_PIXEL_RATIO, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtDistanceMax1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MAX_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEdtDistanceMin1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MIN_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strOutput);
		}
	}
}


void CDlgLineToLineDistanceTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableLineMask_1 = !m_bEnableLineMask_1;
	m_pDlgParent2->UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();

}


void CDlgLineToLineDistanceTool::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableLineMask_2 = !m_bEnableLineMask_2;
	m_pDlgParent2->UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEditInspectdisLabel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput, FALSE))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL, strOutput);
		}
	}	
}


void CDlgLineToLineDistanceTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
}


void CDlgLineToLineDistanceTool::OnBnClickedChkShowLineCaliper2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
}


void CDlgLineToLineDistanceTool::OnEnChangeEdtLineCalipernum1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
}


void CDlgLineToLineDistanceTool::OnEnChangeEdtLineCalipernum2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
}


void CDlgLineToLineDistanceTool::OnEnSetfocusEditLineFilterHalfWidth1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strOutput);
		}
	}

}

void CDlgLineToLineDistanceTool::OnEnSetfocusEditLineFilterHalfWidth2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_2, strOutput);
		}
	}

}