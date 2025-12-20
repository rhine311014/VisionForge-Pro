// DlgPatternCornerSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternCornerSearchTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "LogFileSave.h"

// CDlgPatternCornerSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternCornerSearchTool, CDialogEx)

CDlgPatternCornerSearchTool::CDlgPatternCornerSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternCornerSearchTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_bSortByScore2(TRUE)
	, m_bEnableDualEdge(FALSE)
	, m_bEnableDualEdge2(FALSE)
	, m_dAngleBaseValue(90)
	, m_dAngleFluValue(5)
	, m_bShowCaliper_1(FALSE)
	, m_bShowCaliper_2(FALSE)
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
    m_nOldLinePolarityMode1 = m_nLinePolarityMode1;
    m_nOldLinePolarityMode2 = m_nLinePolarityMode2;
	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;

	m_bEnableLineMask_2 = FALSE;
	m_bShowFitPoint_2 = FALSE;


	m_bEnableAngleCheck		= FALSE;
	m_bEnaleAngleCorCheck	= FALSE;
	m_nLineCorModeSelect    = 0;
    m_nOldLineCorModeSelect = m_nLineCorModeSelect;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;
    m_nOldAccuracyMode = m_nPatAccuracyMode;
    m_nOldPatSpeedMode = m_nPatSpeedMode;
}

CDlgPatternCornerSearchTool::~CDlgPatternCornerSearchTool()
{
}

void CDlgPatternCornerSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	//	DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
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
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE2, m_bEnableDualEdge2);
	DDX_Text(pDX, IDC_EDIT_ANGLE_BASE, m_dAngleBaseValue);
	DDX_Text(pDX, IDC_EDIT_ANGLE_FLU, m_dAngleFluValue);
	DDX_Control(pDX, IDC_ANGLE_CHECK, m_bAngleCheckEnabled);
	DDX_Control(pDX, IDC_ANGLE_COR_CHECK, m_bAngleCorCheckEnaled);
	DDX_Check(pDX, IDC_ANGLE_CHECK, m_bEnableAngleCheck);
	DDX_Check(pDX, IDC_ANGLE_COR_CHECK, m_bEnaleAngleCorCheck);

	DDX_Radio(pDX, IDC_RAD_LINE_COR_MODE_1, m_nLineCorModeSelect);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_2, m_bShowCaliper_2);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_2, m_lLineFilterHalfWidth2);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternCornerSearchTool, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, OnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)
//	ON_BN_CLICKED(IDC_BTN_SET_CENTER, OnBtnSetCenter)
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
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE_BASE, OnEnSetfocusEditAngleBase)
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE_FLU, OnEnSetfocusEditAngleFlu)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)

	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgPatternCornerSearchTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgPatternCornerSearchTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgPatternCornerSearchTool::OnBnClickedCheckLineMask2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgPatternCornerSearchTool::OnBnClickedCheckShowFitpoint2)
	ON_BN_CLICKED(IDC_ANGLE_CHECK, &CDlgPatternCornerSearchTool::OnBnClickedAngleCheck)
	ON_BN_CLICKED(IDC_ANGLE_COR_CHECK, &CDlgPatternCornerSearchTool::OnBnClickedAngleCorCheck)
    ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE1, &CDlgPatternCornerSearchTool::OnBnClickedCheckSortByScore1)
    ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE2, &CDlgPatternCornerSearchTool::OnBnClickedCheckSortByScore2)
    ON_BN_CLICKED(IDC_CHK_ENABLE_DUAL_EDGE, &CDlgPatternCornerSearchTool::OnBnClickedChkEnableDualEdge)
    ON_BN_CLICKED(IDC_CHK_ENABLE_DUAL_EDGE2, &CDlgPatternCornerSearchTool::OnBnClickedChkEnableDualEdge2)
    ON_BN_CLICKED(IDC_RAD_LINE_COR_MODE_1, &CDlgPatternCornerSearchTool::OnBnClickedRadLineCorMode1)
    ON_BN_CLICKED(IDC_RAD_LINE_COR_MODE_2, &CDlgPatternCornerSearchTool::OnBnClickedRadLineCorMode2)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgPatternCornerSearchTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgPatternCornerSearchTool::OnBnClickedChkShowLineCaliper2)
END_MESSAGE_MAP()


// CDlgPatternCornerSearchTool 消息处理程序

void CDlgPatternCornerSearchTool::UpdateControlDisplay()
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

void CDlgPatternCornerSearchTool::UpdateTrainData(bool bValidate)
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
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;

		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;
        ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_bEnableDualEdge;

		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_nLineOutLineNum2;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_dLineContrastThre2;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_lLineFilterHalfWidth2;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_nLinePolarityMode2;
		((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_bSortByScore2;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
        ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_bEnableDualEdge2;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

		m_nLineOutLineNum1 =  ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
		m_dLineContrastThre1 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
		m_lLineFilterHalfWidth1 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
		m_nLinePolarityMode1 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
		m_bSortByScore1 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
		m_nLineCaliperNum1 = ((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
		m_bEnableLineMask_1 = ((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];
        m_bEnableDualEdge = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];

		m_nLineOutLineNum2 =  ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
		m_dLineContrastThre2 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
		m_lLineFilterHalfWidth2 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
		m_nLinePolarityMode2 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
		m_bSortByScore2 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
		m_nLineCaliperNum2 = ((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
		m_bEnableLineMask_2 = ((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
        m_bEnableDualEdge2 = ((CPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];

		m_bShowCaliper_1=((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
		m_bShowCaliper_2=((CPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];
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

void CDlgPatternCornerSearchTool::UpdateSearchData(bool bValidate)
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

		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleBaseValue = m_dAngleBaseValue;
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleFluValue = m_dAngleFluValue;
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableAngleCheck = m_bEnableAngleCheck;		
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnaleAngleCorCheck = m_bEnaleAngleCorCheck;	
		((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nLineCorModeSelect	= m_nLineCorModeSelect;	

		((CPatternCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bShowFitPoint_1;
		((CPatternCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bShowFitPoint_2;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre = ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;
		m_dAngleBaseValue = ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleBaseValue;
		m_dAngleFluValue  = ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleFluValue;
		m_bEnableAngleCheck		= ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableAngleCheck;
		m_bEnaleAngleCorCheck	= ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnaleAngleCorCheck;
		m_nLineCorModeSelect	= ((CPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nLineCorModeSelect;

		m_bShowFitPoint_1	= ((CPatternCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;
		m_bShowFitPoint_2	= ((CPatternCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;
		
		if (m_bEnableAngleCheck)
		{
			GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(TRUE);
			GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(FALSE);
			GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
		}
		m_bAngleCheckEnabled.SetCheck(m_bEnableAngleCheck);
		m_bAngleCorCheckEnaled.SetCheck(m_bEnaleAngleCorCheck);
		UpdateData(FALSE);
	}

}

void CDlgPatternCornerSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNCORNERSEARCH_TOOL") == strDlgID)
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

void CDlgPatternCornerSearchTool::CheckDlgDataValidation()
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
	if (m_dAngleBaseValue <= m_dAngleFluValue)
	{
		m_dAngleBaseValue		= 90.0;	
		m_dAngleFluValue		= 5.0;	
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

	UpdateData(FALSE);

}

void CDlgPatternCornerSearchTool::OnBtnGetTrainImage() 
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
    CString strLog;
    strLog.Format(_T("点击[获取训练图像]\n"));
    g_opLogFile.SaveLogText(strLog,6);
	UpdateControlDisplay();

}

void CDlgPatternCornerSearchTool::OnBtnTrain() 
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
    CString strLog;
    strLog.Format(_T("点击[训练]\n"));
    g_opLogFile.SaveLogText(strLog,6);
}

void CDlgPatternCornerSearchTool::OnBtnSetMaskImage()
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
    CString strLog;
    strLog.Format(_T("点击[设置掩膜]\n"));
    g_opLogFile.SaveLogText(strLog,6);
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

void CDlgPatternCornerSearchTool::OnChkEnableMask() 
{

	UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableMask ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[掩模设置][启用]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
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

BOOL CDlgPatternCornerSearchTool::OnInitDialog() 
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

	if (m_bEnableAngleCheck)
	{
		GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
	}

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatternCornerSearchTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();	
    if (m_nOldAccuracyMode!=m_nPatAccuracyMode)
    {
        CString strLog;
        CString strVal[]= {_T("精细"),_T("标准"),_T("粗略")};
        strLog.Format(_T("修改[精细][%s]为[%s]\n"),strVal[m_nOldAccuracyMode],strVal[m_nPatAccuracyMode]);
        g_opLogFile.SaveLogText(strLog,6);
        m_nOldAccuracyMode = m_nPatAccuracyMode;
    }
}

void CDlgPatternCornerSearchTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
    if (m_nOldPatSpeedMode!=m_nPatSpeedMode)
    {
        CString strLog;
        CString strVal[]= {_T("1/4倍速"),_T("1/2倍速"),_T("标准"),_T("2倍速"),_T("4倍速"),_T("8倍速")};
        strLog.Format(_T("修改[速度][%s]为[%s]\n"),strVal[m_nOldPatSpeedMode],strVal[m_nPatSpeedMode]);
        g_opLogFile.SaveLogText(strLog,6);
        m_nOldPatSpeedMode = m_nPatSpeedMode;
    }
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtAngleLow() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[角度范围][下限]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}

}

void CDlgPatternCornerSearchTool::OnSetfocusEdtAngleHigh() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[角度范围][上限]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}

}

void CDlgPatternCornerSearchTool::OnSetfocusEdtSearchNum() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[搜索设置][个数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtAcceptThre() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[搜索设置][相似度1]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}

}

void CDlgPatternCornerSearchTool::OnSetfocusEdtConfusionThre() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[搜索设置][相似度2]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineCalipernum1() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具1][卡尺数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineContrastThre1() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具1][对比度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineOutlineNum1() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具1][忽略数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineCalipernum2() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具2][卡尺数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineContrastThre2() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具2][对比度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnSetfocusEdtLineOutlineNum2() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[找线工具2][忽略数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternCornerSearchTool::OnChangeEdtLineCalipernum1() 
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
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}

}

void CDlgPatternCornerSearchTool::OnChangeEdtLineCalipernum2() 
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
	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{


		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
	
}

void CDlgPatternCornerSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
		{
			CCornerCheckSearchTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
		{
			CCombinationCornerSearchTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
		else
		{
			CPatternCornerSearchTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
		
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
    CString strLog;
    strLog.Format(_T("点击[全图搜索]\n"));
    g_opLogFile.SaveLogText(strLog,6);
}

BOOL CDlgPatternCornerSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPatternCornerSearchTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码
    if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
    {
        UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_bEnableLineMask_1;
        m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
        return;
    }
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_bEnableLineMask_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableLineMask_1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][启用掩模]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedCheckShowFitpoint1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1 = m_bShowFitPoint_1;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
    	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else 	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1 = m_bShowFitPoint_1;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else
	{
		UpdateSearchData(TRUE);
		m_pDlgParent->UpdateGuiDisplay();
	}
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowFitPoint_1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][显示拟合点]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码

    if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
    {
        UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_bEnableLineMask_2;
        m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
        return;
    }
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_bEnableLineMask_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}else
    {
    	UpdateTrainData(TRUE);
	    m_pDlgParent->UpdateGuiDisplay();
    }
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableLineMask_2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][启用掩模]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedCheckShowFitpoint2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCornerCheckSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2 = m_bShowFitPoint_2;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
    	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2 = m_bShowFitPoint_2;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else
	{
		UpdateSearchData(TRUE);
		m_pDlgParent->UpdateGuiDisplay();
	}
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowFitPoint_2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][显示拟合点]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}

//夹角真值键盘输入函数
void CDlgPatternCornerSearchTool::OnEnSetfocusEditAngleBase()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ANGLE_BASE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ANGLE_BASE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ANGLE_BASE, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[夹角真值]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

//夹角波动键盘输入函数
void CDlgPatternCornerSearchTool::OnEnSetfocusEditAngleFlu()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ANGLE_FLU)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ANGLE_FLU, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ANGLE_FLU, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[波动范围]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}


void CDlgPatternCornerSearchTool::OnBnClickedAngleCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bEnableAngleCheck)
	{
		GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ANGLE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_FLU)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANGLE_COR_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
	}
	UpdateSearchData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableAngleCheck ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[启用夹角检测]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedAngleCorCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bEnaleAngleCorCheck)
	{
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RAD_LINE_COR_MODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_COR_MODE_2)->EnableWindow(FALSE);
	}
	UpdateSearchData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnaleAngleCorCheck ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[夹角修正]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}
void CDlgPatternCornerSearchTool::OnLinePolarityChnage1()
{
    UpdateData(TRUE);
    if (m_nOldLinePolarityMode1 != m_nLinePolarityMode1)
    {
        CString strLog;
        CString strVal[] = {_T("暗到亮"),_T("亮到暗"),_T("无极性")};
        strLog.Format(_T("修改[找线工具1][极性][%s]为[%s]\n"),strVal[m_nOldLinePolarityMode1],strVal[m_nLinePolarityMode1]);
        g_opLogFile.SaveLogText(strLog,6);
    }
    m_nOldLinePolarityMode1 = m_nLinePolarityMode1;
}
void CDlgPatternCornerSearchTool::OnLinePolarityChnage2()
{
    UpdateData(TRUE);
    if (m_nOldLinePolarityMode2 != m_nLinePolarityMode2)
    {
        CString strLog;
        CString strVal[] = {_T("暗到亮"),_T("亮到暗"),_T("无极性")};
        strLog.Format(_T("修改[找线工具2][极性][%s]为[%s]\n"),strVal[m_nOldLinePolarityMode2],strVal[m_nLinePolarityMode2]);
        g_opLogFile.SaveLogText(strLog,6);
    }
    m_nOldLinePolarityMode2 = m_nLinePolarityMode2;
}

void CDlgPatternCornerSearchTool::OnBnClickedCheckSortByScore1()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bSortByScore1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][按分数排序]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedCheckSortByScore2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bSortByScore2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][按分数排序]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedChkEnableDualEdge()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableDualEdge ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][启用双极性]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedChkEnableDualEdge2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableDualEdge2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][启用双极性]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedRadLineCorMode1()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLineCorModeChange();
}


void CDlgPatternCornerSearchTool::OnBnClickedRadLineCorMode2()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLineCorModeChange();
}
void CDlgPatternCornerSearchTool::OnLineCorModeChange()
{
    UpdateData(TRUE);
    if (m_nOldLineCorModeSelect != m_nLineCorModeSelect)
    {
        CString strLog;
        CString strVal[] = {_T("线1修正"),_T("线2修正")};
        strLog.Format(_T("修改[夹角修正][%s]为[%s]\n"),strVal[m_nOldLineCorModeSelect],strVal[m_nLineCorModeSelect]);
        g_opLogFile.SaveLogText(strLog,6);
    }
    m_nOldLineCorModeSelect = m_nLineCorModeSelect;
}

void CDlgPatternCornerSearchTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}else 	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else
	{
		UpdateTrainData(TRUE);
	}
	UpdateData(TRUE);
	CString strLog;
	CString strValue = m_bShowCaliper_1 ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[找线工具1][显示卡尺]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,6);
}


void CDlgPatternCornerSearchTool::OnBnClickedChkShowLineCaliper2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	} else 	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else
	{
		UpdateTrainData(TRUE);
	}
	UpdateData(TRUE);
	CString strLog;
	CString strValue = m_bShowCaliper_2 ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[找线工具2][显示卡尺]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,6);
}

void CDlgPatternCornerSearchTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[找线工具1][滤波半宽]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}

}

void CDlgPatternCornerSearchTool::OnEnSetfocusEditLineFilterHalfWidth2() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[找线工具2][滤波半宽]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}

}