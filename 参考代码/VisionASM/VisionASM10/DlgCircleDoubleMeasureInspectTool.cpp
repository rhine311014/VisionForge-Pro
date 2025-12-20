// DlgCircleMeasureInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCircleDoubleMeasureInspectTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgCircleDoubleMeasureInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgCircleDoubleMeasureInspectTool, CDialogEx)

CDlgCircleDoubleMeasureInspectTool::CDlgCircleDoubleMeasureInspectTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCircleDoubleMeasureInspectTool::IDD, pParent)
	, m_bEnableDistCheck(FALSE)
	, m_dCurRadius(0)
	, m_dCurDistance1(0)
	, m_dCurDistance2(0)
	, m_bEnableBlobInspect(FALSE)
	, m_strSizeLabel1(_T("D1:"))
	, m_strDistLabel1(_T("D4:"))
	, m_strDistLabel2(_T("D5:"))
	, m_dCurBlobArea(0)
	, m_nCircleIndex(FALSE)
	//, m_nCircleMeasureIndex(0)
	, m_bEnableDispLineSeg(FALSE)
	, m_dAreaScaleRatio(0.9)
	, m_bEnableSizeCheck(FALSE)
	, m_bEnableDispLineSeg2(FALSE)
	, m_bBlobInspectUseToTool(FALSE)
	, m_bDistCheckUseToTool(FALSE)
	, m_bSizeCheckUseToTool(FALSE)
// 	, m_bEnableMaxCircleRadius2(FALSE)
// 	, m_dDistanceMaxCircleRadius2(0)
// 	, m_bEnableMinCircleRadius2(FALSE)
// 	, m_dDistanceMinCircleRadius2(0)
	, m_strSizeLabel2(_T("D2:"))
	, m_strSizeLabel3(_T("D3:"))
// 	, m_bEnableMaxCircleRadius3(FALSE)
// 	, m_dDistanceMaxCircleRadius3(0)
// 	, m_bEnableMinCircleRadius3(FALSE)
// 	, m_dDistanceMinCircleRadius3(0)
	, m_strCurSizeResult(_T(""))
	, m_strCurDistResult(_T(""))
	, m_strCurInspectResult(_T(""))
	, m_dCurRadius2(0)
	, m_dCurRadius3(0)
	, m_nDistanceType(0)
	, m_nCircleIndexDistance(0)
	, m_dDistanceCircleRadiusBase1(1)
	, m_dDistanceCircleRadiusBase2(1)
	, m_dDistanceCircleRadiusBase3(1)
	, m_dDistanceCircleRadiusFlu1(0)
	, m_dDistanceCircleRadiusFlu2(0)
	, m_dDistanceCircleRadiusFlu3(0)
	, m_dDistanceBase1(1)
	, m_dDistanceBase2(1)
	, m_dDistanceFlu1(0.2)
	, m_dDistanceFlu2(0.2)
	, m_bEnableCircleRadius1(FALSE)
	, m_bEnableCircleRadius2(FALSE)
	, m_bEnableCircleRadius3(FALSE)
	, m_bEnableDistance1(FALSE)
	, m_bEnableDistance2(FALSE)
{
	m_nCircleCaliperNum = 20;
	m_nCircleOutLineNum = 10;
	m_dCircleContrastThre = 10.0;
	m_nCirclePolarityMode = (int)eDontCare;
	m_nCircleInwardMode = (int)eInward;
	m_lCircleFilterHalfWidth = 1;
	m_bCircleSortByScore = TRUE;
	m_bShowCircleFitPoint = FALSE;

	for (int it =0 ;it<2;it++)
	{
		m_nCircleCaliperNumTmp[it] =20;
		m_nCircleOutLineNumTmp[it] =10;
		m_dCircleContrastThreTmp[it] =10.0;
		m_nCirclePolarityModeTmp[it] =(int)eDontCare;
		m_nCircleInwardModeTmp[it] =(int)eInward;
		m_lCircleFilterHalfWidthTmp[it] =1;
		m_bCircleSortByScoreTmp[it] =TRUE;
		m_bShowCircleFitPointTmp[it] =FALSE;
	}

	for (int it=0;it<3;it++)
	{
		m_bEnableDistance1Tmp[it] = FALSE;
		m_dDistanceBase1Tmp[it] = 1;
		m_dDistanceFlu1Tmp[it] = 0.2;
// 		m_bEnableMax1Tmp[it] =  FALSE ;
// 		m_dDistanceMax1Tmp[it] = 1.0;
// 		m_bEnableMin1Tmp[it] =FALSE;
// 		m_dDistanceMin1Tmp[it] =  0.0;

		m_bEnableDistance1Tmp[it] = FALSE;
		m_dDistanceBase1Tmp[it] = 1;
		m_dDistanceFlu1Tmp[it] = 0.2;
// 		m_bEnableCircle2Tmp[it] = FALSE;
// 		m_dCircleRadiusBase2Tmp[it] = 1;
// 		m_dCircleRadiusFlu2Tmp[it] = 0;
// 		m_bEnableMax2Tmp[it] =  FALSE ;
// 		m_dDistanceMax2Tmp[it] =1.0;
// 		m_bEnableMin2Tmp[it] = FALSE ;
// 		m_dDistanceMin2Tmp[it] =0.0;
	}

	m_nPreCicleIndex = m_nCircleIndex;
	m_nPreCicleIndex2 = m_nCircleIndexDistance;
	m_dPixelRatio = 1.0;
// 	m_bEnableMax1 = FALSE;
// 	m_dDistanceMax1 = 1.0;
// 	m_bEnableMin1 = FALSE;
// 	m_dDistanceMin1 = 0.0;

// 	m_bEnableMax2 = FALSE;
// 	m_dDistanceMax2 = 1.0;
// 	m_bEnableMin2 = FALSE;
// 	m_dDistanceMin2 = 0.0;

// 	m_bEnableMaxCircleRadius1 = FALSE ;
// 	m_dDistanceMaxCircleRadius1  = 1.0;
// 	m_bEnableMinCircleRadius1 = FALSE;
// 	m_dDistanceMinCircleRadius1 = 0.0;


	m_nBinaryThre = 80;
	m_nMinAcceptArea = 3;
	m_nMaxAcceptArea = 100000;
	m_dMaxDefectArea = 50.0;
}

CDlgCircleDoubleMeasureInspectTool::~CDlgCircleDoubleMeasureInspectTool()
{
}

void CDlgCircleDoubleMeasureInspectTool::DoDataExchange(CDataExchange* pDX)
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

	DDX_Check(pDX, IDC_CHECK_DIST_CONTROL_USE_TO_TOOL, m_bDistCheckUseToTool);
	DDX_Check(pDX, IDC_CHECK_SIZE_CONTROL_USE_TO_TOOL, m_bSizeCheckUseToTool);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SIZE_CONTROL, m_bEnableSizeCheck);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS1, m_bEnableMaxCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, m_dDistanceMaxCircleRadius1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS1, m_bEnableMinCircleRadius1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, m_dDistanceMinCircleRadius1);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL1, m_strSizeLabel1);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS2, m_bEnableMaxCircleRadius2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS2, m_dDistanceMaxCircleRadius2);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS2, m_bEnableMinCircleRadius2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS2, m_dDistanceMinCircleRadius2);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL4, m_strSizeLabel2);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_CIRCLE_RADIUS3, m_bEnableMaxCircleRadius3);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS3, m_dDistanceMaxCircleRadius3);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_CIRCLE_RADIUS3, m_bEnableMinCircleRadius3);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS3, m_dDistanceMinCircleRadius3);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL5, m_strSizeLabel3);

	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL2, m_strDistLabel1);	
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_2, m_bEnableMax2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_2, m_dDistanceMax2);
	// 	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_2, m_bEnableMin2);
	// 	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_2, m_dDistanceMin2);
	DDX_Text(pDX, IDC_EDIT_INSPECTDIS_LABEL3, m_strDistLabel2);

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
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA, m_dCurBlobArea);
	DDX_Radio(pDX, IDC_RADIO_CIRCLE_1, m_nCircleIndex);
	//DDX_Radio(pDX, IDC_RADIO_CIRCLE_MEASURE_1, m_nCircleMeasureIndex);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG, m_bEnableDispLineSeg);
	DDX_Text(pDX, IDC_EDT_CIRCLE_AREA_SCALE_RATIO, m_dAreaScaleRatio);
	DDX_Check(pDX, IDC_CHECK_DISP_LINE_SEG2, m_bEnableDispLineSeg2);
	DDX_Check(pDX, IDC_CHECK_BLOB_CONTROL_USE_TO_TOOL, m_bBlobInspectUseToTool);
	DDX_Text(pDX, IDC_EDT_SIZE_CONTROL_RESULT, m_strCurSizeResult);
	DDX_Text(pDX, IDC_EDT_CIRCLE_DIST_CONTROL_RESULT, m_strCurDistResult);
	DDX_Text(pDX, IDC_EDT_BLOB_CONTROL_RESULT, m_strCurInspectResult);
	DDX_Text(pDX, IDC_EDT_DISTANCE_RADIUS2, m_dCurRadius2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_RADIUS3, m_dCurRadius3);
	DDX_CBIndex(pDX, IDC_COMBO_DISTANCETYPE, m_nDistanceType);
	DDX_Control(pDX, IDC_COMBO_DISTANCETYPE, m_comboDistanceType);
	DDX_Control(pDX, IDC_COMBO_CIRCLE_CONTROL_OPTION, m_CComboCircleDistance);
	DDX_CBIndex(pDX, IDC_COMBO_CIRCLE_CONTROL_OPTION, m_nCircleIndexDistance);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_1, m_dDistanceCircleRadiusBase1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_2, m_dDistanceCircleRadiusBase2);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_BASE_3, m_dDistanceCircleRadiusBase3);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_1, m_dDistanceCircleRadiusFlu1);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_2, m_dDistanceCircleRadiusFlu2);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS_FLU_3, m_dDistanceCircleRadiusFlu3);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_1, m_dDistanceBase1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_BASE_2, m_dDistanceBase2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_1, m_dDistanceFlu1);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_FLU_2, m_dDistanceFlu2);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_1, m_bEnableCircleRadius1);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_2, m_bEnableCircleRadius2);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_RADIUS_3, m_bEnableCircleRadius3);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_1, m_bEnableDistance1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_2, m_bEnableDistance2);
}


BEGIN_MESSAGE_MAP(CDlgCircleDoubleMeasureInspectTool, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleCalipernum)
//	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleDoubleMeasureInspectTool::OnEnChangeEdtCircleCalipernum)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleOutlineNum)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleContrastThre)
ON_EN_SETFOCUS(IDC_EDT_BINARY_THRE, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtBinaryThre)
ON_EN_SETFOCUS(IDC_EDT_MAX_AREA, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMaxArea)
ON_EN_SETFOCUS(IDC_EDT_MIN_AREA, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMinArea)
ON_EN_SETFOCUS(IDC_EDT_MAX_DEFECT_AREA, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea)
ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgCircleDoubleMeasureInspectTool::OnEnChangeEdtCircleCalipernum)
ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadCircleInwardMode0)
ON_BN_CLICKED(IDC_CHECK_SHOW_CIRCLE_FITPOINT, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedCheckShowCircleFitpoint)
ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadCircleInwardMode1)
ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtPixelRatio)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL3, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1)
ON_BN_CLICKED(IDC_RADIO_CIRCLE_1, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircle2)
ON_BN_CLICKED(IDC_RADIO_CIRCLE_2, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircle2)
ON_BN_CLICKED(IDC_RADIO_CIRCLE_MEASURE_1, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircleMeasure1)
ON_BN_CLICKED(IDC_RADIO_CIRCLE_MEASURE_2, &CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircleMeasure2)
ON_EN_SETFOCUS(IDC_EDT_CIRCLE_AREA_SCALE_RATIO, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL4, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel4)
ON_EN_SETFOCUS(IDC_EDIT_INSPECTDIS_LABEL5, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel5)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMax1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMin1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMax2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMin2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius2)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS3, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius3)
// ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS3, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius3)
ON_CBN_SELCHANGE(IDC_COMBO_DISTANCETYPE, &CDlgCircleDoubleMeasureInspectTool::OnCbnSelchangeComboDistancetype)

ON_CBN_SELCHANGE(IDC_COMBO_CIRCLE_CONTROL_OPTION, &CDlgCircleDoubleMeasureInspectTool::OnCbnSelchangeComboCircleControlOption)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase2)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu2)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_BASE_3, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase3)
ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_RADIUS_FLU_3, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu3)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceBase1)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_1, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceFlu1)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_BASE_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceBase2)
ON_EN_SETFOCUS(IDC_EDIT_DISTANCE_FLU_2, &CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceFlu2)
END_MESSAGE_MAP()


void CDlgCircleDoubleMeasureInspectTool::CheckDlgDataValidation()
{

}

// 更新控件显示
void CDlgCircleDoubleMeasureInspectTool::UpdateControlDisplay()
{

}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleCalipernum()
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



void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleOutlineNum()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtCircleContrastThre()
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


// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMax1()
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
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMin1()
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
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMax2()
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
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMin2()
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


// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius1()
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
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius1()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtBinaryThre()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMaxArea()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMinArea()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtMaxDefectArea()
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


BOOL CDlgCircleDoubleMeasureInspectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;

	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
	}

	m_comboDistanceType.InsertString(0,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR1));
	m_comboDistanceType.InsertString(1,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR2));
	m_comboDistanceType.InsertString(2,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR3));
	m_comboDistanceType.InsertString(3,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR4));
	m_comboDistanceType.SetCurSel(0);
	
	m_comboDefectType.InsertString(0, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_WHITE));
	m_comboDefectType.InsertString(1, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_BLACK));
	m_comboDefectType.SetCurSel(0);

	m_CComboCircleDistance.InsertString(0,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR5));
	m_CComboCircleDistance.InsertString(1,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR6));
	m_CComboCircleDistance.InsertString(2,m_psaSysInfoStrings->GetAt(IDS_CMB_STR_CIRCLE_MEASURE_INSPECT_STR7));
	m_CComboCircleDistance.SetCurSel(0);
	OnCbnSelchangeComboDistancetype();
	return TRUE;  
}

void CDlgCircleDoubleMeasureInspectTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_CIRCLE_DOUBLE_MEASURE_INSPECT_TOOL") == strDlgID)
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


void CDlgCircleDoubleMeasureInspectTool::OnEnChangeEdtCircleCalipernum()
{
	UpdateData(TRUE);
	if ((m_nCircleCaliperNum < 2) || (m_nCircleCaliperNum > 300))
	{
		return;
	}
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[m_nCircleIndex] = m_nCircleCaliperNum;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadCircleInwardMode0()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[m_nCircleIndex] = (CaliperSearchDirection)m_nCircleInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedCheckShowCircleFitpoint()
{
	m_bShowCircleFitPoint = !m_bShowCircleFitPoint;
	UpdateData(TRUE);
	((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[m_nCircleIndex] = m_bShowCircleFitPoint;
	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadCircleInwardMode1()
{
	UpdateData(TRUE);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[m_nCircleIndex] = (CaliperSearchDirection)m_nCircleInwardMode;
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtPixelRatio()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel2()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel3()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel4()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL4, strOutput);
		}
	}
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel1()
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


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircle1()
{
	//UpdateCircleTrainData();
}

void CDlgCircleDoubleMeasureInspectTool::UpdateCircleTrainData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	
	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		m_nCircleCaliperNumTmp[m_nCircleIndex] =m_nCircleCaliperNum;
		m_nCircleOutLineNumTmp[m_nCircleIndex] =m_nCircleOutLineNum;
		m_dCircleContrastThreTmp[m_nCircleIndex] =m_dCircleContrastThre;
		m_nCirclePolarityModeTmp[m_nCircleIndex] =m_nCirclePolarityMode;
		m_nCircleInwardModeTmp[m_nCircleIndex] =m_nCircleInwardMode;
		m_lCircleFilterHalfWidthTmp[m_nCircleIndex] =m_lCircleFilterHalfWidth;
		m_bCircleSortByScoreTmp[m_nCircleIndex] =m_bCircleSortByScore;
		m_bShowCircleFitPointTmp[m_nCircleIndex] =m_bShowCircleFitPoint;
	}
	else
	{
		m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

		for (int it =0;it<2;it++)
		{
			m_nCircleCaliperNumTmp[it] =((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[it];
			m_nCircleOutLineNumTmp[it] =((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[it];
			m_dCircleContrastThreTmp[it] =((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[it];
			m_nCirclePolarityModeTmp[it] =((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[it];
			m_nCircleInwardModeTmp[it] =((CCircleCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[it];
			m_lCircleFilterHalfWidthTmp[it] =((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[it];
			m_bCircleSortByScoreTmp[it] =((CCircleCornerCheckSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[it];
			m_bShowCircleFitPointTmp[it] =((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[it];
		}
		
		m_nCircleCaliperNum = m_nCircleCaliperNumTmp[m_nCircleIndex] ;
		m_nCircleOutLineNum = m_nCircleOutLineNumTmp[m_nCircleIndex] ;
		m_dCircleContrastThre = m_dCircleContrastThreTmp[m_nCircleIndex] ;
		m_nCirclePolarityMode = m_nCirclePolarityModeTmp[m_nCircleIndex] ;
		m_nCircleInwardMode = m_nCircleInwardModeTmp[m_nCircleIndex] ;
		m_lCircleFilterHalfWidth = m_lCircleFilterHalfWidthTmp[m_nCircleIndex] ;
		m_bCircleSortByScore = m_bCircleSortByScoreTmp[m_nCircleIndex] ;
		m_bShowCircleFitPoint = m_bShowCircleFitPointTmp[m_nCircleIndex] ;
		UpdateData(FALSE);
	}

}


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircle2()
{
	UpdateData(TRUE);
	m_nCircleCaliperNumTmp[m_nPreCicleIndex] =m_nCircleCaliperNum;
	m_nCircleOutLineNumTmp[m_nPreCicleIndex] =m_nCircleOutLineNum;
	m_dCircleContrastThreTmp[m_nPreCicleIndex] =m_dCircleContrastThre;
	m_nCirclePolarityModeTmp[m_nPreCicleIndex] =m_nCirclePolarityMode;
	m_nCircleInwardModeTmp[m_nPreCicleIndex] =m_nCircleInwardMode;
	m_lCircleFilterHalfWidthTmp[m_nPreCicleIndex] =m_lCircleFilterHalfWidth;
	m_bCircleSortByScoreTmp[m_nPreCicleIndex] =m_bCircleSortByScore;
	m_bShowCircleFitPointTmp[m_nPreCicleIndex] =m_bShowCircleFitPoint;
	//UpdateCircleTrainData();

	m_nCircleCaliperNum = m_nCircleCaliperNumTmp[m_nCircleIndex] ;
	m_nCircleOutLineNum = m_nCircleOutLineNumTmp[m_nCircleIndex] ;
	m_dCircleContrastThre = m_dCircleContrastThreTmp[m_nCircleIndex] ;
	m_nCirclePolarityMode = m_nCirclePolarityModeTmp[m_nCircleIndex] ;
	m_nCircleInwardMode = m_nCircleInwardModeTmp[m_nCircleIndex] ;
	m_lCircleFilterHalfWidth = m_lCircleFilterHalfWidthTmp[m_nCircleIndex] ;
	m_bCircleSortByScore = m_bCircleSortByScoreTmp[m_nCircleIndex] ;
	m_bShowCircleFitPoint = m_bShowCircleFitPointTmp[m_nCircleIndex] ;
	UpdateData(FALSE);

	m_nPreCicleIndex=m_nCircleIndex;
}


void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircleMeasure1()
{
	UpdateCircleMeasureData();
}

void CDlgCircleDoubleMeasureInspectTool::UpdateCircleMeasureData()
{
	//int nOldCircleIndex = m_nCircleIndex;
	////int nOldCircleMeasureIndex = m_nCircleMeasureIndex;
	//UpdateData(TRUE);
	////((CCircleCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[nOldCircleIndex] = m_bShowCircleFitPoint;

	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck = m_bEnableDistCheck;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio      = m_dPixelRatio;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[nOldCircleMeasureIndex]      = m_bEnableDispLineSeg;

	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0 + nOldCircleMeasureIndex*3]   = m_strDistLabel1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0 + nOldCircleMeasureIndex*3]   = m_bEnableMaxCircleRadius1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0 + nOldCircleMeasureIndex*3] = m_dDistanceMaxCircleRadius1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0 + nOldCircleMeasureIndex*3]   = m_bEnableMinCircleRadius1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0 + nOldCircleMeasureIndex*3] = m_dDistanceMinCircleRadius1;

	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1 + nOldCircleMeasureIndex*3]   = m_strDistLabel2;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1 + nOldCircleMeasureIndex*3]   = m_bEnableMax1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1 + nOldCircleMeasureIndex*3] = m_dDistanceMax1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1 + nOldCircleMeasureIndex*3]	 = m_bEnableMin1;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1 + nOldCircleMeasureIndex*3] = m_dDistanceMin1;

	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2 + nOldCircleMeasureIndex*3]   = m_strDistLabel3;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2 + nOldCircleMeasureIndex*3]   = m_bEnableMax2;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2 + nOldCircleMeasureIndex*3] = m_dDistanceMax2;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2 + nOldCircleMeasureIndex*3]   = m_bEnableMin2;
	//((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2 + nOldCircleMeasureIndex*3] = m_dDistanceMin2;


	//int nCurCircleIndex = m_nCircleIndex;
	////int nCurCircleMeasureIndex = m_nCircleMeasureIndex;


	//m_bEnableDistCheck = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDistCheck ;
	//m_dPixelRatio      = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio ;
	//m_bEnableDispLineSeg      = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableDispLineSeg[nCurCircleMeasureIndex] ;

	//m_strDistLabel1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[0 + nCurCircleMeasureIndex * 3] ;
	//m_bEnableMaxCircleRadius1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[0  + nCurCircleMeasureIndex * 3] ;
	//m_dDistanceMaxCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[0  + nCurCircleMeasureIndex * 3];
	//m_bEnableMinCircleRadius1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[0  + nCurCircleMeasureIndex * 3]  ;
	//m_dDistanceMinCircleRadius1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[0  + nCurCircleMeasureIndex * 3] ;

	//m_strDistLabel2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[1  + nCurCircleMeasureIndex * 3] ;
	//m_bEnableMax1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[1 + nCurCircleMeasureIndex * 3];
	//m_dDistanceMax1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[1 + nCurCircleMeasureIndex * 3] ;
	//m_bEnableMin1   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[1 + nCurCircleMeasureIndex * 3];
	//m_dDistanceMin1 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[1 + nCurCircleMeasureIndex * 3];

	//m_strDistLabel3 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[2 + nCurCircleMeasureIndex * 3] ;
	//m_bEnableMax2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[2 + nCurCircleMeasureIndex * 3];
	//m_dDistanceMax2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[2 + nCurCircleMeasureIndex * 3] ;
	//m_bEnableMin2   = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[2 + nCurCircleMeasureIndex * 3];
	//m_dDistanceMin2 = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[2 + nCurCircleMeasureIndex * 3];
	//UpdateData(FALSE);
}

void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadioCircleMeasure2()
{
	UpdateCircleMeasureData();
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtAreaScaleRatio()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditInspectdisLabel5()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INSPECTDIS_LABEL5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INSPECTDIS_LABEL5, strOutput);
		}
	}
}


// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius2()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS2, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS2, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius2()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS2, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS2, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMaxCircleRadius3()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS3)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS3, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MAX_CIRCLE_RADIUS3, strOutput);
// 		}
// 	}
// }
// 
// 
// void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEdtDistanceMinCircleRadius3()
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS3)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS3, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_DISTANCE_MIN_CIRCLE_RADIUS3, strOutput);
// 		}
// 	}
// }


void CDlgCircleDoubleMeasureInspectTool::OnCbnSelchangeComboDistancetype()
{
	m_nDistanceType = m_comboDistanceType.GetCurSel();
	if (m_nDistanceType ==3)
	{
		GetDlgItem(IDC_STATIC_DISTANCE_LABEL_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_CIRCLE_CONTROL_OPTION)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DISTANCE_LABEL_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CIRCLE_CONTROL_OPTION)->ShowWindow(SW_HIDE);
	}
}


void CDlgCircleDoubleMeasureInspectTool::UpdateCircleSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	
	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
	    
		m_strDistLabel1Tmp[m_nCircleIndexDistance] = m_strDistLabel1;
		m_bEnableDistance1Tmp[m_nCircleIndexDistance] = m_bEnableDistance1;
		m_dDistanceBase1Tmp[m_nCircleIndexDistance] = m_dDistanceBase1;
		m_dDistanceFlu1Tmp[m_nCircleIndexDistance] = m_dDistanceFlu1;
// 		m_bEnableMax1Tmp[m_nCircleIndexDistance] = m_bEnableMax1;
// 		m_dDistanceMax1Tmp[m_nCircleIndexDistance] =m_dDistanceMax1;
// 		m_bEnableMin1Tmp[m_nCircleIndexDistance] = m_bEnableMin1;
// 		m_dDistanceMin1Tmp[m_nCircleIndexDistance] = m_dDistanceMin1;

		m_strDistLabel2Tmp[m_nCircleIndexDistance] = m_strDistLabel2;
		m_bEnableDistance2Tmp[m_nCircleIndexDistance] = m_bEnableDistance2;
		m_dDistanceBase2Tmp[m_nCircleIndexDistance] = m_dDistanceBase2;
		m_dDistanceFlu2Tmp[m_nCircleIndexDistance] = m_dDistanceFlu2;
// 		m_bEnableMax2Tmp[m_nCircleIndexDistance] = m_bEnableMax2;
// 		m_dDistanceMax2Tmp[m_nCircleIndexDistance] =m_dDistanceMax2;
// 		m_bEnableMin2Tmp[m_nCircleIndexDistance] = m_bEnableMin2;
// 		m_dDistanceMin2Tmp[m_nCircleIndexDistance] = m_dDistanceMin2;
	}
	else
	{

		m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		for (int it=0;it<3;it++)
		{
			m_bDistCheckUseToTool =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bDistCheckUseToTool;
			m_strDistLabel1Tmp[it] =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[3+it*2] ;
			
			m_bEnableDistance1Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[3+it*2] ;
			m_dDistanceBase1Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[3+it*2] ;
			m_dDistanceFlu1Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[3+it*2] ;
// 			m_bEnableMax1Tmp[it] =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[3+it*2] ;
// 			m_dDistanceMax1Tmp[it] =((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[3+it*2];
// 			m_bEnableMin1Tmp[it] =((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[3+it*2];
// 			m_dDistanceMin1Tmp[it] =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[3+it*2];

			m_strDistLabel2Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_strInspectDistanceValueLabel[4+it*2] ;

			m_bEnableDistance2Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFlu[4+it*2] ;
			m_dDistanceBase2Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceBase[4+it*2] ;
			m_dDistanceFlu2Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceFlu[4+it*2] ;
// 			m_bEnableMax2Tmp[it] =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax1[4+it*2] ;
// 			m_dDistanceMax2Tmp[it] = ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax1[4+it*2];
// 			m_bEnableMin2Tmp[it] =  ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin1[4+it*2] ;
// 			m_dDistanceMin2Tmp[it] =((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin1[4+it*2];
		}
		m_strDistLabel1 = m_strDistLabel1Tmp[m_nCircleIndexDistance] ;
		m_bEnableDistance1 = m_bEnableDistance1Tmp[m_nCircleIndexDistance];
		m_dDistanceBase1 = m_dDistanceBase1Tmp[m_nCircleIndexDistance];
		m_dDistanceFlu1 = m_dDistanceFlu1Tmp[m_nCircleIndexDistance];

// 		m_bEnableMax1 = m_bEnableMax1Tmp[m_nCircleIndexDistance] ;
// 		m_dDistanceMax1 = m_dDistanceMax1Tmp[m_nCircleIndexDistance] ;
// 		m_bEnableMin1= m_bEnableMin1Tmp[m_nCircleIndexDistance] ;
// 		m_dDistanceMin1 = m_dDistanceMin1Tmp[m_nCircleIndexDistance] ;

		m_strDistLabel2 = m_strDistLabel2Tmp[m_nCircleIndexDistance] ;
		m_bEnableDistance2 = m_bEnableDistance2Tmp[m_nCircleIndexDistance];
		m_dDistanceBase2 = m_dDistanceBase2Tmp[m_nCircleIndexDistance];
		m_dDistanceFlu2 = m_dDistanceFlu2Tmp[m_nCircleIndexDistance];

// 		m_bEnableMax2 = m_bEnableMax2Tmp[m_nCircleIndexDistance] ;
// 		m_dDistanceMax2 = m_dDistanceMax2Tmp[m_nCircleIndexDistance] ;
// 		m_bEnableMin2 = m_bEnableMin2Tmp[m_nCircleIndexDistance] ;
// 		m_dDistanceMin2 = m_dDistanceMin2Tmp[m_nCircleIndexDistance] ;
		UpdateData(FALSE);
	}
}

//void CDlgCircleDoubleMeasureInspectTool::OnBnClickedRadio3Circle1()
//{
//	UpdateData(TRUE);
//	m_strDistLabel1Tmp[m_nPreCicleIndex2] = m_strDistLabel1;
//	m_bEnableMax1Tmp[m_nPreCicleIndex2] = m_bEnableMax1;
//	m_dDistanceMax1Tmp[m_nPreCicleIndex2] =m_dDistanceMax1;
//	m_bEnableMin1Tmp[m_nPreCicleIndex2] = m_bEnableMin1;
//	m_dDistanceMin1Tmp[m_nPreCicleIndex2] = m_dDistanceMin1;
//
//	m_strDistLabel2Tmp[m_nPreCicleIndex2] = m_strDistLabel2;
//	m_bEnableMax2Tmp[m_nPreCicleIndex2] = m_bEnableMax2;
//	m_dDistanceMax2Tmp[m_nPreCicleIndex2] =m_dDistanceMax2;
//	m_bEnableMin2Tmp[m_nPreCicleIndex2] = m_bEnableMin2;
//	m_dDistanceMin2Tmp[m_nPreCicleIndex2] = m_dDistanceMin2;
//
//
//
//	m_strDistLabel1 = m_strDistLabel1Tmp[m_nCircleIndexDistance] ;
//	m_bEnableMax1 = m_bEnableMax1Tmp[m_nCircleIndexDistance] ;
//	m_dDistanceMax1 = m_dDistanceMax1Tmp[m_nCircleIndexDistance] ;
//	m_bEnableMin1= m_bEnableMin1Tmp[m_nCircleIndexDistance] ;
//	m_dDistanceMin1 = m_dDistanceMin1Tmp[m_nCircleIndexDistance] ;
//
//	m_strDistLabel2 = m_strDistLabel2Tmp[m_nCircleIndexDistance] ;
//	m_bEnableMax2 = m_bEnableMax2Tmp[m_nCircleIndexDistance] ;
//	m_dDistanceMax2 = m_dDistanceMax2Tmp[m_nCircleIndexDistance] ;
//	m_bEnableMin2 = m_bEnableMin2Tmp[m_nCircleIndexDistance] ;
//	m_dDistanceMin2 = m_dDistanceMin2Tmp[m_nCircleIndexDistance] ;
//	UpdateData(FALSE);
//
//	m_nPreCicleIndex2=m_nCircleIndexDistance;
//}


void CDlgCircleDoubleMeasureInspectTool::OnCbnSelchangeComboCircleControlOption()
{
	UpdateData(TRUE);
	int nCurSel = m_CComboCircleDistance.GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}

	m_strDistLabel1Tmp[m_nPreCicleIndex2] = m_strDistLabel1;
	m_bEnableDistance1Tmp[m_nPreCicleIndex2] = m_bEnableDistance1;
	m_dDistanceBase1Tmp[m_nPreCicleIndex2] = m_dDistanceBase1;
	m_dDistanceFlu1Tmp[m_nPreCicleIndex2] = m_dDistanceFlu1;

// 	m_bEnableMax1Tmp[m_nPreCicleIndex2] = m_bEnableMax1;
// 	m_dDistanceMax1Tmp[m_nPreCicleIndex2] =m_dDistanceMax1;
// 	m_bEnableMin1Tmp[m_nPreCicleIndex2] = m_bEnableMin1;
// 	m_dDistanceMin1Tmp[m_nPreCicleIndex2] = m_dDistanceMin1;

	m_strDistLabel2Tmp[m_nPreCicleIndex2] = m_strDistLabel2;
	m_bEnableDistance2Tmp[m_nPreCicleIndex2] = m_bEnableDistance2;
	m_dDistanceBase2Tmp[m_nPreCicleIndex2] = m_dDistanceBase2;
	m_dDistanceFlu2Tmp[m_nPreCicleIndex2] = m_dDistanceFlu2;
// 	m_bEnableMax2Tmp[m_nPreCicleIndex2] = m_bEnableMax2;
// 	m_dDistanceMax2Tmp[m_nPreCicleIndex2] =m_dDistanceMax2;
// 	m_bEnableMin2Tmp[m_nPreCicleIndex2] = m_bEnableMin2;
// 	m_dDistanceMin2Tmp[m_nPreCicleIndex2] = m_dDistanceMin2;

	m_strDistLabel1 = m_strDistLabel1Tmp[m_nCircleIndexDistance] ;

	m_bEnableDistance1 = m_bEnableDistance1Tmp[m_nCircleIndexDistance];
	m_dDistanceBase1 = m_dDistanceBase1Tmp[m_nCircleIndexDistance];
	m_dDistanceFlu1 = m_dDistanceFlu1Tmp[m_nCircleIndexDistance];

// 	m_bEnableMax1 = m_bEnableMax1Tmp[m_nCircleIndexDistance] ;
// 	m_dDistanceMax1 = m_dDistanceMax1Tmp[m_nCircleIndexDistance] ;
// 	m_bEnableMin1= m_bEnableMin1Tmp[m_nCircleIndexDistance] ;
// 	m_dDistanceMin1 = m_dDistanceMin1Tmp[m_nCircleIndexDistance] ;
	
	m_strDistLabel2 = m_strDistLabel2Tmp[m_nCircleIndexDistance] ;
	m_bEnableDistance2 = m_bEnableDistance2Tmp[m_nCircleIndexDistance];
	m_dDistanceBase2 = m_dDistanceBase2Tmp[m_nCircleIndexDistance];
	m_dDistanceFlu2 = m_dDistanceFlu2Tmp[m_nCircleIndexDistance];
// 	m_bEnableMax2 = m_bEnableMax2Tmp[m_nCircleIndexDistance] ;
// 	m_dDistanceMax2 = m_dDistanceMax2Tmp[m_nCircleIndexDistance] ;
// 	m_bEnableMin2 = m_bEnableMin2Tmp[m_nCircleIndexDistance] ;
// 	m_dDistanceMin2 = m_dDistanceMin2Tmp[m_nCircleIndexDistance] ;
	UpdateData(FALSE);
	
	m_nPreCicleIndex2=m_nCircleIndexDistance;
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase1()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu1()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_BASE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_FLU_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusBase3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_BASE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_BASE_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆3半径基准值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditCircleRadiusFlu3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_RADIUS_FLU_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_RADIUS_FLU_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[圆3半径公差值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceBase1()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceFlu1()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceBase2()
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


void CDlgCircleDoubleMeasureInspectTool::OnEnSetfocusEditDistanceFlu2()
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


BOOL CDlgCircleDoubleMeasureInspectTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
