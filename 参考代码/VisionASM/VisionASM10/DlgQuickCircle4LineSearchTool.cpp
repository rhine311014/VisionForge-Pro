// DlgPatternCircleLineSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgQuickCircle4LineSearchTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"


// CDlgQuickCircle4LineSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgQuickCircle4LineSearchTool, CDialogEx)

CDlgQuickCircle4LineSearchTool::CDlgQuickCircle4LineSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgQuickCircle4LineSearchTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_nCurFindLineIndex(0)
	, m_bEnableShowFitCirclePoint(FALSE)
	, m_bEnableShowFitLinePoint(FALSE)
	, m_nSearchOutLineAngleType(0)
	, m_nSearchOutPointType(0)
	, m_bEnableLineAngleJudge(FALSE)
	, m_dJudgeLineAngle(1)
	, m_bEnableLine(TRUE)
	, m_bEnable2LineAngleJudge(FALSE)
	, m_dAngleRange(0)
	, m_bShowCircleCaliper(FALSE)
	, m_bShowLineCaliper1(FALSE)
	, m_bEnableCircleDualEdge(FALSE)
	, m_bEnableLineDualEdge(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_bEnableLineMask(FALSE)
	, m_nImageChannel(0)
{

	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_lNoiseThre = 10;
	m_lTotalMag = 500;
	m_dGrau = 6.0;

	m_nLineCaliperNum1 = 20;
	m_dLineContrastThre1 = 10.0;
	m_lLineFilterHalfWidth1 = 1;
	m_nLineOutLineNum1 = 10;
	m_nLinePolarityMode1 = (int)eLightToDark;


	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 5;
	m_nLineAngleType = 0;

	// 圆参数
	m_nCircleInwardMode			= (int)eInward;
	m_nCirclePolarityMode		= (int)eDarkToLight;
	m_nCircleCaliperNum			= 20;
	m_dCircleContrastThre		= 10.0;
	m_lCircleFilterHalfWidth	= 1;
	m_nCircleOutLineNum			= 10;
	m_bCircleSortByScore1 = 0;
}

CDlgQuickCircle4LineSearchTool::~CDlgQuickCircle4LineSearchTool()
{
}

void CDlgQuickCircle4LineSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);

	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_comboLineAngleType);

	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM_1, m_nCircleOutLineNum);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_bCircleSortByScore1);
	DDX_Radio(pDX, IDC_RADIO_FINDLINE_INDEX1, m_nCurFindLineIndex);
	DDX_Control(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_checkCircleSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SHOW_FIT_CIRCLE_POINT, m_checkShowFitCirclePoint);
	DDX_Check(pDX, IDC_CHECK_SHOW_FIT_CIRCLE_POINT, m_bEnableShowFitCirclePoint);
	DDX_Control(pDX, IDC_CHECK_SHOW_FIT_LINE_POINT, m_checkShowFitLinePoint);
	DDX_Check(pDX, IDC_CHECK_SHOW_FIT_LINE_POINT, m_bEnableShowFitLinePoint);
	DDX_Control(pDX, IDC_COMBO_SEARCH_OUT_LINE_ANGLE_TYPE, m_combSearchOutLineAngleType);
	DDX_CBIndex(pDX, IDC_COMBO_SEARCH_OUT_LINE_ANGLE_TYPE, m_nSearchOutLineAngleType);
	DDX_Radio(pDX, IDC_RAD_SEARCH_OUT_POINT_TYPE_0, m_nSearchOutPointType);
	DDX_Check(pDX, IDC_CHECK_ENABLE_ANGLE_JUDGE, m_bEnableLineAngleJudge);
	DDX_Text(pDX, IDC_EDIT_LINE_ANGLE_JUDGE, m_dJudgeLineAngle);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_GRAU,      m_dGrau);
	DDX_Check(pDX, IDC_CHECK_ENABLE_LINE, m_bEnableLine);
	DDX_Check(pDX, IDC_CHECK_ENABLE_2LINE_ANGLE_JUDGE, m_bEnable2LineAngleJudge);
	DDX_Text(pDX, IDC_EDIT_ANGLE_RANGE, m_dAngleRange);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowLineCaliper1);
	DDX_Check(pDX, IDC_CHK_CIRCLR_ENABLE_DUAL_EDGE, m_bEnableCircleDualEdge);
	DDX_Check(pDX, IDC_CHK_LINE_ENABLE_DUAL_EDGE, m_bEnableLineDualEdge);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_MASK, m_bEnableCircleMask);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK, m_bEnableLineMask);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);

	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgQuickCircle4LineSearchTool, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)

	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1,OnEnSetfocusEditCircleFilterHalfWidth)

	ON_CBN_SELCHANGE(IDC_COMBO_LINE_ANGLE_TYPE, &CDlgQuickCircle4LineSearchTool::OnCbnSelchangeComboLineAngleType)
	ON_BN_CLICKED(IDC_RADIO_FINDLINE_INDEX1, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex1)
	ON_BN_CLICKED(IDC_RADIO_FINDLINE_INDEX2, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex2)
	ON_BN_CLICKED(IDC_RADIO_FINDLINE_INDEX3, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex3)
	ON_BN_CLICKED(IDC_RADIO_FINDLINE_INDEX4, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex4)
	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgQuickCircle4LineSearchTool::OnEnChangeEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleContrastThre)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadCircleInwardMode1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FIT_CIRCLE_POINT, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckShowFitCirclePoint)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FIT_LINE_POINT, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckShowFitLinePoint)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH_OUT_LINE_ANGLE_TYPE, &CDlgQuickCircle4LineSearchTool::OnCbnSelchangeComboSearchOutLineAngleType)
	ON_BN_CLICKED(IDC_RAD_SEARCH_OUT_POINT_TYPE_0, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType0)
	ON_BN_CLICKED(IDC_RAD_SEARCH_OUT_POINT_TYPE_1, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType1)
	ON_BN_CLICKED(IDC_RAD_SEARCH_OUT_POINT_TYPE_2, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType2)
	ON_BN_CLICKED(IDC_RAD_SEARCH_OUT_POINT_TYPE_3, &CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType3)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_ANGLE_JUDGE, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckEnableAngleJudge)
	ON_EN_SETFOCUS(IDC_EDIT_LINE_ANGLE_JUDGE, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditLineAngleJudge)
	ON_EN_SETFOCUS(IDC_EDT_GRAU, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtGrau)
	ON_EN_KILLFOCUS(IDC_EDT_GRAU, &CDlgQuickCircle4LineSearchTool::OnEnKillfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtNoisethre)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtTotalMag)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckEnableLine)
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE_RANGE, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditAngleRange)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM_1, &CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleOutlineNum1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgQuickCircle4LineSearchTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgQuickCircle4LineSearchTool::OnBnClickedChkShowCircleCaliper)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_MASK, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckCircleMask)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK, &CDlgQuickCircle4LineSearchTool::OnBnClickedCheckLineMask)
END_MESSAGE_MAP()


// CDlgQuickCircle4LineSearchTool 消息处理程序

void CDlgQuickCircle4LineSearchTool::UpdateControlDisplay()
{
 	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(TRUE);
	}

	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{
			GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(m_bEnableMask);

			if (m_pDlgParent->m_imageTrain.IsRGB24() || m_pDlgParent->m_imageTrain.IsRGB32())
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);

			cpImage imageTrain;
			m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
			if (imageTrain.IsValid() && 
				(imageTrain.IsRGB24() || imageTrain.IsRGB32()))
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(m_bEnableMask);

		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}

}

void CDlgQuickCircle4LineSearchTool::UpdateTrainData(bool bValidate)
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
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel; 
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau; 
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;

		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[m_nCurFindLineIndex]   = m_nLineOutLineNum1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[m_nCurFindLineIndex] = m_dLineContrastThre1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[m_nCurFindLineIndex] = m_lLineFilterHalfWidth1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[m_nCurFindLineIndex]   = (EdgePolarity)m_nLinePolarityMode1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[m_nCurFindLineIndex]   = m_bSortByScore1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[m_nCurFindLineIndex] = m_nLineAngleType;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLine[m_nCurFindLineIndex]    = m_bEnableLine;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[m_nCurFindLineIndex]      = m_nLineCaliperNum1;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[m_nCurFindLineIndex] = m_bShowLineCaliper1;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[m_nCurFindLineIndex] = m_bEnableLineMask;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineDualEdge[m_nCurFindLineIndex] = m_bEnableLineDualEdge;

		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum = m_nCircleOutLineNum;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre = m_dCircleContrastThre;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth = m_lCircleFilterHalfWidth;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty = (EdgePolarity)m_nCirclePolarityMode;		
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore = m_bCircleSortByScore1;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum = m_nCircleCaliperNum;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_nCircleInwardMode;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible = m_bShowCircleCaliper;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask = m_bEnableCircleMask;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCircleDualEdge = m_bEnableCircleDualEdge;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel	= ((CQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_dGrau			= ((CQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
		m_lNoiseThre	= ((CQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
		m_lTotalMag		= ((CQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
		m_bEnableMask	= ((CQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

		m_nLineOutLineNum1 =  ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[m_nCurFindLineIndex];
		m_dLineContrastThre1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[m_nCurFindLineIndex];
		m_lLineFilterHalfWidth1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[m_nCurFindLineIndex];
		m_nLinePolarityMode1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[m_nCurFindLineIndex];
		m_bSortByScore1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[m_nCurFindLineIndex];
		m_nLineAngleType = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[m_nCurFindLineIndex];
		m_bEnableLine = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLine[m_nCurFindLineIndex];
		m_nLineCaliperNum1 = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[m_nCurFindLineIndex];
		m_bShowLineCaliper1 = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[m_nCurFindLineIndex];
		m_bEnableLineMask = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[m_nCurFindLineIndex];
		m_bEnableLineDualEdge = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineDualEdge[m_nCurFindLineIndex];
		//圆参数
		m_nCircleOutLineNum =  ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum;
		m_dCircleContrastThre = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre;
		m_lCircleFilterHalfWidth = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth;
		m_nCirclePolarityMode = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty;
		m_bCircleSortByScore1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore;
		m_nCircleCaliperNum = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum;
		m_nCircleInwardMode = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection;
		m_bShowCircleCaliper = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible;
		m_bEnableCircleMask = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask;
		m_bEnableCircleDualEdge = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCircleDualEdge;

		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}
		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);
		m_checkCircleSortByScore1.SetCheck(m_bCircleSortByScore1);
		UpdateData(FALSE);
	}

}



void CDlgQuickCircle4LineSearchTool::UpdateSearchData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);

		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum	= m_nSearchNum;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow	= m_dAngleLow;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh	= m_dAngleHigh;

		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchOutLineAngleType	= m_nSearchOutLineAngleType;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchOutPointType		= m_nSearchOutPointType;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableLineAngleJudge	= m_bEnableLineAngleJudge;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dJudgeLineAngle			= m_dJudgeLineAngle;

		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnable2LineAngleJudge	= m_bEnable2LineAngleJudge;
		((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleRange			= m_dAngleRange;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);

		((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitCirclePoint = m_bEnableShowFitCirclePoint;
		((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitLinePoint[m_nCurFindLineIndex]   = m_bEnableShowFitLinePoint;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dAngleLow		= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;

		m_nSearchOutLineAngleType	= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchOutLineAngleType;
		m_nSearchOutPointType		= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchOutPointType;
		m_bEnableLineAngleJudge		= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableLineAngleJudge ;
		m_dJudgeLineAngle			= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dJudgeLineAngle ;
		
		m_bEnable2LineAngleJudge		= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnable2LineAngleJudge ;
		m_dAngleRange			= ((CQuickCircle4LineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleRange ;
		

		m_bEnableShowFitCirclePoint = ((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitCirclePoint;
		m_bEnableShowFitLinePoint	= ((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitLinePoint[m_nCurFindLineIndex];
		m_combSearchOutLineAngleType.SetCurSel(m_nSearchOutLineAngleType);
		UpdateData(FALSE);
	}
}

void CDlgQuickCircle4LineSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_QUICK_CIRCLE_4LINESEARCH_TOOL") == strDlgID)
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

void CDlgQuickCircle4LineSearchTool::CheckDlgDataValidation()
{

	UpdateData(TRUE);
	
	if (m_nSearchNum < 1)
	{
		m_nSearchNum = 1;
	}

	if ((m_dAcceptThre < 0.0) || (m_dAcceptThre > 1.0))
	{
		m_dAcceptThre = 0.8;
	}
	if ((m_dGrau < 1.0) || (m_dGrau > 19.0))
	{
		m_dGrau = 6.0;
	}
	if ((m_lNoiseThre < 1) || (m_lNoiseThre > 255))
	{
		m_lNoiseThre = 10;
	}
	if (m_lTotalMag < 0)
	{
		m_lTotalMag = 500;
	}
	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
	}

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

void CDlgQuickCircle4LineSearchTool::OnBtnGetTrainImage() 
{

	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	UpdateData(TRUE);
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;
	if (m_bEnableMask)
	{
		if ( (m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
			|| (m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
		{
			m_imageMask.Release();
		}
	}
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();

}

void CDlgQuickCircle4LineSearchTool::OnBtnTrain() 
{

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
	UpdateTrainData(true);

	m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);
	if (m_bEnableMask)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
	}

	if (m_pDlgParent->m_pSearchTool->Train())
	{
		cpImage trainImage;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
		//m_pDlgParent->m_pSearchTool->GetPatternImage(trainImage);
		m_GuiPatternDisplay.RemoveImage();
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		CString strErrInfo=m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}

}

void CDlgQuickCircle4LineSearchTool::OnBtnSetMaskImage()
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	/*CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask.Height()) )
	{
		m_imageMask.Release();
	}*/
	cpImage tmpImage(m_pDlgParent->m_imageTrain.Width(),m_pDlgParent->m_imageTrain.Height(),epGray8);
	m_pDlgParent->m_pSearchTool->GetProcessImage(m_pDlgParent->m_pTrainDataParam,m_pDlgParent->m_imageTrain,tmpImage);

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(tmpImage);

	if ( (tmpImage.Width() != m_imageMask.Width())
		|| (tmpImage.Height() != m_imageMask.Height()) )
	{
		m_imageMask.Release();
	}

	dlgMask.m_GuiMaskEdit.SetMaskImage(m_imageMask);
	dlgMask.SetSysInfoStringsRef(m_psaSysInfoStrings);

	if (dlgMask.DoModal() == IDOK)
	{
		dlgMask.m_GuiMaskEdit.GetMaskImage(m_imageMask);

		// 		if (m_bEnableMask)
		// 		{
		// 			m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
		// 		}
	}

}

void CDlgQuickCircle4LineSearchTool::OnChkEnableMask() 
{

	UpdateData(TRUE);

	if (m_bEnableMask)
	{
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);

		//		UpdateTrainData(true);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);

		//		UpdateTrainData(true);
	}

}

BOOL CDlgQuickCircle4LineSearchTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	// 	m_btnSetCenter.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	
	m_GuiPatternDisplay.Create(IDC_STC_PATTERN_DISPLAY, this, 0);
	m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiPatternDisplay.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiPatternDisplay.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiPatternDisplay.SetLanguage(1);
		break;
	}

	m_comboLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_ANYANGLE));
	m_comboLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
	m_comboLineAngleType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));
	m_comboLineAngleType.SetCurSel(m_nLineAngleType);

	m_combSearchOutLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
	m_combSearchOutLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));
	m_nSearchOutLineAngleType = 0;
	m_combSearchOutLineAngleType.SetCurSel(m_nSearchOutLineAngleType);


	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtAngleLow() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ANGLE_LOW)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ANGLE_LOW, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_ANGLE_LOW, strOutput);
		}
	}

}

void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtAngleHigh() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ANGLE_HIGH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ANGLE_HIGH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_ANGLE_HIGH, strOutput);
		}
	}

}

void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtSearchNum() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_SEARCH_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_SEARCH_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_SEARCH_NUM, strOutput);
		}
	}
}

void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtAcceptThre() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ACCEPT_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ACCEPT_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_ACCEPT_THRE, strOutput);
		}
	}

}



void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtLineCalipernum1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strOutput);
		}
	}
}

void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtLineContrastThre1() 
{
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

void CDlgQuickCircle4LineSearchTool::OnSetfocusEdtLineOutlineNum1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strOutput);
		}
	}
}

void CDlgQuickCircle4LineSearchTool::OnChangeEdtLineCalipernum1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum1 < 2) || (m_nLineCaliperNum1 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgQuickCircle4LineSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CQuickCircle4LineSearchTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FULL_IMAGE_SEARCH_SUCCEED_AND_RETRAINED);	//_T("全图搜索设置成功，模板已重新训练");
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FULL_IMAGE_SEARCH_FAILED);	//_T("全图搜索设置失败");
				m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PATTERN_RETRAIN_FAILED);		//_T("模板重新训练失败");
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}

BOOL CDlgQuickCircle4LineSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgQuickCircle4LineSearchTool::OnCbnSelchangeComboLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
}

// 卡尺的参数和界面之间的数据交互
void CDlgQuickCircle4LineSearchTool::UpdateTrainLineData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	if (bValidate)	// 从界面获取训练参数
	{
		int oldIndex = m_nCurFindLineIndex;
		UpdateData();
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[oldIndex]  = m_nLineOutLineNum1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[oldIndex] = m_dLineContrastThre1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[oldIndex] = m_lLineFilterHalfWidth1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[oldIndex] = m_bSortByScore1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[oldIndex] = m_nLineAngleType;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[oldIndex] = m_nLineCaliperNum1;
		
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[oldIndex] = (EdgePolarity)m_nLinePolarityMode1;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLine[oldIndex]  = m_bEnableLine;

		((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitLinePoint[oldIndex]   = m_bEnableShowFitLinePoint;
		
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[oldIndex]	= m_bShowLineCaliper1;
		//((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible	= m_bShowCircleCaliper;
		((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[oldIndex]	= m_bEnableLineMask;
		((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineDualEdge[oldIndex] = m_bEnableLineDualEdge;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}else
	{
		m_nLineOutLineNum1 =  ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[m_nCurFindLineIndex];
		m_dLineContrastThre1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[m_nCurFindLineIndex];
		m_lLineFilterHalfWidth1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[m_nCurFindLineIndex];
		m_nLinePolarityMode1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[m_nCurFindLineIndex];
		m_bEnableLine = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLine[m_nCurFindLineIndex];

		m_bSortByScore1 = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[m_nCurFindLineIndex];
		m_nLineAngleType = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[m_nCurFindLineIndex];
		m_nLineCaliperNum1 = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[m_nCurFindLineIndex];
		m_bEnableShowFitLinePoint = ((CQuickCircle4LineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowFitLinePoint[m_nCurFindLineIndex];
		
		m_bShowLineCaliper1 = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[m_nCurFindLineIndex];
		//m_bShowCircleCaliper = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible;
		m_bEnableLineMask = ((CQuickCircle4LineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[m_nCurFindLineIndex];
		m_bEnableLineDualEdge = ((CQuickCircle4LineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineDualEdge[m_nCurFindLineIndex];
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}
		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);
		UpdateData(FALSE);
	}
}



void CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex1()
{
	// TODO: 在此添加控件通知处理程序代码
	//把上一次的参数保存到参数中
	UpdateTrainLineData(true);
	//保存这次的参数
	UpdateTrainLineData(false);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex2()
{
	// TODO: 在此添加控件通知处理程序代码
	//把上一次的参数保存到参数中
	UpdateTrainLineData(true);

	//把这一次的参数显示到界面
	UpdateTrainLineData(false);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex3()
{
	// TODO: 在此添加控件通知处理程序代码
	//把上一次的参数保存到参数中
	UpdateTrainLineData(true);

	//把这一次的参数显示到界面
	UpdateTrainLineData(false);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadioFindlineIndex4()
{
	// TODO: 在此添加控件通知处理程序代码
	//把上一次的参数保存到参数中
	UpdateTrainLineData(true);

	//把这一次的参数显示到界面
	UpdateTrainLineData(false);
}


void CDlgQuickCircle4LineSearchTool::OnEnChangeEdtCircleCalipernum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ((m_nCircleCaliperNum < 2) || (m_nCircleCaliperNum > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
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


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleOutlineNum()
{
	// TODO: 在此添加控件通知处理程序代码
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


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
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


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadCircleInwardMode0()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadCircleInwardMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckShowFitCirclePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableShowFitCirclePoint = !m_bEnableShowFitCirclePoint;

	CQuickCircle4LineSearchSearchGuiParam searchGuiParam;
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);

	searchGuiParam.m_bEnableShowFitCirclePoint= m_bEnableShowFitCirclePoint;

	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckShowFitLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainLineData(true);
}


void CDlgQuickCircle4LineSearchTool::OnCbnSelchangeComboSearchOutLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedRadSearchOutPointType3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckEnableAngleJudge()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableLineAngleJudge = !m_bEnableLineAngleJudge;

	CQuickCircle4LineSearchSearchDataParam searchDataParam;
	m_pDlgParent->m_pSearchTool->GetSearchDataParam(&searchDataParam);
	searchDataParam.m_bEnableLineAngleJudge= m_bEnableLineAngleJudge;
	m_pDlgParent->m_pSearchTool->SetSearchDataParam(&searchDataParam);
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditLineAngleJudge()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LINE_ANGLE_JUDGE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LINE_ANGLE_JUDGE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LINE_ANGLE_JUDGE, strOutput);
			UpdateSearchData(true);
		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtGrau()
{
	TRACE(_T("CDlgQuickCircle4LineSearchTool EdtGrau SetFocus\n"));
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, (WPARAM)GetDlgItem(IDC_EDT_GRAU)->GetSafeHwnd(), NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_GRAU)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_GRAU, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_GRAU, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[速度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}

		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnEnKillfocusEdtGrau()
{
	TRACE(_T("CDlgQuickCircle4LineSearchTool EdtGrau KillFocus\n"));
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtNoisethre()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_NOISETHRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_NOISETHRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_NOISETHRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对比度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtTotalMag()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TOTAL_MAG)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TOTAL_MAG, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TOTAL_MAG, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[边缘强度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckEnableLine()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainLineData(true);
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditAngleRange()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ANGLE_RANGE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ANGLE_RANGE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ANGLE_RANGE, strOutput);
		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEdtCircleOutlineNum1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CIRCLE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_OUTLINE_NUM_1, strOutput);
		}
	}
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckCircleMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgQuickCircle4LineSearchTool::OnBnClickedCheckLineMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditLineFilterHalfWidth() 
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

void CDlgQuickCircle4LineSearchTool::OnEnSetfocusEditCircleFilterHalfWidth() 
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
		GetDlgItem(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, strOutput);
		}
	}

}