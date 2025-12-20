// DlgPatternChordSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternChordSearchTool.h"
#include "afxdialogex.h"
#include "VisionASM.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>


// CDlgPatternChordSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternChordSearchTool, CDialogEx)

CDlgPatternChordSearchTool::CDlgPatternChordSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternChordSearchTool::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_nLinePolarity(0)
	, m_bLineSortByScore(TRUE)
	, m_bCheckCorrectAngle(FALSE)
	, m_bEnableLineMask(FALSE)
	, m_bShowLinePoint(FALSE)
	, m_bShowCircleCaliper(FALSE)
	, m_bShowLineCaliper(FALSE)
	, m_bShowCirclePoint(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_bCircleEnableDualEdge(FALSE)
	, m_bLineEnableDualEdge(FALSE)
	, m_nImageChannel(0)
	, m_bEnableFitLineDMS(TRUE)
	, m_dFitLineDMS(0.3)
	, m_dOffsetRatio(1.0)
{
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8; 
	m_nChordInwardMode = (int)eInward;
	m_nChordPolarityMode = (int)eDarkToLight;
	m_nChordCaliperNum = 20;
	m_dChordContrastThre = 10.0;
	m_lCircleFilterHalfWidth = 1;
	m_nChordOutLineNum = 10;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_nLineAngleType = 0;
	m_lLineFilterHalfWidth = 1;
	m_nChordLineCaliperNum = 20;
	m_nLineOutlineNum = 10;
	m_nLineContrastThre = 10;
	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;

	m_nAccuray = 0;
	m_nCoarseScale = 4;
	m_bEnableLineCircleCross = FALSE;
	m_nLineCircleCrossMode = 0;
}

CDlgPatternChordSearchTool::~CDlgPatternChordSearchTool()
{
}

void CDlgPatternChordSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX,IDC_EDT_CONFUSION_THRE,m_dConfusionThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nChordInwardMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nChordPolarityMode);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nChordCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dChordContrastThre);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM, m_nChordOutLineNum);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM, m_nChordLineCaliperNum);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM, m_nLineOutlineNum);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE, m_nLineContrastThre);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0, m_nLinePolarity);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bLineSortByScore);
	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_comboLineAngleType);
	DDX_Control(pDX, IDC_COMBO_ACCURACY_MODE,m_Accuracy);
	DDX_Control(pDX,IDC_COMBO_SPEED_MODE,m_CoarseScale);
	DDX_Check(pDX, IDC_CHECK_CORRECT_ANGLE, m_bCheckCorrectAngle);
	DDX_Check(pDX, IDC_CHECK_ENABLE_LINE_MASK, m_bEnableLineMask);
	DDX_Check(pDX, IDC_CHECK_SHOW_LINE_POINT, m_bShowLinePoint);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER, m_bShowLineCaliper);
	DDX_Check(pDX, IDC_CHECK_SHOW_CIRCLE_POINT, m_bShowCirclePoint);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_MASK, m_bEnableCircleMask);
	DDX_Check(pDX, IDC_CHK_CIRCLR_ENABLE_DUAL_EDGE, m_bCircleEnableDualEdge);
	DDX_Check(pDX, IDC_CHK_LINE_ENABLE_DUAL_EDGE, m_bLineEnableDualEdge);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
	
	DDX_Check(pDX, IDC_CHECK_ENABLE_LINE_CIRCLE_CROSS, m_bEnableLineCircleCross);
	DDX_Radio(pDX, IDC_RAD_LINE_LEFT_MODE, m_nLineCircleCrossMode);

	DDX_Check(pDX, IDC_CHECK_FIT_LINE_DMS, m_bEnableFitLineDMS);
	DDX_Text(pDX, IDC_EDIT_FIT_LINE_DMS, m_dFitLineDMS);
	DDX_Text(pDX, IDC_EDIT_OFFSET_RATIO, m_dOffsetRatio);

}


BEGIN_MESSAGE_MAP(CDlgPatternChordSearchTool, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, OnChangeEdtChordCalipernum)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, OnRadChordInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, OnRadChordInwardMode1)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, OnSetfocusEdtChordCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, OnSetfocusEdtChordContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, OnSetfocusEdtChordOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE,OnSetfocusEdtConfusionThre)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1,OnEnSetfocusEditCircleFilterHalfWidth)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternChordSearchTool::OnEnSetfocusEdtLineCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM, &CDlgPatternChordSearchTool::OnEnSetfocusEdtLineOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE, &CDlgPatternChordSearchTool::OnEnSetfocusEdtLineContrastThre)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternChordSearchTool::OnEnChangeEdtLineCalipernum)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE_MASK, &CDlgPatternChordSearchTool::OnBnClickedCheckEnableLineMask)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternChordSearchTool::OnBnClickedCheckShowLinePoint)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgPatternChordSearchTool::OnBnClickedChkShowCircleCaliper)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER, &CDlgPatternChordSearchTool::OnBnClickedChkShowLineCaliper)
	ON_BN_CLICKED(IDC_CHECK_SHOW_CIRCLE_POINT, &CDlgPatternChordSearchTool::OnBnClickedCheckShowCirclePoint)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_MASK, &CDlgPatternChordSearchTool::OnBnClickedCheckCircleMask)
	
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE_CIRCLE_CROSS, &CDlgPatternChordSearchTool::OnBnClickedCheckEnableLineCircleCross)
	ON_EN_SETFOCUS(IDC_EDIT_FIT_LINE_DMS, &CDlgPatternChordSearchTool::OnEnSetfocusEditFitLineDms)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_RATIO, &CDlgPatternChordSearchTool::OnEnSetfocusEditOffsetRatio)
END_MESSAGE_MAP()

void CDlgPatternChordSearchTool::UpdateControlDisplay()
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
	
	if (m_bEnableLineCircleCross)
	{
		GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(FALSE);
	}
	
}

void CDlgPatternChordSearchTool::UpdateTrainData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	CPatternChordSearchTrainDataParam* pTrainDataParam = (CPatternChordSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam;
	CPatternChordSearchTrainGuiParam* pTrainGuiParam = (CPatternChordSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam;
	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		pTrainDataParam->m_nImageChannel = m_nImageChannel;
		pTrainDataParam->m_CoarseScale = (CoarseScale)(m_CoarseScale.GetCurSel()+1);
		pTrainDataParam->m_Accuracy    = (Accuracy)(1<<m_Accuracy.GetCurSel());
		pTrainDataParam->m_dAngleLow   = m_dAngleLow;
		pTrainDataParam->m_dAngleHigh  = m_dAngleHigh;
		pTrainDataParam->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		pTrainDataParam->m_nOutLineNum = m_nChordOutLineNum;
		pTrainDataParam->m_dContrastThre = m_dChordContrastThre;
		pTrainDataParam->m_lFilterHalfWidth = m_lCircleFilterHalfWidth;
		pTrainDataParam->m_edgeProperty = (EdgePolarity)m_nChordPolarityMode;		
		pTrainDataParam->m_bSortByScore = m_bSortByScore1;
		pTrainGuiParam->m_nCaliperNum = m_nChordCaliperNum;

		pTrainDataParam->m_dLineContrastThre = m_nLineContrastThre;
		pTrainDataParam->m_lLineFilterHalfWidth = m_lLineFilterHalfWidth;
		pTrainDataParam->m_nLineOutLineNum = m_nLineOutlineNum;
		pTrainDataParam->m_edgeLineProperty = (EdgePolarity)m_nLinePolarity;
		pTrainDataParam->m_bLineSortByScore = m_bLineSortByScore;
		pTrainDataParam->m_nLineAngleType = m_comboLineAngleType.GetCurSel();

		pTrainDataParam->m_bCircleEnableDualEdge = m_bCircleEnableDualEdge;
		pTrainDataParam->m_bLineEnableDualEdge = m_bLineEnableDualEdge;
		pTrainDataParam->m_bEnableFitLineDMS = m_bEnableFitLineDMS;
		pTrainDataParam->m_dFitLineDMS = m_dFitLineDMS;
		pTrainGuiParam->m_bCircleEnableMask = m_bEnableCircleMask;
		pTrainGuiParam->m_bLineEnableMask = m_bEnableLineMask;

		pTrainGuiParam->m_CaliperSearchDirection = (CaliperSearchDirection)m_nChordInwardMode;
		pTrainGuiParam->m_nLineCaliperNum = m_nChordLineCaliperNum;

		pTrainGuiParam->m_bCorrectAngle = m_bCheckCorrectAngle;
		pTrainGuiParam->m_bCircleCaliperVisible = m_bShowCircleCaliper;
		pTrainGuiParam->m_bLineCaliperVisible = m_bShowLineCaliper;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	}
	else // 设置界面值
	{		
		m_nImageChannel = pTrainDataParam->m_nImageChannel;
		m_CoarseScale.SetCurSel(((int)pTrainDataParam->m_CoarseScale)-1);
		m_Accuracy.SetCurSel(((int)pTrainDataParam->m_Accuracy)/2);
		m_nAccuray = m_Accuracy.GetCurSel();
		m_nCoarseScale = m_CoarseScale.GetCurSel();
		m_dAngleLow     = pTrainDataParam->m_dAngleLow;
		m_dAngleHigh    = pTrainDataParam->m_dAngleHigh;
		m_bEnableMask	= pTrainDataParam->m_bMaskEnable;
		m_nChordOutLineNum =  pTrainDataParam->m_nOutLineNum;
		m_dChordContrastThre = pTrainDataParam->m_dContrastThre;
		m_lCircleFilterHalfWidth = pTrainDataParam->m_lFilterHalfWidth;
		m_nChordPolarityMode = pTrainDataParam->m_edgeProperty;
		m_bSortByScore1 = pTrainDataParam->m_bSortByScore;
		m_nChordCaliperNum = pTrainGuiParam->m_nCaliperNum;
		m_nChordInwardMode = pTrainGuiParam->m_CaliperSearchDirection;

		m_nLineContrastThre = pTrainDataParam->m_dLineContrastThre;
		m_nLineOutlineNum = pTrainDataParam->m_nLineOutLineNum;
		m_nLinePolarity = pTrainDataParam->m_edgeLineProperty;
		m_bLineSortByScore = pTrainDataParam->m_bLineSortByScore;
		m_comboLineAngleType.SetCurSel(pTrainDataParam->m_nLineAngleType);
		m_lLineFilterHalfWidth = pTrainDataParam->m_lLineFilterHalfWidth;

		m_bCircleEnableDualEdge = pTrainDataParam->m_bCircleEnableDualEdge;
		m_bLineEnableDualEdge = pTrainDataParam->m_bLineEnableDualEdge;
		m_bEnableCircleMask = pTrainGuiParam->m_bCircleEnableMask;
		m_bEnableLineMask = pTrainGuiParam->m_bLineEnableMask;

		m_nChordLineCaliperNum =  pTrainGuiParam->m_nLineCaliperNum;
		m_bCheckCorrectAngle = pTrainGuiParam->m_bCorrectAngle;
		m_bShowCircleCaliper = pTrainGuiParam->m_bCircleCaliperVisible;
		m_bShowLineCaliper = pTrainGuiParam->m_bLineCaliperVisible;
		m_bEnableFitLineDMS = pTrainDataParam->m_bEnableFitLineDMS;
		m_dFitLineDMS = pTrainDataParam->m_dFitLineDMS;
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}

		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		UpdateData(FALSE);
	}
}

void CDlgPatternChordSearchTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	CPatternChordSearchSearchDataParam* pSearchDataParam = (CPatternChordSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam;
	CPatternChordSearchSearchGuiParam* pSearchGuiParam = (CPatternChordSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam;
	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);

		pSearchDataParam->m_nSearchNum = m_nSearchNum;
		pSearchDataParam->m_dAcceptThre = m_dAcceptThre;
		pSearchDataParam->m_dConfusionThre = m_dConfusionThre;
		pSearchDataParam->m_bEnableLineCircleCross = m_bEnableLineCircleCross;
		pSearchDataParam->m_nLineCircleCrossMode = m_nLineCircleCrossMode;
		pSearchDataParam->m_dOffsetRatio = m_dOffsetRatio; // 偏移比例;

		pSearchGuiParam->m_bEnableShowCirclePoint = m_bShowCirclePoint;
		pSearchGuiParam->m_bEnableShowLinePoint = m_bShowLinePoint;
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		//m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{

		m_nSearchNum	= pSearchDataParam->m_nSearchNum;
		m_dAcceptThre	= pSearchDataParam->m_dAcceptThre;
		m_dConfusionThre= pSearchDataParam->m_dConfusionThre;

		m_bEnableLineCircleCross = pSearchDataParam->m_bEnableLineCircleCross;
		m_nLineCircleCrossMode = pSearchDataParam->m_nLineCircleCrossMode;
		m_bShowLinePoint = pSearchGuiParam->m_bEnableShowLinePoint;
		m_dOffsetRatio = pSearchDataParam->m_dOffsetRatio;
		if (m_bEnableLineCircleCross)
		{
			GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_OFFSET_RATIO)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_OFFSET_RATIO)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_OFFSET_RATIO)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OFFSET_RATIO)->EnableWindow(FALSE);
		}
		m_bShowCirclePoint = pSearchGuiParam->m_bEnableShowCirclePoint;
		UpdateData(FALSE);
	}

}

void CDlgPatternChordSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNCHORDSEARCH_TOOL") == strDlgID)
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

void CDlgPatternChordSearchTool::OnBtnGetTrainImage() 
{
	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);
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

void CDlgPatternChordSearchTool::OnBtnTrain() 
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
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
	}
	else
	{
		CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
	}
}

void CDlgPatternChordSearchTool::OnBtnSetMaskImage() 
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
	}
}

void CDlgPatternChordSearchTool::OnChkEnableMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bEnableMask)
	{
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
	}
}

BOOL CDlgPatternChordSearchTool::OnInitDialog()
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

	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);

	m_comboLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_ANYANGLE));
	m_comboLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
	m_comboLineAngleType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));

	m_comboLineAngleType.SetCurSel(m_nLineAngleType);

	m_Accuracy.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_Accuracy.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_Accuracy.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_Accuracy.SetCurSel(m_nAccuray);

	m_CoarseScale.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_CoarseScale.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_CoarseScale.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_CoarseScale.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_CoarseScale.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_CoarseScale.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_CoarseScale.SetCurSel(m_nCoarseScale);


	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatternChordSearchTool::OnChangeEdtChordCalipernum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nChordCaliperNum < 2) || (m_nChordCaliperNum > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternChordSearchTool::OnRadChordInwardMode0() 
{
	// TODO: Add your control notification handler code here
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternChordSearchTool::OnRadChordInwardMode1() 
{
	// TODO: Add your control notification handler code here
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternChordSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
	if (m_nSearchNum < 1)
	{
		m_nSearchNum = 1;
	}
	if ((m_dAcceptThre < 0.0) || (m_dAcceptThre > 1.0))
	{
		m_dAcceptThre = 0.6;
	}
	if((m_dConfusionThre<0.0) || (m_dConfusionThre) > 1.0)
	{
		m_dConfusionThre = 0.8;
	}
	if(m_dAcceptThre > m_dConfusionThre)
	{
		m_dAcceptThre = m_dConfusionThre;
	}
	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -10.0;
		m_dAngleHigh = 10.0;
	}

	if (m_nChordCaliperNum < 3)
	{
		m_nChordCaliperNum = 3;
	}
	if ((m_nChordOutLineNum < 0) || (m_nChordOutLineNum > (m_nChordCaliperNum - 3)))
	{
		m_nChordOutLineNum = 0;
	}
	if ((m_dChordContrastThre < 0.0) || (m_dChordContrastThre > 255.0))
	{
		m_dChordContrastThre = 10;
	}
	if (m_lLineFilterHalfWidth < 1)
	{
		m_lLineFilterHalfWidth = 1;
	}
	if (m_lCircleFilterHalfWidth < 1)
	{
		m_lCircleFilterHalfWidth = 1;
	}
	UpdateData(FALSE);
}

void CDlgPatternChordSearchTool::OnSetfocusEdtChordCalipernum() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtChordContrastThre() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtChordOutlineNum() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtConfusionThre()
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

void CDlgPatternChordSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgPatternChordSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgPatternChordSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternChordSearchTrainGuiParam trainGuiParam;
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

void CDlgPatternChordSearchTool::OnEnSetfocusEdtLineCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM, strOutput);
		}
	}
}


void CDlgPatternChordSearchTool::OnEnSetfocusEdtLineOutlineNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM, strOutput);
		}
	}
}


void CDlgPatternChordSearchTool::OnEnSetfocusEdtLineContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE, strOutput);
		}
	}	
}

void CDlgPatternChordSearchTool::OnEnChangeEdtLineCalipernum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ((m_nChordLineCaliperNum < 2) || (m_nChordLineCaliperNum > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternChordSearchTool::OnBnClickedCheckEnableLineMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateTrainData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
	UpdateData(FALSE);
}

void CDlgPatternChordSearchTool::OnBnClickedCheckShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateSearchData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
	UpdateData(FALSE);
}


// CDlgPatternChordSearchTool 消息处理程序


void CDlgPatternChordSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(TRUE);
}


void CDlgPatternChordSearchTool::OnBnClickedChkShowLineCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(TRUE);
}


void CDlgPatternChordSearchTool::OnBnClickedCheckShowCirclePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateSearchData(TRUE);
	m_pDlgParent->UpdateGuiDisplay();
	UpdateData(FALSE);
}


void CDlgPatternChordSearchTool::OnBnClickedCheckCircleMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(TRUE);
}


BOOL CDlgPatternChordSearchTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgPatternChordSearchTool::OnEnSetfocusEditLineFilterHalfWidth() 
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


void CDlgPatternChordSearchTool::OnBnClickedCheckEnableLineCircleCross()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_bEnableLineCircleCross)
	{
		GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_OFFSET_RATIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_RATIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RAD_LINE_LEFT_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_RIGHT_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_UP_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_LINE_DOWN_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_OFFSET_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_RATIO)->EnableWindow(FALSE);
	}
}


void CDlgPatternChordSearchTool::OnEnSetfocusEditFitLineDms()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_FIT_LINE_DMS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_FIT_LINE_DMS, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_FIT_LINE_DMS, strOutput);
			UpdateSearchData(true);
		}
	}
}


void CDlgPatternChordSearchTool::OnEnSetfocusEditCircleFilterHalfWidth() 
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


void CDlgPatternChordSearchTool::OnEnSetfocusEditOffsetRatio()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_RATIO)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_RATIO, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_RATIO, strOutput);
			UpdateSearchData(true);
		}
	}

}