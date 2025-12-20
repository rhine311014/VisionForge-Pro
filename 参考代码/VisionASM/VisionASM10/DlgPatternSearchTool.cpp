// DlgPatternSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgPatternSearchTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"
#include "LogFileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternSearchTool dialog


CDlgPatternSearchTool::CDlgPatternSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternSearchTool::IDD, pParent)
	, m_bEnableSharpness(FALSE)
	, m_dSharpnessScore(0.5)
	, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgPatternSearchTool)
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	//}}AFX_DATA_INIT
	
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;
    m_nOldAccuracyMode=m_nPatAccuracyMode;
    m_nOldPatSpeedMode=m_nPatSpeedMode;
}


void CDlgPatternSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatternSearchTool)
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
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_ENABLE_SHARPNESS, m_bEnableSharpness);
	DDX_Text(pDX, IDC_EDT_SHARPNESS_THRE, m_dSharpnessScore);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternSearchTool, CDialog)
	//{{AFX_MSG_MAP(CDlgPatternSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCURACY_MODE, OnSelchangeComboAccuracyMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, OnBtnSetCenter)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_CONFUSION_THRE, OnSetfocusEdtConfusionThre)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_ENABLE_SHARPNESS, &CDlgPatternSearchTool::OnBnClickedChkEnableSharpness)
	ON_EN_SETFOCUS(IDC_EDT_SHARPNESS_THRE, &CDlgPatternSearchTool::OnEnSetfocusEdtSharpnessThre)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternSearchTool message handlers
void CDlgPatternSearchTool::UpdateControlDisplay()
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
			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()==ePatternSearchTool)
			{
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
			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()==ePatternSearchTool)
			{
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

void CDlgPatternSearchTool::UpdateRadioButtonDisplay()
{
	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{
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
		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
}

void CDlgPatternSearchTool::UpdateTrainData(bool bValidate)
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

		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode = ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;				
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

void CDlgPatternSearchTool::UpdateSearchData(bool bValidate)
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
		
		((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;
		((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSharpScore = m_bEnableSharpness;
		((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSharpThre = m_dSharpnessScore;
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
		
	}
	else // 设置界面值
	{
		
		m_nSearchNum	= ((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre = ((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;
		m_bEnableSharpness = ((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableSharpScore;
		m_dSharpnessScore = ((CPatternSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSharpThre;
		GetDlgItem(IDC_EDT_SHARPNESS_THRE)->EnableWindow(m_bEnableSharpness);

		UpdateData(FALSE);
	}
	
}

void CDlgPatternSearchTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_PATTERNSEARCH_TOOL") == strDlgID)
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

void CDlgPatternSearchTool::CheckDlgDataValidation()
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
	if ((m_dSharpnessScore < 0.0) || (m_dSharpnessScore > 1.0))
	{
		m_dSharpnessScore = 0.5;
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
	
	UpdateData(FALSE);
}

void CDlgPatternSearchTool::OnBtnGetTrainImage() 
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

void CDlgPatternSearchTool::OnBtnTrain() 
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
		CString strErrInfo =m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}

    CString strLog;
    strLog.Format(_T("点击[训练]\n"));
    g_opLogFile.SaveLogText(strLog,6);
}

void CDlgPatternSearchTool::OnBtnSetMaskImage() 
{
	// TODO: Add your control notification handler code here
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
    CString strLog;
    strLog.Format(_T("点击[设置掩膜]\n"));
    g_opLogFile.SaveLogText(strLog,6);

	cpImage tmpImage(m_pDlgParent->m_imageTrain.Width(),m_pDlgParent->m_imageTrain.Height(),epGray8);
	m_pDlgParent->m_pSearchTool->GetProcessImage(m_pDlgParent->m_pTrainDataParam,m_pDlgParent->m_imageTrain,tmpImage);

// 	CDlgMaskEdit dlgMask;
// 	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);
// 
// 	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask.Width())
// 		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask.Height()) )
// 	{
// 		m_imageMask.Release();
// 	}

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


void CDlgPatternSearchTool::OnChkEnableMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
    CString strLog;
    CString strValue = m_bEnableMask ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[掩模设置][启用]为%s\n"),strValue);
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

BOOL CDlgPatternSearchTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	// 设置按钮
	m_btnSetMask.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_btnSetCenter.SetThemeHelper(&m_themeHelper);
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
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPatternSearchTool::OnSelchangeComboAccuracyMode() 
{
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();
    if (m_nOldAccuracyMode!=m_nPatAccuracyMode)
    {
        CString strLog;
        CString strVal[]= {_T("精细"),_T("标准"),_T("粗略")};
        strLog.Format(_T("修改[精细][%s]为[%s]\n"),strVal[m_nOldAccuracyMode],strVal[m_nPatAccuracyMode]);
        g_opLogFile.SaveLogText(strLog,6);
        m_nOldAccuracyMode = m_nPatAccuracyMode;
    }
}

void CDlgPatternSearchTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
    if (m_nOldPatSpeedMode!=m_nPatSpeedMode)
    {
        CString strLog;
        CString strVal[]= {_T("1/4倍速"),_T("1/2倍速"),_T("标准"),_T("2倍速"),_T("4倍速"),_T("8倍速")};
        strLog.Format(_T("修改[速度][%s]为[%s]\n"),strVal[m_nOldPatSpeedMode],strVal[m_nPatSpeedMode]);
        g_opLogFile.SaveLogText(strLog,6);
        m_nOldPatSpeedMode = m_nPatSpeedMode;
    }
}

void CDlgPatternSearchTool::OnBtnSetCenter() 
{
    CString strLog;
    strLog.Format(_T("点击[提取标记点]\n"));
    g_opLogFile.SaveLogText(strLog,6);
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		if (m_pDlgParent->m_pSearchTool->GetSearchToolType() == eCornerSearchCheckInspectTool)
		{
			CCornerCheckSearchTrainGuiParam *pTrainGuiParam = new CCornerCheckSearchTrainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
			double dMarkX = pTrainGuiParam->m_ModelPoint1.GetX();
			double dMarkY = pTrainGuiParam->m_ModelPoint1.GetY();
			imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect1);
			dMarkX -= pTrainGuiParam->m_TrainRect1.GetUL().GetX();
			dMarkY -= pTrainGuiParam->m_TrainRect1.GetUL().GetY();


			scRect rcTrain = pTrainGuiParam->m_TrainRect1;
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
				pTrainGuiParam->m_ModelPoint1.SetX(dMarkX+pTrainGuiParam->m_TrainRect1.GetUL().GetX());
				pTrainGuiParam->m_ModelPoint1.SetY(dMarkY+pTrainGuiParam->m_TrainRect1.GetUL().GetY());
				m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
				m_pDlgParent->m_pSearchTool->Train();
				m_pDlgParent->m_comboDisplayType.SetCurSel(1);
				m_pDlgParent->UpdateGuiDisplay();
			}
			delete pTrainGuiParam;
			pTrainGuiParam = NULL;
		}
		else if(m_pDlgParent->m_pSearchTool->GetSearchToolType() == eSpecialPickSearchTool)
		{
			CSpecialPickSearchTrainGuiParam *pTrainGuiParam = new CSpecialPickSearchTrainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
			double dMarkX = pTrainGuiParam->m_RoughSearchTrainGuiParam.m_ModelPoint.GetX();
			double dMarkY = pTrainGuiParam->m_RoughSearchTrainGuiParam.m_ModelPoint.GetY();
			imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect);
			dMarkX -= pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect.GetUL().GetX();
			dMarkY -= pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect.GetUL().GetY();


			scRect rcTrain = pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect;
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
				pTrainGuiParam->m_RoughSearchTrainGuiParam.m_ModelPoint.SetX(dMarkX+pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect.GetUL().GetX());
				pTrainGuiParam->m_RoughSearchTrainGuiParam.m_ModelPoint.SetY(dMarkY+pTrainGuiParam->m_RoughSearchTrainGuiParam.m_TrainRect.GetUL().GetY());
				m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
				m_pDlgParent->m_pSearchTool->Train();
				m_pDlgParent->m_comboDisplayType.SetCurSel(1);
				m_pDlgParent->UpdateGuiDisplay();
			}
			delete pTrainGuiParam;
			pTrainGuiParam = NULL;
		}else if (m_pDlgParent->m_pSearchTool->GetSearchToolType() == eCombinationCornerSearchTool)
		{
			CCombinationCornerSearchTrainGuiParam *pTrainGuiParam = new CCombinationCornerSearchTrainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
			double dMarkX = pTrainGuiParam->m_ModelPoint1.GetX();
			double dMarkY = pTrainGuiParam->m_ModelPoint1.GetY();
			imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect1);
			dMarkX -= pTrainGuiParam->m_TrainRect1.GetUL().GetX();
			dMarkY -= pTrainGuiParam->m_TrainRect1.GetUL().GetY();


			scRect rcTrain = pTrainGuiParam->m_TrainRect1;
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
				pTrainGuiParam->m_ModelPoint1.SetX(dMarkX+pTrainGuiParam->m_TrainRect1.GetUL().GetX());
				pTrainGuiParam->m_ModelPoint1.SetY(dMarkY+pTrainGuiParam->m_TrainRect1.GetUL().GetY());
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
			CPatternSearchTrainGuiParam *pTrainGuiParam = new CPatternSearchTrainGuiParam;
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
	}
	else
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_CENTER_SET_DIALOG_FAILED);	//_T("打开提取界面失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NOT_TRAINED);			//_T("定位工具未训练");
		m_pDlgParent->UpdateStatusBar();
	}
}

void CDlgPatternSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgPatternSearchTool::OnSetfocusEdtAngleHigh() 
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

void CDlgPatternSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgPatternSearchTool::OnSetfocusEdtAcceptThre() 
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
                strLog.Format(_T("修改[搜索设置][相似度1]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}


void CDlgPatternSearchTool::OnSetfocusEdtConfusionThre() 
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[搜索设置][相似度2]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}

void CDlgPatternSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternSearchTrainGuiParam trainGuiParam;
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
    CString strLog;
    strLog.Format(_T("点击[全图搜索]\n"));
    g_opLogFile.SaveLogText(strLog,6);
}

BOOL CDlgPatternSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgPatternSearchTool::OnBnClickedChkEnableSharpness()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableSharpness ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[清晰度分数]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,6);
	GetDlgItem(IDC_EDT_SHARPNESS_THRE)->EnableWindow(m_bEnableSharpness);
}


void CDlgPatternSearchTool::OnEnSetfocusEdtSharpnessThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_SHARPNESS_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_SHARPNESS_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_SHARPNESS_THRE, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[清晰度分数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
		}
	}
}
