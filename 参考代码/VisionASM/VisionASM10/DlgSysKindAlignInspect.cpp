// DlgSysKindAlignInspect.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindAlignInspect.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"

#include "DlgSysKind.h"
#include "DlgSysKind.h"
#include "DlgSearchToolCopyPos.h"
#include "DlgSearchToolModelLibrary.h"

// CDlgSysKindAlignInspect 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindAlignInspect, CDialog)

CDlgSysKindAlignInspect::CDlgSysKindAlignInspect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindAlignInspect::IDD, pParent)
/*	,m_nInspectMarkEnable(0)*/
	,m_nInspectCandidateEnable(0)
	,m_nInspectCandidateSync(0)
	, m_bInspectCandidateOffsetEnable1(FALSE)
	, m_bInspectCandidateOffsetEnable2(FALSE)
	, m_bInspectCandidateOffsetEnable3(FALSE)
	, m_bInspectCandidateOffsetEnable4(FALSE)
	, m_nInspectSingleSearch(0)
{
	m_psaSysInfoStrings = NULL;

	m_nPlatformIndex = 0;
	m_nPrevPlatformIdx = m_nPlatformIndex;

	m_nPrevProductIdx = 0;
	m_nCurProductIdx = 0;
	m_bProdPatternChanged = FALSE;
	m_bSysKeyboardEnabled = FALSE;


	m_bObjectShutterEnable0 = FALSE;
	m_bObjectShutterEnable1 = FALSE;
	m_bObjectShutterEnable2 = FALSE;
	m_bObjectShutterEnable3 = FALSE;
	m_bObjectShutterEnable4 = FALSE;

	m_bObjectGainEnable0 = FALSE;
	m_bObjectGainEnable1 = FALSE;
	m_bObjectGainEnable2 = FALSE;
	m_bObjectGainEnable3 = FALSE;
	m_bObjectGainEnable4 = FALSE;

	m_bObjectGammaEnable0 = FALSE;
	m_bObjectGammaEnable1 = FALSE;
	m_bObjectGammaEnable2 = FALSE;
	m_bObjectGammaEnable3 = FALSE;
	m_bObjectGammaEnable4 = FALSE;

}

CDlgSysKindAlignInspect::~CDlgSysKindAlignInspect()
{
	DestroyWindow();
}

void CDlgSysKindAlignInspect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//	DDX_Radio(pDX, IDC_RADIO_INSPECT_SEARCH_MODE0, m_nInspectMarkEnable);
	DDX_Radio(pDX, IDC_RADIO_INSPECTC_SEARCH_MODE0, m_nInspectCandidateEnable);
	DDX_Radio(pDX, IDC_RADIO_INSPECTC_SEARCH_SYNC0, m_nInspectCandidateSync);

	DDX_Control(pDX, IDC_STATIC_POS0, m_labelPos0);
	DDX_Control(pDX, IDC_STATIC_POS1, m_labelPos1);
	DDX_Control(pDX, IDC_STATIC_POS2, m_labelPos2);
	DDX_Control(pDX, IDC_STATIC_POS3, m_labelPos3);
	DDX_Control(pDX, IDC_STATIC_POS4, m_labelPos4);
	DDX_Control(pDX, IDC_STATIC_POS5, m_labelPos5);
	DDX_Control(pDX, IDC_STATIC_POS6, m_labelPos6);
	DDX_Control(pDX, IDC_STATIC_POS7, m_labelPos7);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1, m_bInspectCandidateOffsetEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2, m_bInspectCandidateOffsetEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3, m_bInspectCandidateOffsetEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4, m_bInspectCandidateOffsetEnable4);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE6,  m_bObjectShutterEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE5, m_bObjectShutterEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2, m_bObjectShutterEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3, m_bObjectShutterEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4, m_bObjectShutterEnable4);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE,  m_bObjectGainEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1, m_bObjectGainEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE2, m_bObjectGainEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE3, m_bObjectGainEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE4, m_bObjectGainEnable4);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE,  m_bObjectGammaEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1, m_bObjectGammaEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE2, m_bObjectGammaEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE3, m_bObjectGammaEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE4, m_bObjectGammaEnable4);

	DDX_Check(pDX, IDC_CHECK_INSPECT_SINGLESEARCH, m_nInspectSingleSearch);
}


BEGIN_MESSAGE_MAP(CDlgSysKindAlignInspect, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM1, OnGuiDisplayMenuCustom1)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM2, OnGuiDisplayMenuCustom2)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM3, OnGuiDisplayMenuCustom3)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE6, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE5, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE2, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE3, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE4, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE2, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE3, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE4, &CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable)

	ON_BN_CLICKED(IDC_RADIO_INSPECT_SEARCH_MODE0, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectSearchMode0)
	ON_BN_CLICKED(IDC_RADIO_INSPECT_SEARCH_MODE1, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectSearchMode0)
	ON_BN_CLICKED(IDC_RADIO_INSPECTC_SEARCH_MODE1, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchMode1)
	ON_BN_CLICKED(IDC_RADIO_INSPECT_SEARCH_SYNC1, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchMode1)
	ON_BN_CLICKED(IDC_RADIO_INSPECTC_SEARCH_SYNC0, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchSync0)
	ON_BN_CLICKED(IDC_RADIO_INSPECTC_SEARCH_SYNC1, &CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchSync0)
	ON_BN_CLICKED(IDC_CHECK_INSPECT_SINGLESEARCH, &CDlgSysKindAlignInspect::OnBnClickedCheckInspectSinglesearch)
END_MESSAGE_MAP()


// CDlgSysKindAlignInspect 消息处理程序

void CDlgSysKindAlignInspect::UpdateDialogLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

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

		if(_T("IDD_SYS_KIND_ALIGN_INSPECT") == strDlgID)
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

void CDlgSysKindAlignInspect::InitDlgItem()
{
	m_vpLabelPos.push_back(&m_labelPos0);
	m_vpLabelPos.push_back(&m_labelPos1);
	m_vpLabelPos.push_back(&m_labelPos2);
	m_vpLabelPos.push_back(&m_labelPos3);
	m_vpLabelPos.push_back(&m_labelPos4);
	m_vpLabelPos.push_back(&m_labelPos5);
	m_vpLabelPos.push_back(&m_labelPos6);
	m_vpLabelPos.push_back(&m_labelPos7);

	// 调整控制板
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	m_vvpInspectGuiDisplay.resize(MAXPOSNUM);
	scGuiDisplay *pGUIDisplay = NULL;

	for (int i=0; i<MAXPOSNUM; i++)
	{
		for (int j = 0; j < INSPECTCANDIDATENUM; j++)
		{
			pGUIDisplay = new scGuiDisplay;
			pGUIDisplay->Create(IDC_STATIC_TARGETC_DISPLAY0+ i*INSPECTCANDIDATENUM + j, this, WS_BORDER);
			pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CLICK | SV_DISPLAY_CUSTOM1 | SV_DISPLAY_CUSTOM2 | SV_DISPLAY_CUSTOM3);

			switch (lgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				pGUIDisplay->SetLanguage(0);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("Clear Search Tool"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("Copy Search Tool To ..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("Get Search Tool From Model library"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			}

			m_vvpInspectGuiDisplay[i].push_back(pGUIDisplay);
			pGUIDisplay = NULL;
		}
	}

	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}

	//EnableAndShowControl(optionInfo.m_bEnableSearchShutter && optionInfo.m_bInspectEnable ,optionInfo.m_bEnableSearchShutter && optionInfo.m_bInspectEnable);
	EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);
}

void CDlgSysKindAlignInspect::EnableAndShowControl(bool bEnable,bool bShow)
{
	bool bShowTemplateSet = false;
	bool bEnableTemplateSet = false;
	if(NULL != m_pParentKind)
	{
		bShowTemplateSet = m_pParentKind->m_bShowInspectTemplateSet;
		bEnableTemplateSet = m_pParentKind->m_bEnableInspectTemplateSet;
	}
	bEnable = bEnable && bEnableTemplateSet;
	bShow = bShow && bShowTemplateSet;

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE6)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE5)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE6)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE5)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4)->ShowWindow(bShow);

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE4)->ShowWindow(bShow);

	//暂时禁用Gamma实时拍照切换
	{
		bEnable = false;
		bShow = false;
	}

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE4)->ShowWindow(bShow);

}

void CDlgSysKindAlignInspect::MoveDlgItem()
{

	if (GetDlgItem(IDC_STATIC_POS0)->GetSafeHwnd() != NULL)
	{
		int i = 0, j = 0;

		// 调整控制板
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());


		// 动态计算并调整对话框大小
		CRect rcClient;
		GetClientRect(rcClient);

		int nStatusBarHeight = 0;
		CRect rcItem1;

		for(i = 0; i < 8; i++)
		{
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			rcItem1.top = rcClient.top;
			rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->MoveWindow(rcItem1);
		}

		//////////////////////////////////////////////////////////////////////////
		// 根据默认平台调整界面
		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		int nPosNum = platformInfo.m_nPositionNum;
		CPlatformOptionInfo optionInfo;
		if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
		{
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
		}
		// 检测界面修改
		//if (TRUE == optionInfo.m_bInspectEnable)
		{
			ShowAndEnableControl(m_pParentKind->m_bEnableInspectTemplateSet);

			for (i=0; i<MAXPOSNUM; i++)
			{
				if (i < nPosNum)
				{
					GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
		
					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
					}

					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
					m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);


					for(j = 0; j < INSPECTCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
						m_vvpInspectGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
					}
				}
				else
				{
					GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(SW_HIDE);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
					}

					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

					for(j = 0; j < INSPECTCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
						m_vvpInspectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}
				}
			}
			
			EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);
		}
		/*else
		{
			ShowAndEnableControl(FALSE);

			for (i=0; i<MAXPOSNUM; i++)
			{
				GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < INSPECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpInspectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}

			EnableAndShowControl(FALSE,FALSE);
		}*/
	}

//	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignInspect::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	UpdateData(TRUE);

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);

	int nCurProdIdx = 0;
	CProductData product;

	if (bSaveAndValidate)
	{
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		product = pTotalProduct->GetItem(nCurProdIdx);
//		product.m_pAlignerParam->SetAlignInspectEnable(m_nInspectMarkEnable);
		product.m_pAlignerParam->SetInspectCandidateEnable(m_nInspectCandidateEnable);
		product.m_pAlignerParam->SetInspectCandidateSyncEnable(m_nInspectCandidateSync);
		
		product.m_pAlignerParam->SetInspectSingleSearchEnable(m_nInspectSingleSearch);

		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(1, m_bInspectCandidateOffsetEnable1);
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(2, m_bInspectCandidateOffsetEnable2);
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(3, m_bInspectCandidateOffsetEnable3);	
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(4, m_bInspectCandidateOffsetEnable4);

		int i=0; 
		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable2;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable2;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable2;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable3;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable3;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable3;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;	
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable4;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable4;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable4;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else
	{
		BeginWaitCursor();
		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
		product = pTotalProduct->GetItem(nCurProdIdx);

//		m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;


//		m_nInspectMarkEnable = product.m_pAlignerParam->GetAlignInspectEnable();
		m_nInspectCandidateEnable = product.m_pAlignerParam->GetInspectCandidateEnable();
		m_nInspectCandidateSync = product.m_pAlignerParam->GetInspectCandidateSyncEnable();

		m_nInspectSingleSearch = product.m_pAlignerParam->GetInspectSingleSearchEnable();

		m_bInspectCandidateOffsetEnable1 = product.m_pAlignerParam->GetInspectCandidateOffsetEnable(1);
		m_bInspectCandidateOffsetEnable2 = product.m_pAlignerParam->GetInspectCandidateOffsetEnable(2);
		m_bInspectCandidateOffsetEnable3 = product.m_pAlignerParam->GetInspectCandidateOffsetEnable(3);
		m_bInspectCandidateOffsetEnable4 = product.m_pAlignerParam->GetInspectCandidateOffsetEnable(4);

		int i = 0;
		int j = 0;

		i = 0;
		m_bObjectShutterEnable0 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable;
		m_bObjectGainEnable0 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable;
		m_bObjectGammaEnable0 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable;

		i++;
		m_bObjectShutterEnable1 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable;
		m_bObjectGainEnable1 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable;
		m_bObjectGammaEnable1 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable;

		i++;
		m_bObjectShutterEnable2 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable;
		m_bObjectGainEnable2 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable;
		m_bObjectGammaEnable2 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable;

		i++;
		m_bObjectShutterEnable3 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable;
		m_bObjectGainEnable3 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable;
		m_bObjectGammaEnable3 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable;

		i++;
		m_bObjectShutterEnable4 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable;
		m_bObjectGainEnable4 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable;
		m_bObjectGammaEnable4 = product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable;

		// 检测Mark模板图像显示
		for (i=0; i<MAXPOSNUM; i++)
		{
			for (j = 0; j < INSPECTCANDIDATENUM; j++)
			{
				m_vvpInspectGuiDisplay[i][j]->ClearScreen();
			}
		}

		if ( product.m_pAlignerParam->GetSysPlatformInspectEnable() == TRUE )
		{
			CBaseSearchTool* pInspectSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetInspectMarkPatternNum(); i++)
			{

				for(j = 0; j<INSPECTCANDIDATENUM; j++)
				{
					pInspectSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,i, j);


					BOOL bExistSearchTool = IsSearchToolSelected(pInspectSearchTool);			

					if (pInspectSearchTool != NULL && bExistSearchTool)
					{
						image.Release();
						pInspectSearchTool->GetPatternImage(image);
						m_vvpInspectGuiDisplay[i][j]->ClearScreen();
						m_vvpInspectGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpInspectGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}

// 保存前一个产品的设置
BOOL CDlgSysKindAlignInspect::SavePrevProduct()
{
	BOOL bSucceed = TRUE;
	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	m_nPrevProductIdx = pView->m_nPrevProductIdx;

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	{
		// 保存对位参数
		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
		product.m_pAlignerParam->SetInspectCandidateEnable(m_nInspectCandidateEnable);
		product.m_pAlignerParam->SetInspectCandidateSyncEnable(m_nInspectCandidateSync);

		product.m_pAlignerParam->SetInspectSingleSearchEnable(m_nInspectSingleSearch);
		//		product.m_pAlignerParam->SetAlignInspectEnable(m_nInspectMarkEnable);

		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(1, m_bInspectCandidateOffsetEnable1);
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(2, m_bInspectCandidateOffsetEnable2);
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(3, m_bInspectCandidateOffsetEnable3);	
		product.m_pAlignerParam->SetInspectCandidateOffsetEnable(4, m_bInspectCandidateOffsetEnable4);

		int i=0; 
		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable2;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable2;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable2;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable3;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable3;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable3;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

		i++;	
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable4;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable4;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable4;
		product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);
		
		pTotalProduct->SetItem(m_nPrevProductIdx, product);
		m_bProdPatternChanged = FALSE;
	}

	return bSucceed;
}

void CDlgSysKindAlignInspect::OnPlatformChange() 
{
	UpdateData(TRUE);

	SysPlatformInfo platformInfo;
	int nPosNum = 0, i = 0, j = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = platformInfo.m_nPositionNum;



	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}
	

	//if (TRUE == optionInfo.m_bInspectEnable)
	{
		// 		GetDlgItem(IDC_STATIC_INSPECT_SEARCH_MODE)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_RADIO_INSPECT_SEARCH_MODE0)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_RADIO_INSPECT_SEARCH_MODE1)->EnableWindow(TRUE);

		/*GetDlgItem(IDC_STATIC_INSPECT_SEARCH_SYNC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE2)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_INSPECTC_SEARCH_SYNC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC1)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET3)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET4)->EnableWindow(TRUE);*/

		ShowAndEnableControl(m_pParentKind->m_bEnableInspectTemplateSet);

		EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);

		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);

				for(j = 0; j < INSPECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
					m_vvpInspectGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowInspectTemplateSet);
				}

			}
			else
			{
				GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < INSPECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpInspectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	//else
	//{

	//	// 		GetDlgItem(IDC_STATIC_INSPECT_SEARCH_MODE)->EnableWindow(FALSE);
	//	// 		GetDlgItem(IDC_RADIO_INSPECT_SEARCH_MODE0)->EnableWindow(FALSE);
	//	// 		GetDlgItem(IDC_RADIO_INSPECT_SEARCH_MODE1)->EnableWindow(FALSE);

	//	/*GetDlgItem(IDC_STATIC_INSPECT_SEARCH_SYNC)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE0)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE2)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_INSPECTC_SEARCH_SYNC)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC0)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC1)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET1)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET2)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET3)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET4)->EnableWindow(FALSE);*/
	//	ShowAndEnableControl(FALSE);

	//	for (i=0; i<MAXPOSNUM; i++)
	//	{
	//		GetDlgItem(IDC_STATIC_INSPECT_PATTERN0+i)->ShowWindow(SW_HIDE);

	//		if (i < MAXPOSNUM -1)
	//		{
	//			GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
	//		}
	//		m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
	//		m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

	//		for(j = 0; j < INSPECTCANDIDATENUM; j++)
	//		{
	//			GetDlgItem(IDC_STATIC_INSPECTC_PATTERN0+i*INSPECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
	//			m_vvpInspectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
	//		}
	//	}
	//}

	UpdateDlgData(FALSE);

	//CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_nPrevProductIdx = m_nCurProductIdx;

}

void CDlgSysKindAlignInspect::OnButtonNewProduct()
{
	UpdateDlgData(FALSE);
}

void CDlgSysKindAlignInspect::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	MoveDlgItem();

}

BOOL CDlgSysKindAlignInspect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_strModuleDir = m_pParentKind->m_strModuleDir;

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	int nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_strProductNameOld = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);

	UpdateDialogLanguage();

	InitDlgItem();
	UpdateDlgData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 双击Display控件响应
LRESULT CDlgSysKindAlignInspect::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		OnButtonNewProduct();
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < INSPECTCANDIDATENUM; j++)
		{
			if (wParam == m_vvpInspectGuiDisplay[i][j]->GetID())
			{
				CString strLog;
				if (j == 0)
				{
					strLog.Format(_T("双击[位置%d]检测模板"),i+1);
					g_opLogFile.SaveLogText(strLog,4);
				}
				else
				{
					strLog.Format(_T("双击[位置%d]检测候选模板%d"),i+1,j);
					g_opLogFile.SaveLogText(strLog,4);
				}

				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,i,j);

				// 检查存储空间是否超过阈值
				if(1)
				{
					if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
					{
						if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
						{
							double dUsedRate = 0.0;
							CString StrTmp = m_strModuleDir;
							m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) && pSearchTool==NULL)
							{
								CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
								CString strTmp2;
								strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
								CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

								AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！"));

								CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
								// 避免后续无法保存的情况

								return S_OK;

							}
						}
					}
				}

				LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
				if (m_psaSysInfoStrings != NULL)
				{
					lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
				}
				switch (lgidLanguage)
				{
				case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
					if(pSearchTool!=NULL) pSearchTool->SetLanguage(0);
					break;
				case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
					if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
					break;
				case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
				default:
					if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
					break;
				}

				CDlgSearch dlgSearch(this, pSearchTool);
				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);

				CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
				CProductData product = pTotalProduct->GetItem(nCurProdIdx);
//				dlgSearch.SetProductSearchShutter(product.m_pAlignerParam->GetInspectSearchShutter(i));
//				dlgSearch.SetEnableSearchShutter(product.m_pAlignerParam->GetEnableInspectSearchShutter());

				CameraParamEnable cameraParamEnable;
				cameraParamEnable = product.m_pAlignerParam->GetEnableInspectSearchShutter(j);
				BOOL bSearchShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

				CPlatformOptionInfo optionInfo;
				if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
				{
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
				}

				BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

				if (bSearchShutter && bSystemOptionShutter)
				{
					dlgSearch.SetProductSearchShutterInfo(product.m_pAlignerParam->GetEnableInspectSearchShutter(j) ,product.m_pAlignerParam->GetInspectSearchShutter(i,j));
				}


				if (dlgSearch.DoModal() == IDOK)
				{
					if (bSearchShutter && bSystemOptionShutter)
					{
						product.m_pAlignerParam->SetInspectSearchShutter(i,j, dlgSearch.GetProductSearchShutterInfoShutter());
						pTotalProduct->SetItem(nCurProdIdx, product);
					}

					pSearchTool = dlgSearch.GetSearchTool();
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnInspectSearchTool(nCurProdIdx,i, j, pSearchTool);

					// 显示模板训练图像
					if (pSearchTool != NULL)
					{
						cpImage imagePattern;
						pSearchTool->GetPatternImage(imagePattern);

						m_vvpInspectGuiDisplay[i][j]->ClearScreen();
						m_vvpInspectGuiDisplay[i][j]->SetImage(imagePattern, TRUE);
					}
					else
					{
						m_vvpInspectGuiDisplay[i][j]->ClearScreen();
					}
					// 标记产品模板已修改
					m_bProdPatternChanged = TRUE;
					// 标记指定位置检测模板已经被修改了			
					m_pParentKind->m_vvbAlnInspectSearchToolChanged[i][j] = TRUE;
				}
				return S_OK;
			}
		}

	}
	return S_OK ;
}

// 自定义菜单响应
LRESULT CDlgSysKindAlignInspect::OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < INSPECTCANDIDATENUM; j++)
		{
			if (wParam == m_vvpInspectGuiDisplay[i][j]->GetID())
			{
				CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
				CProductData product = pTotalProduct->GetItem(nCurProdIdx);
				
				CameraParamEnable cameraParamEnable;
				cameraParamEnable = product.m_pAlignerParam->GetEnableInspectSearchShutter(j);
				BOOL bSearchShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

				CPlatformOptionInfo optionInfo;
				if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
				{
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
				}
				BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

				if (bSearchShutter && bSystemOptionShutter)
				{
					CameraParamSet tmpameraParamSet;
					product.m_pAlignerParam->SetInspectSearchShutter(i,j, tmpameraParamSet);
					pTotalProduct->SetItem(nCurProdIdx, product);
				}

             	CString strLog;
				if (j == 0)
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d对象模板\n"),i+1);
					g_opLogFile.SaveLogText(strLog,4);
				}
				else
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d候选模板%d\n"),i+1,j);
					g_opLogFile.SaveLogText(strLog,4);
				}
				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,i,j);
				if (pSearchTool == NULL)
				{
					return S_OK;
				}

				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnInspectSearchTool(nCurProdIdx,i, j);
				m_vvpInspectGuiDisplay[i][j]->ClearScreen();

				// 标记产品模板已修改
				m_bProdPatternChanged = TRUE;
				// 标记指定位置检测模板已经被修改了			
				m_pParentKind->m_vvbAlnInspectSearchToolChanged[i][j] = TRUE;
				return S_OK;
			}
		}
	}

	return S_OK ;
}

LRESULT CDlgSysKindAlignInspect::OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0,nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < INSPECTCANDIDATENUM; j++)
		{
			if (wParam == m_vvpInspectGuiDisplay[i][j]->GetID())
			{
				CString strLog;
				strLog.Format(_T("点击右键选择[拷贝定位工具到...]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,i,j);

				CDlgSearchToolCopyPos dlgSearchToolCopyPos;
				dlgSearchToolCopyPos.m_nPosNum = nPatternNum;
				dlgSearchToolCopyPos.m_nPosCNum = INSPECTCANDIDATENUM;
				dlgSearchToolCopyPos.m_nSrcCandidateIndex = j;
				dlgSearchToolCopyPos.SetSysInfoStringsRef(m_psaSysInfoStrings);
				
				if (dlgSearchToolCopyPos.DoModal() == IDOK)
				{
					int k = 0;
					nDstCandidateIndex = dlgSearchToolCopyPos.m_nCandidateIndex;
					for (k = 0; k < MAXPOSNUM && k < dlgSearchToolCopyPos.m_vbCopyPos.size(); k++)
					{
						if (dlgSearchToolCopyPos.m_vbCopyPos.at(k) == TRUE && ((k != i)||(nDstCandidateIndex != j)))
						{	
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
							{
								CString strWarning;
								strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),k+1,nDstCandidateIndex);
								strWarning = strWarning + 
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
								MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
								return S_OK;

							}

							// 检查存储空间是否超过阈值
							if(1)
							{
								if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
								{
									if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
									{
										double dUsedRate = 0.0;
										CString StrTmp = m_strModuleDir;
										m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

										if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchTool != NULL)
										{
											CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
											CString strTmp2;
											strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
											CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

											AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

											CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
											m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
											// 避免后续无法保存的情况

											return S_OK;

										}
									}
								}
							}

							CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
							CProductData product = pTotalProduct->GetItem(nCurProdIdx);

							// 拷贝源为空模板时，将拷贝目标位置的产品执行清空操作
							if (pSearchTool == NULL)
							{
								CameraParamEnable cameraParamEnable;
								cameraParamEnable = product.m_pAlignerParam->GetEnableInspectSearchShutter(nDstCandidateIndex);
								BOOL bSearchShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

								if (bSearchShutter && bSystemOptionShutter)
								{
									CameraParamSet tmpameraParamSet;
									product.m_pAlignerParam->SetInspectSearchShutter(k,nDstCandidateIndex, tmpameraParamSet);
									pTotalProduct->SetItem(nCurProdIdx, product);
								}
								CBaseSearchTool *pSearchToolDst = NULL;
								pSearchToolDst = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								if (pSearchToolDst == NULL)
								{
									continue;
								}

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnInspectSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								m_vvpInspectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();

							}
							else
							{
								CameraParamEnable cameraParamEnable;
								cameraParamEnable = product.m_pAlignerParam->GetEnableInspectSearchShutter(j);
								BOOL bSearchShutter =  (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);
								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;
								if (bSearchShutter && bSystemOptionShutter)
								{
									product.m_pAlignerParam->SetInspectSearchShutter(k,nDstCandidateIndex, product.m_pAlignerParam->GetInspectSearchShutter(i,j));
									pTotalProduct->SetItem(nCurProdIdx, product);
								}


								//获得拷贝之前的窗口 
								scGuiDisplay* pOldDisPlay = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosDisplay(k);

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnInspectSearchTool(nCurProdIdx, k, nDstCandidateIndex, pSearchTool);

								// 显示模板训练图像
								if (pSearchTool != NULL)
								{
									cpImage imagePattern;
									pSearchTool->GetPatternImage(imagePattern);

									m_vvpInspectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
									m_vvpInspectGuiDisplay[k][nDstCandidateIndex]->SetImage(imagePattern, TRUE);
								}
								else
								{
									m_vvpInspectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
								}

								//设置拷贝之前的窗口 
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosDisplay(k, pOldDisPlay); 
							}

							
							
							// 标记指定位置检测模板已经被修改了			
							m_pParentKind->m_vvbAlnInspectSearchToolChanged[k][nDstCandidateIndex] = TRUE;
							///////////////////////////////////////////////////////////////////////

							strLog.Format(_T("将位置%d候选%d检测模板复制到位置%d候选%d\n"),i+1,j,k+1,nDstCandidateIndex);
							g_opLogFile.SaveLogText(strLog,5);

							/////////////////////////////////////////////////////////////////////////
						}
					}


					m_bProdPatternChanged = TRUE;
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}


LRESULT CDlgSysKindAlignInspect::OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0,nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;


	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < INSPECTCANDIDATENUM; j++)
		{
			if (wParam == m_vvpInspectGuiDisplay[i][j]->GetID())
			{
				CString strLog,strText;
				strLog.Format(_T("点击右键选择[获取定位工具从模板库]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				CDlgSearchToolModelLibrary dlgSearchToolModelLibrary;
				dlgSearchToolModelLibrary.m_vpVisionASM= m_pParent->m_vpVisionASM;
				dlgSearchToolModelLibrary.m_nCurPlatformIndex = m_pParentKind->m_nPlatformIndex;
				dlgSearchToolModelLibrary.SetSysInfoStringsRef(m_psaSysInfoStrings);
				dlgSearchToolModelLibrary.EnableSysKeyboard(m_bSysKeyboardEnabled);

				dlgSearchToolModelLibrary.m_nSearchModeFrom = eInspectSearch;
				dlgSearchToolModelLibrary.m_nPosIndexFrom = i;
				dlgSearchToolModelLibrary.m_nCandidatePosFrom = j;

				if (dlgSearchToolModelLibrary.DoModal() == IDOK)
				{
					CBaseSearchTool *pSearchTool =NULL;
					pSearchTool = dlgSearchToolModelLibrary.GetSelectedSearchTool();
					if(NULL == pSearchTool)
					{
						return S_FALSE;
					}

					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
					{
						CString strWarning;
						strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),i+1,j);
						strWarning = strWarning + 
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
						MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
						return S_FALSE;

					}

					// 检查存储空间是否超过阈值
					if(1)
					{
						if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
							{
								double dUsedRate = 0.0;
								CString StrTmp = m_strModuleDir;
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

								// 从其他模板拷贝到空产品时，需要进行资源管控
								CBaseSearchTool *pSearchToolDstTmp = NULL;
								pSearchToolDstTmp = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nCurProdIdx,i,j);
								if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchToolDstTmp == NULL)
								{
									CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
									CString strTmp2;
									strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
									CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

									AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

									CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
									m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
									// 避免后续无法保存的情况

									return S_OK;

								}
							}
						}
					}

					bool bRet = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnInspectSearchTool(nCurProdIdx,i,j,pSearchTool);
					if(!bRet)
					{
						return S_FALSE;
					}
					cpImage imagePattern;
					bRet = dlgSearchToolModelLibrary.GetSelectedImage(imagePattern);
					if(!bRet)
					{
						return S_FALSE;
					}
					m_vvpInspectGuiDisplay[i][j]->ClearScreen();
					m_vvpInspectGuiDisplay[i][j]->SetImage(imagePattern);

					// 标记产品模板已修改
					m_bProdPatternChanged = TRUE;
					// 标记指定位置目标模板已经被修改了
					m_pParentKind->m_vvbAlnInspectSearchToolChanged[i][j] = TRUE;

					///////////////////////////////////////////////////////////////////////
					dlgSearchToolModelLibrary.GetImageContent(dlgSearchToolModelLibrary.GetSelectImageIndex(),strLog);

					strText.Format(_T("将%s复制到产品%d位置%d候选%d\n"),strLog, m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData()->GetCurProductIndex()
						,i+1,j);
					g_opLogFile.SaveLogText(strText,5);


					/////////////////////////////////////////////////////////////////////////
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}



BOOL CDlgSysKindAlignInspect::IsProductChanged()
{
	if (m_bProdPatternChanged)
	{
		return TRUE;
	}

	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);// 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	ASSERT(pTotalProduct != NULL);
	CProductData product;
	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	{
		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	}
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	if (m_nInspectCandidateEnable != (int)product.m_pAlignerParam->GetInspectCandidateEnable())
	{
		return TRUE;
	}

	if (m_nInspectCandidateSync != (int)product.m_pAlignerParam->GetInspectCandidateSyncEnable())
	{
		return TRUE;
	}

	if (m_nInspectSingleSearch != (int)product.m_pAlignerParam->GetInspectSingleSearchEnable())
	{
		return TRUE;
	}

	if (m_bInspectCandidateOffsetEnable1 != product.m_pAlignerParam->GetInspectCandidateOffsetEnable(1))
	{
		return TRUE;
	}

	if (m_bInspectCandidateOffsetEnable2 != product.m_pAlignerParam->GetInspectCandidateOffsetEnable(2))
	{
		return TRUE;
	}

	if (m_bInspectCandidateOffsetEnable3 != product.m_pAlignerParam->GetInspectCandidateOffsetEnable(3))
	{
		return TRUE;
	}

	if (m_bInspectCandidateOffsetEnable4 != product.m_pAlignerParam->GetInspectCandidateOffsetEnable(4))
	{
		return TRUE;
	}

	int i = 0;

	if (m_bObjectShutterEnable0 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable
		|| m_bObjectGainEnable0 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable
		|| m_bObjectGammaEnable0 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
	i++;
	if (m_bObjectShutterEnable1 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable
		|| m_bObjectGainEnable1 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable
		|| m_bObjectGammaEnable1 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
	i++;
	if (m_bObjectShutterEnable2 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable
		|| m_bObjectGainEnable2 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable
		|| m_bObjectGammaEnable2 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
	i++;
	if (m_bObjectShutterEnable3 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable
		|| m_bObjectGainEnable3 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable
		|| m_bObjectGammaEnable3 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
	i++;
	if (m_bObjectShutterEnable4 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bShutterEnable
		|| m_bObjectGainEnable4 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGainEnable
		|| m_bObjectGammaEnable4 != product.m_pAlignerParam->GetEnableInspectSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
// 	if (m_nInspectMarkEnable != (int)product.m_pAlignerParam->GetAlignInspectEnable())
// 	{
// 		return TRUE;
// 	}

	return FALSE;
}

BOOL CDlgSysKindAlignInspect::IsSearchToolSelected(CBaseSearchTool* pSearchTool)
{
    //UpdateData(TRUE);
    BOOL bExistSearchTool =TRUE;
    SysPlatformInfo sysPlat;
    DlgSearchToolInfo dlgSearchTool;
    CSearchToolType nSearchToolType = eSearchToolUnknown;
    m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlat);
    dlgSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDlgSearchToolData();
    if (pSearchTool!= NULL)
    {
        nSearchToolType = pSearchTool->GetSearchToolType();
        vector<map<pair<int,int>,BOOL>>::iterator iterSys;
        map<pair<int,CString>,BOOL>::iterator iterDlg;
        iterSys = sysPlat.m_vmapSysSearchToolInfo.begin();
        for (;iterSys != sysPlat.m_vmapSysSearchToolInfo.end();iterSys++)
        {
            if (nSearchToolType == (*iterSys).begin()->first.second)
            {
                for (iterDlg = dlgSearchTool.m_mapDlgSearchToolInfo.begin();iterDlg != dlgSearchTool.m_mapDlgSearchToolInfo.end();iterDlg++)
                {
                    if ((*iterSys).begin()->first.first == iterDlg->first.first)
                    {
                        bExistSearchTool = iterDlg->second;
                        break;
                    }
                }
                break;
            }
        }
    }

    return bExistSearchTool;
}

BOOL CDlgSysKindAlignInspect::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	for(int i = 0; i < m_vvpInspectGuiDisplay.size(); i++)
	{
		for(int j = 0; j < m_vvpInspectGuiDisplay[i].size(); j++)
		{
			delete m_vvpInspectGuiDisplay[i][j];
			m_vvpInspectGuiDisplay[i][j] = NULL;
		}
		m_vvpInspectGuiDisplay[i].clear();
	}
	m_vvpInspectGuiDisplay.clear();

	return CDialog::DestroyWindow();
}

void CDlgSysKindAlignInspect::OnBnClickedRadioInspectSearchMode0()
{
	//// TODO: 在此添加控件通知处理程序代码
	//int nInspectMarkEnableOld = m_nInspectMarkEnable;
	//UpdateDlgData(TRUE);

	//CString strVal[] = {_T("关"), _T("开")};
	//if (nInspectMarkEnableOld != m_nInspectMarkEnable)
	//{
	//	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	//	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	//	CString strLog;
	//	strLog.Format(_T("修改[检测搜索]由%s到%s\n"),strVal[nInspectMarkEnableOld],strVal[m_nInspectMarkEnable]);
	//	g_opLogFile.SaveLogText(strLog,4);
	//}
}



void CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nInspectCandidateEnableOld = m_nInspectCandidateEnable;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nInspectCandidateEnableOld != m_nInspectCandidateEnable)
	{
		CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
		m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
		CString strLog;
		strLog.Format(_T("修改[候选搜索]由%s到%s\n"),strVal[nInspectCandidateEnableOld],strVal[m_nInspectCandidateEnable]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}



void CDlgSysKindAlignInspect::OnBnClickedRadioInspectcSearchSync0()
{
	// TODO: 在此添加控件通知处理程序代码
	int nInspectCandidateSyncOld = m_nInspectCandidateSync;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nInspectCandidateSyncOld != m_nInspectCandidateSync)
	{
		CString strLog;
		strLog.Format(_T("修改[成对搜索]由%s到%s\n"),strVal[nInspectCandidateSyncOld],strVal[m_nInspectCandidateSync]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignInspect::GetDlgTempData(CProductData& product)
{
	UpdateData(TRUE);

	product.m_pAlignerParam->SetInspectCandidateEnable(m_nInspectCandidateEnable);
	product.m_pAlignerParam->SetInspectCandidateSyncEnable(m_nInspectCandidateSync);

	product.m_pAlignerParam->SetInspectSingleSearchEnable(m_nInspectSingleSearch);

	product.m_pAlignerParam->SetInspectCandidateOffsetEnable(1, m_bInspectCandidateOffsetEnable1);
	product.m_pAlignerParam->SetInspectCandidateOffsetEnable(2, m_bInspectCandidateOffsetEnable2);
	product.m_pAlignerParam->SetInspectCandidateOffsetEnable(3, m_bInspectCandidateOffsetEnable3);	
	product.m_pAlignerParam->SetInspectCandidateOffsetEnable(4, m_bInspectCandidateOffsetEnable4);

	int i=0; 
	CameraParamEnable cameraParamEnable;
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
	product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

	i++;
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;
	product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

	i++;
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable2;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable2;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable2;
	product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

	i++;
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable3;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable3;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable3;
	product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);

	i++;	
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable4;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable4;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable4;
	product.m_pAlignerParam->SetEnableInspectSearchShutter(i, cameraParamEnable);
}


void CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcShutterEnable()
{
	// TODO: 在此添加控件通知处理程序代码
    BOOL bInspectShutterEnable0Old = m_bObjectShutterEnable0;
    BOOL bInspectShutterEnable1Old = m_bObjectShutterEnable1;
    BOOL bInspectShutterEnable2Old = m_bObjectShutterEnable2;
    BOOL bInspectShutterEnable3Old = m_bObjectShutterEnable3;
    BOOL bInspectShutterEnable4Old = m_bObjectShutterEnable4;
	UpdateData(TRUE);
    
    if (bInspectShutterEnable0Old != m_bObjectShutterEnable0)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable0 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bInspectShutterEnable1Old != m_bObjectShutterEnable1)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable1 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置1]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bInspectShutterEnable2Old != m_bObjectShutterEnable2)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable2 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置2]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bInspectShutterEnable3Old != m_bObjectShutterEnable3)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable3 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置3]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bInspectShutterEnable4Old != m_bObjectShutterEnable4)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable4 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置4]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
}

void CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGainEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerObjectGainEnable0Old = m_bObjectGainEnable0;
	BOOL bAlignerObjectGainEnable1Old = m_bObjectGainEnable1;
	BOOL bAlignerObjectGainEnable2Old = m_bObjectGainEnable2;
	BOOL bAlignerObjectGainEnable3Old = m_bObjectGainEnable3;
	BOOL bAlignerObjectGainEnable4Old = m_bObjectGainEnable4;
	UpdateData(TRUE);
	if (bAlignerObjectGainEnable0Old != m_bObjectGainEnable0)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGainEnable1Old != m_bObjectGainEnable1)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGainEnable2Old != m_bObjectGainEnable2)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGainEnable3Old != m_bObjectGainEnable3)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGainEnable4Old != m_bObjectGainEnable4)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignInspect::OnBnClickedCheckAlignerObjectcGammaEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerObjectGammaEnable0Old = m_bObjectGammaEnable0;
	BOOL bAlignerObjectGammaEnable1Old = m_bObjectGammaEnable1;
	BOOL bAlignerObjectGammaEnable2Old = m_bObjectGammaEnable2;
	BOOL bAlignerObjectGammaEnable3Old = m_bObjectGammaEnable3;
	BOOL bAlignerObjectGammaEnable4Old = m_bObjectGammaEnable4;
	UpdateData(TRUE);
	if (bAlignerObjectGammaEnable0Old != m_bObjectGammaEnable0)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGammaEnable1Old != m_bObjectGammaEnable1)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGammaEnable2Old != m_bObjectGammaEnable2)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGammaEnable3Old != m_bObjectGammaEnable3)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGammaEnable4Old != m_bObjectGammaEnable4)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignInspect::ShowAndEnableControl(BOOL bShow)
{
	int nShow = 0;
	if (bShow)
	{
		nShow = 5;
	}

	GetDlgItem(IDC_STATIC_INSPECT_SEARCH_SYNC)->EnableWindow(bShow);
	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE0)->EnableWindow(bShow);
	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE2)->EnableWindow(bShow);

	GetDlgItem(IDC_STATIC_INSPECTC_SEARCH_SYNC)->EnableWindow(bShow);
	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC0)->EnableWindow(bShow);
	GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC1)->EnableWindow(bShow);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4)->EnableWindow(bShow);

	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET1)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET2)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET3)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET4)->EnableWindow(bShow);


	//GetDlgItem(IDC_STATIC_INSPECT_SEARCH_SYNC)->ShowWindow(nShow);
	//GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE0)->ShowWindow(nShow);
	//GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_MODE2)->ShowWindow(nShow);

	//GetDlgItem(IDC_STATIC_INSPECTC_SEARCH_SYNC)->ShowWindow(nShow);
	//GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC0)->ShowWindow(nShow);
	//GetDlgItem(IDC_RADIO_INSPECTC_SEARCH_SYNC1)->ShowWindow(nShow);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4)->ShowWindow(nShow);

	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET1)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET2)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET3)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_GRP_ALIGN_TARGETC_OFFSET4)->ShowWindow(nShow);

}

void CDlgSysKindAlignInspect::EnableWindowByAuthorizeset(bool bEnable)
{
	//CPlatformOptionInfo optionInfo;
	//if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	//{
	//	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	//}

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		//pWnd->EnableWindow(bEnable && optionInfo.m_bInspectEnable);
		pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysKindAlignInspect::OnBnClickedCheckInspectSinglesearch()
{
	// TODO: 在此添加控件通知处理程序代码
	int nInspectSingleSearchOld = m_nInspectSingleSearch;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nInspectSingleSearchOld != m_nInspectSingleSearch)
	{
		CString strLog;
		strLog.Format(_T("修改[候补单独搜索]由%s到%s\n"),strVal[nInspectSingleSearchOld],strVal[m_nInspectSingleSearch]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}
