// DlgQuickSearchCheckTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgQuickSearchCheckTool.h"
#include "afxdialogex.h"

#include "VisionASM.h"
#include <IO.H>

#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"


// CDlgQuickSearchCheckTool 对话框

IMPLEMENT_DYNAMIC(CDlgQuickSearchCheckTool, CDialogEx)

CDlgQuickSearchCheckTool::CDlgQuickSearchCheckTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgQuickSearchCheckTool::IDD, pParent)
	, m_nImageChannel(0)
{
	m_dGrau = 2.0;
	m_dGrau1 = 2.0;
	m_lNoiseThre = 10;
	m_lNoiseThre1 = 10;
	m_lTotalMag = 500;
	m_lTotalMag1 = 500;
	m_bEnableMask = FALSE;

	m_bEnableCheckRegion1 = TRUE;
	m_bEnableCheckRegion2 = TRUE;

	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dAcceptThre2 = 0.6;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_dSizeHigh = 1.0;
	m_dSizeLow = 1.0;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgQuickSearchCheckTool::~CDlgQuickSearchCheckTool()
{
}

void CDlgQuickSearchCheckTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_NOISETHRE1, m_lNoiseThre1);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG1, m_lTotalMag1);
	DDX_Text(pDX, IDC_EDT_GRAU, m_dGrau);
	DDX_Text(pDX, IDC_EDT_GRAU1, m_dGrau1);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	DDX_Text(pDX, IDC_EDT_SIZE_HIGH, m_dSizeHigh);
	DDX_Text(pDX, IDC_EDT_SIZE_LOW, m_dSizeLow);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE_CHECK, m_dAcceptThre2);
	DDX_Check(pDX, IDC_CHECK_REGION1_ENABLE, m_bEnableCheckRegion1);
	DDX_Check(pDX, IDC_CHECK_REGION2_ENABLE, m_bEnableCheckRegion2);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgQuickSearchCheckTool, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgQuickSearchCheckTool::OnBnClickedBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgQuickSearchCheckTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgQuickSearchCheckTool::OnBnClickedBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgQuickSearchCheckTool::OnBnClickedChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, &CDlgQuickSearchCheckTool::OnBnClickedBtnSetCenter)
	ON_EN_SETFOCUS(IDC_EDT_GRAU, &CDlgQuickSearchCheckTool::OnSetfocusEdtGrau)
	ON_EN_KILLFOCUS(IDC_EDT_GRAU, &CDlgQuickSearchCheckTool::OnKillfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, &CDlgQuickSearchCheckTool::OnSetfocusEdtNoisethre)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, &CDlgQuickSearchCheckTool::OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, &CDlgQuickSearchCheckTool::OnSetfocusEdtSearchNum)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgQuickSearchCheckTool::OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgQuickSearchCheckTool::OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgQuickSearchCheckTool::OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_SIZE_LOW, &CDlgQuickSearchCheckTool::OnSetfocusEdtSizeLow)
	ON_EN_SETFOCUS(IDC_EDT_SIZE_HIGH, &CDlgQuickSearchCheckTool::OnSetfocusEdtSizeHigh)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgQuickSearchCheckTool::OnBnClickedBtnFullImageSearch)
	ON_EN_SETFOCUS(IDC_EDT_GRAU1, &CDlgQuickSearchCheckTool::OnSetfocusEdtGrau1)
	ON_EN_KILLFOCUS(IDC_EDT_GRAU1, &CDlgQuickSearchCheckTool::OnKillfocusEdtGrau1)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE1, &CDlgQuickSearchCheckTool::OnSetfocusEdtNoisethre1)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG1, &CDlgQuickSearchCheckTool::OnSetfocusEdtTotalMag1)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE_CHECK, &CDlgQuickSearchCheckTool::OnEnSetfocusEdtAcceptThreCheck)
	ON_BN_CLICKED(IDC_CHECK_REGION1_ENABLE, &CDlgQuickSearchCheckTool::OnBnClickedCheckRegion1Enable)
	ON_BN_CLICKED(IDC_CHECK_REGION2_ENABLE, &CDlgQuickSearchCheckTool::OnBnClickedCheckRegion2Enable)
END_MESSAGE_MAP()


// CDlgQuickSearchCheckTool 消息处理程序

void CDlgQuickSearchCheckTool::UpdateControlDisplay()
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

void CDlgQuickSearchCheckTool::UpdateTrainData(bool bValidate)
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
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel; 
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau; 
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau1 = m_dGrau1; 
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre1 = m_lNoiseThre1;
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag1 = m_lTotalMag1;
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;

		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion1 = m_bEnableCheckRegion1;
		((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion2 = m_bEnableCheckRegion2;

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);	
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel	= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_dGrau			= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
		m_dGrau1		= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau1;
		m_lNoiseThre	= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
		m_lNoiseThre1	= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre1;
		m_lTotalMag		= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
		m_lTotalMag1	= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag1;
		m_bEnableMask	= ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

		m_bEnableCheckRegion1 = ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion1;
		m_bEnableCheckRegion2 = ((CQuickSearchCheckTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableCheckRegion2;

		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}

		((CButton*)GetDlgItem(IDC_CHECK_REGION1_ENABLE))->SetCheck(m_bEnableCheckRegion1);
		((CButton*)GetDlgItem(IDC_CHECK_REGION2_ENABLE))->SetCheck(m_bEnableCheckRegion2);
	

		UpdateData(FALSE);	
	}
}

void CDlgQuickSearchCheckTool::UpdateSearchData(bool bValidate)
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

		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2 = m_dAcceptThre2;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_dAngleLow;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_dAngleHigh;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow = m_dSizeLow;
		((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh = m_dSizeHigh;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		m_nSearchNum	= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dAcceptThre2	= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2;
		m_dAngleLow		= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
		m_dSizeLow		= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow;
		m_dSizeHigh		= ((CQuickSearchCheckSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh;

		UpdateData(FALSE);
	}

}

void CDlgQuickSearchCheckTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_QUICKSEARCH_CHECK_TOOL") == strDlgID)
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

void CDlgQuickSearchCheckTool::CheckDlgDataValidation()
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
	if ((m_dAcceptThre2 < 0.0) || (m_dAcceptThre2 > 1.0))
	{
		m_dAcceptThre2 = 0.8;
	}
	if ((m_dGrau < 1.0) || (m_dGrau > 19.0))
	{
		m_dGrau = 2.0;
	}
	if ((m_dGrau1 < 1.0) || (m_dGrau1 > 19.0))
	{
		m_dGrau1 = 1.0;
	}
	if ((m_lNoiseThre < 1) || (m_lNoiseThre > 255))
	{
		m_lNoiseThre = 10;
	}
	if ((m_lNoiseThre1 < 1) || (m_lNoiseThre1 > 255))
	{
		m_lNoiseThre1 = 10;
	}
	if (m_lTotalMag < 0)
	{
		m_lTotalMag = 500;
	}
	if (m_lTotalMag1 < 0)
	{
		m_lTotalMag1 = 500;
	}
	if (m_dAngleLow > m_dAngleHigh)
	{
		m_dAngleLow = -5.0;
		m_dAngleHigh = 5.0;
	}

	if (m_dSizeLow < 0.8 || m_dSizeLow > 1.2)
	{
		m_dSizeLow = 1.0;	
	}

	if (m_dSizeHigh < 0.8 || m_dSizeHigh > 1.2)
	{
		m_dSizeHigh = 1.0;
	}

	if (m_dSizeLow > m_dSizeHigh)
	{
		m_dSizeLow = 1.0;
		m_dSizeHigh = 1.0;
	}

	UpdateData(FALSE);	
}


void CDlgQuickSearchCheckTool::OnBnClickedBtnGetTrainImage()
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


void CDlgQuickSearchCheckTool::OnBnClickedBtnTrain()
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
		//m_pDlgParent->m_pSearchTool->GetPatternImage(trainImage);
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);

		cpImage trainImageCheck1;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage2(trainImageCheck1);
		//m_pDlgParent->m_pSearchTool->GetPatternImage2(trainImageCheck1);
		m_GuiPatternDisplayCheck1.RemoveImage();
		m_GuiPatternDisplayCheck1.SetImage(trainImageCheck1, TRUE);

		cpImage trainImageCheck2;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage3(trainImageCheck2);
		//m_pDlgParent->m_pSearchTool->GetPatternImage3(trainImageCheck2);
		m_GuiPatternDisplayCheck2.RemoveImage();
		m_GuiPatternDisplayCheck2.SetImage(trainImageCheck2, TRUE);
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


void CDlgQuickSearchCheckTool::OnBnClickedBtnSetMaskImage()
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


void CDlgQuickSearchCheckTool::OnBnClickedChkEnableMask()
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

BOOL CDlgQuickSearchCheckTool::OnInitDialog()
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

	m_GuiPatternDisplayCheck1.Create(IDC_STC_PATTERN_DISPLAY_CHECK_1, this, 0);
	m_GuiPatternDisplayCheck1.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	m_GuiPatternDisplayCheck2.Create(IDC_STC_PATTERN_DISPLAY_CHECK_2, this, 0);
	m_GuiPatternDisplayCheck2.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	UpdateDialogLanguage();

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgQuickSearchCheckTool::OnBnClickedBtnSetCenter()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CQuickSearchCheckTrainGuiParam *pTrainGuiParam = new CQuickSearchCheckTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint.GetY();
		scRect rcTrain = pTrainGuiParam->m_TrainAffineRect.BoundingBox();
		imageTrain.GetSubImage(imagePattern, rcTrain);
		dMarkX -= rcTrain.GetUL().GetX();
		dMarkY -= rcTrain.GetUL().GetY();

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
			pTrainGuiParam->m_ModelPoint.SetX(dMarkX+rcTrain.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint.SetY(dMarkY+rcTrain.GetUL().GetY());
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


void CDlgQuickSearchCheckTool::OnSetfocusEdtGrau()
{
	// TODO: 在此添加控件通知处理程序代码

	TRACE(_T("CDlgQuickSearchTool EdtGrau SetFocus\n"));
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, (WPARAM)GetDlgItem(IDC_EDT_GRAU)->GetSafeHwnd(), NULL);
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


void CDlgQuickSearchCheckTool::OnKillfocusEdtGrau()
{
	// TODO: 在此添加控件通知处理程序代码

	TRACE(_T("CDlgQuickSearchTool EdtGrau KillFocus\n"));
}

void CDlgQuickSearchCheckTool::OnSetfocusEdtGrau1()
{
	// TODO: 在此添加控件通知处理程序代码

	TRACE(_T("CDlgQuickSearchTool EdtGrau1 SetFocus\n"));
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, (WPARAM)GetDlgItem(IDC_EDT_GRAU1)->GetSafeHwnd(), NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_GRAU1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_GRAU1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_GRAU1, strOutput);
		}
	}
	
}


void CDlgQuickSearchCheckTool::OnKillfocusEdtGrau1()
{
	// TODO: 在此添加控件通知处理程序代码

	TRACE(_T("CDlgQuickSearchTool EdtGrau1 KillFocus\n"));
}


void CDlgQuickSearchCheckTool::OnSetfocusEdtNoisethre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_NOISETHRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_NOISETHRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_NOISETHRE, strOutput);
		}
	}
}

void CDlgQuickSearchCheckTool::OnSetfocusEdtNoisethre1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_NOISETHRE1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_NOISETHRE1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_NOISETHRE1, strOutput);
		}
	}
}



void CDlgQuickSearchCheckTool::OnSetfocusEdtTotalMag()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TOTAL_MAG)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TOTAL_MAG, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TOTAL_MAG, strOutput);
		}
	}
}

void CDlgQuickSearchCheckTool::OnSetfocusEdtTotalMag1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TOTAL_MAG1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TOTAL_MAG1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TOTAL_MAG1, strOutput);
		}
	}
}



void CDlgQuickSearchCheckTool::OnSetfocusEdtSearchNum()
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


void CDlgQuickSearchCheckTool::OnSetfocusEdtAcceptThre()
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

void CDlgQuickSearchCheckTool::OnEnSetfocusEdtAcceptThreCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ACCEPT_THRE_CHECK)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ACCEPT_THRE_CHECK, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_ACCEPT_THRE_CHECK, strOutput);
		}
	}
}


void CDlgQuickSearchCheckTool::OnSetfocusEdtAngleLow()
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


void CDlgQuickSearchCheckTool::OnSetfocusEdtAngleHigh()
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


void CDlgQuickSearchCheckTool::OnSetfocusEdtSizeLow()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_SIZE_LOW)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_SIZE_LOW, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_SIZE_LOW, strOutput);
		}
	}
}


void CDlgQuickSearchCheckTool::OnSetfocusEdtSizeHigh()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_SIZE_HIGH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_SIZE_HIGH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_SIZE_HIGH, strOutput);
		}
	}
}


void CDlgQuickSearchCheckTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CQuickSearchCheckTrainGuiParam trainGuiParam;
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


BOOL CDlgQuickSearchCheckTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CDlgQuickSearchCheckTool::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (message)
	{
	case WM_SETFOCUS:
		{
			TRACE(_T("CDlgQuickSearchCheckTool WM_SETFOCUS\n"));
		}
		break;
	case WM_KILLFOCUS:
		{
			TRACE(_T("CDlgQuickSearchCheckTool WM_KILLFOCUS\n"));
		}
		break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CDlgQuickSearchCheckTool::OnBnClickedCheckRegion1Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableCheckRegion1 = !m_bEnableCheckRegion1;

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgQuickSearchCheckTool::OnBnClickedCheckRegion2Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableCheckRegion2 = !m_bEnableCheckRegion2;

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}
