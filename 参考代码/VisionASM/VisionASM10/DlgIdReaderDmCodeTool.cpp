// DlgIdReaderDmCodeToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgIdReaderDmCodeTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include <IO.H>
#include "LogFileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIdReaderDmCodeTool Dialog

CDlgIdReaderDmCodeTool::CDlgIdReaderDmCodeTool(CWnd* pParent /*=NULL*/)
: CDialog(CDlgIdReaderDmCodeTool::IDD, pParent)
, m_nDecodeMethod(0)
, m_nAppMode(0)
, m_nDiscreteFlag(0)
, m_nDistortionFlag(0)
, m_nMirrorMode(0)
, m_nPolarityMode(0)
, m_nRectangleFlag(0)
, m_nCodeMaxNum(0)
, m_nMinBarSize(0)
, m_nMaxBarSize(0)
, m_nSampleLevel(0)
, m_nTimeOut(0)
, m_nDebugFlag(0)
, m_nAdvanceParam(0)
, m_nAdvanceParam2(0)
, m_nPatSpeedMode(0)
, m_nCodeType(0)
, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgIdReaderDmCodeTool)
	// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}


void CDlgIdReaderDmCodeTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DEFALUT, m_nDecodeMethod);
	DDX_Radio(pDX, IDC_RADIO_APP_MODE_NORMAL, m_nAppMode);
	DDX_Radio(pDX, IDC_RADIO_CONTINUOUS, m_nDiscreteFlag);
	DDX_Radio(pDX, IDC_RADIO_DISTORTIONCLOSE, m_nDistortionFlag);
	DDX_Radio(pDX, IDC_RADIO_MIRRORCLOSE, m_nMirrorMode);
	DDX_Radio(pDX, IDC_RADIO_BLACKONWHITE, m_nPolarityMode);
	DDX_Radio(pDX, IDC_RADIO_SQUARE, m_nRectangleFlag);
	DDX_Text(pDX, IDC_EDIT_CODEMAXNUM, m_nCodeMaxNum);
	DDX_Text(pDX, IDC_EDIT_MINBARSIZE, m_nMinBarSize);
	DDX_Text(pDX, IDC_EDIT_MAXBARSIZE, m_nMaxBarSize);
	DDX_Text(pDX, IDC_EDIT_SAMPLELEVEL, m_nSampleLevel);
	DDX_Text(pDX, IDC_EDIT_WAITTIME, m_nTimeOut);
	DDX_Text(pDX, IDC_EDIT_DEBUG_FLAG, m_nDebugFlag);
	DDX_Text(pDX, IDC_EDIT_ADVANCE_PARAM, m_nAdvanceParam);
	DDX_Text(pDX, IDC_EDIT_ADVANCE_PARAM2, m_nAdvanceParam2);
	DDX_Control(pDX, IDC_COMBO_SPEED_MODE, m_comboSpeed);
	//{{AFX_DATA_MAP(CDlgIdReaderDmCodeTool)
	DDX_Radio(pDX, IDC_RADIO__DM_QR_TYPE, m_nCodeType);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgIdReaderDmCodeTool, CDialog)
//{{AFX_MSG_MAP(CDlgIdReaderDmCodeTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_BN_CLICKED(IDC_RADIO_DEFALUT, OnBnClickedRadioDecodeMethod)
	ON_BN_CLICKED(IDC_RADIO_VSB, OnBnClickedRadioDecodeMethod)
	ON_BN_CLICKED(IDC_RADIO_APP_MODE_NORMAL, OnBnClickedRadioAppMode)
	ON_BN_CLICKED(IDC_RADIO_APP_MODE_PRO, OnBnClickedRadioAppMode)
	ON_BN_CLICKED(IDC_RADIO_APP_MODE_FAST, OnBnClickedRadioAppMode)
	ON_BN_CLICKED(IDC_RADIO_CONTINUOUS, OnBnClickedRadioDiscreteFlag)
	ON_BN_CLICKED(IDC_RADIO_DISPERSE, OnBnClickedRadioDiscreteFlag)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnBnClickedRadioDiscreteFlag)
	ON_BN_CLICKED(IDC_RADIO_DISTORTIONCLOSE, OnBnClickedRadioDistortionFlag)
	ON_BN_CLICKED(IDC_RADIO_DISTORTIONOPEN, OnBnClickedRadioDistortionFlag)
	ON_BN_CLICKED(IDC_RADIO_MIRRORCLOSE, OnBnClickedRadioMirrorMode)
	ON_BN_CLICKED(IDC_RADIO_MIRROROPEN, OnBnClickedRadioMirrorMode)
	ON_BN_CLICKED(IDC_RADIO_MIRRORALL, OnBnClickedRadioMirrorMode)
	ON_BN_CLICKED(IDC_RADIO_BLACKONWHITE, OnBnClickedRadioPolarityMode)
	ON_BN_CLICKED(IDC_RADIO_WHITEONBLACK, OnBnClickedRadioPolarityMode)
	ON_BN_CLICKED(IDC_RADIO_BWALL, OnBnClickedRadioPolarityMode)
	ON_BN_CLICKED(IDC_RADIO_SQUARE, OnBnClickedRadioRectangleFlag)
	ON_BN_CLICKED(IDC_RADIO_RECTANGLE, OnBnClickedRadioRectangleFlag)
	ON_BN_CLICKED(IDC_RADIO_RECTALL, OnBnClickedRadioRectangleFlag)
	ON_EN_SETFOCUS(IDC_EDIT_CODEMAXNUM, OnEnSetfocusEditCodemaxnum)
	ON_EN_KILLFOCUS(IDC_EDIT_CODEMAXNUM, OnEnKillfocusEditCodemaxnum)
	ON_EN_SETFOCUS(IDC_EDIT_MINBARSIZE, OnEnSetfocusEditMinbarsize)
	ON_EN_KILLFOCUS(IDC_EDIT_MINBARSIZE, OnEnKillfocusEditMinbarsize)
	ON_EN_SETFOCUS(IDC_EDIT_MAXBARSIZE, OnEnSetfocusEditMaxbarsize)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXBARSIZE, OnEnKillfocusEditMaxbarsize)
	ON_EN_SETFOCUS(IDC_EDIT_SAMPLELEVEL, OnEnSetfocusEditSamplelevel)
	ON_EN_KILLFOCUS(IDC_EDIT_SAMPLELEVEL, OnEnKillfocusEditSamplelevel)
	ON_EN_SETFOCUS(IDC_EDIT_WAITTIME, OnEnSetfocusEditWaittime)
	ON_EN_KILLFOCUS(IDC_EDIT_WAITTIME, OnEnKillfocusEditWaittime)
	ON_EN_SETFOCUS(IDC_EDIT_DEBUG_FLAG, OnEnSetfocusEditDebugFlag)
	ON_EN_KILLFOCUS(IDC_EDIT_DEBUG_FLAG, OnEnKillfocusEditDebugFlag)
	ON_EN_SETFOCUS(IDC_EDIT_ADVANCE_PARAM, OnEnSetfocusEditAdvanceParam)
	ON_EN_KILLFOCUS(IDC_EDIT_ADVANCE_PARAM, OnEnKillfocusEditAdvanceParam)
	ON_EN_SETFOCUS(IDC_EDIT_ADVANCE_PARAM2, OnEnSetfocusEditAdvanceParam2)
	ON_EN_KILLFOCUS(IDC_EDIT_ADVANCE_PARAM2, OnEnKillfocusEditAdvanceParam2)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_MODE, OnSelchangeComboSpeedMode)

	ON_BN_CLICKED(IDC_RADIO__DM_QR_TYPE2, &CDlgIdReaderDmCodeTool::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO__DM_QR_TYPE,&CDlgIdReaderDmCodeTool::OnBnClickedRadio)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIdReaderDmCodeTool message handlers

void CDlgIdReaderDmCodeTool::UpdateControlDisplay()
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

			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()== eIdReaderDmCodeTool)
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
			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()== eIdReaderDmCodeTool)
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
		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
	
}

void CDlgIdReaderDmCodeTool::UpdateTrainData(bool bValidate)
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
		((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCodeType = m_nCodeType;
		((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;

		// QR码时 目前只采用VSB
		if(m_nCodeType==1)
		{
			m_nDecodeMethod = 1;
		}

		if(m_nCodeType ==0)
		{
			FindDMCodePara &findDMCodePara = (((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindDMCodePara);
			findDMCodePara.m_methodDecode = (DecodeMethod)m_nDecodeMethod;
			findDMCodePara.m_modeApp = (AppMode)m_nAppMode;
			findDMCodePara.m_flagDiscrete = (DiscreteFlag)m_nDiscreteFlag;
			findDMCodePara.m_flagDistortion = (DistortionFlag)m_nDistortionFlag;
			findDMCodePara.m_modeMirror = (MirrorMode)m_nMirrorMode;
			findDMCodePara.m_modePolarity = (PolarityMode)m_nPolarityMode;
			findDMCodePara.m_flagRectangle = (RectangleFlag)m_nRectangleFlag;
			findDMCodePara.m_nMaxCodeNum = m_nCodeMaxNum;
			findDMCodePara.m_nMinBarSize = m_nMinBarSize;
			findDMCodePara.m_nMaxBarSize = m_nMaxBarSize;
			findDMCodePara.m_nSampleLevel = m_nSampleLevel;
			findDMCodePara.m_nWaitingTime = m_nTimeOut;
			findDMCodePara.m_nDebugFlag = m_nDebugFlag;
			findDMCodePara.m_nAdvanceParam = m_nAdvanceParam;
			findDMCodePara.m_nAdvanceParam2 = m_nAdvanceParam2;
		}
		else if(m_nCodeType ==1)
		{
			FindQRCodePara &findQRCodePara = (((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindQRCodePara);
			findQRCodePara.m_methodDecode = (DecodeMethod)m_nDecodeMethod;
			findQRCodePara.m_modeApp = (AppMode)m_nAppMode;
			findQRCodePara.m_flagDiscrete = (DiscreteFlag)m_nDiscreteFlag;
			findQRCodePara.m_flagDistortion = (DistortionFlag)m_nDistortionFlag;
			findQRCodePara.m_modeMirror = (MirrorMode)m_nMirrorMode;
			findQRCodePara.m_modePolarity = (PolarityMode)m_nPolarityMode;
			findQRCodePara.m_flagRectangle = (RectangleFlag)m_nRectangleFlag;
			findQRCodePara.m_nMaxCodeNum = m_nCodeMaxNum;
			findQRCodePara.m_nMinBarSize = m_nMinBarSize;
			findQRCodePara.m_nMaxBarSize = m_nMaxBarSize;
			findQRCodePara.m_nSampleLevel = m_nSampleLevel;
			findQRCodePara.m_nWaitingTime = m_nTimeOut;
			findQRCodePara.m_nDebugFlag = m_nDebugFlag;
			findQRCodePara.m_nAdvanceParam = m_nAdvanceParam;
			findQRCodePara.m_nAdvanceParam2 = m_nAdvanceParam2;
		}


		

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);		
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nPatSpeedMode = ((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_nCodeType = ((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nCodeType;
		m_nImageChannel = ((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		if(m_nCodeType==0)
		{
			FindDMCodePara &findDMCodePara = (((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindDMCodePara);
			m_nDecodeMethod = findDMCodePara.m_methodDecode;
			m_nAppMode = findDMCodePara.m_modeApp;
			m_nDiscreteFlag = findDMCodePara.m_flagDiscrete;
			m_nDistortionFlag = findDMCodePara.m_flagDistortion;
			m_nMirrorMode = findDMCodePara.m_modeMirror;
			m_nPolarityMode = findDMCodePara.m_modePolarity;
			m_nRectangleFlag = findDMCodePara.m_flagRectangle;
			m_nCodeMaxNum = findDMCodePara.m_nMaxCodeNum ;
			m_nMinBarSize = findDMCodePara.m_nMinBarSize;
			m_nMaxBarSize = findDMCodePara.m_nMaxBarSize;
			m_nSampleLevel = findDMCodePara.m_nSampleLevel;
			m_nTimeOut = findDMCodePara.m_nWaitingTime;
			m_nDebugFlag = findDMCodePara.m_nDebugFlag;
			m_nAdvanceParam = findDMCodePara.m_nAdvanceParam;
			m_nAdvanceParam2 = findDMCodePara.m_nAdvanceParam2;
		}
		else if(m_nCodeType==1)
		{
			FindQRCodePara &findQRCodePara = (((CIdReaderDmCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindQRCodePara);
			m_nDecodeMethod = findQRCodePara.m_methodDecode;
			m_nAppMode = findQRCodePara.m_modeApp;
			m_nDiscreteFlag = findQRCodePara.m_flagDiscrete;
			m_nDistortionFlag = findQRCodePara.m_flagDistortion;
			m_nMirrorMode = findQRCodePara.m_modeMirror;
			m_nPolarityMode = findQRCodePara.m_modePolarity;
			m_nRectangleFlag = findQRCodePara.m_flagRectangle;
			m_nCodeMaxNum = findQRCodePara.m_nMaxCodeNum ;
			m_nMinBarSize = findQRCodePara.m_nMinBarSize;
			m_nMaxBarSize = findQRCodePara.m_nMaxBarSize;
			m_nSampleLevel = findQRCodePara.m_nSampleLevel;
			m_nTimeOut = findQRCodePara.m_nWaitingTime;
			m_nDebugFlag = findQRCodePara.m_nDebugFlag;
			m_nAdvanceParam = findQRCodePara.m_nAdvanceParam;
			m_nAdvanceParam2 = findQRCodePara.m_nAdvanceParam2;
		}

		// QR码时 目前只采用VSB
		if(m_nCodeType==1)
		{
			m_nDecodeMethod = 1;
			GetDlgItem(IDC_RADIO_DEFALUT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_RADIO_DEFALUT)->EnableWindow(TRUE);
		}
		m_comboSpeed.SetCurSel(m_nPatSpeedMode);
		UpdateData(FALSE);
	}
	
}

void CDlgIdReaderDmCodeTool::UpdateSearchData(bool bValidate)
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
		
		
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
		
	}
	else // 设置界面值
	{
		
		UpdateData(FALSE);
	}
	
}

void CDlgIdReaderDmCodeTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_TOOL_ID_READER_DM_CODE_TOOL") == strDlgID)
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

void CDlgIdReaderDmCodeTool::OnBtnGetTrainImage() 
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

	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();
    CString strLog;
    strLog.Format(_T("点击[获取训练图像]\n"));
    g_opLogFile.SaveLogText(strLog,6);
	UpdateControlDisplay();
}

void CDlgIdReaderDmCodeTool::OnBtnTrain() 
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
	

	if (m_pDlgParent->m_pSearchTool->Train())
	{
		cpImage trainImage;
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED));
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
    CString strLog;
    strLog.Format(_T("点击[训练]\n"));
    g_opLogFile.SaveLogText(strLog,6);
}


BOOL CDlgIdReaderDmCodeTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	m_comboSpeed.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_ETHAL));
	m_comboSpeed.SetCurSel(m_nPatSpeedMode);

	FindDMCodePara findDMCodePara;
	m_nDecodeMethod = eDecodeMethod_Vsb;
	m_nAppMode = findDMCodePara.m_modeApp;
	m_nDiscreteFlag = findDMCodePara.m_flagDiscrete;
	m_nDistortionFlag = findDMCodePara.m_flagDistortion;
	m_nMirrorMode = findDMCodePara.m_modeMirror;
	m_nPolarityMode = findDMCodePara.m_modePolarity;
	m_nRectangleFlag = findDMCodePara.m_flagRectangle;
	m_nCodeMaxNum = findDMCodePara.m_nMaxCodeNum ;
	m_nMinBarSize = findDMCodePara.m_nMinBarSize;
	m_nMaxBarSize = findDMCodePara.m_nMaxBarSize;
	m_nSampleLevel = findDMCodePara.m_nSampleLevel;
	m_nTimeOut = findDMCodePara.m_nWaitingTime;
	m_nDebugFlag = findDMCodePara.m_nDebugFlag;
	m_nAdvanceParam = findDMCodePara.m_nAdvanceParam;
	m_nAdvanceParam2 = findDMCodePara.m_nAdvanceParam2;
	UpdateData(FALSE);
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgIdReaderDmCodeTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);

	if (m_nCodeMaxNum <1)
	{
		m_nCodeMaxNum = 1;
	}

	if (m_nCodeMaxNum >100)
	{
		m_nCodeMaxNum = 100;
	}

	if (m_nMinBarSize <20)
	{
		m_nMinBarSize = 20;
	}
	if (m_nMinBarSize >10000)
	{
		m_nMinBarSize = 10000;
	}

	if (m_nMaxBarSize < m_nMinBarSize)
	{
		m_nMaxBarSize = m_nMinBarSize;
	}

	if (m_nMaxBarSize <20)
	{
		m_nMaxBarSize = 20;
	}

	if (m_nMaxBarSize >10000)
	{
		m_nMaxBarSize = 10000;
	}


	if (m_nSampleLevel <1)
	{
		m_nSampleLevel = 1;
	}

	if (m_nSampleLevel >50)
	{
		m_nSampleLevel = 50;
	}

	if (m_nTimeOut <1)
	{
		m_nTimeOut = 1;
	}

	if (m_nTimeOut >10000)
	{
		m_nTimeOut = 10000;
	}
	
	if(m_nAdvanceParam<0)
	{
		m_nAdvanceParam = 0;
	}

	if(m_nAdvanceParam2<0)
	{
		m_nAdvanceParam2 = 0;
	}

	UpdateData(FALSE);
}


void CDlgIdReaderDmCodeTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eIdReaderDmCodeTool)
		{

			CIdReaderDmCodeTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_TrainRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
	}
}

BOOL CDlgIdReaderDmCodeTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgIdReaderDmCodeTool::OnBnClickedRadioDecodeMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nDecodeMethod;

	UpdateData();

}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioAppMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nAppMode;

	UpdateData();

}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioDiscreteFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nDiscreteFlag;

	UpdateData();

}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioDistortionFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nDistortionFlag;

	UpdateData();


}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioMirrorMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nMirrorMode;

	UpdateData();

}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioPolarityMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nPolarityMode;

	UpdateData();

}

void CDlgIdReaderDmCodeTool::OnBnClickedRadioRectangleFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nRectangleFlag;

	UpdateData();

}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditCodemaxnum()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_CODEMAXNUM, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_CODEMAXNUM)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_CODEMAXNUM, m_strOutput);
		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditCodemaxnum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_CODEMAXNUM, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditMinbarsize()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_MINBARSIZE, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_MINBARSIZE)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_MINBARSIZE, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditMinbarsize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_MINBARSIZE, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditMaxbarsize()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_MAXBARSIZE, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_MAXBARSIZE)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_MAXBARSIZE, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditMaxbarsize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_MAXBARSIZE, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditSamplelevel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SAMPLELEVEL)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditSamplelevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditWaittime()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_WAITTIME, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_WAITTIME)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_WAITTIME, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditWaittime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_WAITTIME, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditDebugFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_DEBUG_FLAG, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_DEBUG_FLAG)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_DEBUG_FLAG, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditDebugFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_DEBUG_FLAG, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditAdvanceParam()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_ADVANCE_PARAM, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_ADVANCE_PARAM)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_ADVANCE_PARAM, m_strOutput);


		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditAdvanceParam()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_ADVANCE_PARAM, m_strOutput);


	}
}


void CDlgIdReaderDmCodeTool::OnEnSetfocusEditAdvanceParam2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_ADVANCE_PARAM2, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		GetDlgItem(IDC_EDIT_ADVANCE_PARAM2)->GetWindowRect(&rcWnd);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_ADVANCE_PARAM2, m_strOutput);

		}
	}
}


void CDlgIdReaderDmCodeTool::OnEnKillfocusEditAdvanceParam2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_ADVANCE_PARAM2, m_strOutput);


	}
}

void CDlgIdReaderDmCodeTool::OnSelchangeComboSpeedMode() 
{
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}


void CDlgIdReaderDmCodeTool::OnBnClickedRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_nCodeType==1)
	{
		m_nDecodeMethod = 1;
		GetDlgItem(IDC_RADIO_DEFALUT)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_DEFALUT)->EnableWindow(TRUE);
	}

}
