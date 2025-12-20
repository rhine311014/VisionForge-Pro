// DlgDmCodeCalibrateTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgDmCodeCalibrateTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// CDlgDmCodeCalibrateTool 对话框

IMPLEMENT_DYNAMIC(CDlgDmCodeCalibrateTool, CDialog)

CDlgDmCodeCalibrateTool::CDlgDmCodeCalibrateTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDmCodeCalibrateTool::IDD, pParent)
	, m_nDmCodeMaxNum(1)
	, m_nDecodeMaxTime(1000)
	, m_dGridThre(16)
	, m_dGridSize(1)
	, m_bRecode(FALSE)
	, m_bShowOutputPoints(FALSE)
	, m_nDecodeMethod(0)
	, m_nDmCodeMirror(2)
	, m_nSampleLevel(1)
	, m_nMinBarSize(20)
	, m_nMaxBarSize(500)
	, m_nImageChannel(0)
{
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgDmCodeCalibrateTool::~CDlgDmCodeCalibrateTool()
{
}

void CDlgDmCodeCalibrateTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEMAXNUM, m_nDmCodeMaxNum);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEMAXTIME, m_nDecodeMaxTime);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEGRIDTHRE, m_dGridThre);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEGRIDSIZE, m_dGridSize);
	DDX_Check(pDX, IDC_CHECK_DMCODEREDECODE, m_bRecode);
	DDX_Check(pDX, IDC_CHECK_DMCODEOUTPOINT, m_bShowOutputPoints);
	DDX_Radio(pDX, IDC_RDO_DMCODE_METHOD_DEFAULT, m_nDecodeMethod);
	DDX_Radio(pDX, IDC_RDO_MIRROR_MODE_CLOSE, m_nDmCodeMirror);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_LEVEL, m_nSampleLevel);
	DDX_Text(pDX, IDC_EDIT_MIN_BAR_SIZE, m_nMinBarSize);
	DDX_Text(pDX, IDC_EDIT_MAX_BAR_SIZE, m_nMaxBarSize);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgDmCodeCalibrateTool, CDialog)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgDmCodeCalibrateTool::OnBnClickedBtnGetTrainImage)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEMAXNUM, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodemaxnum)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEMAXTIME, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodemaxtime)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEGRIDTHRE, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodegridthre)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEGRIDSIZE, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodegridsize)
	ON_BN_CLICKED(IDC_BTN_DMCODE_FULLIMAGE, &CDlgDmCodeCalibrateTool::OnBnClickedBtnDmcodeFullimage)
	ON_BN_CLICKED(IDC_BTN_CORNERS_FULLIMAGE, &CDlgDmCodeCalibrateTool::OnBnClickedBtnCornersFullimage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgDmCodeCalibrateTool::OnBnClickedBtnTrain)
	ON_EN_SETFOCUS(IDC_EDIT_SAMPLE_LEVEL, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditSampleLevel)
	ON_EN_SETFOCUS(IDC_EDIT_MIN_BAR_SIZE, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditMinBarSize)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_BAR_SIZE, &CDlgDmCodeCalibrateTool::OnEnSetfocusEditMaxBarSize)
	ON_BN_CLICKED(IDC_RDO_DMCODE_METHOD_DEFAULT, &CDlgDmCodeCalibrateTool::OnBnClickedRdoDmcodeMethodDefault)
	ON_BN_CLICKED(IDC_RDO_DECODE_METHOD_VSB, &CDlgDmCodeCalibrateTool::OnBnClickedRdoDecodeMethodVsb)
END_MESSAGE_MAP()


// CDlgDmCodeCalibrateTool 消息处理程序


void CDlgDmCodeCalibrateTool::OnBnClickedBtnGetTrainImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[获取训练图像]"));
	g_opLogFile.SaveLogText(strLog, 6);

	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;
	//m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}
void CDlgDmCodeCalibrateTool::UpdateControlDisplay()
{	
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);

	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
	}

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

void CDlgDmCodeCalibrateTool::UpdateTrainData(bool bValidate)
{
	if(!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	if(bValidate)
	{
		UpdateData(TRUE);
		((CDmCodeCalibrateTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
	}
	else
	{
		m_nImageChannel = ((CDmCodeCalibrateTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		UpdateData(FALSE);
	}
	UpdateSearchData(bValidate);
}

void CDlgDmCodeCalibrateTool::UpdateSearchData(bool bValidate)
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
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_dGridSize = m_dGridSize;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_dGridThre = m_dGridThre;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_bOutputOutPoints = m_bShowOutputPoints;

		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxCodeNum = m_nDmCodeMaxNum;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nWaitingTime = m_nDecodeMaxTime;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_modeMirror = (MirrorMode)m_nDmCodeMirror;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_methodDecode = (DecodeMethod)m_nDecodeMethod;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMinBarSize = m_nMinBarSize;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxBarSize = m_nMaxBarSize;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nSampleLevel = m_nSampleLevel;
		((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_bEnableReDecode = m_bRecode;
		
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		m_dGridSize=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_dGridSize;
		m_dGridThre=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_dGridThre;
		m_bShowOutputPoints=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindCornerPara.m_bOutputOutPoints;
		m_nDmCodeMaxNum=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxCodeNum;
		m_nDecodeMaxTime=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nWaitingTime;
		m_nDmCodeMirror=(int)((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_modeMirror;
		m_nDecodeMethod=(int)((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_methodDecode;
		m_nMinBarSize=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMinBarSize;
		m_nMaxBarSize=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxBarSize;
		m_nSampleLevel=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_FindDMCodePara.m_nSampleLevel;
		m_bRecode=((CDmCodeCalibrateSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_DmCodeSearchParam.m_bEnableReDecode;
		//if (m_nDecodeMethod == 0)
		//{
		//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(FALSE);
		//}
		//else
		//{
		//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(TRUE);
		//}
		UpdateData(FALSE);
	}

}

void CDlgDmCodeCalibrateTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_DMCODE_CALIBRATE_TOOL") == strDlgID)
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

void CDlgDmCodeCalibrateTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
	if (m_nDmCodeMaxNum<1)
	{
		m_nDmCodeMaxNum = 1;
	}
	if (m_nDecodeMaxTime<0)
	{
		m_nDecodeMaxTime = 1;
	}
	if (m_dGridSize<0)
	{
		m_dGridSize = 1;
	}
	if (m_dGridThre<0)
	{
		m_dGridSize = 16;
	}
	if (m_nSampleLevel<=0)
	{
		m_nSampleLevel = 1;
	}
	if (m_nMinBarSize<=0)
	{
		m_nMinBarSize = 20;
	}
	if (m_nMaxBarSize<=0)
	{
		m_nMaxBarSize = 500;
	}
	UpdateData(FALSE);

}


BOOL CDlgDmCodeCalibrateTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	// 设置按钮
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	//if (m_nDecodeMethod == 0)
	//{
	//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(FALSE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(TRUE);
	//}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodemaxnum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEMAXNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[识别个数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodemaxtime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEMAXTIME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[解码时间]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodegridthre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEGRIDTHRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[强度阈值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditValDmcodegridsize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEGRIDSIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[单元尺寸]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnBnClickedBtnDmcodeFullimage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CDmCodeCalibrateTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_DmCodeSearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();	
	}
	CString strLog;
	strLog.Format(_T("点击[二维码全图搜索]\n"));
	g_opLogFile.SaveLogText(strLog,6);
}


void CDlgDmCodeCalibrateTool::OnBnClickedBtnCornersFullimage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CDmCodeCalibrateTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_CornersSearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();	
	}
	CString strLog;
	strLog.Format(_T("点击[角点全图搜索]\n"));
	g_opLogFile.SaveLogText(strLog,6);
}


void CDlgDmCodeCalibrateTool::OnBnClickedBtnTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[训练]训练模板"));
	g_opLogFile.SaveLogText(strLog, 6);

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
		m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED));
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditSampleLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[采样水平]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditMinBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最小尺寸]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}


void CDlgDmCodeCalibrateTool::OnEnSetfocusEditMaxBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大尺寸]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,6);
			}
		}
	}
}

void CDlgDmCodeCalibrateTool::OnDmcodeMethodChange()
{
	UpdateData(TRUE);
	//if (m_nDecodeMethod == 0)
	//{
	//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(FALSE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(TRUE);
	//}
}
void CDlgDmCodeCalibrateTool::OnBnClickedRdoDmcodeMethodDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDmcodeMethodChange();
}


void CDlgDmCodeCalibrateTool::OnBnClickedRdoDecodeMethodVsb()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDmcodeMethodChange();
}
