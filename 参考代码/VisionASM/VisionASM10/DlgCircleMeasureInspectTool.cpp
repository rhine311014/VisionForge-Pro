// DlgCircleMeasureInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCircleMeasureInspectTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgCircleMeasureInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgCircleMeasureInspectTool, CDialogEx)

CDlgCircleMeasureInspectTool::CDlgCircleMeasureInspectTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCircleMeasureInspectTool::IDD, pParent)
	, m_bEnableDistCheck(FALSE)
	, m_dCurRadius(0)
	, m_dCurDistance1(0)
	, m_dCurDistance2(0)
	, m_bEnableBlobInspect(FALSE)
	, m_strSizetLabel1(_T("D1:"))
	, m_strDistLabel2(_T("D2:"))
	, m_strDistLabel3(_T("D3:"))
	, m_dCurBlobArea(0)
	, m_bEnableDispLineSeg(FALSE)
	, m_dAreaScaleRatio(0.9)
	, m_bEnableDispLineSeg2(FALSE)
	, m_bSizeCheckUseToTool(FALSE)
	, m_bDistCheckUseToTool(FALSE)
	, m_strCurSizeResult(_T(""))
	, m_strCurDistResult(_T(""))
	, m_strCurInspectResult(_T(""))
	, m_bEnableSizeCheck(FALSE)
	, m_bBlobInspectUseToTool(FALSE)
	, m_bEnableCircleRadius1(FALSE)
	, m_dDistanceCircleRadiusBase1(1)
	, m_dDistanceCircleRadiusFlu1(0.2)
	, m_bEnableDistance1(FALSE)
	, m_dDistanceBase1(1)
	, m_dDistanceFlu1(0.2)

	, m_bEnableDistance2(FALSE)
	, m_dDistanceBase2(1)
	, m_dDistanceFlu2(0.2)
{
	m_nCircleCaliperNum = 20;
	m_nCircleOutLineNum = 10;
	m_dCircleContrastThre = 10.0;
	m_nCirclePolarityMode = (int)eDontCare;
	m_nCircleInwardMode = (int)eInward;
	m_lCircleFilterHalfWidth = 1;
	m_bCircleSortByScore = TRUE;
	m_bShowCircleFitPoint = FALSE;

	m_dPixelRatio = 1.0;
 		
// 	m_bEnableMax1 = FALSE;
// 	m_dDistanceMax1 = 1.0;
// 	m_bEnableMin1 = FALSE;
// 	m_dDistanceMin1 = 0.0;
// 
// 	m_bEnableMax2 = FALSE;
// 	m_dDistanceMax2 = 1.0;
// 	m_bEnableMin2 = FALSE;
// 	m_dDistanceMin2 = 0.0;
// 
// 	m_bEnableMaxCircleRadius1 = FALSE ;
// 	m_dDistanceMaxCircleRadius1  = 1.0;
// 	m_bEnableMinCircleRadius1 = FALSE;
// 	m_dDistanceMinCircleRadius1 = 0.0;


	m_nBinaryThre = 80;
	m_nMinAcceptArea = 3;
	m_nMaxAcceptArea = 100000;
	m_dMaxDefectArea = 50.0;
}

CDlgCircleMeasureInspectTool::~CDlgCircleMeasureInspectTool()
{
}

void CDlgCircleMeasureInspectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM, m_nCircleOutLineNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_bCircleSortByScore);
	DDX_Control(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_checkCircleSortByScore);
	DDX_Check(pDX, IDC_CHECK_SHOW_CIRCLE_FITPOINT, m_bShowCircleFitPoint);

	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS1, m_bEnableMaxCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, m_dDistanceMaxCircleRadius1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS1, m_bEnableMinCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, m_dDistanceMinCircleRadius1);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);
	// 
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_2, m_bEnableMax2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_2, m_dDistanceMax2);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_2, m_bEnableMin2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_2, m_dDistanceMin2);
	DDX_Check(pDX, IDC_CHECK_ENABLE_DISTANCE_CONTROL, m_bEnableDistCheck);
	DDX_Text(pDX, IDC_EDT_DISTANCE_RADIUS, m_dCurRadius);
	DDX_Text(pDX, IDC_EDT_DISTANCE_1, m_dCurDistance1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_2, m_dCurDistance2);

	DDX_Control(pDX, IDC_COMBO_DEFECT_COLOR_TYPE, m_comboDefectType);
	DDX_Text(pDX, IDC_EDT_BINARY_THRE, m_nBinaryThre);
	DDX_Text(pDX, IDC_EDT_MIN_AREA, m_nMinAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_AREA, m_nMaxAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_DEFECT_AREA, m_dMaxDefectArea);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BLOB_INSPECT, m_bEnableBlobInspect);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL1, m_strSizetLabel1);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL2, m_strDistLabel2);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL3, m_strDistLabel3);
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA, m_dCurBlobArea);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG, m_bEnableDispLineSeg);
	DDX_Text(pDX, IDC_EDT_CIRCLE_AREA_SCALE_RATIO, m_dAreaScaleRatio);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG2, m_bEnableDispLineSeg2);
	DDX_Check(pDX, IDC_CHECK_SIZE_CONTROL_USE_TO_TOOL, m_bSizeCheckUseToTool);
	DDX_Check(pDX, IDC_CHECK_DIST_CONTROL_USE_TO_TOOL, m_bDistCheckUseToTool);
	DDX_Text(pDX, IDC_EDT_SIZE_CONTROL_RESULT, m_strCurSizeResult);
	DDX_Text(pDX, IDC_EDT_DIST_CONTROL_RESULT, m_strCurDistResult);
	DDX_Text(pDX, IDC_EDT_BLOB_CONTROL_RESULT, m_strCurInspectResult);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SIZE_CONTROL, m_bEnableSizeCheck);
	DDX_Check(pDX, IDC_CHECK_BLOB_CONTROL_USE_TO_TOOL, m_bBlobInspectUseToTool);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_1, m_bEnableCircleRadius1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_1, m_dDistanceCircleRadiusBase1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_1, m_dDistanceCircleRadiusFlu1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_1, m_bEnableDistance1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_1, m_dDistanceBase1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_1, m_dDistanceFlu1);

	DDX_Check(pDX, IDC_CHECK_DISTANCE_2, m_bEnableDistance2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_2, m_dDistanceBase2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_2, m_dDistanceFlu2);
}


BEGIN_MESSAGE_MAP(CDlgCircleMeasureInspectTool, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleCalipernum)
//	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleMeasureInspectTool::OnEnChangeEdtCircleCalipernum)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleOutlineNum)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleContrastThre)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMax1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMin1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_2, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMax2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_2, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMin2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1)
ON_EN_SETFOCUS(IDC_EDT_BINARY_THRE, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtBinaryThre)
ON_EN_SETFOCUS(IDC_EDT_MAX_AREA, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMaxArea)
ON_EN_SETFOCUS(IDC_EDT_MIN_AREA, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMinArea)
ON_EN_SETFOCUS(IDC_EDT_MAX_DEFECT_AREA, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea)
ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleMeasureInspectTool::OnEnChangeEdtCircleCalipernum)
ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgCircleMeasureInspectTool::OnBnClickedRadCircleInwardMode0)
ON_BN_CLICKED(IDC_CHECK_SHOW_CIRCLE_FITPOINT, &CDlgCircleMeasureInspectTool::OnBnClickedCheckShowCircleFitpoint)
ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgCircleMeasureInspectTool::OnBnClickedRadCircleInwardMode1)
ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtPixelRatio)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL2, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL3, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_AREA_SCALE_RATIO, &CDlgCircleMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceBase1)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_1, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceFlu1)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_2, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceBase2)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_2, &CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceFlu2)
END_MESSAGE_MAP()


void CDlgCircleMeasureInspectTool::CheckDlgDataValidation()
{

}

// 更新控件显示
void CDlgCircleMeasureInspectTool::UpdateControlDisplay()
{

}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleCalipernum()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_CALIPERNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_CALIPERNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_CALIPERNUM, strOutput);
		}
	}
}



void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleOutlineNum()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_OUTLINE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtCircleContrastThre()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_CONTRAST_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_CONTRAST_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_CONTRAST_THRE, strOutput);
		}
	}
}


// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMax1()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MAX_1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMin1()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MIN_1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMax2()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MAX_2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMin2()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MIN_2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MIN_2, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MIN_2, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, strOutput);
// 		}
// 	}
// }




void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtBinaryThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BINARY_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BINARY_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BINARY_THRE, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMaxArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_AREA, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMinArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MIN_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MIN_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MIN_AREA, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_DEFECT_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_DEFECT_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_DEFECT_AREA, strOutput);
		}
	}
}


BOOL CDlgCircleMeasureInspectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;

	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
	}

	m_comboDefectType.InsertString(0, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_WHITE));
	m_comboDefectType.InsertString(1, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_BLACK));
	m_comboDefectType.SetCurSel(0);

	return TRUE;  
}

void CDlgCircleMeasureInspectTool::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
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

		if(_T("IDD_DLG_CIRCLE_MEASURE_INSPECT_TOOL") == strDlgID)
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


void CDlgCircleMeasureInspectTool::OnEnChangeEdtCircleCalipernum()
{
	UpdateData(TRUE);
	if ((m_nCircleCaliperNum < 2) || (m_nCircleCaliperNum > 300))
	{
		return;
	}
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[0] = m_nCircleCaliperNum;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleMeasureInspectTool::OnBnClickedRadCircleInwardMode0()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[0] = (CaliperSearchDirection)m_nCircleInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleMeasureInspectTool::OnBnClickedCheckShowCircleFitpoint()
{
	m_bShowCircleFitPoint = !m_bShowCircleFitPoint;

	((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[0] = m_bShowCircleFitPoint;
	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleMeasureInspectTool::OnBnClickedRadCircleInwardMode1()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[0] = (CaliperSearchDirection)m_nCircleInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtPixelRatio()
{
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[像素当量值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL2, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL3, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel4()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL3, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL1, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_AREA_SCALE_RATIO)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_AREA_SCALE_RATIO, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_AREA_SCALE_RATIO, strOutput);
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_BASE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆1半径基准值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_FLU_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆1半径公差值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceBase1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_BASE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_BASE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISTANCE_BASE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[距离2基准值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceFlu1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_FLU_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_FLU_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISTANCE_FLU_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[距离1公差值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceBase2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_BASE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_BASE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISTANCE_BASE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[距离2基准值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleMeasureInspectTool::OnEnSetfocusEditDistanceFlu2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_FLU_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_FLU_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISTANCE_FLU_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[距离2公差值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


BOOL CDlgCircleMeasureInspectTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
