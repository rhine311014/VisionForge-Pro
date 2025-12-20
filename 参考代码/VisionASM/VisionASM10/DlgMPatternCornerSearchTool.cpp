// DlgPatternCornerSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgMPatternCornerSearchTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"


// CDlgPatternCornerSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgMPatternCornerSearchTool, CDialogEx)

CDlgMPatternCornerSearchTool::CDlgMPatternCornerSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMPatternCornerSearchTool::IDD, pParent)
	, m_nCliperIndex(0)
	, m_bShowCaliper_1(FALSE)
	, m_nImageChannel(0)
	, m_bEnable24CorLineCombine(FALSE)
	, m_bEnable13ProjectTo24Combine(FALSE)
	, m_bEnableOutput13MidCombine(FALSE)
	, m_bGetFourResultEnable(FALSE)
{
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_nPreCliperIndex = m_nCliperIndex;

	for (int i=0;i<4;i++)
	{
		m_nLineOutLineNum[i]=10;
		m_dLineContrastThre[i]=10.0;
		m_lLineFilterHalfWidth[i]=1;
		m_nLinePolarityMode[i]=1;
		m_nLineCaliperNum[i]=20;

		m_bSortByScore[i] = FALSE;
		m_bEnableDualEdge[i] = FALSE;
		m_bEnableLineMask[i] = FALSE;
		m_bShowFitPoint[i] = FALSE;
		m_bShowCaliper[i] = FALSE;
	}

	m_nLineCaliperNum1 = 20;
	m_dLineContrastThre1 = 10.0;
	m_lLineFilterHalfWidth1 = 1;
	m_nLineOutLineNum1 = 10;
	m_nLinePolarityMode1 = (int)eLightToDark;
	m_bSortByScore1 = FALSE;
	m_bEnableDualEdge1 = FALSE;
	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;


	m_bCoordSortEnable = FALSE;
	m_dCoordSortGap = 64;
	m_nCoordSortMethod = 0;
	m_nCoordSortOrderX = 0;
	m_nCoordSortOrderY = 0;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;

}

CDlgMPatternCornerSearchTool::~CDlgMPatternCornerSearchTool()
{
}

void CDlgMPatternCornerSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_COMBO_SPEED_MODE, m_comboSpeed);
	DDX_Control(pDX, IDC_COMBO_ACCURACY_MODE, m_comboAccuracy);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_CONFUSION_THRE, m_dConfusionThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	DDX_Radio(pDX, IDC_RADIO_CALIPER_1, m_nCliperIndex);

	DDX_Check(pDX, IDC_CHK_ENABLE_MASK2, m_bCoordSortEnable);
	DDX_Text(pDX, IDC_EDT_COORD_SORT_GAP2, m_dCoordSortGap);
	DDX_Control(pDX, IDC_COMBO_COORD_SORT_METHOD, m_comboCoordSortMethod);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_X_DE, m_nCoordSortOrderX);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_Y_DE, m_nCoordSortOrderY);

	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);

	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER, m_bShowCaliper_1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
	DDX_Check(pDX, IDC_CHK_24_COR_LINE_COMBINE_P, m_bEnable24CorLineCombine);
	DDX_Check(pDX, IDC_CHK_13_PROJECT_TO_24_COMBINE_P, m_bEnable13ProjectTo24Combine);
	DDX_Check(pDX, IDC_CHK_OUTPUT_13_MID_P, m_bEnableOutput13MidCombine);
	DDX_Check(pDX, IDC_CHK_GET_FOUR_RESULT_P, m_bGetFourResultEnable);
}


BEGIN_MESSAGE_MAP(CDlgMPatternCornerSearchTool, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, OnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)
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
	
	ON_BN_CLICKED(IDC_RADIO_CALIPER_1, OnCaliperChange)
	ON_BN_CLICKED(IDC_RADIO_CALIPER_2, OnCaliperChange)
	ON_BN_CLICKED(IDC_RADIO_CALIPER_3, OnCaliperChange)
	ON_BN_CLICKED(IDC_RADIO_CALIPER_4, OnCaliperChange)
	ON_CBN_SELCHANGE(IDC_COMBO_COORD_SORT_METHOD, OnCbnSelchangeComboCoordSortMethod)
	ON_EN_SETFOCUS(IDC_EDT_COORD_SORT_GAP2, OnEnSetfocusEdtCoordSortGap2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE1, &CDlgMPatternCornerSearchTool::OnBnClickedCheckSortByScore1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgMPatternCornerSearchTool::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgMPatternCornerSearchTool::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHK_ENABLE_DUAL_EDGE, &CDlgMPatternCornerSearchTool::OnBnClickedChkEnableDualEdge)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER, &CDlgMPatternCornerSearchTool::OnBnClickedChkShowLineCaliper)
	ON_BN_CLICKED(IDC_CHK_OUTPUT_13_MID_P, &CDlgMPatternCornerSearchTool::OnBnClickedChkOutput13Mid)
	ON_BN_CLICKED(IDC_CHK_24_COR_LINE_COMBINE_P, &CDlgMPatternCornerSearchTool::OnBnClickedChk24CorLineCombine)

	ON_BN_CLICKED(IDC_CHK_GET_FOUR_RESULT_P, &CDlgMPatternCornerSearchTool::OnBnClickedChkGetFourResult2)
END_MESSAGE_MAP()


// CDlgPatternCornerSearchTool 消息处理程序

void CDlgMPatternCornerSearchTool::UpdateControlDisplay()
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

void CDlgMPatternCornerSearchTool::UpdateTrainData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	CMPatternCornerSearchTrainDataParam* pTrainDataParam = (CMPatternCornerSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam;
	CMPatternCornerSearchTrainGuiParam* pTrainGuiParam = (CMPatternCornerSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam;
	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		pTrainDataParam->m_nImageChannel = m_nImageChannel;
		pTrainDataParam->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		pTrainDataParam->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		pTrainDataParam->m_bMaskEnable = m_bEnableMask;
		pTrainDataParam->m_dAngleLow  =  m_dAngleLow;
		pTrainDataParam->m_dAngleHigh = m_dAngleHigh;

		m_nLineOutLineNum[m_nCliperIndex]  = m_nLineOutLineNum1;
		m_dLineContrastThre[m_nCliperIndex] = m_dLineContrastThre1;
		m_nLinePolarityMode[m_nCliperIndex] = (EdgePolarity)m_nLinePolarityMode1;
		m_nLineCaliperNum[m_nCliperIndex] = m_nLineCaliperNum1;
		m_lLineFilterHalfWidth[m_nCliperIndex] = m_lLineFilterHalfWidth1;

		m_bSortByScore[m_nCliperIndex] = m_bSortByScore1;
		m_bEnableDualEdge[m_nCliperIndex] = m_bEnableDualEdge1;
		m_bEnableLineMask[m_nCliperIndex] = m_bEnableLineMask_1;
		m_bShowCaliper[m_nCliperIndex] = m_bShowCaliper_1;
		for (int i = 0; i < FIND_LINE_TOOL_NUM; ++i)
		{
			pTrainDataParam->m_nOutLineNum[i]  = m_nLineOutLineNum[i];
			pTrainDataParam->m_dContrastThre[i] = m_dLineContrastThre[i];
			pTrainDataParam->m_lFilterHalfWidth[i] = m_lLineFilterHalfWidth[i];
			pTrainDataParam->m_edgeProperty[i] = (EdgePolarity)m_nLinePolarityMode[i];
			pTrainDataParam->m_bSortByScore[i] = m_bSortByScore[i];
			pTrainDataParam->m_bEnableDualEdge[i] = m_bEnableDualEdge[i];

			pTrainGuiParam->m_nCaliperNum[i] = m_nLineCaliperNum[i];
			pTrainGuiParam->m_bEnableLineMask[i] = m_bEnableLineMask[i];
			pTrainGuiParam->m_bCaliperVisible[i] = m_bShowCaliper[i];
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = pTrainDataParam->m_nImageChannel;
		m_nPatAccuracyMode = pTrainDataParam->m_Accuracy/2;
		m_nPatSpeedMode = pTrainDataParam->m_CoarseScale-1;
		m_dAngleLow	    = pTrainDataParam->m_dAngleLow;
		m_dAngleHigh	= pTrainDataParam->m_dAngleHigh;
		m_bEnableMask = pTrainDataParam->m_bMaskEnable;	

		for (int i = 0; i < FIND_LINE_TOOL_NUM; ++i)
		{
			m_nLineOutLineNum[i] =  pTrainDataParam->m_nOutLineNum[i];
			m_dLineContrastThre[i] = pTrainDataParam->m_dContrastThre[i];
			m_lLineFilterHalfWidth[i] = pTrainDataParam->m_lFilterHalfWidth[i];
			m_nLinePolarityMode[i] = pTrainDataParam->m_edgeProperty[i];
			m_bSortByScore[i] = pTrainDataParam->m_bSortByScore[i];
			m_bEnableDualEdge[i] = pTrainDataParam->m_bEnableDualEdge[i];

			m_nLineCaliperNum[i] = pTrainGuiParam->m_nCaliperNum[i];
			m_bEnableLineMask[i] = pTrainGuiParam->m_bEnableLineMask[i];
			m_bShowCaliper[i] = pTrainGuiParam->m_bCaliperVisible[i];
		}

		m_nLineOutLineNum1= m_nLineOutLineNum[m_nCliperIndex];
		m_dLineContrastThre1=m_dLineContrastThre[m_nCliperIndex] ;
		m_nLinePolarityMode1= m_nLinePolarityMode[m_nCliperIndex];
		m_nLineCaliperNum1= m_nLineCaliperNum[m_nCliperIndex];
		m_lLineFilterHalfWidth1 = m_lLineFilterHalfWidth[m_nCliperIndex];

		m_bSortByScore1= m_bSortByScore[m_nCliperIndex];
		m_bEnableDualEdge1= m_bEnableDualEdge[m_nCliperIndex];
		m_bEnableLineMask_1= m_bEnableLineMask[m_nCliperIndex];
		m_bShowCaliper_1 = m_bShowCaliper[m_nCliperIndex];

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

		UpdateData(FALSE);
	}

}

void CDlgMPatternCornerSearchTool::UpdateSearchData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	CMPatternCornerSearchSearchDataParam* pSearchDataParam = (CMPatternCornerSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam;
	CMPatternCornerSearchSearchGuiParam* pSearchGuiParam = (CMPatternCornerSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam;
	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);

		pSearchDataParam->m_nSearchNum = m_nSearchNum;
		pSearchDataParam->m_dAcceptThre = m_dAcceptThre;
		pSearchDataParam->m_dConfusionThre = m_dConfusionThre;
		pSearchDataParam->m_bCoordSortEnable = m_bCoordSortEnable;
		pSearchDataParam->m_nCoordSortMethod = (CoordSortMethod)m_nCoordSortMethod;
		pSearchDataParam->m_dCoordSortGap = m_dCoordSortGap;
		pSearchDataParam->m_nCoordSortOrderX = (SortOrder)m_nCoordSortOrderX;
		pSearchDataParam->m_nCoordSortOrderY =  (SortOrder)m_nCoordSortOrderY;

		pSearchDataParam->m_bGetFourResultEnable = m_bGetFourResultEnable;
		pSearchDataParam->m_bEnable24CorLineCombine = m_bEnable24CorLineCombine;
		pSearchDataParam->m_bEnable13ProjectTo24Combine = m_bEnable13ProjectTo24Combine;
		pSearchDataParam->m_bEnableOutput13MidCombine = m_bEnableOutput13MidCombine;

		m_bShowFitPoint[m_nCliperIndex] = m_bShowFitPoint_1;

		pSearchGuiParam->m_bEnableShowLinePoint = m_bShowFitPoint[0];
		pSearchGuiParam->m_bEnableShowLinePoint1 = m_bShowFitPoint[1];
		pSearchGuiParam->m_bEnableShowLinePoint2 = m_bShowFitPoint[2];
		pSearchGuiParam->m_bEnableShowLinePoint3 = m_bShowFitPoint[3];

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		m_nSearchNum	= pSearchDataParam->m_nSearchNum;
		m_dAcceptThre	= pSearchDataParam->m_dAcceptThre;	
		m_dConfusionThre = pSearchDataParam->m_dConfusionThre;

		m_bCoordSortEnable = pSearchDataParam->m_bCoordSortEnable;
		m_nCoordSortMethod = pSearchDataParam->m_nCoordSortMethod;
		m_dCoordSortGap = pSearchDataParam->m_dCoordSortGap;
		m_nCoordSortOrderX = pSearchDataParam->m_nCoordSortOrderX;
		m_nCoordSortOrderY = pSearchDataParam->m_nCoordSortOrderY;

		m_bGetFourResultEnable = pSearchDataParam->m_bGetFourResultEnable;
		m_bEnable24CorLineCombine = pSearchDataParam->m_bEnable24CorLineCombine;
		m_bEnable13ProjectTo24Combine = pSearchDataParam->m_bEnable13ProjectTo24Combine;
		m_bEnableOutput13MidCombine = pSearchDataParam->m_bEnableOutput13MidCombine;

		m_bShowFitPoint[0] = pSearchGuiParam->m_bEnableShowLinePoint;
		m_bShowFitPoint[1] = pSearchGuiParam->m_bEnableShowLinePoint1;
		m_bShowFitPoint[2] = pSearchGuiParam->m_bEnableShowLinePoint2;
		m_bShowFitPoint[3] = pSearchGuiParam->m_bEnableShowLinePoint3;

		m_bShowFitPoint_1= m_bShowFitPoint[m_nCliperIndex];

		m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);
		UpdateData(FALSE);
	}

}

void CDlgMPatternCornerSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_MPATTERNCORNERSEARCH_TOOL") == strDlgID)
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

void CDlgMPatternCornerSearchTool::CheckDlgDataValidation()
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

	UpdateData(FALSE);

}

void CDlgMPatternCornerSearchTool::OnBtnGetTrainImage() 
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

void CDlgMPatternCornerSearchTool::OnBtnTrain() 
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

void CDlgMPatternCornerSearchTool::OnBtnSetMaskImage()
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
	}

}

void CDlgMPatternCornerSearchTool::OnChkEnableMask() 
{

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

BOOL CDlgMPatternCornerSearchTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
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

	m_comboCoordSortMethod.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_X)/*_T("按X坐标排序")*/);
	m_comboCoordSortMethod.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_Y)/*_T("按Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_XFIRST_Y)/*_T("先X后Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_YFIRST_X)/*_T("先Y后X坐标排序")*/);
	m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMPatternCornerSearchTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();	
}

void CDlgMPatternCornerSearchTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}

void CDlgMPatternCornerSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtConfusionThre() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtLineCalipernum1() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtLineContrastThre1() 
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

void CDlgMPatternCornerSearchTool::OnSetfocusEdtLineOutlineNum1() 
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

void CDlgMPatternCornerSearchTool::OnChangeEdtLineCalipernum1() 
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


void CDlgMPatternCornerSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CMPatternCornerSearchTrainGuiParam trainGuiParam;
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

BOOL CDlgMPatternCornerSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMPatternCornerSearchTool::OnCaliperChange()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);		
	m_nLineOutLineNum[m_nPreCliperIndex]  = m_nLineOutLineNum1;
	m_dLineContrastThre[m_nPreCliperIndex] = m_dLineContrastThre1;
	m_nLinePolarityMode[m_nPreCliperIndex] = (EdgePolarity)m_nLinePolarityMode1;
	m_nLineCaliperNum[m_nPreCliperIndex] = m_nLineCaliperNum1;
	m_bSortByScore[m_nPreCliperIndex] = m_bSortByScore1;
	m_bEnableDualEdge[m_nPreCliperIndex] = m_bEnableDualEdge1;
	m_bEnableLineMask[m_nPreCliperIndex] = m_bEnableLineMask_1;
	m_bShowFitPoint[m_nPreCliperIndex] = m_bShowFitPoint_1;
	m_bShowCaliper[m_nPreCliperIndex] = m_bShowCaliper_1;
	m_lLineFilterHalfWidth[m_nPreCliperIndex] = m_lLineFilterHalfWidth1;

	m_nLineOutLineNum1= m_nLineOutLineNum[m_nCliperIndex];
	m_dLineContrastThre1=m_dLineContrastThre[m_nCliperIndex] ;
	m_nLinePolarityMode1= m_nLinePolarityMode[m_nCliperIndex];
	m_nLineCaliperNum1= m_nLineCaliperNum[m_nCliperIndex];
	m_bSortByScore1= m_bSortByScore[m_nCliperIndex];
	m_bEnableDualEdge1= m_bEnableDualEdge[m_nCliperIndex];
	m_bEnableLineMask_1= m_bEnableLineMask[m_nCliperIndex];
	m_bShowFitPoint_1= m_bShowFitPoint[m_nCliperIndex];
	m_bShowCaliper_1= m_bShowCaliper[m_nCliperIndex];
	m_lLineFilterHalfWidth1 = m_lLineFilterHalfWidth[m_nCliperIndex];
	UpdateData(FALSE);

	m_nPreCliperIndex = m_nCliperIndex;
}
void CDlgMPatternCornerSearchTool::OnCbnSelchangeComboCoordSortMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCoordSortMethod = m_comboCoordSortMethod.GetCurSel();
}

void CDlgMPatternCornerSearchTool::OnEnSetfocusEdtCoordSortGap2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_COORD_SORT_GAP2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_COORD_SORT_GAP2, strOutput);
		}
	}
}

void CDlgMPatternCornerSearchTool::OnBnClickedCheckSortByScore1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgMPatternCornerSearchTool::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgMPatternCornerSearchTool::OnBnClickedCheckShowFitpoint1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(true);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgMPatternCornerSearchTool::OnBnClickedChkEnableDualEdge()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgMPatternCornerSearchTool::OnBnClickedChkShowLineCaliper()
{
	UpdateTrainData(TRUE);
}

void CDlgMPatternCornerSearchTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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

void CDlgMPatternCornerSearchTool::OnBnClickedChkGetFourResult2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_bGetFourResultEnable)
	{
		m_bEnable24CorLineCombine  = FALSE;
		m_bEnableOutput13MidCombine  = FALSE;
		UpdateData(FALSE);
	}
}

void CDlgMPatternCornerSearchTool::OnBnClickedChkOutput13Mid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_bEnableOutput13MidCombine)
	{
		m_bEnable24CorLineCombine = FALSE;
		m_bGetFourResultEnable  = FALSE;
		UpdateData(FALSE);
	}
}


void CDlgMPatternCornerSearchTool::OnBnClickedChk24CorLineCombine()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_bEnable24CorLineCombine)
	{
		m_bEnableOutput13MidCombine = FALSE;
		m_bGetFourResultEnable  = FALSE;
		UpdateData(FALSE);
	}
}
