// DlgPatternCornerLineSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternCornerLineSearchTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"
// CDlgPatternCornerLineSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternCornerLineSearchTool, CDialogEx)

CDlgPatternCornerLineSearchTool::CDlgPatternCornerLineSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternCornerLineSearchTool::IDD, pParent)
	
	, m_iRDFindCornerLine1(0)
	, m_iRdLinePolarityMode1(0)
	, m_iRdEdgeProperty1(0)
	, m_iEditLineCaliperNum3(0)
	, m_iEditLineOutlineNum3(0)
	, m_iEditLineContrastThre3(0)
	, m_iRdLinePolarityMode3(0)
	, m_iRdSingleEdgeProperty3(0)
	, m_iEditLineCaliperNum1(0)
	, m_iEditLineOutllneNum1(0)
	, m_iEditLineContrastThre1(0)
	, m_bShowCaliper_1(FALSE)
	, m_bShowCaliper_3(FALSE)
	, m_bEnableDualEdge_1(FALSE)
	, m_bEnableDualEdge_3(FALSE)
	, m_lLineFilterHalfWidth1(1)
	, m_lLineFilterHalfWidth3(1)
	, m_nImageChannel(0)
{
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;

	m_nLineAngleType = 0;

	for(int i =0 ;i < 3; i++)
	{
		m_nLineCaliperNum[i] = 20;
		m_dLineContrastThre[i]= 10.0;
		m_lLineFilterHalfWidth[i]= 1;
		m_nLineOutLineNum[i]= 10;
		m_nLinePolarityMode[i]= (int)eLightToDark;
		m_bSortByScore[i]=false;
		m_bEnableLineMask[i]=false;
		m_bEnableShowLinePoint[i]=false;
		m_nLineEdgeProperty[i] = 0;
		m_bShowCaliper[i] = 0;
		m_bEnableDualEdge[i] = false;
	}
}

CDlgPatternCornerLineSearchTool::~CDlgPatternCornerLineSearchTool()
{
}

void CDlgPatternCornerLineSearchTool::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE,m_comboLineAngleType);


	DDX_Radio(pDX, IDC_RD_LINE_CALIPER1, m_iRDFindCornerLine1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_iRdLinePolarityMode1);
	DDX_Radio(pDX, IDC_RD_SINGLE_EDGE_PROPERTY1, m_iRdEdgeProperty1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_iEditLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_iEditLineOutllneNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_iEditLineContrastThre1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_CHECK_ENABLE_LINE_MASK, m_checkEnableLineMask1);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_POINT, m_checkShowLinePoint1);

	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_3, m_iEditLineCaliperNum3);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_3, m_iEditLineOutlineNum3);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_3, m_iEditLineContrastThre3);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_3, m_iRdLinePolarityMode3);
	DDX_Radio(pDX, IDC_RD_SINGLE_EDGE_PROPERTY3, m_iRdSingleEdgeProperty3);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE3, m_checkSortByScore3);
	DDX_Control(pDX, IDC_CHECK_ENABLE_LINE_MASK3, m_checkEnableLineMask3);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_POINT4, m_checkShowLinePoint3);

	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_3, m_bShowCaliper_3);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge_1);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE2, m_bEnableDualEdge_3);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_3, m_lLineFilterHalfWidth3);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternCornerLineSearchTool, CDialogEx)
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
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE_MASK, OnBnClickedCheckEnableLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, OnBnClickedCheckShowLinePoint1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_3, OnChangeEdtLineCalipernum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_3, OnSetfocusEdtLineCalipernum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_3, OnSetfocusEdtLineContrastThre3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_3, OnSetfocusEdtLineOutlineNum3)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE_MASK3, OnBnClickedCheckEnableLineMask3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT4, OnBnClickedCheckShowLinePoint3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_3, OnEnSetfocusEditLineFilterHalfWidth3)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_ANGLE_TYPE,OnCbnSelchangeComboLineAngleType)

	ON_BN_CLICKED(IDC_RD_LINE_CALIPER1, &CDlgPatternCornerLineSearchTool::OnBnClickedRdLineCaliper1)
	ON_BN_CLICKED(IDC_RD_LINE_CALIPER2, &CDlgPatternCornerLineSearchTool::OnBnClickedRdLineCaliper2)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgPatternCornerLineSearchTool::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_3, &CDlgPatternCornerLineSearchTool::OnBnClickedChkShowLineCaliper3)
END_MESSAGE_MAP()


// CDlgPatternCornerLineSearchTool 消息处理程序

BOOL CDlgPatternCornerLineSearchTool::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);

}

void CDlgPatternCornerLineSearchTool::UpdateControlDisplay()
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
void CDlgPatternCornerLineSearchTool::UpdateTrainData(bool bValidate)
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
		UpdateGuiLineParam(TRUE, m_iRDFindCornerLine1);
		UpdateGuiLineParam(TRUE, 2);
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;

		((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType = m_nLineAngleType;

		for (int i = 0; i< 3; i++)
		{
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[i]  = m_nLineOutLineNum[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[i] = m_dLineContrastThre[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[i] = m_lLineFilterHalfWidth[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[i] = (EdgePolarity)m_nLinePolarityMode[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[i] = m_bSortByScore[i];
			((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[i] = m_nLineCaliperNum[i];
			((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[i] = m_bEnableLineMask[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i] = m_nLineEdgeProperty[i];
			((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[i] = m_bShowCaliper[i];
			((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i] = m_bEnableDualEdge[i];
		}

		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

		for(int i = 0; i < 3; i++)
		{
			m_nLineOutLineNum[i]=  ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[i];
			m_dLineContrastThre[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[i];
			m_lLineFilterHalfWidth[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[i];
			m_nLinePolarityMode[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[i];
			m_bSortByScore[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[i];
			m_nLineCaliperNum[i] = ((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[i];
			m_bEnableLineMask[i] = ((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[i];
			m_nLineEdgeProperty[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i];
			m_bShowCaliper[i] = ((CPatternCornerLineSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[i];
			m_bEnableDualEdge[i] = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i];
		}

		m_nLineAngleType = ((CPatternCornerLineSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType;

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
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);



		UpdateData(FALSE);
		UpdateGuiLineParam(FALSE, m_iRDFindCornerLine1);
		UpdateGuiLineParam(FALSE, 2);
	}
	
	
}

void CDlgPatternCornerLineSearchTool::UpdateSearchData(bool bValidate)
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

		((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);

		for(int i = 0 ; i < 3; i++)
		{
			m_bEnableShowFitPoint[i] = ((CPatternCornerLineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint[i];
		}
		//((CPatternCornerLineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bEnableShowLinePoint1;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre = ((CPatternCornerLineSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		//m_bEnableShowLinePoint1 = ((CPatternCornerLineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;

		for(int i = 0 ; i < 3; i++)
		{
			m_bEnableShowFitPoint[i] = ((CPatternCornerLineSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint[i];
		}
		UpdateData(FALSE);
	}
}

void CDlgPatternCornerLineSearchTool::CheckDlgDataValidation()
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
	////
	for(int i = 0; i< 3; i++)
	{
		if (m_nLineCaliperNum[i] < 2)
			m_nLineCaliperNum[i] = 3;
		if (m_nLineCaliperNum[i] > 100)
			m_nLineCaliperNum[i] = 100;

		if ((m_nLineOutLineNum[i] < 0) || (m_nLineOutLineNum[i]  > (m_nLineCaliperNum[i] - 2)))
		{
			m_nLineOutLineNum[i] = 0;
		}


		if ((m_dLineContrastThre[i] < 0.0) || (m_dLineContrastThre[i] > 255.0))
		{
			m_dLineContrastThre[i] = 10;
		}


		if (m_lLineFilterHalfWidth[i] < 1)
		{
			m_lLineFilterHalfWidth[i] = 1;
		}
	}

	UpdateData(FALSE);

}


void CDlgPatternCornerLineSearchTool::UpdateGuiLineParam(bool bUpdate, int iLine)
{
	UpdateData(TRUE);
	if (bUpdate)
	{
		if (iLine != 2)
		{
			m_nLineCaliperNum[iLine] = m_iEditLineCaliperNum1;
			m_dLineContrastThre[iLine] = m_iEditLineContrastThre1;
			m_nLineOutLineNum[iLine] = m_iEditLineOutllneNum1;
			m_nLinePolarityMode[iLine] = m_iRdLinePolarityMode1;
			m_nLineEdgeProperty[iLine] = m_iRdEdgeProperty1;
			m_bSortByScore[iLine] = m_checkSortByScore1.GetCheck(); 
			m_bEnableLineMask[iLine] = m_checkEnableLineMask1.GetCheck();
			m_bEnableShowLinePoint[iLine] = m_checkShowLinePoint1.GetCheck();
			m_bShowCaliper[iLine] = m_bShowCaliper_1;
			m_bEnableDualEdge[iLine] = m_bEnableDualEdge_1;
			m_lLineFilterHalfWidth[iLine] = m_lLineFilterHalfWidth1;
		}
		else
		{
			m_nLineCaliperNum[iLine] = m_iEditLineCaliperNum3;
			m_dLineContrastThre[iLine] = m_iEditLineContrastThre3;
			m_nLineOutLineNum[iLine] = m_iEditLineOutlineNum3;
			m_nLinePolarityMode[iLine] = m_iRdLinePolarityMode3;
			m_nLineEdgeProperty[iLine] = m_iRdSingleEdgeProperty3;
			m_bSortByScore[iLine] = m_checkSortByScore3.GetCheck(); 
			m_bEnableLineMask[iLine] = m_checkEnableLineMask3.GetCheck();
			m_bEnableShowLinePoint[iLine] = m_checkShowLinePoint3.GetCheck();
			m_nLineAngleType = m_comboLineAngleType.GetCurSel();
			m_bShowCaliper[iLine] = m_bShowCaliper_3;
			m_bEnableDualEdge[iLine] = m_bEnableDualEdge_3;
			m_lLineFilterHalfWidth[iLine] = m_lLineFilterHalfWidth3;
		}

	}
	else
	{
		if (iLine != 2)
		{
			m_iEditLineCaliperNum1 = m_nLineCaliperNum[iLine] ;
			m_iEditLineContrastThre1  = m_dLineContrastThre[iLine];
			m_iEditLineOutllneNum1 = m_nLineOutLineNum[iLine] ;
			m_iRdLinePolarityMode1 = m_nLinePolarityMode[iLine];
			m_iRdEdgeProperty1 = m_nLineEdgeProperty[iLine];
			m_checkSortByScore1.SetCheck(m_bSortByScore[iLine]); 
			m_checkEnableLineMask1.SetCheck(m_bEnableLineMask[iLine]);
			m_checkShowLinePoint1.SetCheck(m_bEnableShowLinePoint[iLine] );
			m_bShowCaliper_1 = m_bShowCaliper[iLine];
			m_bEnableDualEdge_1 = m_bEnableDualEdge[iLine];
			m_lLineFilterHalfWidth1 = m_lLineFilterHalfWidth[iLine];
		}
		else 
		{
			m_iEditLineCaliperNum3 = m_nLineCaliperNum[iLine] ;
			m_iEditLineContrastThre3  = m_dLineContrastThre[iLine];
			m_iEditLineOutlineNum3 = m_nLineOutLineNum[iLine] ;
			m_iRdLinePolarityMode3 = m_nLinePolarityMode[iLine];
			m_iRdSingleEdgeProperty3 = m_nLineEdgeProperty[iLine];
			m_checkSortByScore3.SetCheck(m_bSortByScore[iLine]); 
			m_checkEnableLineMask3.SetCheck(m_bEnableLineMask[iLine]);
			m_checkShowLinePoint3.SetCheck(m_bEnableShowLinePoint[iLine] );
			m_comboLineAngleType.SetCurSel(m_nLineAngleType);
			m_bShowCaliper_3 = m_bShowCaliper[iLine];
			m_bEnableDualEdge_3 = m_bEnableDualEdge[iLine];
			m_lLineFilterHalfWidth3 = m_lLineFilterHalfWidth[iLine];
		}
		UpdateData(FALSE);
	}

}

void CDlgPatternCornerLineSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERN_CORNER_LINE_SEARCH_TOOL") == strDlgID)
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


void CDlgPatternCornerLineSearchTool::OnBtnGetTrainImage() 
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

void CDlgPatternCornerLineSearchTool::OnBtnTrain() 
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
		CString strErrInfo=m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}

}

void CDlgPatternCornerLineSearchTool::OnBtnSetMaskImage()
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

void CDlgPatternCornerLineSearchTool::OnChkEnableMask() 
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

BOOL CDlgPatternCornerLineSearchTool::OnInitDialog() 
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

	UpdateGuiLineParam(FALSE, m_iRDFindCornerLine1);
	UpdateGuiLineParam(false, 2);

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

	m_comboLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_ANYANGLE));
	m_comboLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
	m_comboLineAngleType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));
	m_comboLineAngleType.SetCurSel(m_nLineAngleType);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatternCornerLineSearchTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();	
}

void CDlgPatternCornerLineSearchTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtConfusionThre() 
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

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineCalipernum1() 
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




void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineCalipernum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_3, strOutput);
		}
	}
}


void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineContrastThre1() 
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



void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineContrastThre3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_3, strOutput);
		}
	}
}

void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineOutlineNum1() 
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



void CDlgPatternCornerLineSearchTool::OnSetfocusEdtLineOutlineNum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_3, strOutput);
		}
	}
}

void CDlgPatternCornerLineSearchTool::OnChangeEdtLineCalipernum1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE, m_iRDFindCornerLine1);
	if ((m_nLineCaliperNum[m_iRDFindCornerLine1] < 2) || (m_nLineCaliperNum[m_iRDFindCornerLine1] > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}



void CDlgPatternCornerLineSearchTool::OnChangeEdtLineCalipernum3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE,2);
	if ((m_nLineCaliperNum[2] < 2) || (m_nLineCaliperNum[2]  > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternCornerLineSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternCornerLineSearchTrainGuiParam trainGuiParam;
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


void CDlgPatternCornerLineSearchTool::OnCbnSelchangeComboLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
}


void CDlgPatternCornerLineSearchTool::OnBnClickedCheckEnableLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE, m_iRDFindCornerLine1);

	//m_bEnableLineMask[m_iRDFindCornerLine1]= !m_bEnableLineMask[m_iRDFindCornerLine1];

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}




void CDlgPatternCornerLineSearchTool::OnBnClickedCheckEnableLineMask3()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bEnableLineMask[2]= !m_bEnableLineMask[2];

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}



void CDlgPatternCornerLineSearchTool::OnBnClickedCheckShowLinePoint1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE, m_iRDFindCornerLine1);

//	m_bEnableShowLinePoint[m_iRDFindCornerLine1] = !m_bEnableShowLinePoint[m_iRDFindCornerLine1];

	CPatternCornerLineSearchSearchGuiParam searchGuiParam;
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);

	searchGuiParam.m_bEnableShowLinePoint[m_iRDFindCornerLine1]= m_bEnableShowLinePoint[m_iRDFindCornerLine1];

	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternCornerLineSearchTool::OnBnClickedCheckShowLinePoint3()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_bEnableShowLinePoint[2] = !m_bEnableShowLinePoint[2];

	CPatternCornerLineSearchSearchGuiParam searchGuiParam;
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);

	searchGuiParam.m_bEnableShowLinePoint[2]= m_bEnableShowLinePoint[2];

	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);

	m_pDlgParent->UpdateGuiDisplay();
}



void CDlgPatternCornerLineSearchTool::OnBnClickedRdLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateGuiLineParam(true, 1);
	UpdateGuiLineParam(false,0);
	

}


void CDlgPatternCornerLineSearchTool::OnBnClickedRdLineCaliper2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateGuiLineParam(true, 0);
	UpdateGuiLineParam(false,1);

}


void CDlgPatternCornerLineSearchTool::OnBnClickedChkShowLineCaliper1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE, m_iRDFindCornerLine1);
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerLineSearchTool::OnBnClickedChkShowLineCaliper3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateGuiLineParam(TRUE,2);
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerLineSearchTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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

void CDlgPatternCornerLineSearchTool::OnEnSetfocusEditLineFilterHalfWidth3() 
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