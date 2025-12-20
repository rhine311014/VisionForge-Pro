// DlgPatternCornerCircleSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPatternCornerCircleSearchTool.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgMaskEdit.h"

// CDlgPatternCornerCircleSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternCornerCircleSearchTool, CDialog)

CDlgPatternCornerCircleSearchTool::CDlgPatternCornerCircleSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternCornerCircleSearchTool::IDD, pParent)
	, m_nLineCaliperNum(0)
	, m_bShowLineCaliper(FALSE)
	, m_bShowCircleCaliper(FALSE)
	, m_bEnableLineDualEdge(FALSE)
	, m_bEnableCircleDualEdge(FALSE)
	, m_bEnableCircleMask(FALSE)
	, m_nImageChannel(0)
{
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;

	m_nPatSpeedMode = 3;
	m_nPatAccuracyMode = 1;

	m_dAngleLow = -5.0;
	m_dAngleHigh = 5.0;

	m_nLineCaliperIndex = 0;
	m_nLineCaliperNum = 20;
	m_nLineOutLineNum = 10.0;
	m_dLineContrastThre = 10.0;
	m_nLinePolarityMode = (int)eDontCare;
	m_lLineFilterHalfWidth = 1;
	m_bLineSortByScore = TRUE;
	m_bEnableLineMask = FALSE;
	m_bShowLineFitPoint = FALSE;
	m_nLineAngleType = 0;

	m_nCircleCaliperNum = 20;
	m_nCircleOutLineNum = 10;
	m_dCircleContrastThre = 10.0;
	m_nCirclePolarityMode = (int)eDontCare;
	m_nCircleInwardMode = (int)eInward;
	m_lCircleFilterHalfWidth = 1;
	m_bCircleSortByScore = TRUE;
	m_bShowCircleFitPoint = FALSE;

	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_nSearchNum = 1;


	m_nPreLineCaliperIndex = m_nLineCaliperIndex;
	for (int i=0;i<LINE_NUM_TEMP;i++)
	{
		m_nTempLineCaliperNum[i] = 20;
		m_nTempLineOutLineNum[i] = 10;
		m_dTempLineContrastThre[i] = 10.0;	
		m_nTempLinePolarityMode[i] = (int)eDontCare;
		m_lTempLineFilterHalfWidth[i] = 1;
		m_bTempLineSortByScore[i] = TRUE;
		m_bTempEnableLineMask[i] = FALSE;
		m_bTempShowLineFitPoint[i] = FALSE;
		m_bTempShowLineCaliper[i]=FALSE;
		m_bTempEnableLineDualEdge[i] = FALSE;
	}

}

CDlgPatternCornerCircleSearchTool::~CDlgPatternCornerCircleSearchTool()
{
}

void CDlgPatternCornerCircleSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_FULL_IMAGE_SEARCH, m_btnFullImageSearch);

	DDX_Control(pDX, IDC_COMBO_ACCURACY_MODE, m_comboAccuracy);
	DDX_Control(pDX, IDC_COMBO_SPEED_MODE, m_comboSpeed);
	DDX_CBIndex(pDX, IDC_COMBO_SPEED_MODE, m_nPatSpeedMode);
	DDX_CBIndex(pDX, IDC_COMBO_ACCURACY_MODE, m_nPatAccuracyMode);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);

	DDX_Radio(pDX, IDC_RADIO_CALIPER_1, m_nLineCaliperIndex);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM, m_nLineCaliperNum);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM, m_nLineOutLineNum);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE, m_dLineContrastThre);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0, m_nLinePolarityMode);
	DDX_Check(pDX, IDC_CHECK_LINE_SORT_BY_SCORE, m_bLineSortByScore);
	DDX_Control(pDX, IDC_CHECK_LINE_SORT_BY_SCORE, m_checkLineSortByScore);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK, m_bEnableLineMask);
	DDX_Check(pDX, IDC_CHECK_SHOW_LINE_FITPOINT, m_bShowLineFitPoint);
	DDX_Control(pDX, IDC_COMBO_LINE_ANGLE_TYPE, m_comboLineAngleType);

	DDX_Text(pDX, IDC_EDT_CIRCLE_CALIPERNUM, m_nCircleCaliperNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_OUTLINE_NUM, m_nCircleOutLineNum);
	DDX_Text(pDX, IDC_EDT_CIRCLE_CONTRAST_THRE, m_dCircleContrastThre);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_POLARITY_MODE0, m_nCirclePolarityMode);
	DDX_Radio(pDX, IDC_RAD_CIRCLE_INWARD_MODE0, m_nCircleInwardMode);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_bCircleSortByScore);
	DDX_Control(pDX, IDC_CHECK_CIRCLE_SORT_BY_SCORE, m_checkCircleSortByScore);
	DDX_Check(pDX, IDC_CHECK_SHOW_CIRCLE_FITPOINT, m_bShowCircleFitPoint);

	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_CONFUSION_THRE, m_dConfusionThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);

	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER, m_bShowLineCaliper);
	DDX_Check(pDX, IDC_CHK_SHOW_CIRCLE_CALIPER, m_bShowCircleCaliper);
	DDX_Check(pDX, IDC_CHK_LINE_ENABLE_DUAL_EDGE, m_bEnableLineDualEdge);
	DDX_Check(pDX, IDC_CHK_CIRCLR_ENABLE_DUAL_EDGE, m_bEnableCircleDualEdge);
	DDX_Check(pDX, IDC_CHECK_CIRCLE_MASK, m_bEnableCircleMask);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth);
	DDX_Text(pDX, IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1, m_lCircleFilterHalfWidth);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternCornerCircleSearchTool, CDialog)

	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgPatternCornerCircleSearchTool::OnBnClickedChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgPatternCornerCircleSearchTool::OnBnClickedBtnSetMaskImage)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternCornerCircleSearchTool::OnBnClickedBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternCornerCircleSearchTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgPatternCornerCircleSearchTool::OnBnClickedBtnFullImageSearch)
	ON_BN_CLICKED(IDC_RADIO_CALIPER_1, OnLineCaliperChange)
	ON_BN_CLICKED(IDC_RADIO_CALIPER_2, OnLineCaliperChange)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_FITPOINT, &CDlgPatternCornerCircleSearchTool::OnBnClickedCheckShowLineFitpoint)
	ON_BN_CLICKED(IDC_CHECK_SHOW_CIRCLE_FITPOINT, &CDlgPatternCornerCircleSearchTool::OnBnClickedCheckShowCircleFitpoint)

	ON_EN_KILLFOCUS(IDC_EDT_ANGLE_LOW, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAngleLow)
	ON_EN_KILLFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAngleHigh)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnEnChangeEdtLineCalipernum)
	ON_EN_KILLFOCUS(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineCalipernum)
	ON_EN_KILLFOCUS(IDC_EDT_LINE_OUTLINE_NUM, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineOutlineNum)
	ON_EN_KILLFOCUS(IDC_EDT_LINE_CONTRAST_THRE, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineContrastThre)

	ON_EN_CHANGE(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnEnChangeEdtCircleCalipernum)
	ON_EN_KILLFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleCalipernum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CALIPERNUM, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleCalipernum)
	ON_EN_KILLFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleOutlineNum)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_OUTLINE_NUM, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleOutlineNum)
	ON_EN_KILLFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleContrastThre)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_CONTRAST_THRE, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleContrastThre)

	ON_EN_KILLFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAcceptThre)
	ON_EN_KILLFOCUS(IDC_EDT_CONFUSION_THRE, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtConfusionThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtConfusionThre)
	ON_EN_KILLFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternCornerCircleSearchTool::OnKillfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgPatternCornerCircleSearchTool::OnSetfocusEdtSearchNum)
	
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK, &CDlgPatternCornerCircleSearchTool::OnBnClickedCheckLineMask)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER, &CDlgPatternCornerCircleSearchTool::OnBnClickedChkShowLineCaliper)
	ON_BN_CLICKED(IDC_CHK_SHOW_CIRCLE_CALIPER, &CDlgPatternCornerCircleSearchTool::OnBnClickedChkShowCircleCaliper)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE1, &CDlgPatternCornerCircleSearchTool::OnBnClickedRadCircleInwardMode1)
	ON_BN_CLICKED(IDC_RAD_CIRCLE_INWARD_MODE0, &CDlgPatternCornerCircleSearchTool::OnBnClickedRadCircleInwardMode0)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_MASK, &CDlgPatternCornerCircleSearchTool::OnBnClickedCheckCircleMask)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth)
	ON_EN_SETFOCUS(IDC_EDT_CIRCLE_FILTER_HALFWIDTH_1,OnEnSetfocusEditCircleFilterHalfWidth)
END_MESSAGE_MAP()


// CDlgPatternCornerCircleSearchTool 消息处理程序


BOOL CDlgPatternCornerCircleSearchTool::OnInitDialog()
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
	m_btnFullImageSearch.SetThemeHelper(&m_themeHelper);
	

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


	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgPatternCornerCircleSearchTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPatternCornerCircleSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNCORNERCIRCLESEARCH_TOOL") == strDlgID)
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

void CDlgPatternCornerCircleSearchTool::UpdateControlDisplay()
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


void CDlgPatternCornerCircleSearchTool::UpdateTrainData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	CPatternCornerCircleSearchTrainDataParam* pTrainDataParam = (CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam;
	CPatternCornerCircleSearchTrainGuiParam* pTrainGuiParam = (CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam;

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		pTrainDataParam->m_nImageChannel = m_nImageChannel;
		pTrainDataParam->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		pTrainDataParam->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		pTrainDataParam->m_bMaskEnable = m_bEnableMask;
		pTrainDataParam->m_dAngleLow  =  m_dAngleLow;
		pTrainDataParam->m_dAngleHigh = m_dAngleHigh;


		// Line
		// TrainGUI
		m_nTempLineCaliperNum[m_nLineCaliperIndex]		= m_nLineCaliperNum;
		m_bTempEnableLineMask[m_nLineCaliperIndex]		= m_bEnableLineMask;
		m_bTempShowLineCaliper[m_nLineCaliperIndex]		= m_bShowLineCaliper;
		m_bTempEnableLineMask[m_nLineCaliperIndex]		= m_bEnableLineMask;
		// TrainData
		m_nTempLineOutLineNum[m_nLineCaliperIndex]		= m_nLineOutLineNum;
		m_dTempLineContrastThre[m_nLineCaliperIndex]	= m_dLineContrastThre;
		m_nTempLinePolarityMode[m_nLineCaliperIndex]	= (EdgePolarity)m_nLinePolarityMode;		
		m_bTempLineSortByScore[m_nLineCaliperIndex]		= m_bLineSortByScore;
		m_lTempLineFilterHalfWidth[m_nLineCaliperIndex] = m_lLineFilterHalfWidth;
		m_bTempEnableLineDualEdge[m_nLineCaliperIndex]	= m_bEnableLineDualEdge;

		for (int i = 0; i < LINE_NUM_TEMP; i++)
		{
			// Train GUI
 			pTrainGuiParam->m_nLineCaliperNum[i] = m_nTempLineCaliperNum[i];
 			pTrainGuiParam->m_bEnableLineMask[i] = m_bTempEnableLineMask[i];
			pTrainGuiParam->m_bLineCaliperVisible[i]=m_bTempShowLineCaliper[i];
			pTrainGuiParam->m_bEnableLineMask[i] = m_bTempEnableLineMask[i];
			// Train Data
			pTrainDataParam->m_nLineOutLineNum[i]  = m_nTempLineOutLineNum[i];
			pTrainDataParam->m_dLineContrastThre[i] = m_dTempLineContrastThre[i];
			pTrainDataParam->m_LineedgeProperty[i] = (EdgePolarity)m_nTempLinePolarityMode[i];
			pTrainDataParam->m_bLineSortByScore[i]  = m_bTempLineSortByScore[i];
			pTrainDataParam->m_lLineFilterHalfWidth[i] = m_lTempLineFilterHalfWidth[i];
			pTrainDataParam->m_bEnableLineDualEdge[i] = m_bTempEnableLineDualEdge[i];
		}


		// Circle
		// Train GUI
		pTrainGuiParam->m_nCircleCaliperNum = m_nCircleCaliperNum;
		pTrainGuiParam->m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_nCircleInwardMode;
		pTrainGuiParam->m_bCircleCaliperVisible=m_bShowCircleCaliper;
		pTrainGuiParam->m_bEnableCircleMask = m_bEnableCircleMask;
		// Train Data
		pTrainDataParam->m_nCircleOutLineNum = m_nCircleOutLineNum;
		pTrainDataParam->m_dCircleContrastThre = m_dCircleContrastThre;
		pTrainDataParam->m_lCircleFilterHalfWidth = m_lCircleFilterHalfWidth;
		pTrainDataParam->m_CircleedgeProperty = (EdgePolarity)m_nCirclePolarityMode;		
		pTrainDataParam->m_bCircleSortByScore = m_bCircleSortByScore;
		pTrainDataParam->m_bEnableCircleDualEdge = m_bEnableCircleDualEdge;

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;



// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;
// 
// 		m_nTempLineCaliperNum[m_nLineCaliperIndex]		= m_nLineCaliperNum;
// 		m_bTempEnableLineMask[m_nLineCaliperIndex]		= m_bEnableLineMask;
// 
// 		m_nTempLineOutLineNum[m_nLineCaliperIndex]		= m_nLineOutLineNum;
// 		m_dTempLineContrastThre[m_nLineCaliperIndex]	= m_dLineContrastThre;
// 		m_nTempLinePolarityMode[m_nLineCaliperIndex]	= (EdgePolarity)m_nLinePolarityMode;		
// 		m_bTempLineSortByScore[m_nLineCaliperIndex]		= m_bLineSortByScore;
// 		m_lTempLineFilterHalfWidth[m_nLineCaliperIndex] = m_lLineFilterHalfWidth;
// 
// 		for (int i = 0; i < LINE_NUM_TEMP; ++i)
// 		{
// 			((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineCaliperNum[i] = m_nTempLineCaliperNum[i];
// 			((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineMask[i] = m_bTempEnableLineMask[i];
// 
// 			((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineOutLineNum[i]  = m_nTempLineOutLineNum[i];
// 			((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dLineContrastThre[i] = m_dTempLineContrastThre[i];
// 			((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_LineedgeProperty[i] = (EdgePolarity)m_nTempLinePolarityMode[i];
// 			((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bLineSortByScore[i]  = m_bTempLineSortByScore[i];
// 			((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lLineFilterHalfWidth[i] = m_lTempLineFilterHalfWidth[i];
// 		}
// 
// 
// 		// Circle
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum = m_nCircleOutLineNum;
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre = m_dCircleContrastThre;
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth = m_lCircleFilterHalfWidth;
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty = (EdgePolarity)m_nCirclePolarityMode;		
// 		((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore = m_bCircleSortByScore;
// 		((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum = m_nCircleCaliperNum;
// 		((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_nCircleInwardMode;
// 
// 		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
// 		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
// 		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
// 
// 		m_pDlgParent->m_bTrainDataUpdated = TRUE;
// 		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{	
		m_nImageChannel		= pTrainDataParam->m_nImageChannel;
		m_nPatAccuracyMode	= pTrainDataParam->m_Accuracy/2;
		m_nPatSpeedMode		= pTrainDataParam->m_CoarseScale-1;
		m_dAngleLow			= pTrainDataParam->m_dAngleLow;
		m_dAngleHigh		= pTrainDataParam->m_dAngleHigh;
		m_bEnableMask		= pTrainDataParam->m_bMaskEnable;	

		// Line
		for (int i = 0; i < LINE_NUM_TEMP; i++)
		{
			// Train GUI
			m_nTempLineCaliperNum[i]		= pTrainGuiParam->m_nLineCaliperNum[i];
			m_bTempEnableLineMask[i]		= pTrainGuiParam->m_bEnableLineMask[i];
			m_bTempShowLineCaliper[i]		= pTrainGuiParam->m_bLineCaliperVisible[i];
			m_bTempEnableLineMask[i]		= pTrainGuiParam->m_bEnableLineMask[i];
			// Train Data
			m_nTempLineOutLineNum[i]		= pTrainDataParam->m_nLineOutLineNum[i];
			m_dTempLineContrastThre[i]		= pTrainDataParam->m_dLineContrastThre[i];
			m_nTempLinePolarityMode[i]		= pTrainDataParam->m_LineedgeProperty[i];
			m_bTempLineSortByScore[i]		= pTrainDataParam->m_bLineSortByScore[i];
			m_lTempLineFilterHalfWidth[i]	= pTrainDataParam->m_lLineFilterHalfWidth[i];
			m_bTempEnableLineDualEdge[i]	= pTrainDataParam->m_bEnableLineDualEdge[i];
		}

		// Train GUI
		m_nLineCaliperNum		= m_nTempLineCaliperNum[m_nLineCaliperIndex];
		m_bShowLineCaliper		= m_bTempShowLineCaliper[m_nLineCaliperIndex];
		m_bEnableLineMask		= m_bTempEnableLineMask[m_nLineCaliperIndex];
		// Train Data
		m_nLineOutLineNum		= m_nTempLineOutLineNum[m_nLineCaliperIndex];
		m_dLineContrastThre		= m_dTempLineContrastThre[m_nLineCaliperIndex];
		m_nLinePolarityMode		= m_nTempLinePolarityMode[m_nLineCaliperIndex];
		m_bLineSortByScore		= m_bTempLineSortByScore[m_nLineCaliperIndex];
		m_lLineFilterHalfWidth	= m_lTempLineFilterHalfWidth[m_nLineCaliperIndex];
		m_bEnableLineDualEdge	= m_bTempEnableLineDualEdge[m_nLineCaliperIndex];

		// Circle
		// Train GUI
		m_nCircleCaliperNum			= pTrainGuiParam->m_nCircleCaliperNum;
		m_nCircleInwardMode			= pTrainGuiParam->m_CircleCaliperSearchDirection;
		m_bShowCircleCaliper		= pTrainGuiParam->m_bCircleCaliperVisible;
		m_bEnableCircleMask			= pTrainGuiParam->m_bEnableCircleMask;
		// Train Data
		m_nCircleOutLineNum			= pTrainDataParam->m_nCircleOutLineNum;
		m_dCircleContrastThre		= pTrainDataParam->m_dCircleContrastThre;
		m_lCircleFilterHalfWidth	= pTrainDataParam->m_lCircleFilterHalfWidth;
		m_nCirclePolarityMode		= pTrainDataParam->m_CircleedgeProperty;
		m_bCircleSortByScore		= pTrainDataParam->m_bCircleSortByScore;
		m_bEnableCircleDualEdge		= pTrainDataParam->m_bEnableCircleDualEdge;
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

		m_checkCircleSortByScore.SetCheck(m_bCircleSortByScore);
		m_checkLineSortByScore.SetCheck(m_bLineSortByScore);


		UpdateData(FALSE);


// 		m_nPatAccuracyMode = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
// 		m_nPatSpeedMode = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
// 		m_dAngleLow	    = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
// 		m_dAngleHigh	= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
// 		m_bEnableMask = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	
// 
// 		for (int i = 0; i < LINE_NUM_TEMP; i++)
// 		{
// 			m_nTempLineCaliperNum[i] = ((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineCaliperNum[i];
// 			m_bTempEnableLineMask[i] = ((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableLineMask[i];
// 
// 			m_nTempLineOutLineNum[i] =  ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLineOutLineNum[i];
// 			m_dTempLineContrastThre[i] = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dLineContrastThre[i];
// 			m_nTempLinePolarityMode[i] = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_LineedgeProperty[i];
// 			m_bTempLineSortByScore[i] =  ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bLineSortByScore[i];
// 			m_lTempLineFilterHalfWidth[i] = ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lLineFilterHalfWidth[i];
// 			
// 		}
// 
// 		m_nLineCaliperNum		= m_nTempLineCaliperNum[m_nLineCaliperIndex];
// 		m_bEnableLineMask		= m_bTempEnableLineMask[m_nLineCaliperIndex];
// 
// 		m_nLineOutLineNum		= m_nTempLineOutLineNum[m_nLineCaliperIndex];
// 		m_dLineContrastThre		= m_dTempLineContrastThre[m_nLineCaliperIndex];
// 		m_nLinePolarityMode		= m_nTempLinePolarityMode[m_nLineCaliperIndex];
// 		m_bLineSortByScore		= m_bTempLineSortByScore[m_nLineCaliperIndex];
// 		m_lLineFilterHalfWidth	= m_lTempLineFilterHalfWidth[m_nLineCaliperIndex];
// 		
// 
// 		// Circle
// 		m_nCircleOutLineNum			= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCircleOutLineNum;
// 		m_dCircleContrastThre		= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dCircleContrastThre;
// 		m_lCircleFilterHalfWidth	= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lCircleFilterHalfWidth;
// 		m_nCirclePolarityMode		= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CircleedgeProperty;
// 		m_bCircleSortByScore		= ((CPatternCornerCircleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bCircleSortByScore;
// 
// 		m_nCircleCaliperNum			= ((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCircleCaliperNum;
// 		m_nCircleInwardMode			= ((CPatternCornerCircleSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_CircleCaliperSearchDirection;
// 
// 		if (m_bEnableMask)
// 		{
// 			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
// 		}
// 
// 		m_comboAccuracy.SetCurSel(m_nPatAccuracyMode);
// 		m_comboSpeed.SetCurSel(m_nPatSpeedMode);
// 
//  		m_checkCircleSortByScore.SetCheck(m_bCircleSortByScore);
// 		m_checkLineSortByScore.SetCheck(m_bLineSortByScore);
// 
// 
// 		UpdateData(FALSE);
	}
}

void CDlgPatternCornerCircleSearchTool::UpdateSearchData(bool bValidate)
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

		m_bTempShowLineFitPoint[m_nLineCaliperIndex]	= m_bShowLineFitPoint;

		((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_bTempShowLineFitPoint[0];
		((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_bTempShowLineFitPoint[1];

		((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;

		((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint = m_bShowCircleFitPoint;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		m_bTempShowLineFitPoint[0] = ((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;
		m_bTempShowLineFitPoint[1] = ((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;

		m_bShowLineFitPoint		= m_bTempShowLineFitPoint[m_nLineCaliperIndex];



		m_nSearchNum	= ((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dConfusionThre = ((CPatternCornerCircleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		m_bShowCircleFitPoint = ((CPatternCornerCircleSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowCirclePoint;

		UpdateData(FALSE);
	}
}

void CDlgPatternCornerCircleSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);

	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
	}



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

	// Line
	if (m_nLineCaliperNum < 3)
	{
		m_nLineCaliperNum = 3;
	}
	if (m_nLineCaliperNum > 1000)
	{
		m_nLineCaliperNum = 1000;
	}

	if ((m_nLineOutLineNum < 0) || (m_nLineOutLineNum > (m_nLineCaliperNum - 2)))
	{
		m_nLineOutLineNum = 0;
	}
	if ((m_dLineContrastThre < 0.0) || (m_dLineContrastThre > 255.0))
	{
		m_dLineContrastThre = 10;
	}
	if (m_lLineFilterHalfWidth < 1)
	{
		m_lLineFilterHalfWidth = 1;
	}


	// Circle
	if (m_nCircleCaliperNum < 3)
	{
		m_nCircleCaliperNum = 3;
	}
	if (m_nCircleCaliperNum > 1000)
	{
		m_nCircleCaliperNum = 1000;
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

}

void CDlgPatternCornerCircleSearchTool::OnBnClickedChkEnableMask()
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


void CDlgPatternCornerCircleSearchTool::OnBnClickedBtnSetMaskImage()
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


void CDlgPatternCornerCircleSearchTool::OnBnClickedBtnGetTrainImage()
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


void CDlgPatternCornerCircleSearchTool::OnBnClickedBtnTrain()
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
		CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternCornerCircleSearchTrainGuiParam trainGuiParam;
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

void CDlgPatternCornerCircleSearchTool::OnBnClickedCheckLineMask()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedCheckShowLineFitpoint()
{
	// TODO: 在此添加控件通知处理程序代码


	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();


// 	m_bShowLineFitPoint = !m_bShowLineFitPoint;
// 
// 	CPatternCornerCircleSearchSearchGuiParam searchGuiParam;
// 	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);
// 
// 
// 	m_bTempShowLineFitPoint[m_nLineCaliperIndex]	= m_bShowLineFitPoint;
// 
// 
// 	searchGuiParam.m_bEnableShowLinePoint= m_bTempShowLineFitPoint[0];
// 	searchGuiParam.m_bEnableShowLinePoint= m_bTempShowLineFitPoint[1];
// 
// 	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);
// 
// 	m_pDlgParent->UpdateGuiDisplay();

}

void CDlgPatternCornerCircleSearchTool::OnBnClickedCheckShowCircleFitpoint()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();

// 	m_bShowCircleFitPoint = !m_bShowCircleFitPoint;
// 
// 	CPatternCornerCircleSearchSearchGuiParam searchGuiParam;
// 	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(&searchGuiParam);
// 
// 	searchGuiParam.m_bEnableShowCirclePoint	= m_bShowCircleFitPoint;
// 
// 	m_pDlgParent->m_pSearchTool->SetSearchGuiParam(&searchGuiParam);
// 
// 	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgPatternCornerCircleSearchTool::OnLineCaliperChange()
{
	UpdateData(TRUE);		
	m_nTempLineCaliperNum[m_nPreLineCaliperIndex]	= m_nLineCaliperNum;
	m_nTempLineOutLineNum[m_nPreLineCaliperIndex]	= m_nLineOutLineNum;
	m_dTempLineContrastThre[m_nPreLineCaliperIndex] = m_dLineContrastThre;
	m_nTempLinePolarityMode[m_nPreLineCaliperIndex] = (EdgePolarity)m_nLinePolarityMode;
	m_bTempLineSortByScore[m_nPreLineCaliperIndex]	= m_bLineSortByScore;
	m_lTempLineFilterHalfWidth[m_nPreLineCaliperIndex]	= m_lLineFilterHalfWidth;
	m_bTempShowLineFitPoint[m_nPreLineCaliperIndex]	= m_bShowLineFitPoint;
	
	m_bTempShowLineCaliper[m_nPreLineCaliperIndex]	= m_bShowLineCaliper;
	m_bTempEnableLineMask[m_nPreLineCaliperIndex]	= m_bEnableLineMask;
	m_bTempEnableLineDualEdge[m_nPreLineCaliperIndex] = m_bEnableLineDualEdge;


	m_nLineCaliperNum		= m_nTempLineCaliperNum[m_nLineCaliperIndex];
	m_bEnableLineMask		= m_bTempEnableLineMask[m_nLineCaliperIndex];

	m_nLineOutLineNum		= m_nTempLineOutLineNum[m_nLineCaliperIndex];
	m_dLineContrastThre		= m_dTempLineContrastThre[m_nLineCaliperIndex];
	m_nLinePolarityMode		= m_nTempLinePolarityMode[m_nLineCaliperIndex];
	m_bLineSortByScore		= m_bTempLineSortByScore[m_nLineCaliperIndex];
	m_lLineFilterHalfWidth	= m_lTempLineFilterHalfWidth[m_nLineCaliperIndex];
	m_bShowLineFitPoint		= m_bTempShowLineFitPoint[m_nLineCaliperIndex];
	
	m_bShowLineCaliper		= m_bTempShowLineCaliper[m_nLineCaliperIndex];
	m_bEnableLineMask		= m_bTempEnableLineMask[m_nLineCaliperIndex];
	m_bEnableLineDualEdge	= m_bTempEnableLineDualEdge[m_nLineCaliperIndex];
	UpdateData(FALSE);

	m_nPreLineCaliperIndex = m_nLineCaliperIndex;
}




void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAngleLow()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAngleLow()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAngleHigh()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAngleHigh()
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

void CDlgPatternCornerCircleSearchTool::OnEnChangeEdtLineCalipernum()
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

void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineCalipernum()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineOutlineNum()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineOutlineNum()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtLineContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtLineContrastThre()
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


void CDlgPatternCornerCircleSearchTool::OnEnChangeEdtCircleCalipernum()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleCalipernum()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleCalipernum()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleOutlineNum()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleOutlineNum()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtCircleContrastThre()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtCircleContrastThre()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtAcceptThre()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtAcceptThre()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtConfusionThre()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtConfusionThre()
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


void CDlgPatternCornerCircleSearchTool::OnKillfocusEdtSearchNum()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPatternCornerCircleSearchTool::OnSetfocusEdtSearchNum()
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


void CDlgPatternCornerCircleSearchTool::OnBnClickedChkShowLineCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedChkShowCircleCaliper()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedRadCircleInwardMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedRadCircleInwardMode0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgPatternCornerCircleSearchTool::OnBnClickedCheckCircleMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
}



void CDlgPatternCornerCircleSearchTool::OnEnSetfocusEditLineFilterHalfWidth() 
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

void CDlgPatternCornerCircleSearchTool::OnEnSetfocusEditCircleFilterHalfWidth() 
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