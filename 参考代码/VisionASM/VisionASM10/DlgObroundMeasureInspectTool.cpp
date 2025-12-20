// DlgObroundMeasureInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgObroundMeasureInspectTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgObroundMeasureInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgObroundMeasureInspectTool, CDialogEx)

	CDlgObroundMeasureInspectTool::CDlgObroundMeasureInspectTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgObroundMeasureInspectTool::IDD, pParent)
	, m_bEnableDistCheck(FALSE)
	, m_dCurRadius(0)
	, m_dCurDistance1(0)
	, m_dCurDistance2(0)
	, m_bEnableBlobInspect(FALSE)
	, m_strSizeLabel1(_T("D1:"))
	, m_strDistLabel1(_T("D3:"))
	, m_strDistLabel2(_T("D4:"))
	, m_dCurBlobArea(0)
	, m_bEnableDispLineSeg(FALSE)
	, m_dAreaScaleRatio(0.9)
	, m_bEnableSizeCheck(FALSE)
	, m_bEnableDispLineSeg2(FALSE)
	, m_bSizeCheckUseToTool(FALSE)
	, m_bDistCheckUseToTool(FALSE)
	, m_bBlobInspectUseToTool(FALSE)
// 	, m_bEnableMaxCircleRadius2(FALSE)
// 	, m_dDistanceMaxCircleRadius2(0)
// 	, m_bEnableMinCircleRadius2(FALSE)
// 	, m_dDistanceMinCircleRadius2(0)
	, m_strSizeLabel2(_T("D2:"))
	, m_strCurSizeResult(_T(""))
	, m_strCurDistResult(_T(""))
	, m_strCurInspectResult(_T(""))
	, m_dCurRadius2(0)
	, m_bEnableCircleRadius1(FALSE)
	, m_bEnableCircleRadius2(FALSE)
	, m_dDistanceCircleRadiusBase1(1)
	, m_dDistanceCircleRadiusBase2(1)
	, m_dDistanceCircleRadiusFlu1(0)
	, m_dDistanceCircleRadiusFlu2(0)
	, m_bEnableDistance1(FALSE)
	, m_bEnableDistance2(FALSE)
	, m_dDistanceBase1(1)
	, m_dDistanceBase2(1)
	, m_dDistanceFlu1(0.2)
	, m_dDistanceFlu2(0.2)
{
	m_nObroundCaliperNum = 20;
	m_nObroundOutLineNum = 10;
	m_dObroundContrastThre = 10.0;
	m_nObroundPolarityMode = (int)eDontCare;
	m_nObroundInwardMode = (int)eInward;
	m_lObroundFilterHalfWidth = 1;
	m_bObroundSortByScore = TRUE;
	m_bShowObroundFitPoint = FALSE;

	m_dPixelRatio = 1.0;
// 	m_bEnableMax1 = FALSE;
// 	m_dDistanceMax1 = 1.0;
// 	m_bEnableMin1 = FALSE;
// 	m_dDistanceMin1 = 0.0;

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

CDlgObroundMeasureInspectTool::~CDlgObroundMeasureInspectTool()
{
}

void CDlgObroundMeasureInspectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_OBROUND_CALIPERNUM, m_nObroundCaliperNum);
	DDX_Text(pDX, IDC_EDT_OBROUND_OUTLINE_NUM, m_nObroundOutLineNum);
	DDX_Text(pDX, IDC_EDT_OBROUND_CONTRAST_THRE, m_dObroundContrastThre);
	DDX_Radio(pDX, IDC_RAD_OBROUND_POLARITY_MODE0, m_nObroundPolarityMode);
	DDX_Radio(pDX, IDC_RAD_OBROUND_INWARD_MODE0, m_nObroundInwardMode);
	DDX_Check(pDX, IDC_CHECK_OBROUND_SORT_BY_SCORE, m_bObroundSortByScore);
	DDX_Control(pDX, IDC_CHECK_OBROUND_SORT_BY_SCORE, m_checkObroundSortByScore);
	DDX_Check(pDX, IDC_CHECK_SHOW_OBROUND_FITPOINT, m_bShowObroundFitPoint);

	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS1, m_bEnableMaxCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, m_dDistanceMaxCircleRadius1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS1, m_bEnableMinCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, m_dDistanceMinCircleRadius1);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);

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
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL1, m_strSizeLabel1);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL2, m_strDistLabel1);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL3, m_strDistLabel2);
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA, m_dCurBlobArea);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG, m_bEnableDispLineSeg);
	DDX_Text(pDX, IDC_EDT_CIRCLE_AREA_SCALE_RATIO, m_dAreaScaleRatio);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SIZE_CONTROL, m_bEnableSizeCheck);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG3, m_bEnableDispLineSeg2);
	DDX_Check(pDX, IDC_CHECK_SIZE_CONTROL_USE_TO_TOOL2, m_bSizeCheckUseToTool);
	DDX_Check(pDX, IDC_CHECK_DIST_CONTROL_USE_TO_TOOL2, m_bDistCheckUseToTool);
	DDX_Check(pDX, IDC_CHECK_BLOB_CONTROL_USE_TO_TOOL2, m_bBlobInspectUseToTool);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS4, m_bEnableMaxCircleRadius2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS4, m_dDistanceMaxCircleRadius2);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS4, m_bEnableMinCircleRadius2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS4, m_dDistanceMinCircleRadius2);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL6, m_strSizeLabel2);
	DDX_Text(pDX, IDC_EDT_SIZE_CONTROL_RESULT, m_strCurSizeResult);
	DDX_Text(pDX, IDC_EDT_CIRCLE_DIST_CONTROL_RESULT, m_strCurDistResult);
	DDX_Text(pDX, IDC_EDT_BLOB_CONTROL_RESULT, m_strCurInspectResult);
	DDX_Text(pDX, IDC_EDT_DISTANCE_RADIUS4, m_dCurRadius2);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_1, m_bEnableCircleRadius1);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_2, m_bEnableCircleRadius2);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_1, m_dDistanceCircleRadiusBase1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_2, m_dDistanceCircleRadiusBase2);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_1, m_dDistanceCircleRadiusFlu1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_2, m_dDistanceCircleRadiusFlu2);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_1, m_bEnableDistance1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_2, m_bEnableDistance2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_1, m_dDistanceBase1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_2, m_dDistanceBase2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_1, m_dDistanceFlu1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_2, m_dDistanceFlu2);
}


BEGIN_MESSAGE_MAP(CDlgObroundMeasureInspectTool, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDT_OBROUND_CALIPERNUM, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundCalipernum)
	//	ON_EN_CHANGE(IDC_EDT_OBROUND_CALIPERNUM, &CDlgObroundMeasureInspectTool::OnEnChangeEdtObroundCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_OBROUND_OUTLINE_NUM, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_OBROUND_CONTRAST_THRE, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundContrastThre)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMax1)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMin1)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMax2)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMin2)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1)
// 	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1)
	ON_EN_SETFOCUS(IDC_EDT_BINARY_THRE, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtBinaryThre)
	ON_EN_SETFOCUS(IDC_EDT_MAX_AREA, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMaxArea)
	ON_EN_SETFOCUS(IDC_EDT_MIN_AREA, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMinArea)
	ON_EN_SETFOCUS(IDC_EDT_MAX_DEFECT_AREA, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea)
	ON_EN_CHANGE(IDC_EDT_OBROUND_CALIPERNUM, &CDlgObroundMeasureInspectTool::OnEnChangeEdtObroundCalipernum)
	ON_BN_CLICKED(IDC_RAD_OBROUND_INWARD_MODE0, &CDlgObroundMeasureInspectTool::OnBnClickedRadObroundInwardMode0)
	ON_BN_CLICKED(IDC_CHECK_SHOW_OBROUND_FITPOINT, &CDlgObroundMeasureInspectTool::OnBnClickedCheckShowObroundFitpoint)
	ON_BN_CLICKED(IDC_RAD_OBROUND_INWARD_MODE1, &CDlgObroundMeasureInspectTool::OnBnClickedRadObroundInwardMode1)
	ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtPixelRatio)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL3, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_AREA_SCALE_RATIO, &CDlgObroundMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio)
	ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL6, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel6)
	ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1)
	ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1)
	ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase2)
	ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu2)
	ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceBase1)
	ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_1, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceFlu1)
	ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceBase2)
	ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_2, &CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceFlu2)
END_MESSAGE_MAP()


void CDlgObroundMeasureInspectTool::CheckDlgDataValidation()
{

}

// 更新控件显示
void CDlgObroundMeasureInspectTool::UpdateControlDisplay()
{

}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundCalipernum()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_OBROUND_CALIPERNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_OBROUND_CALIPERNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_OBROUND_CALIPERNUM, strOutput);
		}
	}
}



void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundOutlineNum()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_OBROUND_OUTLINE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_OBROUND_OUTLINE_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_OBROUND_OUTLINE_NUM, strOutput);
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtObroundContrastThre()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_OBROUND_CONTRAST_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_OBROUND_CONTRAST_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_OBROUND_CONTRAST_THRE, strOutput);
		}
	}
}


// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMax1()
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
// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMin1()
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
// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMax2()
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
// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMin2()
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
// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1()
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
// void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtBinaryThre()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMaxArea()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMinArea()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea()
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


BOOL CDlgObroundMeasureInspectTool::OnInitDialog()
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

void CDlgObroundMeasureInspectTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_OBROUND_MEASURE_INSPECT_TOOL") == strDlgID)
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


void CDlgObroundMeasureInspectTool::OnEnChangeEdtObroundCalipernum()
{
	UpdateData(TRUE);
	if ((m_nObroundCaliperNum < 2) || (m_nObroundCaliperNum > 300))
	{
		return;
	}
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nObroundCaliperNum = m_nObroundCaliperNum;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgObroundMeasureInspectTool::OnBnClickedRadObroundInwardMode0()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_ObroundCaliperSearchDirection = (CaliperSearchDirection)m_nObroundInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgObroundMeasureInspectTool::OnBnClickedCheckShowObroundFitpoint()
{
	m_bShowObroundFitPoint = !m_bShowObroundFitPoint;

	((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowObroundPoint = m_bShowObroundFitPoint;
	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgObroundMeasureInspectTool::OnBnClickedRadObroundInwardMode1()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_ObroundCaliperSearchDirection = (CaliperSearchDirection)m_nObroundInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtPixelRatio()
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
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel4()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio()
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditInspectdisLabel6()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL6, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL6, strOutput);
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_BASE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_FLU_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_BASE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆2半径基准值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_FLU_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆2半径公差值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceBase1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_BASE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_BASE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceFlu1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_FLU_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_FLU_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceBase2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_BASE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_BASE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISTANCE_BASE_2, strOutput);
			CString strLog;
			strLog.Format(_T("修改[距离2基准值]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,6);
		}
	}
}


void CDlgObroundMeasureInspectTool::OnEnSetfocusEditDistanceFlu2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISTANCE_FLU_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISTANCE_FLU_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
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


BOOL CDlgObroundMeasureInspectTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
