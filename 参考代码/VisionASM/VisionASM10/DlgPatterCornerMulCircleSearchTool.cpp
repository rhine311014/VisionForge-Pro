// DlgPatternCornerMulCircleSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatterCornerMulCircleSearchTool.h"
#include "afxdialogex.h"


#include "VisionASM.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>

// CDlgPatternCornerMulCircleSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternCornerMulCircleSearchTool, CDialog)

	CDlgPatternCornerMulCircleSearchTool::CDlgPatternCornerMulCircleSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternCornerMulCircleSearchTool::IDD, pParent)
	, m_bShowLineCaliper(FALSE)
	, m_bShowCircleCaliper(FALSE)
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

	m_nLineIndex = 0;
	m_nLineCaliperNum = 20;
	m_nLineOutLineNum = 10;
	m_dLineContrastThre = 10.0;
	m_nLinePolarityMode = 0;
	m_bLineSortByScore = FALSE;
	m_bEnableShowLinePoint = FALSE;
	m_bLineEnableMask = FALSE;

	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_nSearchNum = 1;


	m_nPreLineIndex = m_nLineIndex;

	for (int i = 0; i < DLG_PATTERN_LINE_NUM; i++)
	{
		m_nTempLineCaliperNum[i] = 20;
		m_nTempLineOutLineNum[i] = 10;
		m_dTempLineContrastThre[i] = 10;
		m_nTempLinePolarityMode[i] = 0;
		m_lTempLineFilterHalfWidth[i] = 1;
		m_bTempLineSortByScore[i] = FALSE;
		m_bTempEnableShowLinePoint[i] = FALSE;
		m_bTempLineEnableMask[i] = FALSE;
		m_bTempShowLineCaliper[i] = FALSE;
	}



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
		m_bTempShowCircleCaliper[i] = FALSE;
	}
	m_nLineAngleType = 0;
}

CDlgPatternCornerMulCircleSearchTool::~CDlgPatternCornerMulCircleSearchTool()
{
}

void CDlgPatternCornerMulCircleSearchTool::DoDataExchange(CDataExchange* pDX)
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

	DDX_Radio(pDX, IDC_RADIO_LINE_INDEX1, m_nLineIndex);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM, m_nLineCaliperNum);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM, m_nLineOutLineNum);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE, m_dLineContrastThre);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE1, m_nLinePolarityMode);
	DDX_Check(pDX, IDC_CHECK_LINE_SORT_BY_SCORE, m_bLineSortByScore);
	DDX_Control(pDX, IDC_CHECK_LINE_SORT_BY_SCORE, m_checkLineSortByScore);
	DDX_Check(pDX, IDC_CHECK_LINE_SHOW_LINE_POINT, m_bEnableShowLinePoint);
	DDX_Control(pDX, IDC_CHECK_LINE_SHOW_LINE_POINT, m_checkShowLinePoint);
	DDX_Check(pDX, IDC_CHECK_LINE_ENABLE_MASK, m_bLineEnableMask);
	DDX_Control(pDX, IDC_CHECK_LINE_ENABLE_MASK, m_checkLineEnableMask);


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


	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER, m_bShowLineCaliper);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
}


BEGIN_MESSAGE_MAP(CDlgPatternCornerMulCircleSearchTool, CDialog)

	// 	ON_EN_SETFOCUS(IDC_EDT_GRAU, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtGrau)
	// 	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtNoisethre)
	// 	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAngleHigh)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnSetMaskImage)

	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternCornerMulCircleSearchTool::OnEnChangeEdtCircleCalipernum)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE0, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_POLARITY_MODE2, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCircleInwardMode0)

	ON_BN_CLICKED(IDC_CHECK_SORT_BY_SCORE1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckSortByScore1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_POINT, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckShowLinePoint)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_INDEX1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioCircleIndex1)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_INDEX2, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioCircleIndex1)

	ON_CBN_SELCHANGE(IDC_COMBO_LINE_ANGLE_TYPE, &CDlgPatternCornerMulCircleSearchTool::OnCbnSelchangeComboLineAngleType)

	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnFullImageSearch)

	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtSearchConfusionThre)



	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, &CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtConfusionThre)
	ON_BN_CLICKED(IDC_RADIO_LINE_INDEX1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioLineIndex1)
	ON_BN_CLICKED(IDC_RADIO_LINE_INDEX2, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioLineIndex1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM, &CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE, &CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineContrastThre)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternCornerMulCircleSearchTool::OnEnChangeEdtLineCalipernum)
	ON_BN_CLICKED(IDC_CHECK_LINE_SHOW_LINE_POINT, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineShowLinePoint)
	ON_BN_CLICKED(IDC_CHECK_LINE_SORT_BY_SCORE, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineSortByScore)
	ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE1, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadLinePolarityMode1)
	ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE2, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadLinePolarityMode1)
	ON_BN_CLICKED(IDC_RAD_LINE_POLARITY_MODE3, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadLinePolarityMode1)
	ON_BN_CLICKED(IDC_CHECK_LINE_ENABLE_MASK, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineEnableMask)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkShowLineCaliper)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkShowCircleCaliper)
END_MESSAGE_MAP()


// CDlgPatternCornerMulCircleSearchTool 消息处理程序

void CDlgPatternCornerMulCircleSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNCORNERMULCIRCLESEARCHTOOL") == strDlgID)
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


void CDlgPatternCornerMulCircleSearchTool::UpdateControlDisplay()
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
		}
		else
		{
			GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(m_bEnableMask);
	}
}

void CDlgPatternCornerMulCircleSearchTool::UpdateTrainData(bool bValidate)
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

		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_iAccracy);
		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_iCoarseScale+1);
		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow = m_dAngleLow;
		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;
		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask;
		((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType = m_nLineAngleType;


		m_nTempLineCaliperNum[m_nLineIndex] = m_nLineCaliperNum;
		m_nTempLineOutLineNum[m_nLineIndex] = m_nLineOutLineNum;
		m_dTempLineContrastThre[m_nLineIndex] = m_dLineContrastThre;
		m_nTempLinePolarityMode[m_nLineIndex] = m_nLinePolarityMode;
		m_bTempLineSortByScore[m_nLineIndex] = m_bLineSortByScore;
		m_bTempLineEnableMask[m_nLineIndex] = m_bLineEnableMask;
		m_bTempShowLineCaliper[m_nLineIndex] = m_bShowLineCaliper;
		for (int i = 0; i < DLG_PATTERN_LINE_NUM; i++)
		{
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nLineCaliperNum[i]		= m_nTempLineCaliperNum[i];
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineOutLineNum[i]		= m_nTempLineOutLineNum[i];
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dLineContrastThre[i]	= m_dTempLineContrastThre[i];
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeLineProperty[i]	= (EdgePolarity)m_nTempLinePolarityMode[i];				
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bLineSortByScore[i]	= m_bTempLineSortByScore[i];
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineEnableLineMask[i]	= m_bTempLineEnableMask[i];
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[i] = m_bTempShowLineCaliper[i];
		}

		m_nTempCircleCaliperNum[m_nCircleIndex]			= m_nCircleCaliperNum;
		m_nTempCircleOutLineNum[m_nCircleIndex]			= m_nCircleOutLineNum;
		m_dTempCircleContrastThre[m_nCircleIndex]		= m_dCircleContrastThre;
		m_nTempCirclePolarityMode[m_nCircleIndex]		= m_nCirclePolarityMode;
		m_nTempCircleInwardMode[m_nCircleIndex]			= m_nCircleInwardMode;
		m_bTempCircleSortByScore[m_nCircleIndex]		= m_bCircleSortByScore;
		m_bTempShowCircleCaliper[m_nCircleIndex]		= m_bShowCircleCaliper;

		for (int i = 0; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[i]		= m_nTempCircleCaliperNum[i];
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)m_nTempCircleInwardMode[i];

			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[i]		= m_nTempCircleOutLineNum[i];
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[i]	= m_dTempCircleContrastThre[i];
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[i]	= (EdgePolarity)m_nTempCirclePolarityMode[i];				
			((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[i]	= m_bTempCircleSortByScore[i];
			((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible[i]	= m_bTempShowCircleCaliper[i];
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;

	}
	else // 设置界面值
	{		
		m_iAccracy			= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_iCoarseScale	= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow		= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh		= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask	= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
		m_nLineAngleType = ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineAngleType;

		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}

		for (int i = 0; i < DLG_PATTERN_LINE_NUM; i++)
		{
			m_nTempLineCaliperNum[i] = ((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nLineCaliperNum[i];
			m_nTempLineOutLineNum[i] = ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineOutLineNum[i];
			m_dTempLineContrastThre[i] = ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dLineContrastThre[i];
			m_nTempLinePolarityMode[i]=((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeLineProperty[i];				
			m_bTempLineSortByScore[i] = ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bLineSortByScore[i];
			m_bTempLineEnableMask[i] =((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineEnableLineMask[i];
			m_bTempShowLineCaliper[i] = ((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bLineCaliperVisible[i];
		}


		m_nLineCaliperNum = m_nTempLineCaliperNum[m_nLineIndex];
		m_nLineOutLineNum = m_nTempLineOutLineNum[m_nLineIndex];
		m_dLineContrastThre = m_dTempLineContrastThre[m_nLineIndex];
		m_nLinePolarityMode = m_nTempLinePolarityMode[m_nLineIndex];
		m_bLineSortByScore = m_bTempLineSortByScore[m_nLineIndex];
		m_bLineEnableMask = m_bTempLineEnableMask[m_nLineIndex];
		m_bShowLineCaliper = m_bTempShowLineCaliper[m_nLineIndex];
		m_checkLineEnableMask.SetCheck(m_bLineEnableMask);
		m_checkLineSortByScore.SetCheck(m_bLineSortByScore);

		for (int i = 0;i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			m_nTempCircleCaliperNum[i] = ((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum[i];
			m_nTempCircleInwardMode[i] = ((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection[i];

			m_nTempCircleOutLineNum[i]		= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum[i];
			m_dTempCircleContrastThre[i]	= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre[i];
			m_nTempCirclePolarityMode[i]	= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeCircleProperty[i];				
			m_bTempCircleSortByScore[i]		= ((CPatternCornerMulCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore[i];
			m_bTempShowCircleCaliper[i]		= ((CPatternCornerMulCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCircleCaliperVisible[i];
		}

		m_nCircleCaliperNum		= m_nTempCircleCaliperNum[m_nCircleIndex];
		m_nCircleInwardMode		= m_nTempCircleInwardMode[m_nCircleIndex];

		m_nCircleOutLineNum		= m_nTempCircleOutLineNum[m_nCircleIndex];
		m_dCircleContrastThre	= m_dTempCircleContrastThre[m_nCircleIndex];
		m_nCirclePolarityMode	= m_nTempCirclePolarityMode[m_nCircleIndex];
		//m_nCirclePolarityMode	= m_nTempCirclePolarityMode[m_nCircleIndex];
		m_bCircleSortByScore    = m_bTempCircleSortByScore[m_nCircleIndex];
		m_bShowCircleCaliper	= m_bTempShowCircleCaliper[m_nCircleIndex];

		m_checkCircleSortByScore.SetCheck(m_bCircleSortByScore);
		m_checkShowCirclePoint.SetCheck(m_bEnableShowCirclePoint);
		m_comboLineAngleType.SetCurSel(m_nLineAngleType);
		m_comboAccuracy.SetCurSel(m_iAccracy);
		m_comboSpeed.SetCurSel(m_iCoarseScale);
		UpdateData(FALSE);
	}
}

void CDlgPatternCornerMulCircleSearchTool::UpdateSearchData(bool bValidate)
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

		((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum		= m_nSearchNum;
		((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre	= m_dAcceptThre;
		((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre	= m_dConfusionThre;

		m_bTempEnableShowLinePoint[m_nLineIndex] = m_bEnableShowLinePoint;
		for (int i = 0;i < DLG_PATTERN_LINE_NUM; i++)
		{
			((CPatternCornerMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint[i] = m_bTempEnableShowLinePoint[i];
		}

		m_bTempEnableShowCirclePoint[m_nCircleIndex] = m_bEnableShowCirclePoint;

		for (int i = 0 ; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			((CPatternCornerMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[i] = m_bTempEnableShowCirclePoint[i];
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_nSearchNum	= ((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dConfusionThre = ((CPatternCornerMulCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		for (int i = 0; i < DLG_PATTERN_CIRCLE_NUM; i++)
		{
			m_bTempEnableShowCirclePoint[i] = ((CPatternCornerMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint[i];
		}

		m_bEnableShowCirclePoint = m_bTempEnableShowCirclePoint[m_nCircleIndex];

		for (int i = 0;i  < DLG_PATTERN_LINE_NUM; i++)
		{
			m_bTempEnableShowLinePoint[i] = ((CPatternCornerMulCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint[i];
		}

		m_bEnableShowLinePoint = m_bTempEnableShowLinePoint[m_nLineIndex];


		UpdateData(FALSE);
	}

}


void CDlgPatternCornerMulCircleSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);

	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
	}

	if (m_nLineCaliperNum < 3)
	{
		m_nLineCaliperNum = 3;
	}

	if ((m_nLineOutLineNum < 0) || (m_nLineOutLineNum > (m_nLineCaliperNum - 3)))
	{
		m_nLineOutLineNum = 0;
	}

	if ((m_dLineContrastThre < 0.0) || (m_dLineContrastThre > 255.0))
	{
		m_dLineContrastThre = 10;
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


BOOL CDlgPatternCornerMulCircleSearchTool::OnInitDialog()
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


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkEnableMask()
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


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnSetMaskImage()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask.Height()) )
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

void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioCircleIndex1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);		

	m_nTempCircleOutLineNum[m_nPreCircleIndex]		= m_nCircleOutLineNum;
	m_dTempCircleContrastThre[m_nPreCircleIndex]	= m_dCircleContrastThre;
	m_nTempCirclePolarityMode[m_nPreCircleIndex]	= (EdgePolarity)m_nCirclePolarityMode;
	m_nTempCircleInwardMode[m_nPreCircleIndex]		= m_nCircleInwardMode;
	m_bTempCircleSortByScore[m_nPreCircleIndex]		= m_bCircleSortByScore;

	m_nTempCircleCaliperNum[m_nPreCircleIndex]		= m_nCircleCaliperNum;
	m_bTempEnableShowCirclePoint[m_nPreCircleIndex] = m_bEnableShowCirclePoint;
	m_bTempShowCircleCaliper[m_nPreCircleIndex]		= m_bShowCircleCaliper;


	m_nCircleOutLineNum			= m_nTempCircleOutLineNum[m_nCircleIndex];
	m_dCircleContrastThre		= m_dTempCircleContrastThre[m_nCircleIndex];
	m_nCirclePolarityMode		= m_nTempCirclePolarityMode[m_nCircleIndex];
	m_nCircleInwardMode			= m_nTempCircleInwardMode[m_nCircleIndex];
	m_bCircleSortByScore		= m_bTempCircleSortByScore[m_nCircleIndex];

	m_nCircleCaliperNum			= m_nTempCircleCaliperNum[m_nCircleIndex];
	m_bEnableShowCirclePoint	= m_bTempEnableShowCirclePoint[m_nCircleIndex];
	m_bShowCircleCaliper		= m_bTempShowCircleCaliper[m_nCircleIndex];

	m_nPreCircleIndex = m_nCircleIndex;

	UpdateData(FALSE);



}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCirclePolarityMode0()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(true);
	//	UpdateSearchData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadCircleInwardMode0()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(true);
	//	UpdateSearchData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckSortByScore1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(true);
	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgPatternCornerMulCircleSearchTool::OnCbnSelchangeComboLineAngleType()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nLineAngleType = m_comboLineAngleType.GetCurSel();
}

void CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnGetTrainImage()
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


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnTrain()
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
		scImage8 trainImage;
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


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternCornerMulCircleSearchTrainGuiParam trainGuiParam;
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

// void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtGrau()
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


// void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtNoisethre()
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


// void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtTotalMag()
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


void CDlgPatternCornerMulCircleSearchTool::OnEnChangeEdtCircleCalipernum()
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

void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleCalipernum()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleOutlineNum()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtCircleContrastThre()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAcceptThre()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtSearchNum()
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

void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtSearchConfusionThre()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAngleLow()
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


void CDlgPatternCornerMulCircleSearchTool::OnSetfocusEdtAngleHigh()
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



void CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtConfusionThre()
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


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioLineIndex1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);		

	m_nTempLineCaliperNum[m_nPreLineIndex]		= m_nLineCaliperNum;
	m_nTempLineOutLineNum[m_nPreLineIndex]		= m_nLineOutLineNum;
	m_dTempLineContrastThre[m_nPreLineIndex]	= m_dLineContrastThre;
	m_nTempLinePolarityMode[m_nPreLineIndex]	= (EdgePolarity)m_nLinePolarityMode;
	m_bTempLineSortByScore[m_nPreLineIndex]		= m_bLineSortByScore;
	m_bTempLineEnableMask[m_nPreLineIndex]			= m_bLineEnableMask;
	m_bTempEnableShowLinePoint[m_nPreLineIndex] = m_bEnableShowLinePoint;
	m_bTempShowLineCaliper[m_nPreLineIndex]		= m_bShowLineCaliper;

	m_nLineCaliperNum			= m_nTempLineCaliperNum[m_nLineIndex];
	m_nLineOutLineNum			= m_nTempLineOutLineNum[m_nLineIndex];
	m_dLineContrastThre			= m_dTempLineContrastThre[m_nLineIndex];
	m_nLinePolarityMode			= m_nTempLinePolarityMode[m_nLineIndex];
	m_bLineSortByScore			= m_bTempLineSortByScore[m_nLineIndex];
	m_bLineEnableMask			= m_bTempLineEnableMask[m_nLineIndex];
	m_bEnableShowLinePoint	    = m_bTempEnableShowLinePoint[m_nLineIndex];
	m_bShowLineCaliper			= m_bTempShowLineCaliper[m_nLineIndex];

	m_nPreLineIndex = m_nLineIndex;

	UpdateData(FALSE);
}


// void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadioLineIndex2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	OnBnClickedRadioLineIndex1();
// }


void CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM, strOutput);
		}
	}
}


void CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineOutlineNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM, strOutput);
		}
	}
}


void CDlgPatternCornerMulCircleSearchTool::OnEnSetfocusEdtLineContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE, strOutput);
		}
	}
}


void CDlgPatternCornerMulCircleSearchTool::OnEnChangeEdtLineCalipernum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ((m_nLineCaliperNum < 2) || (m_nLineCaliperNum > 1000))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineShowLinePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(true);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineSortByScore()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedRadLinePolarityMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedCheckLineEnableMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkShowLineCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}


void CDlgPatternCornerMulCircleSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}
