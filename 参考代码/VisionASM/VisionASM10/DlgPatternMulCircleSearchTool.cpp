// DlgPatternMulCircleSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternMulCircleSearchTool.h"
#include "afxdialogex.h"


#include "VisionASM.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>

// CDlgPatternMulCircleSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternMulCircleSearchTool, CDialog)

	CDlgPatternMulCircleSearchTool::CDlgPatternMulCircleSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternMulCircleSearchTool::IDD, pParent)
	, m_bShowCircleCaliper(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_bEnableDualEdge(FALSE)
	, m_nImageChannel(0)
{

	m_iAccracy = 1;
	m_iCoarseScale = (int)CoarseScale8-1;
	m_dAngleLow = -5;
	m_dAngleHigh = 5;

	m_bEnableMask = FALSE;

	m_nCircleIndex = 0;
	m_nCircleCaliperNum = 20;
	m_nCircleOutLineNum = 10;
	m_dCircleContrastThre = 10.0;
	m_nCirclePolarityMode = 0;
	m_nCircleInwardMode = 0;
	m_bCircleSortByScore = FALSE;
	m_bEnableShowCirclePoint = FALSE;
	m_lCircleFilterHalfWidth = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_nSearchNum = 1;


	m_nPreCircleIndex = m_nCircleIndex;

	for (int i = 0; i < DLG_PATTERN_CIRCLE_NUM; i++)
	{		
		m_nTempCircleCaliperNum[i] = 20;
		m_nTempCircleOutLineNum[i] = 10;
		m_dTempCircleContrastThre[i] = 10;
		m_nTempCirclePolarityMode[i] = 0;
		m_nTempCircleInwardMode[i] = 0;
		m_lTempCircleFilterHalfWidth[i] = 1;
		m_bTempCircleSortByScore[i] = FALSE;
		m_bTempEnableShowCirclePoint[i] = FALSE;
		m_bTempShowCircleCaliper[i]	= FALSE;
		m_bTempEnableCircleMask[i] = FALSE;
		m_bTempEnableDualEdge[i] = FALSE;
	}
	m_nLineAngleType = 0;
}

CDlgPatternMulCircleSearchTool::~CDlgPatternMulCircleSearchTool()
{
}

void CDlgPatternMulCircleSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);

	DDX_Control(pDX, IDC_CMB_ACCURACY, m_comboAccuracy);
	DDX_CBIndex(pDX, IDC_CMB_ACCURACY, m_iAccracy);

	DDX_Control(pDX, IDC_CMB_COARSESCALE, m_comboSpeed);
	DDX_CBIndex(pDX, IDC_CMB_COARSESCALE, m_iCoarseScale);

	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);


	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);

	DDX_Radio(pDX, IDC_RADIO_CIRCLE_INDEX1, m_nCircleIndex);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM, m_nCircleOutLineNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bCircleSortByScore);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkCircleSortByScore);
	DDX_Check(pDX, IDC_CHECK_SHOW_LINE_POINT, m_bEnableShowCirclePoint);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_POINT, m_checkShowCirclePoint);

	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_comboLineAngleType);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_nLineAngleType);

	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Text(pDX, IDC_EDT_CONFUSION_THRE, m_dConfusionThre);


	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK, m_bEnableCircleMask);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternMulCircleSearchTool, CDialog)

// 	ON_EN_SETFOCUS(IDC_EDT_GRAU, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtGrau)
// 	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtNoisethre)
// 	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtAngleHigh)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgPatternMulCircleSearchTool::OnBnClickedChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgPatternMulCircleSearchTool::OnBnClickedBtnSetMaskImage)

	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternMulCircleSearchTool::OnEnChangeEdtCircleCalipernum)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE0, &CDlgPatternMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE1, &CDlgPatternMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE2, &CDlgPatternMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgPatternMulCircleSearchTool::OnBnClickedRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgPatternMulCircleSearchTool::OnBnClickedRadCircleInwardMode0)

	ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE1, &CDlgPatternMulCircleSearchTool::OnBnClickedCheckSortByScore1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternMulCircleSearchTool::OnBnClickedCheckShowLinePoint)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_INDEX1, &CDlgPatternMulCircleSearchTool::OnBnClickedRadioCircleIndex1)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_INDEX2, &CDlgPatternMulCircleSearchTool::OnBnClickedRadioCircleIndex1)

	ON_CBN_SELCHANGE(IDC_COMBO_LINE_ANGLE_TYPE, &CDlgPatternMulCircleSearchTool::OnCbnSelchangeComboLineAngleType)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternMulCircleSearchTool::OnBnClickedBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternMulCircleSearchTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgPatternMulCircleSearchTool::OnBnClickedBtnFullImageSearch)

	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternMulCircleSearchTool::OnSetfocusEdtSearchConfusionThre)



	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, &CDlgPatternMulCircleSearchTool::OnEnSetfocusEdtConfusionThre)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgPatternMulCircleSearchTool::OnBnClickedChkShowCircleCaliper)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK, &CDlgPatternMulCircleSearchTool::OnBnClickedCheckLineMask)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1,OnEnSetfocusEditCircleFilterHalfWidth)
END_MESSAGE_MAP()


// CDlgPatternMulCircleSearchTool 消息处理程序

void CDlgPatternMulCircleSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNMULCIRCLESEARCH_TOOL") == strDlgID)
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


void CDlgPatternMulCircleSearchTool::UpdateControlDisplay()
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

void CDlgPatternMulCircleSearchTool::UpdateTrainData(bool bValidate)
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
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_iAccracy);
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_iCoarseScale+1);
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow = m_dAngleLow;
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask;
		((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType = m_nLineAngleType;

		m_nTempCircleCaliperNum[m_nCircleIndex]			= m_nCircleCaliperNum;
		m_nTempCircleOutLineNum[m_nCircleIndex]			= m_nCircleOutLineNum;
		m_dTempCircleContrastThre[m_nCircleIndex]		= m_dCircleContrastThre;
		m_nTempCirclePolarityMode[m_nCircleIndex]		= m_nCirclePolarityMode;
		m_nTempCircleInwardMode[m_nCircleIndex]			= m_nCircleInwardMode;
		m_bTempCircleSortByScore[m_nCircleIndex]		= m_bCircleSortByScore;
		m_bTempShowCircleCaliper[m_nCircleIndex]		= m_bShowCircleCaliper;
		m_bTempEnableCircleMask[m_nCircleIndex]			= m_bEnableCircleMask;
		m_bTempEnableDualEdge[m_nCircleIndex]			= m_bEnableDualEdge;
		m_lTempCircleFilterHalfWidth[m_nCircleIndex]	= m_lCircleFilterHalfWidth;
		for (int i = 0; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[i]		= m_nTempCircleCaliperNum[i];
			((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)m_nTempCircleInwardMode[i];
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[i]= m_lTempCircleFilterHalfWidth[i];
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[i]		= m_nTempCircleOutLineNum[i];
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[i]	= m_dTempCircleContrastThre[i];
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[i]	= (EdgePolarity)m_nTempCirclePolarityMode[i];				
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[i]	= m_bTempCircleSortByScore[i];
			((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible[i]		= m_bTempShowCircleCaliper[i];
			((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask[i]			= m_bTempEnableCircleMask[i];
			((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i]			= m_bTempEnableDualEdge[i];
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;

	}
	else // 设置界面值
	{		
		m_nImageChannel			= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_iAccracy			= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_iCoarseScale	= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow		= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh		= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask	= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
		m_nLineAngleType = ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType;

		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}


		for (int i = 0;i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			m_nTempCircleCaliperNum[i] = ((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[i];
			m_nTempCircleInwardMode[i] = ((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[i];
			m_lTempCircleFilterHalfWidth[i] = ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth[i];

			m_nTempCircleOutLineNum[i]		= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[i];
			m_dTempCircleContrastThre[i]	= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[i];
			m_nTempCirclePolarityMode[i]	= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[i];				
			m_bTempCircleSortByScore[i]		= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[i];
			m_bTempShowCircleCaliper[i]		= ((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible[i];
			m_bTempEnableCircleMask[i]		= ((CPatternMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableCircleMask[i];
			m_bTempEnableDualEdge[i]		= ((CPatternMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[i];
		}

		m_nCircleCaliperNum		= m_nTempCircleCaliperNum[m_nCircleIndex];
		m_nCircleInwardMode		= m_nTempCircleInwardMode[m_nCircleIndex];
		m_lCircleFilterHalfWidth = m_lTempCircleFilterHalfWidth[m_nCircleIndex];
		m_nCircleOutLineNum		= m_nTempCircleOutLineNum[m_nCircleIndex];
		m_dCircleContrastThre	= m_dTempCircleContrastThre[m_nCircleIndex];
		m_nCirclePolarityMode	= m_nTempCirclePolarityMode[m_nCircleIndex];
		m_nCirclePolarityMode	= m_nTempCirclePolarityMode[m_nCircleIndex];
		m_bCircleSortByScore    = m_bTempCircleSortByScore[m_nCircleIndex];
		m_bShowCircleCaliper	= m_bTempShowCircleCaliper[m_nCircleIndex];
		m_bEnableCircleMask		= m_bTempEnableCircleMask[m_nCircleIndex];
		m_bEnableDualEdge		= m_bTempEnableDualEdge[m_nCircleIndex];

		m_checkCircleSortByScore.SetCheck(m_bCircleSortByScore);
		m_checkShowCirclePoint.SetCheck(m_bEnableShowCirclePoint);
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);
		m_comboAccuracy.SetCurSel(m_iAccracy);
		m_comboSpeed.SetCurSel(m_iCoarseScale);
		UpdateData(FALSE);
	}
}

void CDlgPatternMulCircleSearchTool::UpdateSearchData(bool bValidate)
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

		((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum		= m_nSearchNum;
		((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre	= m_dAcceptThre;
		((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre	= m_dConfusionThre;



		m_bTempEnableShowCirclePoint[m_nCircleIndex] = m_bEnableShowCirclePoint;

		for (int i = 0 ; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			((CPatternMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[i] = m_bTempEnableShowCirclePoint[i];
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dConfusionThre = ((CPatternMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		for (int i = 0; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			m_bTempEnableShowCirclePoint[i] = ((CPatternMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[i];
		}

		m_bEnableShowCirclePoint = m_bTempEnableShowCirclePoint[m_nCircleIndex];

		UpdateData(FALSE);
	}

}


void CDlgPatternMulCircleSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	
	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
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

	// 	if (m_lCircleFilterHalfWidth < 1)
	// 	{
	// 		m_lCircleFilterHalfWidth = 1;
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
		m_dAcceptThre = 0.8;
	}




	UpdateData(FALSE);
}


BOOL CDlgPatternMulCircleSearchTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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

	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);

	m_comboAccuracy.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_comboAccuracy.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_comboAccuracy.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_comboAccuracy.SetCurSel(m_iAccracy);

	m_comboSpeed.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_comboSpeed.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_comboSpeed.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed.SetCurSel(m_iCoarseScale);




	m_comboLineAngleType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_ANYANGLE));
	m_comboLineAngleType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_HOR));
	m_comboLineAngleType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_ANGLE_TYPE_VER));
	m_comboLineAngleType.SetCurSel(m_nLineAngleType);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPatternMulCircleSearchTool::OnBnClickedChkEnableMask()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternMulCircleSearchTool::OnBnClickedBtnSetMaskImage()
{
	// TODO: 在此添加控件通知处理程序代码

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

void CDlgPatternMulCircleSearchTool::OnBnClickedRadioCircleIndex1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);		

	m_nTempCircleOutLineNum[m_nPreCircleIndex]		= m_nCircleOutLineNum;
	m_dTempCircleContrastThre[m_nPreCircleIndex]	= m_dCircleContrastThre;
	m_nTempCirclePolarityMode[m_nPreCircleIndex]	= (EdgePolarity)m_nCirclePolarityMode;
	m_nTempCircleInwardMode[m_nPreCircleIndex]		= m_nCircleInwardMode;
	m_bTempCircleSortByScore[m_nPreCircleIndex]		= m_bCircleSortByScore;
	m_lTempCircleFilterHalfWidth[m_nPreCircleIndex] = m_lCircleFilterHalfWidth;
	m_nTempCircleCaliperNum[m_nPreCircleIndex]		= m_nCircleCaliperNum;
	m_bTempEnableShowCirclePoint[m_nPreCircleIndex] = m_bEnableShowCirclePoint;
	m_bTempShowCircleCaliper[m_nPreCircleIndex]		= m_bShowCircleCaliper;
	m_bTempEnableCircleMask[m_nPreCircleIndex]		= m_bEnableCircleMask;
	m_bTempEnableDualEdge[m_nPreCircleIndex]		= m_bEnableDualEdge;

	m_nCircleOutLineNum			= m_nTempCircleOutLineNum[m_nCircleIndex];
	m_dCircleContrastThre		= m_dTempCircleContrastThre[m_nCircleIndex];
	m_nCirclePolarityMode		= m_nTempCirclePolarityMode[m_nCircleIndex];
	m_nCircleInwardMode			= m_nTempCircleInwardMode[m_nCircleIndex];
	m_bCircleSortByScore		= m_bTempCircleSortByScore[m_nCircleIndex];
	m_lCircleFilterHalfWidth	= m_lTempCircleFilterHalfWidth[m_nCircleIndex];
	m_nCircleCaliperNum			= m_nTempCircleCaliperNum[m_nCircleIndex];
	m_bEnableShowCirclePoint	= m_bTempEnableShowCirclePoint[m_nCircleIndex];
	m_bShowCircleCaliper		= m_bTempShowCircleCaliper[m_nCircleIndex];
	m_bEnableCircleMask			= m_bTempEnableCircleMask[m_nCircleIndex];
	m_bEnableDualEdge			= m_bTempEnableDualEdge[m_nCircleIndex];
	m_nPreCircleIndex = m_nCircleIndex;

	UpdateData(FALSE);



}


void CDlgPatternMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(true);
	//	UpdateSearchData(true);
}


void CDlgPatternMulCircleSearchTool::OnBnClickedRadCircleInwardMode0()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(true);
	//	UpdateSearchData(true);
}


void CDlgPatternMulCircleSearchTool::OnBnClickedCheckSortByScore1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternMulCircleSearchTool::OnBnClickedCheckShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(true);
	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgPatternMulCircleSearchTool::OnCbnSelchangeComboLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
}

void CDlgPatternMulCircleSearchTool::OnBnClickedBtnGetTrainImage()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternMulCircleSearchTool::OnBnClickedBtnTrain()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternMulCircleSearchTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternMulCircleSearchTrainGuiParam trainGuiParam;
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

// void CDlgPatternMulCircleSearchTool::OnSetfocusEdtGrau()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_GRAU)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_GRAU, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_GRAU, strOutput);
// 		}
// 	}
// }


// void CDlgPatternMulCircleSearchTool::OnSetfocusEdtNoisethre()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_NOISETHRE)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_NOISETHRE, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_NOISETHRE, strOutput);
// 		}
// 	}
// }


// void CDlgPatternMulCircleSearchTool::OnSetfocusEdtTotalMag()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_TOTAL_MAG)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_TOTAL_MAG, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_TOTAL_MAG, strOutput);
// 		}
// 	}
// }


void CDlgPatternMulCircleSearchTool::OnEnChangeEdtCircleCalipernum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if ((m_nCircleCaliperNum < 2) || (m_nCircleCaliperNum > 1000))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleCalipernum()
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


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleOutlineNum()
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


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtCircleContrastThre()
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


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtAcceptThre()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtSearchNum()
{
	// TODO: 在此添加控件通知处理程序代码

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

void CDlgPatternMulCircleSearchTool::OnSetfocusEdtSearchConfusionThre()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CONFUSION_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CONFUSION_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CONFUSION_THRE, strOutput);
		}
	}
}


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtAngleLow()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternMulCircleSearchTool::OnSetfocusEdtAngleHigh()
{
	// TODO: 在此添加控件通知处理程序代码

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



void CDlgPatternMulCircleSearchTool::OnEnSetfocusEdtConfusionThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CONFUSION_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CONFUSION_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CONFUSION_THRE, strOutput);
		}
	}
}


void CDlgPatternMulCircleSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternMulCircleSearchTool::OnBnClickedCheckLineMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


BOOL CDlgPatternMulCircleSearchTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgPatternMulCircleSearchTool::OnEnSetfocusEditCircleFilterHalfWidth() 
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