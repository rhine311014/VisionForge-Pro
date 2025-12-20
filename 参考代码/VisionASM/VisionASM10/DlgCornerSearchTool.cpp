// DlgCornerSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgCornerSearchTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>
#include "LogFileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCornerSearchTool Dialog

CDlgCornerSearchTool::CDlgCornerSearchTool(CWnd* pParent /*=NULL*/)
: CDialog(CDlgCornerSearchTool::IDD, pParent)
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
	//{{AFX_DATA_INIT(CDlgCornerSearchTool)
	// NOTE: the ClassWizard will add member initialization here
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_nLineCaliperNum1 = 20;
	m_nLineCaliperNum2 = 20;
	m_dLineContrastThre1 = 10.0;
	m_dLineContrastThre2 = 10.0;
	m_lLineFilterHalfWidth1 = 1;
	m_lLineFilterHalfWidth2 = 1;
	m_nLineOutLineNum1 = 10;
	m_nLineOutLineNum2 = 10;
	m_lNoiseThre = 10;
	m_lTotalMag = 500;
	m_nLinePolarityMode1 = (int)eLightToDark;
	m_nLinePolarityMode2 = (int)eLightToDark;
    m_nOldLinePolarityMode1 = m_nLinePolarityMode1;
    m_nOldLinePolarityMode2 = m_nLinePolarityMode2;
	m_dGrau = 2.0;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;

	m_bEnableLineMask_2 = FALSE;
	m_bShowFitPoint_2 = FALSE;

	m_bEnableAngleCheck		= FALSE;
	m_bEnaleAngleCorCheck	= FALSE;
	m_nLineCorModeSelect    = 0;
    m_nOldLineCorModeSelect = m_nLineCorModeSelect;
	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}


void CDlgCornerSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCornerSearchTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_2, m_nLineCaliperNum2);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_2, m_dLineContrastThre2);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_2, m_nLineOutLineNum2);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_2, m_nLinePolarityMode2);
	DDX_Text(pDX, IDC_EDT_GRAU, m_dGrau);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE2, m_checkSortByScore2);

	//  DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask);
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


BEGIN_MESSAGE_MAP(CDlgCornerSearchTool, CDialog)
//{{AFX_MSG_MAP(CDlgCornerSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_2, OnChangeEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_GRAU, OnSetfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, OnSetfocusEdtNoisethre)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_2, OnSetfocusEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_2, OnSetfocusEdtLineContrastThre2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_2, OnSetfocusEdtLineOutlineNum2)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE_BASE, OnEnSetfocusEditAngleBase)
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE_FLU, OnEnSetfocusEditAngleFlu)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDT_ACCEPT_THRE, &CDlgCornerSearchTool::OnEnChangeEdtAcceptThre)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgCornerSearchTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgCornerSearchTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgCornerSearchTool::OnBnClickedCheckLineMask2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgCornerSearchTool::OnBnClickedCheckShowFitpoint2)
	ON_BN_CLICKED(IDC_ANGLE_CHECK, &CDlgCornerSearchTool::OnBnClickedAngleCheck)
	ON_BN_CLICKED(IDC_ANGLE_COR_CHECK, &CDlgCornerSearchTool::OnBnClickedAngleCorCheck)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE0_1, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode01)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE1_1, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode11)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE2_1, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode21)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE0_2, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode02)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE1_2, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode12)
    ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE2_2, &CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode22)
    ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE1, &CDlgCornerSearchTool::OnBnClickedCheckSortByScore1)
    ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE2, &CDlgCornerSearchTool::OnBnClickedCheckSortByScore2)
    ON_BN_CLICKED(IDC_CHK_ENABLE_DUAL_EDGE, &CDlgCornerSearchTool::OnBnClickedChkEnableDualEdge)
    ON_BN_CLICKED(IDC_CHK_ENABLE_DUAL_EDGE2, &CDlgCornerSearchTool::OnBnClickedChkEnableDualEdge2)
    ON_BN_CLICKED(IDC_RAD_LINE_COR_MODE_1, &CDlgCornerSearchTool::OnBnClickedRadLineCorMode1)
    ON_BN_CLICKED(IDC_RAD_LINE_COR_MODE_2, &CDlgCornerSearchTool::OnBnClickedRadLineCorMode2)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgCornerSearchTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgCornerSearchTool::OnBnClickedChkShowLineCaliper2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCornerSearchTool message handlers

void CDlgCornerSearchTool::UpdateControlDisplay()
{	
	UpdateData(TRUE);
	
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
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

void CDlgCornerSearchTool::UpdateTrainData(bool bValidate)
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
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_bEnableDualEdge;

		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_nLineOutLineNum2;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_dLineContrastThre2;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_lLineFilterHalfWidth2;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_nLinePolarityMode2;
		((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_bSortByScore2;
		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;	
		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
        ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_bEnableDualEdge2;

		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);		
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel	= ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_dGrau			= ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
		m_lNoiseThre	= ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
		m_lTotalMag		= ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
		m_bEnableMask	= ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
		
		m_nLineOutLineNum1 =  ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
		m_dLineContrastThre1 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
		m_lLineFilterHalfWidth1 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
		m_nLinePolarityMode1 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
		m_bSortByScore1 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
		m_nLineCaliperNum1 = ((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
		m_bEnableLineMask_1 = ((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];
		m_bEnableDualEdge = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];

		m_nLineOutLineNum2 =  ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
		m_dLineContrastThre2 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
		m_lLineFilterHalfWidth2 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
		m_nLinePolarityMode2 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
		m_bSortByScore2 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
		m_nLineCaliperNum2 = ((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
		m_bEnableLineMask_2 = ((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
		m_bEnableDualEdge2 = ((CCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];

		m_bShowCaliper_1=((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
		m_bShowCaliper_2=((CCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}
		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_checkSortByScore2.SetCheck(m_bSortByScore2);
		
		UpdateData(FALSE);
	}
	
}

void CDlgCornerSearchTool::UpdateSearchData(bool bValidate)
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
		
		
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_dAngleLow;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_dAngleHigh;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleBaseValue = m_dAngleBaseValue;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleFluValue = m_dAngleFluValue;
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableAngleCheck = m_bEnableAngleCheck;		
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnaleAngleCorCheck = m_bEnaleAngleCorCheck;	
		((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nLineCorModeSelect	= m_nLineCorModeSelect;	

		((CCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bShowFitPoint_1;
		((CCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bShowFitPoint_2;
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
		
	}
	else // 设置界面值
	{
		m_nSearchNum	= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dAngleLow		= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
		m_dAngleBaseValue = ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleBaseValue;
		m_dAngleFluValue  = ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleFluValue;
		m_bEnableAngleCheck		= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableAngleCheck;
		m_bEnaleAngleCorCheck	= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnaleAngleCorCheck;
		m_nLineCorModeSelect	= ((CCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nLineCorModeSelect;

		m_bShowFitPoint_1	= ((CCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;
		m_bShowFitPoint_2	= ((CCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;
		
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

void CDlgCornerSearchTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_CORNERSEARCH_TOOL") == strDlgID)
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

void CDlgCornerSearchTool::OnBtnGetTrainImage() 
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

void CDlgCornerSearchTool::OnBtnTrain() 
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
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->UpdateGuiDisplay();
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

void CDlgCornerSearchTool::OnBtnSetMaskImage() 
{
	// TODO: Add your control notification handler code here
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

void CDlgCornerSearchTool::OnChkEnableMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableMask ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[掩模]为%s\n"),strValue);
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

BOOL CDlgCornerSearchTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCornerSearchTool::OnChangeEdtLineCalipernum1() 
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
	}else 	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}

}

void CDlgCornerSearchTool::OnChangeEdtLineCalipernum2() 
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
	}else 	if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else
	{
		UpdateTrainData(true);
		UpdateSearchData(true);
	}
	
}

void CDlgCornerSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
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
		m_dGrau = 2.0;
	}
	if ((m_lNoiseThre < 1) || (m_lNoiseThre > 255))
	{
		m_lNoiseThre = 50;
	}
	if (m_lTotalMag < 0)
	{
		m_lTotalMag = 500;
	}
	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -10.0;
		m_dAngleHigh = 10.0;
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
	if (m_dAngleBaseValue <= m_dAngleFluValue)
	{
		m_dAngleBaseValue		= 90.0;	
		m_dAngleFluValue		= 5.0;	
	}
	UpdateData(FALSE);

}


void CDlgCornerSearchTool::OnSetfocusEdtGrau() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
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

void CDlgCornerSearchTool::OnSetfocusEdtNoisethre() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_NOISETHRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_NOISETHRE, strInput);
		
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_NOISETHRE, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[几何定位][对比度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgCornerSearchTool::OnSetfocusEdtTotalMag() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TOTAL_MAG)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TOTAL_MAG, strInput);
		
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
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

void CDlgCornerSearchTool::OnSetfocusEdtLineCalipernum1() 
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

void CDlgCornerSearchTool::OnSetfocusEdtLineContrastThre1() 
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

void CDlgCornerSearchTool::OnSetfocusEdtLineOutlineNum1() 
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

void CDlgCornerSearchTool::OnSetfocusEdtLineCalipernum2() 
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

void CDlgCornerSearchTool::OnSetfocusEdtLineContrastThre2() 
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

void CDlgCornerSearchTool::OnSetfocusEdtLineOutlineNum2() 
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

void CDlgCornerSearchTool::OnSetfocusEdtSearchNum() 
{
	// TODO: Add your control notification handler code here
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

void CDlgCornerSearchTool::OnSetfocusEdtAcceptThre() 
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
                strLog.Format(_T("修改[相似度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgCornerSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgCornerSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgCornerSearchTool::OnBtnFullImageSearch() 
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
		}
		else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
		{
			CCombinationCornerSearchTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
		else if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
		{
			CACFInspectTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
		else
		{
			CCornerSearchTrainGuiParam trainGuiParam;
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

BOOL CDlgCornerSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgCornerSearchTool::OnEnChangeEdtAcceptThre()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CDlgCornerSearchTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码
    if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
    {
        UpdateData();
		 m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
        ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_bEnableLineMask_1;
        m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
        return;
	} 
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[0] = m_bEnableLineMask_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;
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
    CString strValue = m_bEnableLineMask_1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][启用掩模]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedCheckShowFitpoint1()
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
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint1 = m_bShowFitPoint_1;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bShowFitPoint_1;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
    else
    {
	    UpdateSearchData(TRUE);
    }
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowFitPoint_1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][显示拟合点]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码

    if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
    {
        UpdateData();
		 m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
        ((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_bEnableLineMask_2;
        m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
        return;
	} 
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCornerLineMask[1] = m_bEnableLineMask_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
    {
	    UpdateTrainData(TRUE);
    }
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableLineMask_2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][启用掩模]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedCheckShowFitpoint2()
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
	}    
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CCombinationCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bVisibleFitPoint2 = m_bShowFitPoint_2;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bShowFitPoint_2;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
    {
        UpdateSearchData(TRUE);
    }
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowFitPoint_2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][显示拟合点]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
	
}

//夹角真值键盘输入函数
void CDlgCornerSearchTool::OnEnSetfocusEditAngleBase()
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
void CDlgCornerSearchTool::OnEnSetfocusEditAngleFlu()
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


void CDlgCornerSearchTool::OnBnClickedAngleCheck()
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
void CDlgCornerSearchTool::OnBnClickedAngleCorCheck()
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

void CDlgCornerSearchTool::OnLinePolarityChnage1()
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
void CDlgCornerSearchTool::OnLinePolarityChnage2()
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


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode01()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage1();
}


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode11()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage1();
}


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode21()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage1();
}


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode02()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage2();
}


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode12()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage2();
}


void CDlgCornerSearchTool::OnBnClickedRadLinePolarityMode22()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLinePolarityChnage2();
}


void CDlgCornerSearchTool::OnBnClickedCheckSortByScore1()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bSortByScore1 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][按分数排序]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedCheckSortByScore2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bSortByScore2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][按分数排序]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedChkEnableDualEdge()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableDualEdge ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具1][启用双极性]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}


void CDlgCornerSearchTool::OnBnClickedChkEnableDualEdge2()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableDualEdge2 ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[找线工具2][启用双极性]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
}

void CDlgCornerSearchTool::OnLineCorModeChange()
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


void CDlgCornerSearchTool::OnBnClickedRadLineCorMode1()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLineCorModeChange();
}


void CDlgCornerSearchTool::OnBnClickedRadLineCorMode2()
{
    // TODO: 在此添加控件通知处理程序代码
    OnLineCorModeChange();
}


void CDlgCornerSearchTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCornerSearchCheckInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCornerCheckSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
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


void CDlgCornerSearchTool::OnBnClickedChkShowLineCaliper2()
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
	}
	if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eCombinationCornerSearchTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CCombinationCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		return;
	}
	else if (m_pDlgParent->m_pSearchTool && m_pDlgParent->m_pSearchTool->GetSearchToolType()==eACFInspectTool)
	{
		UpdateData();
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
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
	strLog.Format(_T("修改[找线工具1][显示卡尺]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,6);
}

void CDlgCornerSearchTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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

void CDlgCornerSearchTool::OnEnSetfocusEditLineFilterHalfWidth2() 
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