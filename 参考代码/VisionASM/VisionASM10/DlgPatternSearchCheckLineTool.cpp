// DlgPatternSearchCheckLineTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternSearchCheckLineTool.h"
#include "afxdialogex.h"

#include "VisionASM.h"
#include <IO.H>

#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"

// CDlgPatternSearchCheckLineTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternSearchCheckLineTool, CDialogEx)

CDlgPatternSearchCheckLineTool::CDlgPatternSearchCheckLineTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternSearchCheckLineTool::IDD, pParent)
	, m_dAcceptThre2(0)
	, m_dConfusionThre2(0)
	, m_bSortByScore1(TRUE)
	, m_bShowCaliper(FALSE)
	, m_bEnableDualEdge(FALSE)
	, m_nImageChannel(FALSE)
{
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_dAcceptThre2 = 0.6;
	m_dConfusionThre2 = 0.8;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;

	m_bEnableCheckRegion1 = TRUE;
	m_bEnableCheckRegion2 = FALSE;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;


	m_nLineCaliperNum1 = 20;

	m_dLineContrastThre1 = 10.0;

	m_lLineFilterHalfWidth1 = 1;

	m_nLineOutLineNum1 = 10;

	m_nLinePolarityMode1 = (int)eLightToDark;

	m_nLineAngleType = 0;

	m_bEnableLineMask = FALSE;
	m_bEnableShowLinePoint = FALSE;
}

CDlgPatternSearchCheckLineTool::~CDlgPatternSearchCheckLineTool()
{
}

void CDlgPatternSearchCheckLineTool::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_EDT_CHECK_ACCEPT_THRE, m_dAcceptThre2);
	DDX_Text(pDX, IDC_EDT_CHECK_CONFUSION_THRE, m_dConfusionThre2);
	DDX_Check(pDX, IDC_CHECK_REGION1_ENABLE, m_bEnableCheckRegion1);
	DDX_Check(pDX, IDC_CHECK_REGION2_ENABLE, m_bEnableCheckRegion2);

	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_comboLineAngleType);
	DDX_Check(pDX, IDC_CHECK_ENABLE_LINE_MASK, m_bEnableLineMask);
	DDX_Control(pDX, IDC_CHECK_ENABLE_LINE_MASK, m_checkLineMask);
	DDX_Check(pDX, IDC_CHECK_SHOW_LINE_POINT, m_bEnableShowLinePoint);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_POINT, m_checkShowLinePoint);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER, m_bShowCaliper);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE, m_bEnableDualEdge);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternSearchCheckLineTool, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternSearchCheckLineTool::OnBnClickedBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternSearchCheckLineTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, &CDlgPatternSearchCheckLineTool::OnBnClickedBtnSetCenter)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgPatternSearchCheckLineTool::OnBnClickedBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgPatternSearchCheckLineTool::OnBnClickedChkEnableMask)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, &CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, &CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboSpeedMode)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgPatternSearchCheckLineTool::OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgPatternSearchCheckLineTool::OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternSearchCheckLineTool::OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgPatternSearchCheckLineTool::OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, &CDlgPatternSearchCheckLineTool::OnSetfocusEdtConfusionThre)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgPatternSearchCheckLineTool::OnBnClickedBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_CHECK_ACCEPT_THRE, &CDlgPatternSearchCheckLineTool::OnEnSetfocusEdtCheckAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CHECK_CONFUSION_THRE, &CDlgPatternSearchCheckLineTool::OnEnSetfocusEdtCheckConfusionThre)
	ON_BN_CLICKED(IDC_CHECK_REGION1_ENABLE, &CDlgPatternSearchCheckLineTool::OnBnClickedCheckRegion1Enable)
	ON_BN_CLICKED(IDC_CHECK_REGION2_ENABLE, &CDlgPatternSearchCheckLineTool::OnBnClickedCheckRegion2Enable)


	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)

	ON_CBN_SELCHANGE(IDC_COMBO_LINE_ANGLE_TYPE, &CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboLineAngleType)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LINE_MASK, &CDlgPatternSearchCheckLineTool::OnBnClickedCheckEnableLineMask)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternSearchCheckLineTool::OnBnClickedCheckShowLinePoint)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER, &CDlgPatternSearchCheckLineTool::OnBnClickedChkShowLineCaliper)
END_MESSAGE_MAP()


// CDlgPatternSearchCheckLineTool 消息处理程序

void CDlgPatternSearchCheckLineTool::UpdateControlDisplay()
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

void CDlgPatternSearchCheckLineTool::UpdateTrainData(bool bValidate)
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
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion1 = m_bEnableCheckRegion1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion2 = m_bEnableCheckRegion2;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;


		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_nLineOutLineNum1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_dLineContrastThre1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_lLineFilterHalfWidth1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_nLinePolarityMode1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_bSortByScore1;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[0] = m_nLineAngleType;

		((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_nLineCaliperNum1;
		((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask1 = m_bEnableLineMask;
		((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0]=m_bShowCaliper;
		((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0] = m_bEnableDualEdge;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	
		m_bEnableCheckRegion1 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion1;
		m_bEnableCheckRegion2 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion2;

		m_nLineOutLineNum1 =  ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];
		m_dLineContrastThre1 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];
		m_lLineFilterHalfWidth1 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];
		m_nLinePolarityMode1 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];
		m_bSortByScore1 = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];
		m_nLineAngleType = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType[0];

		m_nLineCaliperNum1 = ((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];
		m_bEnableLineMask = ((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask1;
		m_bShowCaliper = ((CPatternSearchCheckLineTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];
		m_bEnableDualEdge = ((CPatternSearchCheckLineTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableDualEdge[0];
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}

		if (m_bEnableCheckRegion1)
		{
			GetDlgItem(IDC_CHECK_REGION1_ENABLE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_REGION1_ENABLE)->EnableWindow(FALSE);
		}

		if (m_bEnableCheckRegion2)
		{
			GetDlgItem(IDC_CHECK_REGION2_ENABLE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_REGION2_ENABLE)->EnableWindow(FALSE);
		}

		m_comboAccuracy.SetCurSel(m_nPatAccuracyMode);
		m_comboSpeed.SetCurSel(m_nPatSpeedMode);

		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);

		m_checkLineMask.SetCheck(m_bEnableLineMask);
		m_checkShowLinePoint.SetCheck(m_bEnableShowLinePoint);

		UpdateData(FALSE);
	}
}

void CDlgPatternSearchCheckLineTool::UpdateSearchData(bool bValidate)
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

		((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;
		((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2 = m_dAcceptThre2;
		((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2 = m_dConfusionThre2;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);

		((CPatternSearchCheckLineSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bEnableShowLinePoint;
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre = ((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;
		m_dAcceptThre2	= ((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2;	
		m_dConfusionThre2 = ((CPatternSearchCheckLineSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2;

		m_bEnableShowLinePoint = ((CPatternSearchCheckLineSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;

		UpdateData(FALSE);
	}

}

void CDlgPatternSearchCheckLineTool::UpdateDialogLanguage()
{
	//return;
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

		if(_T("IDD_DLG_PATTERNSEARCH_CHECK_LINE_TOOL") == strDlgID)
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

void CDlgPatternSearchCheckLineTool::CheckDlgDataValidation()
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
	if ((m_dConfusionThre < 0.0) || (m_dConfusionThre > 1.0))
	{
		m_dConfusionThre = 0.8;
	}
	if (m_dAcceptThre > m_dConfusionThre)
	{
		m_dAcceptThre = 0.6;
		m_dConfusionThre = 0.8;
	}

	if ((m_dAcceptThre2 < 0.0) || (m_dAcceptThre2 > 1.0))
	{
		m_dAcceptThre2 = 0.6;
	}
	if ((m_dConfusionThre2 < 0.0) || (m_dConfusionThre2 > 1.0))
	{
		m_dConfusionThre2 = 0.8;
	}
	if (m_dAcceptThre2 > m_dConfusionThre2)
	{
		m_dAcceptThre2 = 0.6;
		m_dConfusionThre2 = 0.8;
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


void CDlgPatternSearchCheckLineTool::OnBnClickedBtnGetTrainImage()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternSearchCheckLineTool::OnBnClickedBtnTrain()
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

		cpImage trainImageCheck1;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage2(trainImageCheck1);
		m_GuiPatternDisplayCheck1.RemoveImage();
		m_GuiPatternDisplayCheck1.SetImage(trainImageCheck1, TRUE);

		cpImage trainImageCheck2;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage3(trainImageCheck2);
		m_GuiPatternDisplayCheck2.RemoveImage();
		m_GuiPatternDisplayCheck2.SetImage(trainImageCheck2, TRUE);

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

void CDlgPatternSearchCheckLineTool::OnBnClickedBtnSetMaskImage()
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

		// 		if (m_bEnableMask)
		// 		{
		// 			m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
		// 		}
	}

}

void CDlgPatternSearchCheckLineTool::OnBnClickedChkEnableMask()
{
	// TODO: 在此添加控件通知处理程序代码

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


BOOL CDlgPatternSearchCheckLineTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_btnSetCenter.SetThemeHelper(&m_themeHelper);

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	m_GuiPatternDisplay.Create(IDC_STC_PATTERN_DISPLAY, this, 0);
	m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	m_GuiPatternDisplayCheck1.Create(IDC_STC_PATTERN_DISPLAY_CHECK1, this, 0);
	m_GuiPatternDisplayCheck1.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	m_GuiPatternDisplayCheck2.Create(IDC_STC_PATTERN_DISPLAY_CHECK2, this, 0);
	m_GuiPatternDisplayCheck2.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));
	m_GuiPatternDisplayCheck2.ShowWindow(SW_HIDE);


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


	UpdateDialogLanguage();

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboAccuracyMode()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();
}


void CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboSpeedMode()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}

void CDlgPatternSearchCheckLineTool::OnBnClickedBtnSetCenter()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternSearchCheckLineTrainGuiParam *pTrainGuiParam = new CPatternSearchCheckLineTrainGuiParam;
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


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtAngleLow()
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


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtAngleHigh()
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


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtSearchNum()
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


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtAcceptThre()
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
void CDlgPatternSearchCheckLineTool::OnEnSetfocusEdtCheckAcceptThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CHECK_ACCEPT_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CHECK_ACCEPT_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CHECK_ACCEPT_THRE, strOutput);
		}
	}
}


void CDlgPatternSearchCheckLineTool::OnEnSetfocusEdtCheckConfusionThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CHECK_CONFUSION_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CHECK_CONFUSION_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CHECK_CONFUSION_THRE, strOutput);
		}
	}
}


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtConfusionThre()
{
	// TODO: 在此添加控件通知处理程序代码

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


void CDlgPatternSearchCheckLineTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternSearchCheckLineTrainGuiParam trainGuiParam;
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


BOOL CDlgPatternSearchCheckLineTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CDlgPatternSearchCheckLineTool::OnBnClickedCheckRegion1Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableCheckRegion1 = !m_bEnableCheckRegion1;

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternSearchCheckLineTool::OnBnClickedCheckRegion2Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableCheckRegion2 = !m_bEnableCheckRegion2;

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternSearchCheckLineTool::OnSetfocusEdtLineCalipernum1() 
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

void CDlgPatternSearchCheckLineTool::OnSetfocusEdtLineContrastThre1() 
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

void CDlgPatternSearchCheckLineTool::OnSetfocusEdtLineOutlineNum1() 
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

void CDlgPatternSearchCheckLineTool::OnChangeEdtLineCalipernum1() 
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


void CDlgPatternSearchCheckLineTool::OnCbnSelchangeComboLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
}


void CDlgPatternSearchCheckLineTool::OnBnClickedCheckEnableLineMask()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bEnableLineMask = !m_bEnableLineMask;

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternSearchCheckLineTool::OnBnClickedCheckShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bEnableShowLinePoint = !m_bEnableShowLinePoint;

	CPatternSearchCheckLineSearchGuiParam searchGuiParam;
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);

	searchGuiParam.m_bEnableShowLinePoint= m_bEnableShowLinePoint;

	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);

	m_pDlgParent->UpdateGuiDisplay();
}



void CDlgPatternSearchCheckLineTool::OnBnClickedChkShowLineCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}

void CDlgPatternSearchCheckLineTool::OnEnSetfocusEditLineFilterHalfWidth1() 
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