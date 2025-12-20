// CDlgAreaInspect 消息处理程序

// DlgPatternSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgAreaInspect.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternSearchTool dialog


CDlgAreaInspect::CDlgAreaInspect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAreaInspect::IDD, pParent)
    , m_bEnableBaseSearch(TRUE)
	, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgPatternSearchTool)
	m_nBinaryThre = 80;
	m_nMinAcceptArea = 3;
	m_nMaxAcceptArea = 100000;
	m_dMaxDefectArea = 50.0;
	m_dDefectArea    = 0.0;
	m_nMaxGreyVal    = 0;
	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_nDefectType = 1;

    m_dAcceptThre = 0.6;
    m_lNoiseThre = 10;
    m_lTotalMag = 500;
    m_dGrau = 2.0;
    m_bEnableMask = FALSE;
    m_dAngleHigh = 5.0;
    m_dAngleLow = -5.0;

}


void CDlgAreaInspect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatternSearchTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_COMBO_DEFECT_COLOR_TYPE, m_comboDefectType);
	DDX_Text(pDX, IDC_EDT_BINARY_THRE, m_nBinaryThre);
	DDX_Text(pDX, IDC_EDT_MIN_AREA, m_nMinAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_AREA, m_nMaxAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_DEFECT_AREA, m_dMaxDefectArea);
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA, m_dDefectArea);
	DDX_Text(pDX, IDC_EDT_MAX_GREYVAL, m_nMaxGreyVal);
	//DDX_Control(pDX, IDC_BTN_SET_MASK_IMAGE, m_btnSetMask);
	//DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
	DDX_Text(pDX, IDC_EDT_ACCEPT_THRE, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDT_NOISETHRE, m_lNoiseThre);
	DDX_Text(pDX, IDC_EDT_TOTAL_MAG, m_lTotalMag);
	DDX_Text(pDX, IDC_EDT_GRAU, m_dGrau);
	DDX_Check(pDX, IDC_CHK_ENABLE_MASK, m_bEnableMask);
	DDX_Text(pDX, IDC_EDT_ANGLE_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDT_ANGLE_LOW, m_dAngleLow);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_ENABLE_BASE_SEARCH, m_bEnableBaseSearch);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgAreaInspect, CDialog)
	//{{AFX_MSG_MAP(CDlgPatternSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFECT_COLOR_TYPE, OnSelchangeComboDefectType)	
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDT_BINARY_THRE, &CDlgAreaInspect::OnEnSetfocusEdtBinaryThre)
	ON_EN_SETFOCUS(IDC_EDT_MIN_AREA, &CDlgAreaInspect::OnEnSetfocusEdtMinArea)
	ON_EN_SETFOCUS(IDC_EDT_MAX_AREA, &CDlgAreaInspect::OnEnSetfocusEdtMaxArea)
	ON_EN_SETFOCUS(IDC_EDT_MAX_DEFECT_AREA, &CDlgAreaInspect::OnEnSetfocusEdtMaxDefectArea)
    ON_EN_SETFOCUS(IDC_EDT_GRAU, &CDlgAreaInspect::OnEnSetfocusEdtGrau)
    ON_EN_SETFOCUS(IDC_EDT_NOISETHRE, &CDlgAreaInspect::OnEnSetfocusEdtNoisethre)
    ON_EN_SETFOCUS(IDC_EDT_TOTAL_MAG, &CDlgAreaInspect::OnEnSetfocusEdtTotalMag)
    ON_EN_SETFOCUS(IDC_EDT_ACCEPT_THRE, &CDlgAreaInspect::OnEnSetfocusEdtAcceptThre)
    ON_EN_SETFOCUS(IDC_EDT_ANGLE_LOW, &CDlgAreaInspect::OnEnSetfocusEdtAngleLow)
    ON_EN_SETFOCUS(IDC_EDT_ANGLE_HIGH, &CDlgAreaInspect::OnEnSetfocusEdtAngleHigh)
    ON_BN_CLICKED(IDC_CHK_ENABLE_MASK, &CDlgAreaInspect::OnBnClickedChkEnableMask)
    ON_BN_CLICKED(IDC_BTN_SET_MASK_IMAGE, &CDlgAreaInspect::OnBnClickedBtnSetMaskImage)
    ON_BN_CLICKED(IDC_BTN_SET_CENTER, &CDlgAreaInspect::OnBnClickedBtnSetCenter)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_BASE_SEARCH, &CDlgAreaInspect::OnBnClickedCheckEnableBaseSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternSearchTool message handlers
void CDlgAreaInspect::UpdateControlDisplay()
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
        if (m_bEnableBaseSearch)
        {
            GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
        }
        
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(TRUE);
	}
    if (m_pDlgParent->m_pSearchTool != NULL)
    {
        if (m_pDlgParent->m_imageTrain.IsValid())
        {
            if (m_bEnableBaseSearch)
            {
                GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(TRUE);
                GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(m_bEnableMask);
            }
            else
            {
				GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
            }
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

void CDlgAreaInspect::UpdateTrainData(bool bValidate)
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
		m_nDefectType = m_comboDefectType.GetCurSel()+1;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel= m_nImageChannel;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType= m_nDefectType;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre = m_nBinaryThre;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea = m_nMinAcceptArea/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea  =  m_nMaxAcceptArea;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea = m_dMaxDefectArea;

        ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBaseSearch = m_bEnableBaseSearch; 
        ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_dGrau; 
        ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_lNoiseThre;
        ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_lTotalMag;
        ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nDefectType = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType;
		m_nBinaryThre = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre;
		m_nMinAcceptArea	    = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea;
		m_nMaxAcceptArea	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea;
		m_dMaxDefectArea = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea;	

		if (((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
		}
		else
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
		}

		m_dDefectArea = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_dDefectArea;
		m_nMaxGreyVal = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_nMaxGreyVal;

		m_comboDefectType.SetCurSel(m_nDefectType-1);

        m_bEnableBaseSearch	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBaseSearch;
        m_dGrau			= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
        m_lNoiseThre	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
        m_lTotalMag		= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
        m_bEnableMask	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;
        if (m_bEnableMask && m_bEnableBaseSearch)
        {
            GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
        }
        if (m_bEnableBaseSearch)
        {
            GetDlgItem(IDC_EDT_GRAU)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_NOISETHRE)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_TOTAL_MAG)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_ANGLE_HIGH)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_ANGLE_LOW)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_ACCEPT_THRE)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_EDT_GRAU)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_NOISETHRE)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_TOTAL_MAG)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_ANGLE_HIGH)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_ANGLE_LOW)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_ACCEPT_THRE)->EnableWindow(FALSE);
        }

		UpdateData(FALSE);
	}
}

void CDlgAreaInspect::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
    m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		m_nDefectType = m_comboDefectType.GetCurSel()+1;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType= m_nDefectType;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre = m_nBinaryThre;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea = m_nMinAcceptArea/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea  =  m_nMaxAcceptArea;
		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea = m_dMaxDefectArea;

        ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
        ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_dAngleLow;
        ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_dAngleHigh;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
        m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nDefectType = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType;
		m_nBinaryThre = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre;
		m_nMinAcceptArea	    = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea;
		m_nMaxAcceptArea	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea;
		m_dMaxDefectArea = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea;	

// 		if (((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
// 		{
// 			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
// 		}
// 
// 		m_dDefectArea = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_dDefectArea;
// 		m_nMaxGreyVal = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_nMaxGreyVal;

		m_comboDefectType.SetCurSel(m_nDefectType-1);

        m_dAcceptThre	= ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
        m_dAngleLow		= ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
        m_dAngleHigh	= ((CAreaInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
		UpdateData(FALSE);
	}

}

void CDlgAreaInspect::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
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

		if(_T("IDD_DLG_AREAINSPECT_TOOL") == strDlgID)
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

void CDlgAreaInspect::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
	if (m_nDefectType < 1 || m_nDefectType>2)
	{
		m_nDefectType = 1;
	}
	if ((m_nBinaryThre < 0) || (m_nBinaryThre > 255))
	{
		m_nBinaryThre = 80;
	}

	if (m_nMinAcceptArea <0 || m_nMinAcceptArea>=m_nMaxAcceptArea || m_dMaxDefectArea < m_nMinAcceptArea)
	{
		m_nMinAcceptArea = 3;
	}

	if (m_nMaxAcceptArea <0 || m_nMinAcceptArea>=m_nMaxAcceptArea)
	{
		m_nMaxAcceptArea = 100000;
	}
	if (m_dMaxDefectArea < m_nMinAcceptArea || m_dMaxDefectArea < 0)
	{
		m_dMaxDefectArea = 50.0;

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


	UpdateData(FALSE);
}

void CDlgAreaInspect::OnBtnGetTrainImage() 
{
	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	UpdateData(TRUE);
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;

	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}

void CDlgAreaInspect::OnBtnTrain() 
{
	// TODO: Add your control notification handler code here
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
	if (!m_pDlgParent->m_imageTrain.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_FAILED);		//_T("训练失败");
		m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_INVALID_TRAIN_IMAGE);	//_T("训练图像数据为空");
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
        if (m_bEnableBaseSearch)
        {
            m_GuiPatternDisplay.SetImage(trainImage, TRUE);
        }
		m_pDlgParent->UpdateStatusBar(true, m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}






BOOL CDlgAreaInspect::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置按钮

	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings; 

	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
	}



	//m_GuiPatternDisplay.Create(IDC_STC_PATTERN_DISPLAY, this, 0);
	//m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));

// 	switch (lgidLanguage)
// 	{
// 	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
// 		m_GuiPatternDisplay.SetLanguage(0);
// 		break;
// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
// 		m_GuiPatternDisplay.SetLanguage(1);
// 		break;
// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
// 	default:
// 		m_GuiPatternDisplay.SetLanguage(1);
// 		break;
//	}
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	m_comboDefectType.InsertString(0, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_WHITE));
	m_comboDefectType.InsertString(1, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_BLACK));
	m_comboDefectType.SetCurSel(m_nDefectType-1);

    m_GuiPatternDisplay.Create(IDC_STC_PATTERN_DISPLAY, this, 0);
    m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAreaInspect::OnSelchangeComboDefectType() 
{
	m_nDefectType = m_comboDefectType.GetCurSel();	
}





// void CDlgAreaInspect::OnSetfocusEdtConfusionThre() 
// {
// 	if (m_pDlgParent->m_bSysKeyboardEnabled)
// 	{
// 		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDT_CONFUSION_THRE)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDT_CONFUSION_THRE, strInput);
// 
// 		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDT_CONFUSION_THRE, strOutput);
// 		}
// 	}
// }

void CDlgAreaInspect::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CAreaInspectTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
        if (m_bEnableBaseSearch)
        {
            trainGuiParam.m_BaseSearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
        }
        else
        {
		    trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
        }
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_FULL_IMAGE_SEARCH_SUCCEED_AND_RETRAINED);	//_T("全图搜索设置成功，模板已重新训练");
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_FULL_IMAGE_SEARCH_FAILED);	//_T("全图搜索设置失败");
				m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_PATTERN_RETRAIN_FAILED);		//_T("模板重新训练失败");
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}

BOOL CDlgAreaInspect::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAreaInspect::OnEnSetfocusEdtBinaryThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BINARY_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BINARY_THRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BINARY_THRE, strOutput);
		}
	}
}


void CDlgAreaInspect::OnEnSetfocusEdtMinArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MIN_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MIN_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MIN_AREA, strOutput);
		}
	}
}


void CDlgAreaInspect::OnEnSetfocusEdtMaxArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_AREA, strOutput);
		}
	}
}


void CDlgAreaInspect::OnEnSetfocusEdtMaxDefectArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_DEFECT_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_DEFECT_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_DEFECT_AREA, strOutput);
		}
	}
}

void CDlgAreaInspect::UpdateSearchResult()
{

	if (((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
	}
	else
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
	}

	m_dDefectArea = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_dDefectArea;
	m_nMaxGreyVal = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_nMaxGreyVal;


	UpdateData(FALSE);
}


void CDlgAreaInspect::OnEnSetfocusEdtGrau()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[速度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }

        }
    }
}


void CDlgAreaInspect::OnEnSetfocusEdtNoisethre()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[对比度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
        }
    }
}


void CDlgAreaInspect::OnEnSetfocusEdtTotalMag()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[边缘强度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
        }
    }
}


void CDlgAreaInspect::OnEnSetfocusEdtAcceptThre()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[相似度]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
        }
    }
}


void CDlgAreaInspect::OnEnSetfocusEdtAngleLow()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[角度范围][下限]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
        }
    }
}


void CDlgAreaInspect::OnEnSetfocusEdtAngleHigh()
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
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[角度范围][上限]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,6);
            }
        }
    }
}


void CDlgAreaInspect::OnBnClickedChkEnableMask()
{
    // TODO: 在此添加控件通知处理程序代码
    BOOL bEnableMaskOld = m_bEnableMask;
    UpdateData(TRUE);

    if (bEnableMaskOld != m_bEnableMask)
    {
        CString strLog;
        CString strValue = m_bEnableMask ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[掩模]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,6);
    }
    //CString strLog;
    //strLog.Format(_T("修改[缩放范围][上限]由%s到%s\n"),strInput,strOutput);
    //g_opLogFile.SaveLogText(strLog,5);

    if (m_bEnableMask && m_bEnableBaseSearch)
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


void CDlgAreaInspect::OnBnClickedBtnSetMaskImage()
{
    // TODO: 在此添加控件通知处理程序代码
    // TODO: Add your control notification handler code here
    if (!m_pDlgParent->m_pSearchTool)
    {
        return;
    }

   /* CDlgMaskEdit dlgMask;
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

 /*      if (m_bEnableMask)
       {
         	m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
       }*/
    }
}


void CDlgAreaInspect::OnBnClickedBtnSetCenter()
{
    // TODO: 在此添加控件通知处理程序代码
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
            scRect rcTrain = pTrainGuiParam->m_TrainAffineRect1.BoundingBox();
            imageTrain.GetSubImage(imagePattern, rcTrain);
            dMarkX -= rcTrain.GetUL().GetX();
            dMarkY -= rcTrain.GetUL().GetY();

           
			CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);	
            dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
            dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
            if (dlgPatternCenterSet.DoModal() == IDOK)
            {
                dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
                pTrainGuiParam->m_ModelPoint1.SetX(dMarkX+rcTrain.GetUL().GetX());
                pTrainGuiParam->m_ModelPoint1.SetY(dMarkY+rcTrain.GetUL().GetY());
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
            CAreaInspectTrainGuiParam *pTrainGuiParam = new CAreaInspectTrainGuiParam;
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

    }
    else
    {
        m_pDlgParent->m_bStatusBarFlag = FALSE;
        m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_CENTER_SET_DIALOG_FAILED);	//_T("打开提取界面失败");
        m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NOT_TRAINED);			//_T("定位工具未训练");
        m_pDlgParent->UpdateStatusBar();
    }
}


void CDlgAreaInspect::OnBnClickedCheckEnableBaseSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
    ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBaseSearch = m_bEnableBaseSearch;
    m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
    OnBtnGetTrainImage();
    if (m_bEnableMask && m_bEnableBaseSearch)
    {
        GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
    }
    if (m_bEnableBaseSearch)
    {
        GetDlgItem(IDC_EDT_GRAU)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_NOISETHRE)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_TOTAL_MAG)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_ANGLE_HIGH)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_ANGLE_LOW)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_ACCEPT_THRE)->EnableWindow(TRUE);
        
    }
    else
    {
        GetDlgItem(IDC_EDT_GRAU)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_NOISETHRE)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_TOTAL_MAG)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_ENABLE_MASK)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_ANGLE_HIGH)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_ANGLE_LOW)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_ACCEPT_THRE)->EnableWindow(FALSE);
    }
    

}
