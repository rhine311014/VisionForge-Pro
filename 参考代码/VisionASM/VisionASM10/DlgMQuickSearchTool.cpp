// DlgMQuickSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgMQuickSearchTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMQuickSearchTool dialog


CDlgMQuickSearchTool::CDlgMQuickSearchTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMQuickSearchTool::IDD, pParent)
	, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgMQuickSearchTool)
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_lNoiseThre = 10;
	m_lTotalMag = 500;
	m_dGrau = 2.0;
	m_bEnableMask = FALSE;
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_dSizeHigh = 1.0;
	m_dSizeLow = 1.0;

	m_bCoordSortEnable = FALSE;
	m_dCoordSortGap = 64;
	m_nCoordSortMethod = 0;
	m_nCoordSortOrderX = 0;
	m_nCoordSortOrderY = 0;
	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;

}


void CDlgMQuickSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMQuickSearchTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_SEARCH_NUM, m_nSearchNum);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Text(pDX, IDC_EDT_GRAU, m_dGrau);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	DDX_Text(pDX, IDC_EDT_SIZE_HIGH, m_dSizeHigh);
	DDX_Text(pDX, IDC_EDT_SIZE_LOW, m_dSizeLow);
	DDX_Check(pDX, IDC_CHK_ENABLE_MARK2, m_bEnableMark2);
	DDX_Check(pDX, IDC_CHK_COORD_SORT_ENABLE, m_bCoordSortEnable);
	DDX_Text(pDX, IDC_EDT_COORD_SORT_GAP2, m_dCoordSortGap);
	DDX_Control(pDX, IDC_COMBO_COORD_SORT_METHOD, m_comboCoordSortMethod);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_X_DE, m_nCoordSortOrderX);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_Y_DE, m_nCoordSortOrderY);
	//}}AFX_DATA_MAP

	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgMQuickSearchTool, CDialog)
	//{{AFX_MSG_MAP(CDlgMQuickSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, OnBtnSetMaskImage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, OnChkEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, OnBtnSetCenter)
	ON_EN_SETFOCUS(IDC_EDT_GRAU, OnSetfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, OnSetfocusEdtNoisethre)
	ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, OnSetfocusEdtTotalMag)
	ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, OnSetfocusEdtAcceptThre)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, OnSetfocusEdtAngleLow)
	ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, OnSetfocusEdtAngleHigh)
	ON_EN_SETFOCUS(IDC_EDT_SIZE_LOW, OnSetfocusEdtSizeLow)
	ON_EN_SETFOCUS(IDC_EDT_SIZE_HIGH, OnSetfocusEdtSizeHigh)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_EN_KILLFOCUS(IDC_EDT_GRAU, OnKillfocusEdtGrau)
	ON_EN_SETFOCUS(IDC_EDT_SEARCH_NUM, OnSetfocusEdtSearchNum)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_ENABLE_MARK2, &CDlgMQuickSearchTool::OnBnClickedChkEnableMark2)
	ON_CBN_SELCHANGE(IDC_COMBO_COORD_SORT_METHOD, &CDlgMQuickSearchTool::OnCbnSelchangeComboCoordSortMethod)
	ON_EN_SETFOCUS(IDC_EDT_COORD_SORT_GAP2, &CDlgMQuickSearchTool::OnEnSetfocusEdtCoordSortGap2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMQuickSearchTool message handlers

void CDlgMQuickSearchTool::UpdateControlDisplay()
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

void CDlgMQuickSearchTool::UpdateTrainData(bool bValidate)
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
		((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel; 
		((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau; 
		((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
		((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
		((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		
		((CMQuickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bModelPoint2Visible = m_bEnableMark2;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);	
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel	= ((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_dGrau			= ((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
		m_lNoiseThre	= ((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
		m_lTotalMag		= ((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
		m_bEnableMask	= ((CMQuickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
		}

		m_bEnableMark2 = ((CMQuickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bModelPoint2Visible;
		UpdateData(FALSE);	


	}
}

void CDlgMQuickSearchTool::UpdateSearchData(bool bValidate)
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
		
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_dAngleLow;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_dAngleHigh;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow = m_dSizeLow;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh = m_dSizeHigh;

		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable = m_bCoordSortEnable;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod = (CoordSortMethod)m_nCoordSortMethod;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap = m_dCoordSortGap;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX = (SortOrder)m_nCoordSortOrderX;
		((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY =  (SortOrder)m_nCoordSortOrderY;
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		m_nSearchNum	= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre	= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
		m_dAngleLow		= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
		m_dSizeLow		= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeLow;
		m_dSizeHigh	= ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dSizeHigh;

		m_bCoordSortEnable = ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable;
		m_nCoordSortMethod = ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod;
		m_dCoordSortGap = ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap;
		m_nCoordSortOrderX = ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX;
		m_nCoordSortOrderY = ((CMQuickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY;
		
		m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);
		UpdateData(FALSE);
	}

}

void CDlgMQuickSearchTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_MQUICKSEARCH_TOOL") == strDlgID)
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

void CDlgMQuickSearchTool::CheckDlgDataValidation()
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
	if ((m_dGrau < 1.0) || (m_dGrau > 19.0))
	{
		m_dGrau = 2.0;
	}
	if ((m_lNoiseThre < 1) || (m_lNoiseThre > 255))
	{
		m_lNoiseThre = 10;
	}
	if (m_lTotalMag < 0)
	{
		m_lTotalMag = 500;
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

	if (m_dCoordSortGap < 0)
	{
		m_dCoordSortGap = 64;
	}
	UpdateData(FALSE);	
}

void CDlgMQuickSearchTool::OnBtnGetTrainImage() 
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

void CDlgMQuickSearchTool::OnBtnTrain() 
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
		//m_pDlgParent->m_pSearchTool->GetPatternImage(trainImage);
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
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

void CDlgMQuickSearchTool::OnBtnSetMaskImage() 
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

void CDlgMQuickSearchTool::OnChkEnableMask() 
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

BOOL CDlgMQuickSearchTool::OnInitDialog() 
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
	
	m_comboCoordSortMethod.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_X)/*_T("按X坐标排序")*/);
	m_comboCoordSortMethod.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_Y)/*_T("按Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_XFIRST_Y)/*_T("先X后Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_YFIRST_X)/*_T("先Y后X坐标排序")*/);
	m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMQuickSearchTool::OnBtnSetCenter() 
{
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CMQuickSearchTrainGuiParam *pTrainGuiParam = new CMQuickSearchTrainGuiParam;
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
		if (pTrainGuiParam->m_bModelPoint2Visible)
		{
			double dMarkX2 = pTrainGuiParam->m_ModelPoint2.GetX();	//参考点2X zzc add
			double dMarkY2 = pTrainGuiParam->m_ModelPoint2.GetY();	//参考点2Y
			dMarkX2 -= rcTrain.GetUL().GetX();	
			dMarkY2 -= rcTrain.GetUL().GetY();	
			sc2Vector modelPointRoi2(dMarkX2, dMarkY2);
			vecModelPointRoi.push_back(modelPointRoi2);
		}
		if (!imagePattern.IsValid() || !imageTrain.IsValid() || vecModelPointRoi.empty() || rcTrain.IsEmpty())
		{
			return;
		} 
		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, vecModelPointRoi, imageTrain, rcTrain, TRUE, this);
		//CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);	
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			std::vector<sc2Vector> vecModelPointResult;	// 多标记点提取后的结果，vector的第一个元素为第一个参考点，第二个元素为辅助参考点
			int nType = 0;	//默认时，标记点为在RIO选点
			//dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			dlgPatternCenterSet.GetPatternCenter(vecModelPointResult, nType);
			pTrainGuiParam->m_ModelPoint.SetX(vecModelPointResult[0].GetX() + rcTrain.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint.SetY(vecModelPointResult[0].GetY() + rcTrain.GetUL().GetY());
			if (pTrainGuiParam->m_bModelPoint2Visible)
			{
				pTrainGuiParam->m_ModelPoint2.SetX(vecModelPointResult[1].GetX() + rcTrain.GetUL().GetX());
				pTrainGuiParam->m_ModelPoint2.SetY(vecModelPointResult[1].GetY() + rcTrain.GetUL().GetY());
			}
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

void CDlgMQuickSearchTool::OnSetfocusEdtGrau() 
{
	TRACE(_T("CDlgMQuickSearchTool EdtGrau SetFocus\n"));
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

void CDlgMQuickSearchTool::OnKillfocusEdtGrau() 
{
	TRACE(_T("CDlgMQuickSearchTool EdtGrau KillFocus\n"));
}

void CDlgMQuickSearchTool::OnSetfocusEdtNoisethre() 
{
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

void CDlgMQuickSearchTool::OnSetfocusEdtTotalMag() 
{
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

void CDlgMQuickSearchTool::OnSetfocusEdtSearchNum() 
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

void CDlgMQuickSearchTool::OnSetfocusEdtAcceptThre() 
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

void CDlgMQuickSearchTool::OnSetfocusEdtAngleLow() 
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

void CDlgMQuickSearchTool::OnSetfocusEdtAngleHigh() 
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



void CDlgMQuickSearchTool::OnSetfocusEdtSizeLow() 
{
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

void CDlgMQuickSearchTool::OnSetfocusEdtSizeHigh() 
{
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

void CDlgMQuickSearchTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CMQuickSearchTrainGuiParam trainGuiParam;
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

BOOL CDlgMQuickSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgMQuickSearchTool::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_SETFOCUS:
		{
			TRACE(_T("CDlgMQuickSearchTool WM_SETFOCUS\n"));
		}
		break;
	case WM_KILLFOCUS:
		{
			TRACE(_T("CDlgMQuickSearchTool WM_KILLFOCUS\n"));
		}
		break;
	default:
		break;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}


void CDlgMQuickSearchTool::OnBnClickedChkEnableMark2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableMark2 = !m_bEnableMark2;
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	((CMQuickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bModelPoint2Visible = m_bEnableMark2;

	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
}


void CDlgMQuickSearchTool::OnCbnSelchangeComboCoordSortMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCoordSortMethod = m_comboCoordSortMethod.GetCurSel();
}


void CDlgMQuickSearchTool::OnEnSetfocusEdtCoordSortGap2()
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
