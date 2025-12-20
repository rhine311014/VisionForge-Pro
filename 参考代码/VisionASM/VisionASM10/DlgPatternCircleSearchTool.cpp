
// DlgPatternLineCircleTool.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgPatternCircleSearchTool.h"
#include "afxdialogex.h"

#include "VisionASM.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>


/////////////////////////////////////////////////////////////////////////////
// CDlgPatternCircleSearchTool 对话框

CDlgPatternCircleSearchTool::CDlgPatternCircleSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternCircleSearchTool::IDD, pParent)
    , m_bEnableCircleResultOffset(FALSE)
	, m_bShowCircleCaliper(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_bEnableDualEdge(FALSE)
	, m_nImageChannel(0)
{
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_bEnableMask = FALSE;

	// Circle
	m_nCircleCaliperNum = 20;
	m_nCircleOutLineNum = 10;
	m_dCircleContrastThre = 10.0;
	m_nCircleInwardMode = (int)eInward;
	m_nCirclePolarityMode = (int)eDontCare;
	m_lCircleFilterHalfWidth = 1;
	m_bCircleSortByScore = TRUE;


	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_bCoordSortEnable = FALSE;
	m_dCoordSortGap = 64;
	m_nCoordSortMethod = 0;
	m_nCoordSortOrderX = 0;
	m_nCoordSortOrderY = 0;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;

	m_bEnableShowLinePoint = FALSE;
	m_nAuxiliaryPosType = 0;

    m_nCircleCenterMoveMode = 0;
    m_nCircleCenterMoveModeX = 0;
    m_nCircleCenterMoveModeY = 0;
    m_dCircleCenterMoveLengthX = 0.0;
    m_dCircleCenterMoveRadiusMulX = 0.0;
    m_dCircleCenterMoveLengthY  = 0.0;
    m_dCircleCenterMoveRadiusMulY  = 0.0;
    m_dCircleCenterMoveAngle  = 0.0;
    m_dCircleCenterMoveRadiusMul = 0.0;
}


CDlgPatternCircleSearchTool::~CDlgPatternCircleSearchTool()
{
}

void CDlgPatternCircleSearchTool::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
    DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
    DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);

    DDX_Control(pDX, IDC_COMBO_SPEED_MODE, m_comboSpeed);
    DDX_Control(pDX, IDC_COMBO_ACCURACY_MODE, m_comboAccuracy);
    DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
    DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
    DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);

    // Circle
    DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
    DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM, m_nCircleOutLineNum);
    DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
    DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
    DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
    DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bCircleSortByScore);
    DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_CircleCheckSortByScore);

    DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
    DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
    DDX_Text(pDX, IDC_EDT_CONFUSION_THRE, m_dConfusionThre);
    DDX_Check(pDX, IDC_CHK_COORD_SORT_ENABLE, m_bCoordSortEnable);
    DDX_Text(pDX, IDC_EDT_COORD_SORT_GAP, m_dCoordSortGap);
    DDX_Control(pDX, IDC_COMBO_COORD_SORT_METHOD, m_comboCoordSortMethod);
    DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_X_DE, m_nCoordSortOrderX);
    DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_Y_DE, m_nCoordSortOrderY);

	DDX_Check(pDX, IDC_CHECK_SHOW_LINE_POINT, m_bEnableShowLinePoint);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_POINT, m_checkShowLinePoint);
	DDX_Radio(pDX, IDC_RADIO_CIRCLE_CENTER, m_nAuxiliaryPosType);
	DDX_Check(pDX, IDC_CHK_ENABLE_CIRCLERESULT_OFFSET, m_bEnableCircleResultOffset);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK, m_bEnableCircleMask);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}

BEGIN_MESSAGE_MAP(CDlgPatternCircleSearchTool, CDialog)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)

	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, OnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1,OnEnSetfocusEditCircleFilterHalfWidth)

	// Circle
	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, OnChangeEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, OnSetfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, OnSetfocusEdtCircleOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, OnSetfocusEdtCircleContrastThre)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, OnRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, OnRadCircleInwardMode1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK, &CDlgPatternCircleSearchTool::OnBnClickedCheckLineMask)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternCircleSearchTool::OnBnClickedCheckShowLinePoint)

	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, OnSetfocusEdtConfusionThre)

	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternCircleSearchTool::OnBnClickedCheckShowLinePoint)

	ON_CBN_SELCHANGE(IDC_COMBO_COORD_SORT_METHOD, &CDlgPatternCircleSearchTool::OnCbnSelchangeComboCoordSortMethod)
	ON_EN_SETFOCUS(IDC_EDT_COORD_SORT_GAP, &CDlgPatternCircleSearchTool::OnSetfocusEdtCoordSortGap)
    ON_BN_CLICKED(IDC_BTN_RESULT_OFFSET_SETTING, &CDlgPatternCircleSearchTool::OnBnClickedBtnResultOffsetSetting)
    ON_BN_CLICKED(IDC_CHK_ENABLE_CIRCLERESULT_OFFSET, &CDlgPatternCircleSearchTool::OnBnClickedChkEnableCircleresultOffset)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgPatternCircleSearchTool::OnBnClickedChkShowCircleCaliper)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCircleSearchTool message handlers

void CDlgPatternCircleSearchTool::UpdateControlDisplay()
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
        GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(m_bEnableCircleResultOffset);
	}
	else
	{
		GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(m_bEnableMask);
        GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(m_bEnableCircleResultOffset);

		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
}

void CDlgPatternCircleSearchTool::UpdateTrainData(bool bValidate)
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
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;

		// Circle
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum = m_nCircleOutLineNum;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre = m_dCircleContrastThre;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth = m_lCircleFilterHalfWidth;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty = (EdgePolarity)m_nCirclePolarityMode;		
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore = m_bCircleSortByScore;
		((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum = m_nCircleCaliperNum;
		((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CaliperSearchDirection = (CaliperSearchDirection)m_nCircleInwardMode;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nAuxiliaryPosType = m_nAuxiliaryPosType;
		((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible = m_bShowCircleCaliper;
		((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask = m_bEnableCircleMask;
		((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge = m_bEnableDualEdge;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{		
		m_nImageChannel = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

		// Circle
		m_nCircleOutLineNum =  ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum;
		m_dCircleContrastThre = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre;
		m_lCircleFilterHalfWidth = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth;
		m_nCirclePolarityMode = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty;
		m_bCircleSortByScore = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore;
		m_nCircleCaliperNum = ((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum;
		m_nCircleInwardMode = ((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CaliperSearchDirection;
		m_nAuxiliaryPosType =  ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nAuxiliaryPosType;
		m_bShowCircleCaliper = ((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible;
		m_bEnableCircleMask = ((CPatternCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask;
		m_bEnableDualEdge = ((CPatternCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge;
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
		m_CircleCheckSortByScore.SetCheck(m_bCircleSortByScore);
// 		m_LineCheckSortByScore1.SetCheck(m_bLineSortByScore1);
// 		m_comboLineAngleType.SetCurSel(m_nLineAngleType);

		m_checkShowLinePoint.SetCheck(m_bEnableShowLinePoint);

		UpdateData(FALSE);
	}
}


void CDlgPatternCircleSearchTool::UpdateSearchData(bool bValidate)
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

		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;

		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable = m_bCoordSortEnable;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod = (CoordSortMethod)m_nCoordSortMethod;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap = m_dCoordSortGap;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX = (SortOrder)m_nCoordSortOrderX;
		((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY =  (SortOrder)m_nCoordSortOrderY;

		((CPatternCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bEnableShowLinePoint;

        //获取移动圆心界面显示
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableCircleCenterMove     = m_bEnableCircleResultOffset;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveMode		= m_nCircleCenterMoveMode;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveModeX		= m_nCircleCenterMoveModeX;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveModeY		= m_nCircleCenterMoveModeY;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveLengthX	= m_dCircleCenterMoveLengthX;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMulX= m_dCircleCenterMoveRadiusMulX;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveLengthY	= m_dCircleCenterMoveLengthY;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMulY= m_dCircleCenterMoveRadiusMulY;
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveAngle		= m_dCircleCenterMoveAngle;			
        ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMul	= m_dCircleCenterMoveRadiusMul;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dConfusionThre = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		m_bCoordSortEnable = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable;
		m_nCoordSortMethod = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod;
		m_dCoordSortGap    = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap;
		m_nCoordSortOrderX = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX;
		m_nCoordSortOrderY = ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY;

		m_bEnableShowLinePoint = ((CPatternCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;

		m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);
        
        //设置移动圆心界面显示
        m_bEnableCircleResultOffset		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableCircleCenterMove;
        m_nCircleCenterMoveMode			= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveMode;
        m_nCircleCenterMoveModeX		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveModeX;
        m_nCircleCenterMoveModeY		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCircleCenterMoveModeY;
        m_dCircleCenterMoveLengthX		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveLengthX;
        m_dCircleCenterMoveRadiusMulX	= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMulX;
        m_dCircleCenterMoveLengthY		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveLengthY;
        m_dCircleCenterMoveRadiusMulY	= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMulY;
        m_dCircleCenterMoveAngle		= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveAngle;		
        m_dCircleCenterMoveRadiusMul	= ((CPatternCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCircleCenterMoveRadiusMul;

        if (m_bEnableCircleResultOffset)
        {
            GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(FALSE);
        }
		UpdateData(FALSE);
	}

}

void CDlgPatternCircleSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);

	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
	}


	// Circle
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

	// Line
// 	if (m_nLineCaliperNum1 < 3)
// 	{
// 		m_nLineCaliperNum1 = 3;
// 	}
// 
// 	if ((m_nLineOutLineNum1 < 0) || (m_nLineOutLineNum1 > (m_nLineCaliperNum1 - 3)))
// 	{
// 		m_nLineOutLineNum1 = 0;
// 	}
// 	if ((m_dLineContrastThre1 < 0.0) || (m_dLineContrastThre1 > 255.0))
// 	{
// 		m_dLineContrastThre1 = 10;
// 	}
// 	if (m_lLineFilterHalfWidth1 < 1)
// 	{
// 		m_lLineFilterHalfWidth1 = 1;
// 	}


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
	// 	if (m_dAngleLow > m_dAngleHigh)
	// 	{
	// 		m_dAngleLow = -10.0;
	// 		m_dAngleHigh = 10.0;
	// 	}

	if (m_dCoordSortGap < 0)
	{
		m_dCoordSortGap = 64;
	}


	UpdateData(FALSE);
}

void CDlgPatternCircleSearchTool::UpdateDialogLanguage()
{

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

		if(_T("IDD_DLG_PATTERN_CIRCLESEARCH_TOOL") == strDlgID)
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

BOOL CDlgPatternCircleSearchTool::OnInitDialog()
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

// 	m_comboLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_ANYANGLE));
// 	m_comboLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
// 	m_comboLineAngleType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));
// 	m_comboLineAngleType.SetCurSel(m_nLineAngleType);

	m_comboCoordSortMethod.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_X)/*_T("按X坐标排序")*/);
	m_comboCoordSortMethod.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_Y)/*_T("按Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_XFIRST_Y)/*_T("先X后Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_YFIRST_X)/*_T("先Y后X坐标排序")*/);
	m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);


	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgPatternCircleSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPatternCircleSearchTool::OnBtnGetTrainImage() 
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


void CDlgPatternCircleSearchTool::OnBtnTrain() 
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


void CDlgPatternCircleSearchTool::OnBtnSetMaskImage() 
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


void CDlgPatternCircleSearchTool::OnChkEnableMask() 
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

void CDlgPatternCircleSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternCircleSearchTrainGuiParam trainGuiParam;
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

void CDlgPatternCircleSearchTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();	
}

void CDlgPatternCircleSearchTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}

void CDlgPatternCircleSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgPatternCircleSearchTool::OnSetfocusEdtAngleHigh() 
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


void CDlgPatternCircleSearchTool::OnChangeEdtCircleCalipernum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nCircleCaliperNum < 2) || (m_nCircleCaliperNum > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCircleSearchTool::OnSetfocusEdtCircleCalipernum() 
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

void CDlgPatternCircleSearchTool::OnSetfocusEdtCircleOutlineNum() 
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

void CDlgPatternCircleSearchTool::OnSetfocusEdtCircleContrastThre() 
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

void CDlgPatternCircleSearchTool::OnRadCircleInwardMode0() 
{
	// TODO: Add your control notification handler code here
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternCircleSearchTool::OnRadCircleInwardMode1() 
{
	// TODO: Add your control notification handler code here
	UpdateTrainData(true);
	UpdateSearchData(true);
}

// void CDlgPatternCircleSearchTool::OnChangeEdtLineCalipernum1() 
// {
// 	// TODO: If this is a RICHEDIT control, the control will not
// 	// send this notification unless you override the CDialog::OnInitDialog()
// 	// function and call CRichEditCtrl().SetEventMask()
// 	// with the ENM_CHANGE flag ORed into the mask.
// 
// 	UpdateData(TRUE);
// 	if ((m_nLineCaliperNum1 < 2) || (m_nLineCaliperNum1 > 100))
// 	{
// 		return;
// 	}
// 	UpdateTrainData(true);
// 	UpdateSearchData(true);
// }

// void CDlgPatternCircleSearchTool::OnSetfocusEdtLineCalipernum1() 
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strOutput);
// 		}
// 	}
// }

// void CDlgPatternCircleSearchTool::OnSetfocusEdtLineContrastThre1() 
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strOutput);
// 		}
// 	}
// }

// void CDlgPatternCircleSearchTool::OnSetfocusEdtLineOutlineNum1() 
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strOutput);
// 		}
// 	}
// }

// void CDlgPatternCircleSearchTool::OnCbnSelchangeComboLineAngleType()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
// }

void CDlgPatternCircleSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgPatternCircleSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgPatternCircleSearchTool::OnSetfocusEdtConfusionThre() 
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



void CDlgPatternCircleSearchTool::OnBnClickedCheckLineMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(TRUE);
}


void CDlgPatternCircleSearchTool::OnBnClickedCheckShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bEnableShowLinePoint = !m_bEnableShowLinePoint;

	CPatternCircleSearchSearchGuiParam searchGuiParam;
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);

	searchGuiParam.m_bEnableShowLinePoint= m_bEnableShowLinePoint;

	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternCircleSearchTool::OnCbnSelchangeComboCoordSortMethod()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nCoordSortMethod = m_comboCoordSortMethod.GetCurSel();
}


void CDlgPatternCircleSearchTool::OnSetfocusEdtCoordSortGap()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_COORD_SORT_GAP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_COORD_SORT_GAP, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_COORD_SORT_GAP, strOutput);
		}
	}
}


void CDlgPatternCircleSearchTool::OnBnClickedBtnResultOffsetSetting()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgCircleResultOffset dlgResultOffsetSetting;
    dlgResultOffsetSetting.m_pDlgParent = m_pDlgParent;
    dlgResultOffsetSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
    dlgResultOffsetSetting.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
    dlgResultOffsetSetting.m_nCircleCenterMoveMode = m_nCircleCenterMoveMode;
    dlgResultOffsetSetting.m_nCircleCenterMoveModeX = m_nCircleCenterMoveModeX;
    dlgResultOffsetSetting.m_nCircleCenterMoveModeY = m_nCircleCenterMoveModeY;
    dlgResultOffsetSetting.m_dCircleCenterMoveLengthX = m_dCircleCenterMoveLengthX;
    dlgResultOffsetSetting.m_dCircleCenterMoveLengthY = m_dCircleCenterMoveLengthY;
    dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMulX = m_dCircleCenterMoveRadiusMulX;
    dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMulY = m_dCircleCenterMoveRadiusMulY;
    dlgResultOffsetSetting.m_dCircleCenterMoveAngle = m_dCircleCenterMoveAngle;
    dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMul = m_dCircleCenterMoveRadiusMul;
    if (dlgResultOffsetSetting.DoModal() == IDOK)
    {
        m_nCircleCenterMoveMode = dlgResultOffsetSetting.m_nCircleCenterMoveMode;
        m_nCircleCenterMoveModeX = dlgResultOffsetSetting.m_nCircleCenterMoveModeX;
        m_nCircleCenterMoveModeY = dlgResultOffsetSetting.m_nCircleCenterMoveModeY;
        m_dCircleCenterMoveLengthX = dlgResultOffsetSetting.m_dCircleCenterMoveLengthX;
        m_dCircleCenterMoveLengthY = dlgResultOffsetSetting.m_dCircleCenterMoveLengthY;
        m_dCircleCenterMoveRadiusMulX = dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMulX;
        m_dCircleCenterMoveRadiusMulY = dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMulY;
        m_dCircleCenterMoveAngle = dlgResultOffsetSetting.m_dCircleCenterMoveAngle;
        m_dCircleCenterMoveRadiusMul = dlgResultOffsetSetting.m_dCircleCenterMoveRadiusMul;
    }
}


void CDlgPatternCircleSearchTool::OnBnClickedChkEnableCircleresultOffset()
{
    // TODO: 在此添加控件通知处理程序代码
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bEnableCircleResultOffset)
    {
        GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BTN_RESULT_OFFSET_SETTING)->EnableWindow(FALSE);
    }
}


void CDlgPatternCircleSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternCircleSearchTool::OnEnSetfocusEditCircleFilterHalfWidth() 
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