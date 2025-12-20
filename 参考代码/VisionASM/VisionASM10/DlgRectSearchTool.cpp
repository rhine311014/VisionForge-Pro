// DlgRectSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgRectSearchTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRectSearchTool Dialog

CDlgRectSearchTool::CDlgRectSearchTool(CWnd* pParent /*=NULL*/)
: CDialog(CDlgRectSearchTool::IDD, pParent)
, m_bSortByScore1(TRUE)
, m_bSortByScore2(TRUE)
, m_bSortByScore3(TRUE)
, m_bSortByScore4(TRUE)
, m_bShowCaliper_1(FALSE)
, m_bShowCaliper_2(FALSE)
, m_bShowCaliper_3(FALSE)
, m_bShowCaliper_4(FALSE)
, m_nResultOutMode(0)
, m_bEnableDualEdge_1(FALSE)
, m_bEnableDualEdge_2(FALSE)
, m_bEnableDualEdge_3(FALSE)
, m_bEnableDualEdge_4(FALSE)
, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgRectSearchTool)
	// NOTE: the ClassWizard will add member initialization here
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_nLineCaliperNum1 = 20;
	m_nLineCaliperNum2 = 20;
	m_nLineCaliperNum3 = 20;
	m_nLineCaliperNum4 = 20;
	m_dLineContrastThre1 = 10.0;
	m_dLineContrastThre2 = 10.0;
	m_dLineContrastThre3 = 10.0;
	m_dLineContrastThre4 = 10.0;
	m_lLineFilterHalfWidth1 = 1;
	m_lLineFilterHalfWidth2 = 1;
	m_lLineFilterHalfWidth3 = 1;
	m_lLineFilterHalfWidth4 = 1;
	m_nLineOutLineNum1 = 10;
	m_nLineOutLineNum2 = 10;
	m_nLineOutLineNum3 = 10;
	m_nLineOutLineNum4 = 10;
	m_lNoiseThre = 10;
	m_lTotalMag = 500;
	m_nLinePolarityMode1 = (int)eLightToDark;
	m_nLinePolarityMode2 = (int)eLightToDark;
	m_nLinePolarityMode3 = (int)eLightToDark;
	m_nLinePolarityMode4 = (int)eLightToDark;
	m_dGrau = 2.0;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;

	m_bEnableLineMask_2 = FALSE;
	m_bShowFitPoint_2 = FALSE;

	m_bEnableLineMask_3 = FALSE;
	m_bShowFitPoint_3 = FALSE;

	m_bEnableLineMask_4 = FALSE;
	m_bShowFitPoint_4 = FALSE;

	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}


void CDlgRectSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRectSearchTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_2, m_nLineCaliperNum2);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_4, m_nLineCaliperNum3);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_5, m_nLineCaliperNum4);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_2, m_dLineContrastThre2);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_4, m_dLineContrastThre3);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_5, m_dLineContrastThre4);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_2, m_nLineOutLineNum2);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_4, m_nLineOutLineNum3);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_5, m_nLineOutLineNum4);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_2, m_nLinePolarityMode2);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_4, m_nLinePolarityMode3);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_5, m_nLinePolarityMode4);
	DDX_Text(pDX, IDC_EDT_GRAU, m_dGrau);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE4, m_bSortByScore3);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE5, m_bSortByScore4);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE2, m_checkSortByScore2);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE4, m_checkSortByScore3);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE5, m_checkSortByScore4);

	//  DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_2, m_bEnableLineMask_2);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_3, m_bEnableLineMask_3);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_4, m_bEnableLineMask_4);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint_1);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_2, m_bShowFitPoint_2);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_3, m_bShowFitPoint_3);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_4, m_bShowFitPoint_4);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_2, m_bShowCaliper_2);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_3, m_bShowCaliper_3);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_4, m_bShowCaliper_4);
	DDX_Control(pDX, IDC_COMBO_OUTPUTPOINT, m_comboOuput);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUTPOINT, m_nResultOutMode);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_0, m_bEnableDualEdge_1);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_1, m_bEnableDualEdge_2);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_2, m_bEnableDualEdge_3);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_3, m_bEnableDualEdge_4);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_2, m_lLineFilterHalfWidth2);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_3, m_lLineFilterHalfWidth3);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_4, m_lLineFilterHalfWidth4);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgRectSearchTool, CDialog)
//{{AFX_MSG_MAP(CDlgRectSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_2, OnChangeEdtLineCalipernum2)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_4, OnChangeEdtLineCalipernum3)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_5, OnChangeEdtLineCalipernum4)
	ON_EN_SETFOCUS(IDC_EDT_GRAU, OnSetfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, OnSetfocusEdtNoisethre)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_2, OnSetfocusEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_2, OnSetfocusEdtLineContrastThre2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_2, OnSetfocusEdtLineOutlineNum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_4, OnSetfocusEdtLineCalipernum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_4, OnSetfocusEdtLineContrastThre3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_4, OnSetfocusEdtLineOutlineNum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_5, OnSetfocusEdtLineCalipernum4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_5, OnSetfocusEdtLineContrastThre4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_5, OnSetfocusEdtLineOutlineNum4)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_3, OnEnSetfocusEditLineFilterHalfWidth3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_4, OnEnSetfocusEditLineFilterHalfWidth4)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDT_ACCEPT_THRE, &CDlgRectSearchTool::OnEnChangeEdtAcceptThre)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgRectSearchTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgRectSearchTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgRectSearchTool::OnBnClickedCheckLineMask2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgRectSearchTool::OnBnClickedCheckShowFitpoint2)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_3, &CDlgRectSearchTool::OnBnClickedCheckLineMask3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_3, &CDlgRectSearchTool::OnBnClickedCheckShowFitpoint3)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_4, &CDlgRectSearchTool::OnBnClickedCheckLineMask4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_4, &CDlgRectSearchTool::OnBnClickedCheckShowFitpoint4)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgRectSearchTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgRectSearchTool::OnBnClickedChkShowLineCaliper2)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_3, &CDlgRectSearchTool::OnBnClickedChkShowLineCaliper3)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_4, &CDlgRectSearchTool::OnBnClickedChkShowLineCaliper4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRectSearchTool message handlers

void CDlgRectSearchTool::UpdateControlDisplay()
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

void CDlgRectSearchTool::UpdateTrainData(bool bValidate)
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
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_bEnableLineMask_1;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_bShowCaliper_1;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_bEnableDualEdge_1;

		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_nLineOutLineNum2;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_dLineContrastThre2;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_lLineFilterHalfWidth2;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_nLinePolarityMode2;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_bSortByScore2;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_nLineCaliperNum2;	
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_bEnableLineMask_2;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_bShowCaliper_2;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1] = m_bEnableDualEdge_2;

		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[2]  = m_nLineOutLineNum3;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[2] = m_dLineContrastThre3;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[2] = m_lLineFilterHalfWidth3;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[2] = (EdgePolarity)m_nLinePolarityMode3;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[2] = m_bSortByScore3;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[2] = m_nLineCaliperNum3;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[2] = m_bEnableLineMask_3;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[2] = m_bShowCaliper_3;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[2] = m_bEnableDualEdge_3;

		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[3] = m_nLineOutLineNum4;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[3] = m_dLineContrastThre4;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[3] = m_lLineFilterHalfWidth4;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[3] = (EdgePolarity)m_nLinePolarityMode4;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[3] = m_bSortByScore4;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[3] = m_nLineCaliperNum4;	
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[3] = m_bEnableLineMask_4;
		((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[3] = m_bShowCaliper_4;
		((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[3] = m_bEnableDualEdge_4;

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);		
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel	= ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_dGrau			= ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
		m_lNoiseThre	= ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
		m_lTotalMag		= ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
		m_bEnableMask	= ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
		
		m_nLineOutLineNum1 =  ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
		m_dLineContrastThre1 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
		m_lLineFilterHalfWidth1 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
		m_nLinePolarityMode1 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
		m_bSortByScore1 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
		m_nLineCaliperNum1 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
		m_bEnableLineMask_1 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];
		m_bShowCaliper_1 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
		m_bEnableDualEdge_1 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];

		m_nLineOutLineNum2 =  ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
		m_dLineContrastThre2 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
		m_lLineFilterHalfWidth2 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
		m_nLinePolarityMode2 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
		m_bSortByScore2 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
		m_nLineCaliperNum2 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
		m_bEnableLineMask_2 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
		m_bShowCaliper_2 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];
		m_bEnableDualEdge_2 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[1];

		m_nLineOutLineNum3 =  ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[2];
		m_dLineContrastThre3 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[2];
		m_lLineFilterHalfWidth3 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[2];
		m_nLinePolarityMode3 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[2];
		m_bSortByScore3 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[2];
		m_nLineCaliperNum3 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[2];
		m_bEnableLineMask_3 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[2];
		m_bShowCaliper_3 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[2];
		m_bEnableDualEdge_3 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[2];

		m_nLineOutLineNum4 =  ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[3];
		m_dLineContrastThre4 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[3];
		m_lLineFilterHalfWidth4 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[3];
		m_nLinePolarityMode4 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[3];
		m_bSortByScore4 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[3];
		m_nLineCaliperNum4 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[3];
		m_bEnableLineMask_4 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[3];
		m_bShowCaliper_4 = ((CRectSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[3];
		m_bEnableDualEdge_4 = ((CRectSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[3];

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
		m_checkSortByScore3.SetCheck(m_bSortByScore3);
		m_checkSortByScore4.SetCheck(m_bSortByScore4);
	
		UpdateData(FALSE);
	}
	
}

void CDlgRectSearchTool::UpdateSearchData(bool bValidate)
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
		
		
		((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_dAngleLow;
		((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_dAngleHigh;

		((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bShowFitPoint_1;
		((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bShowFitPoint_2;
		((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint2 = m_bShowFitPoint_3;
		((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint3 = m_bShowFitPoint_4;
		((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_nResultOutMode = m_nResultOutMode;
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
		
	}
	else // 设置界面值
	{
		m_nSearchNum	= ((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dAngleLow		= ((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CRectSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;

		m_bShowFitPoint_1	= ((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;
		m_bShowFitPoint_2	= ((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;
		m_bShowFitPoint_3	= ((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint2;
		m_bShowFitPoint_4	= ((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint3;
		m_nResultOutMode = ((CRectSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_nResultOutMode;
		UpdateData(FALSE);
	}
	
}

void CDlgRectSearchTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_RECTSEARCH_TOOL") == strDlgID)
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

void CDlgRectSearchTool::OnBtnGetTrainImage() 
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

void CDlgRectSearchTool::OnBtnTrain() 
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
		m_GuiPatternDisplay.RemoveImage();
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		CString strErrInfo =m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}

void CDlgRectSearchTool::OnBtnSetMaskImage() 
{
	// TODO: Add your control notification handler code here
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

void CDlgRectSearchTool::OnChkEnableMask() 
{
	// TODO: Add your control notification handler code here
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

BOOL CDlgRectSearchTool::OnInitDialog() 
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
	CString Stroutput[9];
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		Stroutput[0] = _T("Center");
		Stroutput[1] = _T("Up");
		Stroutput[2] = _T("Down");
		Stroutput[3] = _T("Left");
		Stroutput[4] = _T("Right");
		Stroutput[5] = _T("Upper Left");
		Stroutput[6] = _T("Upper Right");
		Stroutput[7] = _T("Bottom Left");
		Stroutput[8] = _T("Bottom Right");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		Stroutput[0] = _T("中心");
		Stroutput[1] = _T("上");
		Stroutput[2] = _T("下");
		Stroutput[3] = _T("左");
		Stroutput[4] = _T("右");
		Stroutput[5] = _T("左上");
		Stroutput[6] = _T("右上");
		Stroutput[7] = _T("左下");
		Stroutput[8] = _T("右下");
		break;
	}
	for (int i= 0;i<9;i++)
	{
		m_comboOuput.AddString(Stroutput[i]);
	}
	m_comboOuput.SetCurSel(0);

	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRectSearchTool::OnChangeEdtLineCalipernum1() 
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

void CDlgRectSearchTool::OnChangeEdtLineCalipernum2() 
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

void CDlgRectSearchTool::OnChangeEdtLineCalipernum3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum3 < 2) || (m_nLineCaliperNum3 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgRectSearchTool::OnChangeEdtLineCalipernum4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum4 < 2) || (m_nLineCaliperNum4 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgRectSearchTool::CheckDlgDataValidation()
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


	if (m_nLineCaliperNum3 < 2)
	{
		m_nLineCaliperNum3 = 3;
	}
	if (m_nLineCaliperNum3 > 100)
	{
		m_nLineCaliperNum3 = 100;
	}

	if ((m_nLineOutLineNum3 < 0) || (m_nLineOutLineNum3 > (m_nLineCaliperNum3 - 2)))
	{
		m_nLineOutLineNum3 = 0;
	}
	if ((m_dLineContrastThre3 < 0.0) || (m_dLineContrastThre3 > 255.0))
	{
		m_dLineContrastThre3 = 10;
	}
	if (m_lLineFilterHalfWidth3 < 1)
	{
		m_lLineFilterHalfWidth3 = 1;
	}


	if (m_nLineCaliperNum4 < 2)
	{
		m_nLineCaliperNum4 = 3;
	}
	if (m_nLineCaliperNum4 > 100)
	{
		m_nLineCaliperNum4 = 100;
	}

	if ((m_nLineOutLineNum4 < 0) || (m_nLineOutLineNum4 > (m_nLineCaliperNum4 - 2)))
	{
		m_nLineOutLineNum4 = 0;
	}
	if ((m_dLineContrastThre4 < 0.0) || (m_dLineContrastThre4 > 255.0))
	{
		m_dLineContrastThre4 = 10;
	}
	if (m_lLineFilterHalfWidth4 < 1)
	{
		m_lLineFilterHalfWidth4 = 1;
	}

	UpdateData(FALSE);

}


void CDlgRectSearchTool::OnSetfocusEdtGrau() 
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
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtNoisethre() 
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
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtTotalMag() 
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
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtLineCalipernum1() 
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

void CDlgRectSearchTool::OnSetfocusEdtLineContrastThre1() 
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

void CDlgRectSearchTool::OnSetfocusEdtLineOutlineNum1() 
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

void CDlgRectSearchTool::OnSetfocusEdtLineCalipernum2() 
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

void CDlgRectSearchTool::OnSetfocusEdtLineContrastThre2() 
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

void CDlgRectSearchTool::OnSetfocusEdtLineOutlineNum2() 
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


void CDlgRectSearchTool::OnSetfocusEdtLineCalipernum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_4, strOutput);
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtLineContrastThre3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_4, strOutput);
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtLineOutlineNum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_4, strOutput);
		}
	}
}


void CDlgRectSearchTool::OnSetfocusEdtLineCalipernum4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_5, strOutput);
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtLineContrastThre4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_5, strOutput);
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtLineOutlineNum4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_5, strOutput);
		}
	}
}


void CDlgRectSearchTool::OnSetfocusEdtSearchNum() 
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
		}
	}
}

void CDlgRectSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgRectSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgRectSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgRectSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CRectSearchTrainGuiParam trainGuiParam;
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

BOOL CDlgRectSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgRectSearchTool::OnEnChangeEdtAcceptThre()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CDlgRectSearchTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckShowFitpoint1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckShowFitpoint2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgRectSearchTool::OnBnClickedCheckLineMask3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckShowFitpoint3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckLineMask4()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgRectSearchTool::OnBnClickedCheckShowFitpoint4()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}



void CDlgRectSearchTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgRectSearchTool::OnBnClickedChkShowLineCaliper2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgRectSearchTool::OnBnClickedChkShowLineCaliper3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgRectSearchTool::OnBnClickedChkShowLineCaliper4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}

void CDlgRectSearchTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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

void CDlgRectSearchTool::OnEnSetfocusEditLineFilterHalfWidth2() 
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

void CDlgRectSearchTool::OnEnSetfocusEditLineFilterHalfWidth3() 
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
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_3, strOutput);
		}
	}

}

void CDlgRectSearchTool::OnEnSetfocusEditLineFilterHalfWidth4() 
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
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_4, strOutput);
		}
	}

}