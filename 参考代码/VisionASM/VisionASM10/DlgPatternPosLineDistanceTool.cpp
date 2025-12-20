// DlgPatternPosLineDistanceTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternPosLineDistanceTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"

// CDlgPatternPosLineDistanceTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternPosLineDistanceTool, CDialogEx)

	CDlgPatternPosLineDistanceTool::CDlgPatternPosLineDistanceTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternPosLineDistanceTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_bSortByScore2(TRUE)
	, m_bShowCaliper_1(FALSE)
	, m_bShowCaliper_2(FALSE)
	, m_bEnableDualEdge_1(FALSE)
	, m_bEnableDualEdge_2(FALSE)
	, m_nImageChannel(0)
{

	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_nLineCaliperNum1 = 20;
	m_nLineCaliperNum2 = 20;

	m_dLineContrastThre1 = 10.0;
	m_dLineContrastThre2 = 10.0;

	m_lLineFilterHalfWidth1 = 1;
	m_lLineFilterHalfWidth2 = 1;

	m_nLineOutLineNum1 = 10;
	m_nLineOutLineNum2 = 10;

	m_nLinePolarityMode1 = (int)eLightToDark;
	m_nLinePolarityMode2 = (int)eLightToDark;

	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;

	m_bEnableLineMask_2 = FALSE;
	m_bShowFitPoint_2 = FALSE;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;

	m_bEnableSearchPos = TRUE;
	m_bEnableFindLine2 = TRUE;
	m_dPixelRatio = 1.0;
	m_bEnableMax1 = FALSE;
	m_bEnableMax2 = FALSE;
	m_dDistanceMax1 = 1.0;
	m_dDistanceMax2 = 1.0;
	m_bEnableMin1 = FALSE;
	m_bEnableMin2 = FALSE;
	m_dDistanceMin1 = 0.0;
	m_dDistanceMin2 = 0.0;

	m_dDistance1 = 0.0;
	m_dDistance2 = 0.0;
}

CDlgPatternPosLineDistanceTool::~CDlgPatternPosLineDistanceTool()
{
}

void CDlgPatternPosLineDistanceTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_COMBO_SPEED_MODE, m_comboSpeed);
	DDX_Control(pDX, IDC_COMBO_ACCURACY_MODE, m_comboAccuracy);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_CONFUSION_THRE, m_dConfusionThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);

	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_2, m_nLineCaliperNum2);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_2, m_dLineContrastThre2);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_2, m_nLineOutLineNum2);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_2, m_nLinePolarityMode2);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE2, m_checkSortByScore2);	
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_2, m_bEnableLineMask_2);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint_1);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_2, m_bShowFitPoint_2);

	DDX_Check(pDX, IDC_CHECK_SEARCH_POS, m_bEnableSearchPos);
	DDX_Check(pDX, IDC_CHECK_FIND_LINE_2, m_bEnableFindLine2);
	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_2, m_bEnableMax2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_2, m_dDistanceMax2);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_2, m_bEnableMin2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_2, m_dDistanceMin2);

	DDX_Text(pDX, IDC_EDT_DISTANCE_1, m_dDistance1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_2, m_dDistance2);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_2, m_bShowCaliper_2);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge_1);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE2, m_bEnableDualEdge_2);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_2, m_lLineFilterHalfWidth2);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternPosLineDistanceTool, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, OnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, OnBtnSetCenter)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, OnSetfocusEdtConfusionThre)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_2, OnChangeEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_2, OnSetfocusEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_2, OnSetfocusEdtLineContrastThre2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_2, OnSetfocusEdtLineOutlineNum2)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgPatternPosLineDistanceTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgPatternPosLineDistanceTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgPatternPosLineDistanceTool::OnBnClickedCheckLineMask2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgPatternPosLineDistanceTool::OnBnClickedCheckShowFitpoint2)

	ON_BN_CLICKED(IDC_CHECK_FIND_LINE_2, OnBnClickedCheckFindLine2)
	ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, OnSetfocusEdtPixelRatio)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, OnSetfocusEdtDistanceMax1)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_2, OnSetfocusEdtDistanceMax2)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, OnSetfocusEdtDistanceMin1)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_2, OnSetfocusEdtDistanceMin2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)

	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgPatternPosLineDistanceTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgPatternPosLineDistanceTool::OnBnClickedChkShowLineCaliper2)
END_MESSAGE_MAP()


// CDlgPatternPosLineDistanceTool 消息处理程序

void CDlgPatternPosLineDistanceTool::UpdateControlDisplay()
{
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
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

void CDlgPatternPosLineDistanceTool::UpdateTrainData(bool bValidate)
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
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;

		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_bEnableDualEdge_1;

		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_nLineOutLineNum2;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_dLineContrastThre2;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_lLineFilterHalfWidth2;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_nLinePolarityMode2;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_bSortByScore2;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
		((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_bEnableDualEdge_2;

		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableSearchPos = m_bEnableSearchPos;
		((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableFindLine1 = m_bEnableFindLine2;

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

		m_nLineOutLineNum1 =  ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
		m_dLineContrastThre1 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
		m_lLineFilterHalfWidth1 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
		m_nLinePolarityMode1 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
		m_bSortByScore1 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
		m_nLineCaliperNum1 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
		m_bEnableLineMask_1 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];
		m_bShowCaliper_1 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
		m_bEnableDualEdge_1 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];

		m_nLineOutLineNum2 =  ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
		m_dLineContrastThre2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
		m_lLineFilterHalfWidth2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
		m_nLinePolarityMode2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
		m_bSortByScore2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
		m_nLineCaliperNum2 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
		m_bEnableLineMask_2 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
		m_bShowCaliper_2 = ((CPatternPosLineDistanceTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];
		m_bEnableDualEdge_2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];

		m_bEnableSearchPos = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableSearchPos;
		m_bEnableFindLine2 = ((CPatternPosLineDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableFindLine1;

		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}
		m_comboAccuracy.SetCurSel(m_nPatAccuracyMode);
		m_comboSpeed.SetCurSel(m_nPatSpeedMode);
		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_checkSortByScore2.SetCheck(m_bSortByScore2);

		UpdateData(FALSE);
	}

}

void CDlgPatternPosLineDistanceTool::UpdateSearchData(bool bValidate)
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

		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;

		((CPatternPosLineDistanceSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bShowFitPoint_1;
		((CPatternPosLineDistanceSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bShowFitPoint_2;

		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio = m_dPixelRatio;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[0] = m_bEnableMax1;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[1] = m_bEnableMax2;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[0] = m_dDistanceMax1;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[1] = m_dDistanceMax2;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[0] = m_bEnableMin1;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[1] = m_bEnableMin2;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[0] = m_dDistanceMin1;
		((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[1] = m_dDistanceMin2;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		m_bShowFitPoint_1	= ((CPatternPosLineDistanceSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;
		m_bShowFitPoint_2	= ((CPatternPosLineDistanceSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;

		m_dPixelRatio = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio;
		m_bEnableMax1 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[0];
		m_bEnableMax2 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[1];
		m_dDistanceMax1 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[0];
		m_dDistanceMax2 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[1];
		m_bEnableMin1 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[0];
		m_bEnableMin2 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[1];
		m_dDistanceMin1 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[0];
		m_dDistanceMin2 = ((CPatternPosLineDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[1];

		UpdateData(FALSE);
	}

}

void CDlgPatternPosLineDistanceTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERN_POSLINEDISTANCE_TOOL") == strDlgID)
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

void CDlgPatternPosLineDistanceTool::CheckDlgDataValidation()
{

	UpdateData(TRUE);

	if (m_nSearchNum < 1)
	{
		m_nSearchNum = 1;
	}
	if ((m_dAcceptThre < 0.0) || (m_dAcceptThre > 1.0))
	{
		m_dAcceptThre = 0.6;
	}
	if ((m_dConfusionThre < 0.0) || (m_dConfusionThre > 1.0))
	{
		m_dConfusionThre = 0.8;
	}
	if (m_dAcceptThre > m_dConfusionThre)
	{
		m_dAcceptThre = 0.6;
		m_dConfusionThre = 0.8;
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

	if (m_dPixelRatio <= 0)
	{
		m_dPixelRatio = 1.0;
	}
	if (m_dDistanceMax1 < m_dDistanceMin1 || m_dDistanceMax1 < 0 || m_dDistanceMin1< 0)
	{
		m_dDistanceMax1 = 1.0;
		m_dDistanceMin1 = 0.0;
	}
	if (m_dDistanceMax2 < m_dDistanceMin2 || m_dDistanceMax2 < 0 || m_dDistanceMin2< 0)
	{
		m_dDistanceMax2 = 1.0;
		m_dDistanceMin2 = 0.0;
	}

	UpdateData(FALSE);
}

void CDlgPatternPosLineDistanceTool::OnBtnGetTrainImage() 
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

void CDlgPatternPosLineDistanceTool::OnBtnTrain() 
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
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
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

void CDlgPatternPosLineDistanceTool::OnBtnSetMaskImage()
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

void CDlgPatternPosLineDistanceTool::OnChkEnableMask() 
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


void CDlgPatternPosLineDistanceTool::OnBtnSetCenter() 
{
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternPosLineDistanceTrainGuiParam *pTrainGuiParam = new CPatternPosLineDistanceTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect);
		dMarkX -= pTrainGuiParam->m_TrainRect.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect.GetUL().GetY();

		scRect rcTrain = pTrainGuiParam->m_TrainRect;
		sc2Vector modelPointRoi(dMarkX, dMarkY);	//ROI参考点1相对坐标
		std::vector<sc2Vector> vecModelPointRoi;
		vecModelPointRoi.push_back(modelPointRoi);
		if (!imagePattern.IsValid() || !imageTrain.IsValid() || vecModelPointRoi.empty() || rcTrain.IsEmpty())
		{
			return;
		} 
		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, vecModelPointRoi, imageTrain, rcTrain, FALSE, this);
		//CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			pTrainGuiParam->m_ModelPoint.SetX(dMarkX+pTrainGuiParam->m_TrainRect.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint.SetY(dMarkY+pTrainGuiParam->m_TrainRect.GetUL().GetY());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
			m_pDlgParent->m_pSearchTool->Train();
			m_pDlgParent->m_comboDisplayType.SetCurSel(1);
			m_pDlgParent->UpdateGuiDisplay();
		}
		delete pTrainGuiParam;
		pTrainGuiParam = NULL;
	}
	else
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_CENTER_SET_DIALOG_FAILED);	//_T("打开提取界面失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NOT_TRAINED);			//_T("定位工具未训练");
		m_pDlgParent->UpdateStatusBar();
	}
}


BOOL CDlgPatternPosLineDistanceTool::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
 	m_btnSetCenter.SetThemeHelper(&m_themeHelper);
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
//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	m_comboAccuracy.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_comboAccuracy.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_comboAccuracy.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_comboAccuracy.SetCurSel(m_nPatAccuracyMode);

	m_comboSpeed.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_comboSpeed.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_comboSpeed.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed.SetCurSel(m_nPatSpeedMode);



	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatternPosLineDistanceTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();	
}

void CDlgPatternPosLineDistanceTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtAngleLow() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtAngleHigh() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtSearchNum() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtAcceptThre() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtConfusionThre() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CONFUSION_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CONFUSION_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CONFUSION_THRE, strOutput);
		}
	}
}

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineCalipernum1() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineContrastThre1() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineOutlineNum1() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineCalipernum2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strOutput);
		}
	}
}

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineContrastThre2() 
{
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtLineOutlineNum2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strOutput);
		}
	}
}

void CDlgPatternPosLineDistanceTool::OnChangeEdtLineCalipernum1() 
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

void CDlgPatternPosLineDistanceTool::OnChangeEdtLineCalipernum2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum2 < 2) || (m_nLineCaliperNum2 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternPosLineDistanceTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternPosLineDistanceTrainGuiParam trainGuiParam;
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

BOOL CDlgPatternPosLineDistanceTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPatternPosLineDistanceTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternPosLineDistanceTool::OnBnClickedCheckShowFitpoint1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternPosLineDistanceTool::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternPosLineDistanceTool::OnBnClickedCheckShowFitpoint2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternPosLineDistanceTool::OnBnClickedCheckFindLine2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtPixelRatio() 
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtDistanceMax1() 
{
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtDistanceMax2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MAX_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strOutput);
		}
	}
}

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtDistanceMin1() 
{
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

void CDlgPatternPosLineDistanceTool::OnSetfocusEdtDistanceMin2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MIN_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MIN_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MIN_2, strOutput);
		}
	}
}

void CDlgPatternPosLineDistanceTool::UpdateSearchResult()
{
	CSearchResult searchResult;
	m_pDlgParent->m_pSearchTool->GetResult(0, searchResult);

	m_dDistance1 = searchResult.m_resultPosLineDistanceTool.m_dDistanceMM0;
	m_dDistance2 = searchResult.m_resultPosLineDistanceTool.m_dDistanceMM1;

	UpdateData(FALSE);
}

void CDlgPatternPosLineDistanceTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternPosLineDistanceTool::OnBnClickedChkShowLineCaliper2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}

void CDlgPatternPosLineDistanceTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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

void CDlgPatternPosLineDistanceTool::OnEnSetfocusEditLineFilterHalfWidth2() 
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