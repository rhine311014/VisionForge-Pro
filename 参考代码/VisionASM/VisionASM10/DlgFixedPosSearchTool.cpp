// DlgFixedPosSearchToolSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgFixedPosSearchTool.h"
#include "DlgSearch.h"
#include "DlgPatternCenterSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedPosSearchTool dialog


CDlgFixedPosSearchTool::CDlgFixedPosSearchTool(CWnd* pParent /*=NULL*/)
: CDialog(CDlgFixedPosSearchTool::IDD, pParent)
, m_bEnableSecondModelPoint(FALSE)
, m_nFixedPosMode2(1)
, m_nImageChannel(0)
{
	//{{AFX_DATA_INIT(CDlgFixedPosSearchTool)
	m_nFixedPosMode = 1;
	//}}AFX_DATA_INIT
	
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}


void CDlgFixedPosSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFixedPosSearchTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_CENTER, m_btnSetCenter);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Radio(pDX, IDC_RAD_FIXEDPOS_MODE0, m_nFixedPosMode);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_ENABLE_MARK2, m_bEnableSecondModelPoint);
	DDX_Radio(pDX, IDC_RAD_FIXEDPOS2_MODE0, m_nFixedPosMode2);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgFixedPosSearchTool, CDialog)
//{{AFX_MSG_MAP(CDlgFixedPosSearchTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_RAD_FIXEDPOS_MODE0, OnRadFixedposMode0)
	ON_BN_CLICKED(IDC_RAD_FIXEDPOS_MODE1, OnRadFixedposMode1)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, OnBtnSetCenter)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RAD_FIXEDPOS2_MODE0, &CDlgFixedPosSearchTool::OnBnClickedRadFixedpos2Mode0)
	ON_BN_CLICKED(IDC_RAD_FIXEDPOS2_MODE1, &CDlgFixedPosSearchTool::OnBnClickedRadFixedpos2Mode1)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER2, &CDlgFixedPosSearchTool::OnBnClickedBtnSetCenter2)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MARK2, &CDlgFixedPosSearchTool::OnBnClickedChkEnableMark2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedPosSearchTool message handlers

void CDlgFixedPosSearchTool::UpdateControlDisplay()
{	
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
        
		// Mali 2013.8.16
		if (m_nFixedPosMode == 0)
		{
			GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
		}

		//ZhangRui 20191128
		if(!m_bEnableSecondModelPoint)
		{
			GetDlgItem(IDC_RAD_FIXEDPOS2_MODE0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RAD_FIXEDPOS2_MODE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_RAD_FIXEDPOS2_MODE0)->EnableWindow(TRUE);
			GetDlgItem(IDC_RAD_FIXEDPOS2_MODE1)->EnableWindow(TRUE);
			/*if(m_nFixedPosMode2 == 0)
			{
				GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(TRUE);
			}*/
		}
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

void CDlgFixedPosSearchTool::UpdateTrainData(bool bValidate)
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
		((CFixedPosSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel=m_nImageChannel;
		((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableSecondModelPoint=m_bEnableSecondModelPoint;
		((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FixedPosMode = (CFixedPosMode)m_nFixedPosMode;
		((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FixedPosMode2 = (CFixedPosMode)m_nFixedPosMode2;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CFixedPosSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_bEnableSecondModelPoint = ((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableSecondModelPoint;
		m_nFixedPosMode = ((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FixedPosMode;
		m_nFixedPosMode2 = ((CFixedPosSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FixedPosMode2;
		UpdateData(FALSE);
	}
}

void CDlgFixedPosSearchTool::UpdateSearchData(bool bValidate)
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

void CDlgFixedPosSearchTool::UpdateDialogLanguage()
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
		
		if(_T("IDD_DLG_FIXEDPOSSEARCH_TOOL") == strDlgID)
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

void CDlgFixedPosSearchTool::OnBtnGetTrainImage() 
{
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
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}

void CDlgFixedPosSearchTool::OnBtnTrain() 
{
	// TODO: Add your control notification handler code here
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
		CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}

void CDlgFixedPosSearchTool::OnRadFixedposMode0() 
{
	CString strLog;
	strLog.Format(_T("[固定点模式]选择[图像中心]"));
	g_opLogFile.SaveLogText(strLog, 6);

	UpdateTrainData(true);
	UpdateSearchData(true);
	UpdateData(TRUE);
	if (m_nFixedPosMode == 0)
	{
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
	}
}

void CDlgFixedPosSearchTool::OnRadFixedposMode1() 
{
	CString strLog;
	strLog.Format(_T("[固定点模式]选择[任意点]"));
	g_opLogFile.SaveLogText(strLog, 6);

	UpdateTrainData(true);
	UpdateSearchData(true);
	UpdateData(TRUE);
	if (m_nFixedPosMode == 0)
	{
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_CENTER)->EnableWindow(TRUE);
	}
}


void CDlgFixedPosSearchTool::OnBnClickedRadFixedpos2Mode0()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[固定点2模式]选择[图像中心]"));
	g_opLogFile.SaveLogText(strLog, 6);

	UpdateTrainData(true);
	UpdateSearchData(true);
	UpdateData(TRUE);
	if (m_nFixedPosMode2 == 0)
	{
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(TRUE);
	}
}


void CDlgFixedPosSearchTool::OnBnClickedRadFixedpos2Mode1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[固定点模式]选择[任意点]"));
	g_opLogFile.SaveLogText(strLog, 6);

	UpdateTrainData(true);
	UpdateSearchData(true);
	UpdateData(TRUE);
	if (m_nFixedPosMode2 == 0)
	{
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(TRUE);
	}
}




BOOL CDlgFixedPosSearchTool::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	
	// 设置按钮
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_btnSetCenter.SetThemeHelper(&m_themeHelper);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFixedPosSearchTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here

	UpdateData(FALSE);

}


void CDlgFixedPosSearchTool::OnBtnSetCenter() 
{
	CString strLog;
	strLog.Format(_T("点击[提取标点]\n"));
	g_opLogFile.SaveLogText(strLog, 6);

	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CFixedPosSearchTrainGuiParam *pTrainGuiParam = new CFixedPosSearchTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint.GetY();
		//zzc add begin[在提取标记点窗口可提取多个标记点]
		scRect rcTrain = scRect(0, 0, imageTrain.Width(), imageTrain.Height());
		sc2Vector modelPointRoi(dMarkX, dMarkY);	//ROI参考点1相对坐标
		std::vector<sc2Vector> vecModelPointRoi;
		vecModelPointRoi.push_back(modelPointRoi);
		if (m_bEnableSecondModelPoint && m_nFixedPosMode2 != 0)	//参考点2为任意点
		{
			double dMarkX2 = pTrainGuiParam->m_ModelPoint2.GetX();	//参考点2X zzc add
			double dMarkY2 = pTrainGuiParam->m_ModelPoint2.GetY();	//参考点2Y
			dMarkX2 -= rcTrain.GetUL().GetX();	
			dMarkY2 -= rcTrain.GetUL().GetY();	
			sc2Vector modelPointRoi2(dMarkX2, dMarkY2);
			vecModelPointRoi.push_back(modelPointRoi2);
		}
		if (!imageTrain.IsValid() || vecModelPointRoi.empty() || rcTrain.IsEmpty())
		{
			return;
		} 
		CDlgPatternCenterSet dlgPatternCenterSet(imageTrain, vecModelPointRoi, imageTrain, rcTrain, TRUE, this);
		//zzc add end
		//CDlgPatternCenterSet dlgPatternCenterSet(imageTrain, dMarkX, dMarkY);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			std::vector<sc2Vector> vecModelPointResult;	// zzc 多标记点提取后的结果，vector的第一个元素为第一个参考点，第二个元素为辅助参考点
			int nType = 0;	//默认时，标记点为在RIO选点
			//dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			dlgPatternCenterSet.GetPatternCenter(vecModelPointResult, nType);	//zzc 提取的标记点结果为vector
			pTrainGuiParam->m_ModelPoint.SetX(vecModelPointResult[0].GetX());	//zzc 结果容器中第一个元素对应第一个参考点
			pTrainGuiParam->m_ModelPoint.SetY(vecModelPointResult[0].GetY());
			if (m_bEnableSecondModelPoint && m_nFixedPosMode2 != 0)
			{
				pTrainGuiParam->m_ModelPoint2.SetX(vecModelPointResult[1].GetX());	//zzc 结果容器中第二个元素对应第二个参考点
				pTrainGuiParam->m_ModelPoint2.SetY(vecModelPointResult[1].GetY());	//zzc 结果容器中第二个元素对应第二个参考点
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

BOOL CDlgFixedPosSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}




void CDlgFixedPosSearchTool::OnBnClickedBtnSetCenter2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[提取标点]\n"));
	g_opLogFile.SaveLogText(strLog, 6);

	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CFixedPosSearchTrainGuiParam *pTrainGuiParam = new CFixedPosSearchTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint2.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint2.GetY();

		CDlgPatternCenterSet dlgPatternCenterSet(imageTrain, dMarkX, dMarkY);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			pTrainGuiParam->m_ModelPoint2.SetX(dMarkX);
			pTrainGuiParam->m_ModelPoint2.SetY(dMarkY);
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


void CDlgFixedPosSearchTool::OnBnClickedChkEnableMark2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);
	UpdateControlDisplay();
}
