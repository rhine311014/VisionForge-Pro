// E:\项目\软件代码\VisionASM20200722\VisionASM20200722(V1.99.1.0.0.0)临时版本\VisionASM10\DlgCircleToLineDistanceTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCircleToLineDistanceTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// CDlgCircleToLineDistanceTool 对话框

IMPLEMENT_DYNAMIC(CDlgCircleToLineDistanceTool, CDialogEx)

CDlgCircleToLineDistanceTool::CDlgCircleToLineDistanceTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCircleToLineDistanceTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_bEnableLineMask_1(FALSE)
	, m_bSortByScore2(TRUE)
	, m_bShowFitPoint1(FALSE)
	, m_bEnableMax1(FALSE)
	, m_dDistanceMax1(0.1)
	, m_bEnableMin1(FALSE)
	, m_dDistanceMin1(0.0)
	, m_bEnableLineDistanceTool(FALSE)
	, m_dInspectDistanceValue(0)
	, m_strInspectInfo(_T(""))
	, m_bEanbleOKNG_Decision(FALSE)
	, m_bEnableDualEdge(FALSE)
	, m_strDistanceLabel(_T(""))
	, m_bShowCaliper_1(FALSE)
	, m_bEnableShowFitCirclePoint(FALSE)
	, m_bShowCircleCaliper(FALSE)
	, m_bEnableCircleDualEdge(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_bEnableCircleDistanceTool(FALSE)
{
	m_nLineCaliperNum1 = 20;

	m_dLineContrastThre1 = 10.0;

	m_lLineFilterHalfWidth1 = 1;

	m_nLineOutLineNum1 = 10;
	m_dPixelRatio	   = 0.01;

	m_nLinePolarityMode1 = (int)eLightToDark;

	m_nLineCaliperNum1 = 20;
	m_dLineContrastThre1 = 10.0;
	m_lLineFilterHalfWidth1 = 1;
	m_nLineOutLineNum1 = 10;
	m_nLinePolarityMode1 = (int)eLightToDark;

	// 圆参数
	m_nCircleInwardMode			= (int)eInward;
	m_nCirclePolarityMode		= (int)eDarkToLight;
	m_nCircleCaliperNum			= 20;
	m_dCircleContrastThre		= 10.0;
	m_lCircleFilterHalfWidth	= 1;
	m_nCircleOutLineNum			= 10;
}

CDlgCircleToLineDistanceTool::~CDlgCircleToLineDistanceTool()
{
}

void CDlgCircleToLineDistanceTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);

	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);

	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM_1, m_nCircleOutLineNum);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Control(pDX, IDC_CHECK_SHOW_FIT_CIRCLE_POINT, m_checkShowFitCirclePoint);
	DDX_Check(pDX, IDC_CHECK_SHOW_FIT_CIRCLE_POINT, m_bEnableShowFitCirclePoint);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHK_CIRCLR_ENABLE_DUAL_EDGE, m_bEnableCircleDualEdge);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_MASK, m_bEnableCircleMask);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);


	DDX_Text(pDX, IDC_EDT_DISTANCE_1, m_dInspectDistanceValue);
	DDX_Text(pDX, IDC_EDT_DISTANCE_2, m_strInspectInfo);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint1);
	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);

	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);

	DDX_Check(pDX, IDC_CHECK_ENABLE_DISTANCE_INSPECT, m_bEnableLineDistanceTool);
	DDX_Check(pDX, IDC_CHK_ENABLE_OKNG_DECISION, m_bEanbleOKNG_Decision);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL, m_strDistanceLabel);
	DDV_MaxChars(pDX, m_strDistanceLabel, 5);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Check(pDX, IDC_CHECK_ENABLE_DISTANCE_INSPECT1, m_bEnableCircleDistanceTool);
}


BEGIN_MESSAGE_MAP(CDlgCircleToLineDistanceTool, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DISTANCE_INSPECT, &CDlgCircleToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, &CDlgCircleToLineDistanceTool::OnEnChangeEdtLineCalipernum1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgCircleToLineDistanceTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgCircleToLineDistanceTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgCircleToLineDistanceTool::OnBnClickedCheckLineMask1)
	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleToLineDistanceTool::OnEnChangeEdtCircleCalipernum)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgCircleToLineDistanceTool::OnBnClickedChkShowCircleCaliper)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FIT_CIRCLE_POINT, &CDlgCircleToLineDistanceTool::OnBnClickedCheckShowFitCirclePoint)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_MASK, &CDlgCircleToLineDistanceTool::OnBnClickedCheckCircleMask)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgCircleToLineDistanceTool::OnBnClickedRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgCircleToLineDistanceTool::OnBnClickedRadCircleInwardMode1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineFilterHalfwidth1)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleFilterHalfwidth1)
	ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtPixelRatio)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgCircleToLineDistanceTool::OnEnSetfocusEdtDistanceMax1)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL, &CDlgCircleToLineDistanceTool::OnEnSetfocusEditInspectdisLabel)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DISTANCE_INSPECT1, &CDlgCircleToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect1)
END_MESSAGE_MAP()


// CDlgCircleToLineDistanceTool 消息处理程序

void CDlgCircleToLineDistanceTool::UpdateControlDisplay()
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

void CDlgCircleToLineDistanceTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_CIRCLE_TO_LINE_DISTANCE_INSPECT") == strDlgID)
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


BOOL CDlgCircleToLineDistanceTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgCircleToLineDistanceTool::PreTranslateMessage(MSG* pMsg)
{

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgCircleToLineDistanceTool::CheckDlgDataValidation()
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

	if (m_nCircleCaliperNum < 3)
	{
		m_nCircleCaliperNum = 3;
	}
	if ((m_nCircleOutLineNum < 0) || (m_nCircleOutLineNum > (m_nCircleCaliperNum - 3)))
	{
		m_nCircleOutLineNum = 0;
	}
	if ((m_dCircleContrastThre < 0.0) || (m_dCircleContrastThre > 255.0))
	{
		m_dCircleContrastThre = 10;
	}
	if (m_lCircleFilterHalfWidth < 1)
	{
		m_lCircleFilterHalfWidth = 1;
	}

	UpdateData(FALSE);

}


void CDlgCircleToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect()
{
	UpdateData(TRUE);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENABLE_DISTANCE_INSPECT1);
	pBtn->SetCheck(0);

	m_pDlgParent2->OnBnClickedBtnMarkGettrainimage();
}


void CDlgCircleToLineDistanceTool::OnEnChangeEdtLineCalipernum1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnBnClickedCheckShowFitpoint1()
{
	UpdateData(TRUE);
	m_pDlgParent2->UpdateSearchData(true);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleToLineDistanceTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableLineMask_1 = !m_bEnableLineMask_1;
	m_pDlgParent2->UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgCircleToLineDistanceTool::OnEnChangeEdtCircleCalipernum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnBnClickedCheckShowFitCirclePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pDlgParent2->UpdateSearchData(true);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleToLineDistanceTool::OnBnClickedCheckCircleMask()
{
	m_bEnableCircleMask = !m_bEnableCircleMask;
	m_pDlgParent2->UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgCircleToLineDistanceTool::OnBnClickedRadCircleInwardMode0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
		m_pDlgParent2->UpdateSearchData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnBnClickedRadCircleInwardMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent2->UpdateTrainData(true);
		m_pDlgParent2->UpdateSearchData(true);
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineCalipernum1()
{
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


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineOutlineNum1()
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


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineContrastThre1()
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


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtLineFilterHalfwidth1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strOutput);
		}
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_CALIPERNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_CALIPERNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_CALIPERNUM, strOutput);
		}
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleOutlineNum1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM_1, strOutput);
		}
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_CONTRAST_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_CONTRAST_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_CONTRAST_THRE, strOutput);
		}
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtCircleFilterHalfwidth1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, strOutput);
		}
	}
}


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtPixelRatio()
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


void CDlgCircleToLineDistanceTool::OnEnSetfocusEdtDistanceMax1()
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


void CDlgCircleToLineDistanceTool::OnEnSetfocusEditInspectdisLabel()
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


void CDlgCircleToLineDistanceTool::OnBnClickedCheckEnableDistanceInspect1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	//m_bEnableLineDistanceTool = FALSE;

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ENABLE_DISTANCE_INSPECT);
	pBtn->SetCheck(0);
	

	m_pDlgParent2->OnBnClickedBtnMarkGettrainimage();
}
