// DlgCalibTargetPos2.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibTargetPos2.h"
#include "afxdialogex.h"
#include "DlgCalibTargetDmCodeParam.h"
#include "DlgCalibTargetShutterSetting.h"
#include "DlgSearch.h"
// CDlgCalibTargetPos2 对话框

IMPLEMENT_DYNAMIC(CDlgCalibTargetPos2, CDialog)

CDlgCalibTargetPos2::CDlgCalibTargetPos2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibTargetPos2::IDD, pParent)
{
	// 状态栏相关
	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	m_psaSysInfoStrings = NULL;

	m_pVisionASM = NULL;
	m_pVAPrevMainWnd = NULL;
	m_vpPrevGuiDisplays.clear();

	m_vpGuiObjectDisplay.clear();
	m_vpGuiTargetDisplay.clear();
	m_vpGuiObjectInteractiveContainer.clear();
	m_vpGuiTargetInteractiveContainer.clear();

	m_bIsGrabbing = FALSE;
	m_bCommGetPlatformAxisPosSuccess = FALSE;
	m_bCalibTargetPosSuccess = FALSE;
//	m_vbCalibTargetPosSuccess.resize(m_pVisionASM->GetPosNum()/2, FALSE);

	m_nEvent = 0;
	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;
    m_bIsDmCodeSearch = FALSE;
	m_colorMovePos1 = RGB(255,0,0);
	m_colorMovePos2 = RGB(0,255,0);

	for(int it=0;it<2;it++)
	{
		dShutterList[it] = 0.0;
	}
	m_bStopUpdateDisplayImage = FALSE;
}

CDlgCalibTargetPos2::~CDlgCalibTargetPos2()
{
    //m_vDmCodeCalibTool.clear();
	int i=0;
	if (m_bIsDmCodeSearch)
	{
		for (i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->RemoveGuiCalibrateGraphicsFromDisplay(i,m_vpGuiObjectDisplay.at(i));
		}
		for (i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->RemoveGuiCalibrateGraphicsFromDisplay(i+m_pVisionASM->GetPosNum()/2,m_vpGuiTargetDisplay.at(i));
		}
	}
	for (i=0; i<m_vpGuiObjectDisplay.size(); i++)
	{
		if (m_vpGuiObjectDisplay.at(i))
		{
			delete m_vpGuiObjectDisplay.at(i);
			m_vpGuiObjectDisplay.at(i) = NULL;
		}
	}

	for (i=0; i<m_vpGuiTargetDisplay.size(); i++)
	{
		if (m_vpGuiTargetDisplay.at(i))
		{
			delete m_vpGuiTargetDisplay.at(i);
			m_vpGuiTargetDisplay.at(i) = NULL;
		}
	}
	

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos0.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos0.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos0.at(i);
			m_vpGuiAlnObjectImageMarkPos0.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos1.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos1.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos1.at(i);
			m_vpGuiAlnObjectImageMarkPos1.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnTargetImageMarkPos0.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos0.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos0.at(i);
			m_vpGuiAlnTargetImageMarkPos0.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnTargetImageMarkPos1.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos1.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos1.at(i);
			m_vpGuiAlnTargetImageMarkPos1.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos2.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos2.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos2.at(i);
			m_vpGuiAlnObjectImageMarkPos2.at(i) = NULL;
		}
	}
	for(i=0; i< m_vpGuiAlnObjectImageMarkPos3.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos3.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos3.at(i);
			m_vpGuiAlnObjectImageMarkPos3.at(i) = NULL;
		}
	}
	for(i=0; i< m_vpGuiAlnTargetImageMarkPos2.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos2.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos2.at(i);
			m_vpGuiAlnTargetImageMarkPos2.at(i) = NULL;
		}
	}
	for(i=0; i< m_vpGuiAlnTargetImageMarkPos3.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos3.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos3.at(i);
			m_vpGuiAlnTargetImageMarkPos3.at(i) = NULL;
		}
	}

	for(i=0; i<m_vpGuiObjectText.size();i++)
	{
		if (m_vpGuiObjectText.at(i) != NULL)
		{
			delete m_vpGuiObjectText.at(i);
			m_vpGuiObjectText.at(i) = NULL;
		}
	}


	for(i=0; i<m_vpGuiTargetText.size();i++)
	{
		if (m_vpGuiTargetText.at(i) != NULL)
		{
			delete m_vpGuiTargetText.at(i);
			m_vpGuiTargetText.at(i) = NULL;
		}
	}

	m_pVisionASM = NULL;
}

void CDlgCalibTargetPos2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COMM_GET_PLATFORM_AXIS_POS, m_btnCommGetPlatformAxisPos);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT1_MOVE_POS1, m_btnSearchObject1Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT2_MOVE_POS2, m_btnSearchObject1Pos2);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET2_MOVE_POS1, m_btnSearchTarget2Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET2_MOVE_POS2, m_btnSearchTarget2Pos2);
	DDX_Control(pDX, IDC_STATIC_NOTE_AXIS, m_labelNote);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_CALIB_TARGET_POS, m_btnCalibrateTargetPos);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibTargetPos2, CDialog)
	ON_BN_CLICKED(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS, &CDlgCalibTargetPos2::OnBnClickedBtnCommGetPlatformAxisPos)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1, &CDlgCalibTargetPos2::OnBnClickedBtnSearchObject1MovePos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT2_MOVE_POS2, &CDlgCalibTargetPos2::OnBnClickedBtnSearchObject2MovePos2)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET2_MOVE_POS1, &CDlgCalibTargetPos2::OnBnClickedBtnSearchTarget2MovePos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET2_MOVE_POS2, &CDlgCalibTargetPos2::OnBnClickedBtnSearchTarget2MovePos2)
	ON_BN_CLICKED(IDC_BTN_CALIB_TARGET_POS, &CDlgCalibTargetPos2::OnBnClickedBtnCalibTargetPos)
	ON_BN_CLICKED(IDOK, &CDlgCalibTargetPos2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalibTargetPos2::OnBnClickedCancel)
	ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_SET_DMCODE_PARAM, &CDlgCalibTargetPos2::OnBnClickedBtnSetDmcodeParam)
	ON_BN_CLICKED(IDC_BUTTON_POS1_CAMERA_SHUTTER, &CDlgCalibTargetPos2::OnBnClickedSetCalibTargetShutter1)
	ON_BN_CLICKED(IDC_BUTTON_POS2_CAMERA_SHUTTER, &CDlgCalibTargetPos2::OnBnClickedSetCalibTargetShutter2)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
END_MESSAGE_MAP()


// CDlgCalibTargetPos2 消息处理程序

void CDlgCalibTargetPos2::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

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

		if(_T("IDD_DLG_CALIB_TARGET_POS2") == strDlgID)
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

void CDlgCalibTargetPos2::InitDlgItem()
{
    if (eDmCodeCalib == m_pVisionASM->GetCalibTargetMarkPatternMode())
    {
        m_bIsDmCodeSearch = TRUE;
    }
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	int nPartWidth[3] = {20, 300, -1};
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_LABEL_NOTE2)/*_T("注:请确保各拍照位置的平台θ轴位置一致！！！")*/);

	m_btnCommGetPlatformAxisPos.SetThemeHelper(&m_themeHelper);
	m_btnSearchObject1Pos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchObject1Pos2.SetThemeHelper(&m_themeHelper);
	m_btnSearchTarget2Pos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchTarget2Pos2.SetThemeHelper(&m_themeHelper);
	m_btnCalibrateTargetPos.SetThemeHelper(&m_themeHelper);
	m_btnOK.SetThemeHelper(&m_themeHelper);
	m_btnCancel.SetThemeHelper(&m_themeHelper);


	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}


	int i = 0;
	for (i=0; i<m_vpGuiObjectDisplay.size(); i++)
	{
		delete m_vpGuiObjectDisplay[i];
		m_vpGuiObjectDisplay[i] = NULL;
	}
	m_vpGuiObjectDisplay.clear();
	m_vpGuiObjectInteractiveContainer.clear();


	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetPosNum()/2; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}

		m_pVisionASM->SetPosDisplay(i, pGUIDisplay);

		m_vpGuiObjectDisplay.push_back(pGUIDisplay);
		m_vpGuiObjectDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}


	for (i=0; i<m_pVisionASM->GetPosNum()/2; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i+m_pVisionASM->GetPosNum()/2, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}

		m_pVisionASM->SetPosDisplay(i+m_pVisionASM->GetPosNum()/2, pGUIDisplay);
		m_vpGuiTargetDisplay.push_back(pGUIDisplay);
		m_vpGuiTargetDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}


	scGuiStaticContainer *pGuiSC = NULL;
	scGuiCoordCross* pGuiCoordCross = NULL;
	scGuiText *pGuiText = NULL;
	// 对象
	CString strKey;
	CString strInfo;
	int nMovePosIndex = 0;
	for(i = 0;  i<m_pVisionASM->GetPosNum()/2; i++)
	{
		nMovePosIndex = 0;
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos0.push_back(pGuiCoordCross);


		nMovePosIndex = 1;
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));	
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos1.push_back(pGuiCoordCross);

		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));

		nMovePosIndex = 2;
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos2.push_back(pGuiCoordCross);


		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));

		nMovePosIndex = 3;
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos3.push_back(pGuiCoordCross);

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_OBJECT_POS)/*_T("对象位置%d")*/,i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiObjectText.push_back(pGuiText);


		pGuiSC = m_vpGuiObjectDisplay.at(i)->GetStaticContainer();
		nMovePosIndex = 0;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos0.at(i), strKey);

		nMovePosIndex = 1;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos1.at(i), strKey);

		nMovePosIndex = 2;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos2.at(i), strKey);

		nMovePosIndex = 3;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos3.at(i), strKey);

		strKey.Format(_T("GuiText%d"),i);
		pGuiSC->AddItem(m_vpGuiObjectText.at(i), strKey);
		m_vpGuiObjectDisplay.at(i)->SetStaticContainer(pGuiSC);
	}

	// 目标
	for(i = 0;  i<m_pVisionASM->GetPosNum()/2; i++)
	{
		nMovePosIndex = 0;
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));	
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos0.push_back(pGuiCoordCross);

		nMovePosIndex = 1;
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos1.push_back(pGuiCoordCross);

		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));

		nMovePosIndex = 2;
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos2.push_back(pGuiCoordCross);


		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));

		nMovePosIndex = 3;
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MOVE_POS)/*_T("移动位置%d")*/,nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos3.push_back(pGuiCoordCross);

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_TARGRE_POS)/*_T("目标位置%d")*/,i + 1 + m_pVisionASM->GetPosNum()/2);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiTargetText.push_back(pGuiText);


		pGuiSC = m_vpGuiTargetDisplay.at(i)->GetStaticContainer();
		nMovePosIndex = 0;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos0.at(i), strKey);

		nMovePosIndex = 1;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos1.at(i), strKey);

		nMovePosIndex = 2;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos2.at(i), strKey);

		nMovePosIndex = 3;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos3.at(i), strKey);

		strKey.Format(_T("GuiTargetText%d"),i);
		pGuiSC->AddItem(m_vpGuiTargetText.at(i), strKey);
		m_vpGuiTargetDisplay.at(i)->SetStaticContainer(pGuiSC);
	}

	m_vbCalibTargetPosSuccess.resize(m_pVisionASM->GetPosNum()/2, FALSE);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	if (sysPlatformInfo.m_bTarObjCamSepNoAxisCalib == TRUE)
	{
		GetDlgItem(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS)->ShowWindow(SW_HIDE);
	}
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		for (i=0; i<sysPlatformInfo.m_nMultiCalibExtensionMaxNum+1; i++)
		{
			CString strExProductIndex;
			strExProductIndex.Format(_T("%d"), i+1);
			m_comboExProductIndex.InsertString(i, strExProductIndex);
		}
		m_comboExProductIndex.SetCurSel(0);

		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
	}
    if (m_bIsDmCodeSearch)
    {
        //m_vDmCodeCalibTool.resize(m_pVisionASM->GetPosNum());
        //for (int i=0;i<m_vDmCodeCalibTool.size()/2;i++)
        //{
        //    m_vDmCodeCalibTool.at(i).SetGuiDisplay(m_vpGuiObjectDisplay.at(i));
        //}
        //for (int i=0;i<m_vDmCodeCalibTool.size()/2;i++)
        //{
        //    m_vDmCodeCalibTool.at(i+m_vDmCodeCalibTool.size()/2).SetGuiDisplay(m_vpGuiTargetDisplay.at(i));
        //}
		for (int i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->AddGuiCalibrateGraphicsToDisplay(i,m_vpGuiObjectDisplay.at(i));
		}
		for (int i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->AddGuiCalibrateGraphicsToDisplay(i+m_pVisionASM->GetPosNum()/2,m_vpGuiTargetDisplay.at(i));
		}

		CTotalProductData *pTotalProductData;
		pTotalProductData = m_pVisionASM->GetTotalProuductData();
		int nCurProductIdx = pTotalProductData->GetCurProductIndex();
		// 获取当前产品对应的标定数据
		CProductData* pProductData;
		pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

		scDMCodeParam DmCodeParam;
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		for (int i=0;i<m_pVisionASM->GetPosNum();i++)
		{
			if(optionInfo.m_bEnableSearchShutter)
			{
				DmCodeParam = pProductData->m_vDMcodeCalib.at(i);
				if (DmCodeParam.m_dShutter >= 0.001 )
				{
					m_pVisionASM->SwitchToSearchShutter(i, DmCodeParam.m_dShutter);
				}
			}
		}
		//for (int i=0;i<m_vDmCodeCalibTool.size();i++)
		//{
		//	m_vDmCodeCalibTool.at(i).SetGuiGraphics(m_pVisionASM->m_vpGuiDmCodeGraphics.at(i));
		//}
        //CTotalCalibData *pTotalCalibData;
        //pTotalCalibData = m_pVisionASM->GetTotalCalibData();
        //int m_nCurProductIdx = pTotalCalibData->GetCurCalibIndex();
        //// 获取当前产品对应的标定数据
        //CCalibData* pCalibData;
        //pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
        //scDMCodeParam DmCodeParam;
        //for (int i=0;i<m_vDmCodeCalibTool.size();i++)
        //{
        //    DmCodeParam = pCalibData->m_vpCalibratedInfo.at(i)->m_DMcodeCalib;
        //    m_vDmCodeCalibTool.at(i).SetDmCodeParam(DmCodeParam);
        //}
    }

    if (m_bIsDmCodeSearch)
    {
        //GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_SEARCH_OBJECT2_MOVE_POS2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_BTN_SEARCH_TARGET2_MOVE_POS1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_SEARCH_TARGET2_MOVE_POS2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_BTN_LOAD_LAST_CALIBMARK)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BTN_SET_DMCODE_PARAM)->ShowWindow(SW_SHOW);

    }
}

void CDlgCalibTargetPos2::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgCalibTargetPos2::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplays.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

	m_pVisionASM = (vcXYDVisionAlign*)pVisionAligner;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplays.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplays.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}

}

void CDlgCalibTargetPos2::OnBnClickedBtnSerialGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=0;
	BOOL bSucceed = TRUE;
	int nCamNum = m_pVisionASM->GetPosNum(); 
	std::vector<BOOL> vbSucceed;
	if (m_bIsGrabbing)
	{
		CString strLog;
		strLog.Format(_T("点击[连续采集]\n"));
		g_opLogFile.SaveLogText(strLog, 2);
		for (i=0; i<nCamNum; i++)
		{
			vbSucceed.push_back(m_pVisionASM->FreezePosCamera(i));
		}
		for (i=0; i<nCamNum; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			for (i=0; i<nCamNum; i++)	
			{
				if (vbSucceed.at(i))
				{
					m_pVisionASM->GrabPosCamera(i);
				}
				else
				{
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CPFAST_STR_POS), i+1);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
				strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			UpdateStatusBar();
			return;
		}

		m_bIsGrabbing = FALSE;
	}
	else
	{
		CString strLog;
		strLog.Format(_T("点击[停止采集]\n"));
		g_opLogFile.SaveLogText(strLog, 2);
		for (i=0; i<nCamNum; i++)
		{
			vbSucceed.push_back(m_pVisionASM->GrabPosCamera(i));
		}
		for (i=0; i<nCamNum; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机采集失败，则恢复所有成功采集的相机为停止状态）
			for (i=0; i<nCamNum; i++)	
			{
				if (vbSucceed.at(i))
				{
					m_pVisionASM->FreezePosCamera(i);
				}
				else
				{
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CPFAST_STR_POS), i+1);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
				strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));

			UpdateStatusBar();

			return;
		}

		m_bIsGrabbing = TRUE;
	}
}


BOOL CDlgCalibTargetPos2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (!m_pVisionASM->IsCommOpen())
	{
		m_pVisionASM->OpenComm();
	}

	InitDlgItem();
	UpdateDialogLanguage();
//	UpdateData(FALSE);

	OnBnClickedBtnSerialGrab();

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

//	UpdateDialogLanguage();

	if(m_bIsDmCodeSearch)
    {
		LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
		if (m_psaSysInfoStrings != NULL)
		{
			lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
		}
        switch (lgidLanguage)
        {
        case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
            GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->SetWindowTextA(_T("Object position1"));
            GetDlgItem(IDC_BTN_SEARCH_TARGET2_MOVE_POS1)->SetWindowTextA(_T("Target position2"));
            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
            GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->SetWindowTextA(_T("搜索对象位置1"));
            GetDlgItem(IDC_BTN_SEARCH_TARGET2_MOVE_POS1)->SetWindowTextA(_T("搜索目标位置2"));
            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
        default:
            GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->SetWindowTextA(_T("搜索对象位置1"));
            GetDlgItem(IDC_BTN_SEARCH_TARGET2_MOVE_POS1)->SetWindowTextA(_T("搜索目标位置2"));
            break;
        }
    }

	if(m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);

		double dMin = 0, dMax = 0,dShutter = 0.0;
		for(int nPos=0;nPos<2 && nPos<m_pVisionASM->GetPosNum();nPos++)
		{
			if (m_pVisionASM->GetPosCameraShutter(nPos, dShutter,dMin,dMax))
			{
				dShutterList[nPos] = dShutter;
			}
			int nCam = m_pVisionASM->GetPosCameraIndex(nPos);
			if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
			{
				double dCalibTargetShutter = vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter;
				// 没有设置关联曝光时;设置为系统曝光值
				if(dCalibTargetShutter<=0.01)
				{
					vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter = dShutter;
					dCalibTargetShutter = dShutter;
				}
				m_pVisionASM->SwitchToSearchShutter(nPos, dCalibTargetShutter);
			}
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CDlgCalibTargetPos2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			//m_pVisionASM->UpdateDisplayImage((int)wParam);
			if(m_bStopUpdateDisplayImage==FALSE)
			{
				m_pVisionASM->UpdateDisplayImage((int)wParam);
			}
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1,m_strStatusBarInfo2);
			UpdateStatusBar();
		}
		break;
	case WM_UPDATE_CALIBRATE_TARGET_GRAPHIC:
		{
			//m_pVisionASM->UpdateAlnObjectGraphics((int)wParam, (BOOL)lParam);
			m_pVisionASM->ShowGuiCalibrateGraphics((int)wParam, (BOOL)lParam);
			int nPosIndex = (int)wParam;
			if ( nPosIndex < m_pVisionASM->GetPosNum()/2)
			{
				m_vpGuiObjectDisplay.at(nPosIndex)->Invalidate();
			}
			else
			{
				m_vpGuiTargetDisplay.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->Invalidate();
			}

		}
		break;
	default:
		break;
	}


	return CDialog::WindowProc(message, wParam, lParam);
}


void CDlgCalibTargetPos2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_bExitThread = TRUE;// 使线程自动退出
	//m_pVisionASM->CloseComm();
	m_pVisionASM->StopReceiveComm();
	if (WaitForSingleObject(m_hEventReceiveThreadExit, WAIT_THREAD_QUIT_TIME) == WAIT_TIMEOUT)
	{
		TerminateThread(m_pThreadReceiveData->m_hThread, 0);
	}
	CloseHandle(m_hEventReceiveThreadExit);


	// 状态栏
	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconFailed = NULL;
	}

	if (m_pVisionASM != NULL)
	{
		if (m_bIsGrabbing)
		{
			m_pVisionASM->FreezeAllCameras();
		}		

		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplays.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

}



void CDlgCalibTargetPos2::OnBnClickedBtnCommGetPlatformAxisPos()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 5; 
	CString strLog;
	strLog.Format(_T("点击[获取平台基准位置]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos2::OnBnClickedBtnSearchObject1MovePos1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 1; // 搜索对象1移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索对象1移动位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}

void CDlgCalibTargetPos2::OnBnClickedBtnSearchTarget2MovePos1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 2; // 搜索目标2移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标2移动位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos2::OnBnClickedBtnSearchObject2MovePos2()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 3; // 搜索对象1移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索对象1移动位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos2::OnBnClickedBtnSearchTarget2MovePos2()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nEvent = 4; // 搜索目标2移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索目标2移动位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos2::OnBnClickedBtnCalibTargetPos()
{
	// TODO: 在此添加控件通知处理程序代码

//	m_nRadioCalibTargetPos = 0;
	CString strLog;
	strLog.Format(_T("点击[标定目标位置]\n"));
	g_opLogFile.SaveLogText(strLog, 2);

	if (m_pVisionASM == NULL)
	{
		return;
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	if (sysPlatformInfo.m_bTarObjCamSepNoAxisCalib == TRUE)
	{
		// 不用取平台当前各轴的绝对位置（基准位置）
	}
	else
	{
		// 1. 获取平台当前各轴的绝对位置（基准位置）成功
		if (!m_bCommGetPlatformAxisPosSuccess)
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_AXIS_POS);//.Format(_T("请获取平台轴位置！"));

			UpdateStatusBar();

			return;
		}
	}
	int nExProductIndex = 0;
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}
//	if (m_nRadioCalibTargetPos == 0)			// 同时标定目标位置
    if (m_bIsDmCodeSearch)
    {
        for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
        {
            if (FALSE == m_pVisionASM->m_vDmCodeCalibTool.at(i).IsSearchValid())
            {
                m_bStatusBarFlag = FALSE;
                m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
                m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_OBJECT_AND_TARGET_POS);//.Format(_T("请获取对象、目标位置！"));

                UpdateStatusBar();
                return;
            }
        }

        m_bStatusBarFlag = FALSE;
        m_strStatusBarInfo1.Empty();
        m_strStatusBarInfo2.Empty();

        CString strTempInfo1;
        CString strTempInfo2;
        int nPosIndex = 0;
        for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
        {
            if(TRUE == m_pVisionASM->ExecuteCalibrateDmCode(nPosIndex + m_pVisionASM->GetPosNum()/2+m_pVisionASM->GetPosNum()*nExProductIndex,nPosIndex+m_pVisionASM->GetPosNum()*nExProductIndex))
            {
				m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
               
            }
            else
            {
                m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
            }
        }

        m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(0);
        for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
        {
            m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibTargetPosSuccess.at(nPosIndex);
            if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
            {
                strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_SUCCESS)/*_T("目标位置%d标定成功！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
            }
            else
            {
                strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_FAILE)/*_T("目标位置%d标定失败！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
            }
            m_strStatusBarInfo2 += strTempInfo2;
        }

        if (m_bStatusBarFlag)
        {
            m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS)/*_T("标定成功")*/;
        }
        else
        {
            m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
        }
        UpdateStatusBar();
    }
    else
	{
		{			
			if (m_vpmpCalibMarkImagePos.size() < 2)
			{
				m_bStatusBarFlag = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_OBJECT_AND_TARGET_POS);//.Format(_T("请获取对象、目标位置！"));

				UpdateStatusBar();

				return;
			}

			int nPosIndex = 0;
			int nMovePosIndex = 0;
			for (nMovePosIndex = 0; nMovePosIndex < m_vpmpCalibMarkImagePos.size(); nMovePosIndex++)
			{
				for (nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum(); nPosIndex++)
				{
					if (FALSE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
					{
						m_bStatusBarFlag = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
						m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_OBJECT_AND_TARGET_POS);//.Format(_T("请获取对象、目标位置！"));

						UpdateStatusBar();
						return;
					}
				}	
			}
		}

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Empty();
		m_strStatusBarInfo2.Empty();

		CString strTempInfo1;
		CString strTempInfo2;
		int nPosIndex = 0;
		for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
		{
			if (TRUE == m_pVisionASM->ExecuteCalibrate(m_vpmpCalibMarkImagePos, nPosIndex + m_pVisionASM->GetPosNum()/2+m_pVisionASM->GetPosNum()*nExProductIndex,1))
			{
				m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
			}
			else
			{
				m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
			}
		}

		m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(0);
		for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
		{
			m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibTargetPosSuccess.at(nPosIndex);
			if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
			{
				strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_SUCCESS)/*_T("目标位置%d标定成功！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
			}
			else
			{
				strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_FAILE)/*_T("目标位置%d标定失败！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
			}
			m_strStatusBarInfo2 += strTempInfo2;
		}

		if (m_bStatusBarFlag)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS)/*_T("标定成功")*/;
		}
		else
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
		}
		UpdateStatusBar();

	}
// 	else if (m_nRadioCalibTargetPos > 0)
// 	{
// 		m_bStatusBarFlag = FALSE;
// 		m_strStatusBarInfo1.Empty();
// 		m_strStatusBarInfo2.Empty();
// 
// 		CString strTempInfo1;
// 		CString strTempInfo2;
// 		int nPosIndex = 0;
// 
// 		nPosIndex = m_nRadioCalibTargetPos - 1;
// 		if (TRUE == m_pVisionASM->ExecuteCalibrate(m_vpmpCalibMarkImagePos, nPosIndex + m_pVisionASM->GetPosNum()/2))
// 		{
// 			m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
// 		}
// 		else
// 		{
// 			m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
// 		}
// 
// 		m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(nPosIndex);
// 		if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
// 		{
// 			strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_SUCCESS)/*_T("目标位置%d标定成功！")*/, nPosIndex + m_pVisionASM->GetPosNum()/2);
// 		}
// 		else
// 		{
// 			strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_FAILE)/*_T("目标位置%d标定失败！")*/, nPosIndex + m_pVisionASM->GetPosNum()/2);
// 		}
// 		m_strStatusBarInfo2 += strTempInfo2;
// 
// 
// 		if (m_bStatusBarFlag)
// 		{
// 			m_strStatusBarInfo1= GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS);//.Format(_T("标定成功"));
// 		}
// 		else
// 		{
// 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
// 		}
// 		UpdateStatusBar();
// 	}

}


void CDlgCalibTargetPos2::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存退出]目标标定\n"));
	g_opLogFile.SaveLogText(strLog, 1);
	if (m_pVisionASM == NULL)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_FAILE);//.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();
	}

	m_bCalibTargetPosSuccess = FALSE;
	int nPosIndex = 0;
	m_bCalibTargetPosSuccess = m_vbCalibTargetPosSuccess.at(nPosIndex);
	for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
	{
		m_bCalibTargetPosSuccess = m_bCalibTargetPosSuccess && m_vbCalibTargetPosSuccess.at(nPosIndex);	
	}

	if (m_bCalibTargetPosSuccess)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		//执行复制标定
		if(optionInfo.m_bAutoCopyCalibData)
		{
			SysPlatformInfo sysPlatformInfo;
			m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
			int nExProductIndex = 0;
			if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
			{
				nExProductIndex = m_comboExProductIndex.GetCurSel();
			}
			m_pVisionASM->ExecuteCopyCalibrateData(2,nExProductIndex);
		}
		else
		{
			// 获取系统标定后的所有信息
			std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
			vpAllCalibratedInfo = m_pVisionASM->GetAllCalibratedInfo();	

			/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
			m_pVisionASM->SetCurCalibratedInfo(vpAllCalibratedInfo);	

			// 保存标定后信息到当前产品中及配置文件中
			m_pVisionASM->SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

		}

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_SUCCESS);//.Format(_T("保存成功"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();

		if (/*sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE 
			&& m_pVisionASM->m_pSystempOptionConfig->m_bEnableCommMultiCalibExtension == TRUE
			&&*/ optionInfo.m_bSaveCalibData)
		{
			if (m_bIsDmCodeSearch)
			{
				int nCurExProductIndex = 0;

				nCurExProductIndex = m_comboExProductIndex.GetCurSel();

				// 保存畸变校正图片
				int nCurProdcutIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
				CString strImageFileDirt;
				char ch = '\\';
				strImageFileDirt.Format(m_pVisionASM->GetDataRecordDirectory() + _T("\\CalibRecordData\\ProdcutIndex%d\\GuanLian\\Ex%d"),nCurProdcutIndex,nCurExProductIndex);
				CreateMultipleDirectory(strImageFileDirt,ch);

				for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
				{
					//1.保存图片
					CString strImageName;
					strImageName.Format(_T(" Pos%d_ImageInput.bmp"),i);
					SaveCalibSearchImage(strImageFileDirt,strImageName,m_pVisionASM->m_vDmCodeCalibTool.at(i).m_imageInput);

					//2.保存角点
					std::vector<sc2Vector> vCornersImagePos;
					m_pVisionASM->m_vDmCodeCalibTool.at(i).GetCornersImagePos(vCornersImagePos);

					SYSTEMTIME sys;
					GetLocalTime(&sys);
					CString strTmp;
					strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
					CString strCornersPosFileName;
					strCornersPosFileName.Format(_T("\\%s Pos%d_InterCrossPoint.txt"),strTmp,i);
					CString strImagePointData;
					strImagePointData.Format(_T("\n%d\n"),vCornersImagePos.size());
					for(int nPointIndex = 0 ;nPointIndex < vCornersImagePos.size();nPointIndex ++)
					{
						strImagePointData.AppendFormat(_T("%0.2lf,%0.2lf\n"),vCornersImagePos[nPointIndex].GetX(),vCornersImagePos[nPointIndex].GetY());
					}
					m_pVisionASM->m_vDmCodeCalibTool.at(i).VisionAlignLogRecord(strImageFileDirt+strCornersPosFileName,strImagePointData);
				}
				//3.关联结果
				CString strSaveData;
				GetCalibResultCString(strSaveData);
				SaveCSVFile(strImageFileDirt,_T("GuanLian_Result"), strSaveData);
			}
		}

		if (m_bIsDmCodeSearch)
		{
			for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
			{
				m_pVisionASM->m_vDmCodeCalibTool.at(i).SetSearchValid(FALSE);
				m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearSearchResult();
				m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearRelationResult();
			}
		}
	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SAVE_FAILE);//.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();

		return;
	}
	ResetCameraShutter();
	CDialog::OnOK();
}


void CDlgCalibTargetPos2::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[不保存退出]目标标定\n"));
	g_opLogFile.SaveLogText(strLog, 1);
	if (m_bIsDmCodeSearch)
	{
		for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
		{
			m_pVisionASM->m_vDmCodeCalibTool.at(i).SetSearchValid(FALSE);
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearSearchResult();
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearRelationResult();
		}
	}
	ResetCameraShutter();
	CDialog::OnCancel();
}

UINT CDlgCalibTargetPos2::ThreadReceiveData( LPVOID pParam )
{
	CDlgCalibTargetPos2 *pOwner = (CDlgCalibTargetPos2*)pParam;
	CString strReceived;
	while (!pOwner->m_bExitThread)
	{
		if (pOwner->m_nEvent == 1)		// 搜索对象1移动位置1
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;

            if (pOwner->m_bIsDmCodeSearch)
            {
                pOwner->DmCodeSearch(nPosIndex);
            }
            else
            {
                pOwner->SearchObjectPos(nPosIndex);
            }

// 			if (pOwner->m_nRadioCalibTargetPos == 0)
// 			{
// 				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetPosNum()/2; nPosIndex++)
// 				{
// 					pOwner->SearchObjectPos(nPosIndex);
// 				}
// 			}
// 			else if (pOwner->m_nRadioCalibTargetPos > 0)
// 			{
// 				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
// 				pOwner->SearchObjectPos(nPosIndex);
// 			}
// 			else
// 			{
// 
// 			}

		}		
		else if (pOwner->m_nEvent == 2)	// 搜索目标2移动位置1
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;

            if (pOwner->m_bIsDmCodeSearch)
            {
                pOwner->DmCodeSearch(nPosIndex+pOwner->m_pVisionASM->GetPosNum()/2);
            }
            else
            {
                pOwner->SearchTargetPos(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);
            }

// 			if (pOwner->m_nRadioCalibTargetPos == 0)
// 			{
// 				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetPosNum()/2; nPosIndex++)
// 				{
// 					pOwner->SearchTargetPos(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);
// 				}
// 			}
// 			else if (pOwner->m_nRadioCalibTargetPos > 0)
// 			{
// 				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
// 				pOwner->SearchTargetPos(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);
// 			}
// 			else
// 			{
// 			}
		}
		else if (pOwner->m_nEvent == 3)		// 搜索对象1移动位置2
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;

			pOwner->SearchObjectPos1(nPosIndex);

// 			if (pOwner->m_nRadioCalibTargetPos == 0)
// 			{
// 				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetPosNum()/2; nPosIndex++)
// 				{
// 					pOwner->SearchObjectPos1(nPosIndex);
// 				}
// 			}
// 			else if (pOwner->m_nRadioCalibTargetPos > 0)
// 			{
// 				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
// 				pOwner->SearchObjectPos1(nPosIndex);
// 			}
// 			else
// 			{
// 			}
		}
		else if (pOwner->m_nEvent == 4)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;

			pOwner->SearchTargetPos1(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);

// 			if (pOwner->m_nRadioCalibTargetPos == 0)
// 			{
// 				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetPosNum()/2; nPosIndex++)
// 				{
// 					pOwner->SearchTargetPos1(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);
// 				}
// 			}
// 			else if (pOwner->m_nRadioCalibTargetPos > 0)
// 			{
// 				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
// 				pOwner->SearchTargetPos1(nPosIndex + pOwner->m_pVisionASM->GetPosNum()/2);
// 			}
// 			else
// 			{
// 			}
		}
		else if (pOwner->m_nEvent == 5) // 获取平台轴位置
		{
			pOwner->m_nEvent = 0;
			pOwner->CommGetPlatformAxisPos();
		}

		Sleep(100);
	}

	SetEvent(pOwner->m_hEventReceiveThreadExit);

	return 0;
}

void CDlgCalibTargetPos2::CommGetPlatformAxisPos()
{
	m_bCommGetPlatformAxisPosSuccess = FALSE;
	if (m_pVisionASM == NULL)
	{
 		return;
 	}
 
 	// 通信获取平台轴位置
 	m_bCommGetPlatformAxisPosSuccess = m_pVisionASM->GetCalibAlignerTargetPlatformAxisPos();

}

void CDlgCalibTargetPos2::SearchObjectPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);



	if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point)
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 0;
		m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(FALSE);
		m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(0),pos.GetAuxiliaryPosY(0)),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetLineColor(m_colorMovePos1);

				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(1),pos.GetAuxiliaryPosY(1)),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetLineColor(m_colorMovePos1);
			}
		}
	}
	else
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 0;
		m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(FALSE);

		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetLineColor(m_colorMovePos1);
			}
		}
	}

	

}

void CDlgCalibTargetPos2::SearchTargetPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}
	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);



	if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point)
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 0;
		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(0),pos.GetAuxiliaryPosY(0)),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos1);

				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(1),pos.GetAuxiliaryPosY(1)),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos1);
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 0;
		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos1);
			}
		}
		else
		{
			return;
		}
	}
	
}

void CDlgCalibTargetPos2::SearchObjectPos1(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}
	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);



	if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point)
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 1;
		m_vpGuiAlnObjectImageMarkPos2.at(nPosIndex)->SetVisible(FALSE);
		m_vpGuiAlnObjectImageMarkPos3.at(nPosIndex)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnObjectImageMarkPos2.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(2),pos.GetAuxiliaryPosY(2)),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos2.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos2.at(nPosIndex)->SetLineColor(m_colorMovePos2);

				m_vpGuiAlnObjectImageMarkPos3.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(3),pos.GetAuxiliaryPosY(3)),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos3.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos3.at(nPosIndex)->SetLineColor(m_colorMovePos2);

			}
		}
	}
	else
	{
		if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
		{
			return;
		}
		int nMovePosIndex = 1;
		m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(FALSE);

		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetCenterRotationLengths(
					sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(TRUE);
				m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetLineColor(m_colorMovePos2);
			}
		}
	}

	

}

void CDlgCalibTargetPos2::SearchTargetPos1(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);



	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point)
	{
		int nMovePosIndex = 1;
		m_vpGuiAlnTargetImageMarkPos2.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);

		m_vpGuiAlnTargetImageMarkPos3.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnTargetImageMarkPos2.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(2),pos.GetAuxiliaryPosY(2)),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos2.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos2.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos2);

				m_vpGuiAlnTargetImageMarkPos3.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetAuxiliaryPosX(3),pos.GetAuxiliaryPosY(3)),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos3.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos3.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos2);
			}
		};
	}
	else
	{
		int nMovePosIndex = 1;
		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
		if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
		{
			m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

			if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
				&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
			{
				CCoordPos pos;
				pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
					sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
				m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetLineColor(m_colorMovePos2);
			}
		}
	}
	
}






void CDlgCalibTargetPos2::OnBnClickedBtnSetDmcodeParam()
{
    // TODO: 在此添加控件通知处理程序代码
    // TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	if (pTotalProductData->GetCount()<= 0)
	{
		return;
	}

	CString strLog;
	strLog.Format(_T("点击[二维码参数设置]进入[二维码参数设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	OnBnClickedBtnSerialGrab();
	CDlgCalibTargetDmcodeParam dlg;
	dlg.SetVisionASMRef(m_pVisionASM);	
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlg.SetLogLevel(3);
	dlg.DoModal();

	OnBnClickedBtnSerialGrab();
}

BOOL CDlgCalibTargetPos2::DmCodeSearch(int nPosIndex)
{
    if (m_pVisionASM == NULL)
    {
        return FALSE;
    }
    if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
    {
        return FALSE;
    }

    if (FALSE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExDmCode(nPosIndex))
    {
        return FALSE;
    }
    return TRUE;
}

/************************************************************************/
/* 
功能：
	创建多级目录
参数：
	CString& szPath	文件路径名
	char split		分割字符，根据路径设置
*/
/************************************************************************/
bool CDlgCalibTargetPos2::CreateMultipleDirectory(CString& szPath, char split)
{
	CString strDir(szPath);
	//	检测路径是否以 结尾
	if (strDir.GetAt(strDir.GetLength()-1) != split)
	{
		strDir.AppendChar(split);
	}
	std::vector<CString> vPath;	//	存放每一层目录字符串
	CString strTemp;			//	临时变量,存放目录字符串
	bool bSuccess = false;		//	成功标志
	// 遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != split) // 如果当前字符不是 split('\','//')
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else //如果当前字符是'\\'
		{
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(split);
		}
	}

	//	遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//	如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

//获得关联结果
bool CDlgCalibTargetPos2::GetCalibResultCString(CString & strCalibResult)
{

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = sysPlatformInfo.m_nPositionNum;

	std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
	vpAllCalibratedInfo = m_pVisionASM->GetAllCalibratedInfo();	

	CString strTemp = _T("");
	CString strOut = _T("");
	CCalibratedInfo* pCalibratedInfo;
	BOOL bIsOK = TRUE;  // 关联结果是否OK
	int nExProductIndex = 0;
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}
	for (int nPosIndex = nPosNum/2;nPosIndex<nPosNum;nPosIndex++)
	{

		if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE )
		{
			pCalibratedInfo = vpAllCalibratedInfo.at(nPosIndex+nExProductIndex*nPosNum);
		}
		else
		{
			pCalibratedInfo = vpAllCalibratedInfo.at(nPosIndex);
		}



		if (pCalibratedInfo == NULL)
		{
			return false;
		}

		strTemp.Format(_T("%d,"),nPosIndex+1);strOut += strTemp;

		// 3*3矩阵
		double h[9];
		memset(h, 0.0, sizeof(double)*9);
		h[0] = 1.0;
		h[4] = 1.0;
		h[8] = 1.0;
		pCalibratedInfo->GetPlatformTransferH(h);
		strTemp.Format(_T("%.6f,"), h[0]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[1]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[2]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[3]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[4]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[5]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[6]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[7]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[8]);strOut += strTemp;

		if (sysPlatformInfo.m_bTargetObjectCamSeparate)
		{

			if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point ||
				m_pVisionASM->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
			{
				double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
				strTemp.Format(_T("%.3f,"),dMaxError);
				strOut += strTemp;
			}
			else
			{
				bIsOK =TRUE;
			}
		}
		strTemp.Format(_T("\n"));
		strOut += strTemp;
		strOut += _T(",");
	}

	strCalibResult = strOut;
	return true;
}

// 保存CVS 文件
bool CDlgCalibTargetPos2::SaveCSVFile(CString strImageFileDirt,CString strName,CString& strSaveData )
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strTmp;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strFileName.Format("%s\\%s.csv", strImageFileDirt, strName);

	//判断文件是否存在
	CStdioFile file;
	CFileStatus rStatus;
	BOOL bRes = CFile::GetStatus(strFileName, rStatus);

	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return false;
	}
	file.SeekToEnd();

	if(!bRes)//绘制表头
	{
		file.WriteString(_T("关联时间,位置,h0,h1,h2,h3,h4,h5,h6,h7,h8,MaxError\n"));
	}

	strTmp.Format("%04d-%02d-%02d-%02d-%02d-%02d-%03d,", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	strOut += strTmp;
	strOut += strSaveData;

	strOut+=_T("\n");
	file.WriteString(strOut);
	file.Close();
	return true;
}

// 保存畸变校正前的图片
void CDlgCalibTargetPos2::SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave)
{
	CString strTmp = strImageName;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strTmp.Append(strImageName);
	strFileName.Format("%s\\%s", strImageFileDirt, strTmp);


	scDIB scDibTmp;
	scDibTmp.Init(imageSave);
	scDibTmp.Write(strFileName);

}


void CDlgCalibTargetPos2::OnBnClickedSetCalibTargetShutter1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCalibTargetShutter(0);
}

void CDlgCalibTargetPos2::OnBnClickedSetCalibTargetShutter2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCalibTargetShutter(1);
}

void CDlgCalibTargetPos2::ResetCameraShutter()
{
	if(m_pVisionASM)
	{
		for(int nPos=0;nPos<2 && nPos<m_pVisionASM->GetPosNum();nPos++)
		{
			if(m_pVisionASM->IsCameraFirstPosIndex(nPos))
			{
				m_pVisionASM->SwitchToSearchShutter(nPos, dShutterList[nPos]);
			}
		}
	}
}

void CDlgCalibTargetPos2::SetCalibTargetShutter(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	if(nPosIndex>=m_pVisionASM->GetPosNum())
	{
		return;
	}


	CameraParam para;
	double dMin = 0, dMax = 0,dShutter = 0.0;
	if (!m_pVisionASM->GetPosCameraShutter(nPosIndex, dShutter,dMin,dMax))
	{
		AfxMessageBox(_T("获取相机曝光错误！"));
		return;
	}

	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);

	CDlgCalibTargetShutterSetting dlg;
	dlg.m_nPosIndex = nPosIndex;
	dlg.SetVisionAlignerRef(m_pVisionASM);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);


	dlg.m_dEnvironmentShutter = dShutter;
	double dCalibTargetShutter = dShutter;
	int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
	if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
	{
		dCalibTargetShutter = vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter;
	}


	if (dlg.DoModal() == IDOK)
	{
		int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
		if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
		{
			vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter = dlg.m_dDmCodeShutter;
			m_pVisionASM->SaveCameraInfo();
			m_pVisionASM->SaveJobCameraInfo();
		}
	}
	else
	{
		m_pVisionASM->SwitchToSearchShutter(nPosIndex, dCalibTargetShutter);
	}
}

void CDlgCalibTargetPos2::CallDlgSearch(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	int nProductIdx= 0;

	if(m_pVisionASM->GetTotalProuductData())
	{
		nProductIdx = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	}
	CBaseSearchTool *pSearchTool = NULL;
	pSearchTool = m_pVisionASM->GetRepoAlnCalibTargetSearchTool(nProductIdx,nPosIndex,0);

	if (m_pVisionASM->m_pSystemStatusInfo!=NULL)
	{
		double dUsedRate = 0.0;

		CString strModuelDir;
		GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
		strModuelDir.ReleaseBuffer();
		CString StrTmp = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   

		m_pVisionASM->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

		if ((m_pVisionASM->IsSysMemoryUsedRateWarning() ||  m_pVisionASM->IsDiskUsedRateWarning(StrTmp)) && pSearchTool==NULL)
		{

			CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
			CString strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);
			m_pVisionASM->SetStatusBarInfo(FALSE, strStatusBarInfo1, strStatusBarInfo2 + _T("\n无法新建模板！"));
			m_pVisionASM->PostMessageUpdateStatusBarInfo();

			CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
			m_pVisionASM->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
			// 避免后续无法保存的情况
			return;
		}
	}

	CString strTmpTitle = _T("定位模板设置");
	strTmpTitle.Format(_T("位置%d关联模板设置"),nPosIndex+1);

	CDlgSearch dlgSearch(this, pSearchTool);
	dlgSearch.m_nCurPositionIdx = nPosIndex;
	dlgSearch.m_strTitleName = strTmpTitle;
	dlgSearch.SetVisionASMRef(m_pVisionASM, nPosIndex);
	dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);


	if (dlgSearch.DoModal() == IDOK)
	{
		pSearchTool = dlgSearch.GetSearchTool();
		if(pSearchTool)
		{
			m_pVisionASM->SetRepoAlnCalibTargetSearchTool(nProductIdx, nPosIndex, 0, pSearchTool);
			m_pVisionASM->SaveAlnCalibTargetSearchToolRepository(nProductIdx);
		}
	}

}

void CDlgCalibTargetPos2::SetCalibTargetSearchTool(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	if(nPosIndex<0 || nPosIndex>= m_pVisionASM->GetPosNum())
	{
		return;
	}

	m_pVisionASM->FreezeCamera(m_pVisionASM->GetPosCameraIndex(nPosIndex));
	// 停止刷新图像，避免dlgSearch 图像刷新
	m_bStopUpdateDisplayImage = TRUE;
	CallDlgSearch(nPosIndex);
	m_pVisionASM->GrabCamera(m_pVisionASM->GetPosCameraIndex(nPosIndex));
	m_bStopUpdateDisplayImage = FALSE;

}

LRESULT CDlgCalibTargetPos2::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{

	if(NULL== m_pVisionASM)
	{
		return S_OK;
	}

	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	if (eDmCodeCalib == m_pVisionASM->GetCalibTargetMarkPatternMode() || eDoublePatternSingleMark == m_pVisionASM->GetCalibTargetMarkPatternMode())
	{
		return S_OK;
	}


	for(int it=0;it<m_vpGuiObjectDisplay.size();it++)
	{
		if(m_vpGuiObjectDisplay.at(it) && m_vpGuiObjectDisplay.at(it)->GetID()==wParam)
		{
			int nPos = it;
			SetCalibTargetSearchTool(nPos);
		}
	}

	for(int it=0;it<m_vpGuiTargetDisplay.size();it++)
	{
		if(m_vpGuiTargetDisplay.at(it) && m_vpGuiTargetDisplay.at(it)->GetID()==wParam)
		{
			int nPos = it+m_pVisionASM->GetPosNum()/2;
			SetCalibTargetSearchTool(nPos);
		}
	}

	return S_OK;
}