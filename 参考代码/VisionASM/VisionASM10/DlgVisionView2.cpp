// DlgVisionView2.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionView2.h"
#include "afxdialogex.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "DlgSearch.h"
#include "DlgCalibTargetShutterSetting.h"
// CDlgVisionView2 对话框

IMPLEMENT_DYNAMIC(CDlgVisionView2, CDialog)

CDlgVisionView2::CDlgVisionView2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVisionView2::IDD, pParent)
	, m_nPosShowMode(12)
	, m_nPrePosShowMode(12)
{
	//{{AFX_DATA_INIT(CDlgVisionView2)
	//}}AFX_DATA_INIT
	m_pParent = (CVisionASMDlg*)pParent;
	m_pVisionASM = NULL;
	m_pStatusBar = NULL;
	m_nShowDisplayNum = 1;
	m_nPositionNum = 2;
	m_nShowMode = 0;

	m_hIconFailed = NULL;
	m_hIconSucceed = NULL;
	m_nPlatformID = -1;


	m_nDisplayShowMode = 0;

	for (int i=0; i<MAXPOSNUM; i++)	//hk20180130
	{
		m_nArrayMode[i] = i;
	}

	m_pMainLayout = NULL;
	m_pMainLayout2 = NULL;
	m_pCurMainLayout = NULL;
	m_pViewDisplay = NULL;
	m_pDlgVisionRun = NULL;
	m_pCurDisplayLayout = NULL;
	m_pPosBtnLayout = NULL;
	m_pTitleLayout = NULL;
	m_nVisionRunWidth = 440;
	m_pTabLayout = NULL;
	m_nCurMainLayout = 0;
    m_nFitImage = 0;

	// 	m_nRow = 1;
	// 	m_nCol = 2;
	// 	m_nAlignerInfoRow = 5;
	// 	m_nAlignerInfoHeight = 0;
	// 	m_strTimeCost = _T("0");
	// 	m_strCurPrecisionD = _T("0.0000");
	// 	m_strCurPrecisionX = _T("0.0000");
	// 	m_strCurPrecisionY = _T("0.0000");
	// 	m_strAlignTimes = _T("0");
	// 	m_strProductName = _T("*");
	// 	m_strTargetPrecisionD = _T("0.0000");
	// 	m_strTargetPrecisionX = _T("0.0000");
	// 	m_strTargetPrecisionY = _T("0.0000");
	//	m_nDisplayMode = 4;
	//  m_nPositionShowMode = 0;


}

CDlgVisionView2::~CDlgVisionView2()
{
	m_cfontWSName.DeleteObject();
	if (m_pMainLayout)
	{
		delete m_pMainLayout;
		m_pMainLayout = NULL;
	}

	if (m_pMainLayout2)
	{
		delete m_pMainLayout2;
		m_pMainLayout2 = NULL;
	}

	for (int i = 0; i < m_vpDisplayLayout.size(); i++)
	{
		if (m_vpDisplayLayout[i])
		{
			delete m_vpDisplayLayout[i];
			m_vpDisplayLayout[i] = NULL;
		}
	}

	m_vpDisplayLayout.clear();


	for (int i = 0; i < m_vpChkPosDisplayLayout.size(); i++)
	{
		if (m_vpChkPosDisplayLayout[i])
		{
			delete m_vpChkPosDisplayLayout[i];
		}
	}
	m_vpChkPosDisplayLayout.clear();


	if (m_pTabLayout)
	{
		delete m_pTabLayout;
		m_pTabLayout = NULL;
	}

}

void CDlgVisionView2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_labelStatus);
	DDX_Control(pDX, IDC_STATIC_STATUS_OTHER_PLATFORM, m_labelOtherStatus);
	DDX_Radio(pDX, IDC_RADIO_DISPLAY_POS0, m_nPosShowMode);
	DDX_Control(pDX, IDC_BTN_DISPLAY_VISION, m_btnDisplayVisionRun);
	DDX_Control(pDX, IDC_TAB_RUN_INFO, m_tabRunInfo);
}


BEGIN_MESSAGE_MAP(CDlgVisionView2, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_DISPLAY_VISION, &CDlgVisionView2::OnBnClickedBtnDisplayVision)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MANUAL_TARGET_SEARCH, &CDlgVisionView2::OnBnClickedBtnManualTargetSearch)
	ON_BN_CLICKED(IDC_BTN_MANUAL_OBJECT_SEARCH, &CDlgVisionView2::OnBnClickedBtnManualObjectSearch)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS0, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS1, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS2, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS3, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS4, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS5, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS6, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS7, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS8, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS9, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS10, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS11, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS12, &CDlgVisionView2::OnBnClickedRadioDisplayPos0)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RUN_INFO, &CDlgVisionView2::OnTcnSelchangeTabRunInfo)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)

END_MESSAGE_MAP()


// CDlgVisionView2 消息处理程序



void CDlgVisionView2::InitUIStyle()
{
	//m_btnDisplayVisionRun.SetThemeHelper(&m_themeHelper);
}

void CDlgVisionView2::InitTabRunInfo()
{
   if (m_vpDlgVisionRun.size() > 1)
   {
	   m_tabRunInfo.ShowWindow(SW_HIDE);
	   m_labelOtherStatus.ShowWindow(SW_SHOW);

	   m_pTabLayout = new CHorizontalLayout;
	   
	   CString strPlatformID;
	   for (int i = 0; i < m_vpDlgVisionRun.size(); i++)
	   {
		   strPlatformID.Format("工位%d", m_vpDlgVisionRun[i]->GetPlatformID()+1);
		   m_tabRunInfo.InsertItem(i, strPlatformID);
		   m_pTabLayout->AddWnd(m_vpDlgVisionRun[i], 0, 0, CLayout::eSizePolicy_Expand, 0, 25, 0, 0);
		   m_vpDlgVisionRun[i]->ShowWindow(SW_HIDE);
	   }
	   m_vpDlgVisionRun[0]->ShowWindow(SW_SHOW);
	   m_pTabLayout->SetParent(&m_tabRunInfo);
	   m_pTabLayout->OnSize();
   }
   else
   {
	   m_tabRunInfo.ShowWindow(SW_HIDE);
	   m_labelOtherStatus.ShowWindow(SW_HIDE);
   }
}


// 整个布局分为三成 上中下, 上:标题栏, 中: 显示区域, 下: 状态栏
void CDlgVisionView2::InitUILayout()
{
	 CLayout* pView = new CVerticalLayout;
	 CLayout* pTitleLayout = InitTitleLayout();
	 // 添加标题栏
	 pView->AddLayout(pTitleLayout, 0, 40, CLayout::eSizePolicy_Fixed);
	 // 添加显示区域
	 m_pViewDisplay = new CHorizontalLayout;
	 pView->AddLayout(m_pViewDisplay, 0, 0, CLayout::eSizePolicy_Expand);
//	 pView->AddWnd(GetDlgItem(IDC_STATIC_VIEW_AREA), 0, 0, CLayout::eSizePolicy_Expand);
	 // 添加状态栏
	 CLayout* pStatusLayout = new CHorizontalLayout;
	 pStatusLayout->AddWnd(GetDlgItem(IDC_STATIC_STATUS), 0, 40, CLayout::eSizePolicy_Expand);
	 pStatusLayout->AddWnd(GetDlgItem(IDC_STATIC_STATUS_OTHER_PLATFORM), 0, 40, CLayout::eSizePolicy_Expand);
	 pView->AddLayout(pStatusLayout, 0, 40, CLayout::eSizePolicy_Fixed);

	 //pView->AddWnd(GetDlgItem(IDC_STATIC_STATUS), 0, 40, CLayout::eSizePolicy_Fixed);

	 // 主窗口布局设置
	 m_pMainLayout = new CHorizontalLayout;
	 m_pMainLayout->AddLayout(pView, 0, 0, CLayout::eSizePolicy_Expand);

	 if (m_vpDlgVisionRun.size() == 1)
	 {
		 m_pMainLayout->AddWnd(m_pDlgVisionRun, m_nVisionRunWidth, 0, CLayout::eSizePolicy_Fixed);
	 }
	 else if(m_vpDlgVisionRun.size() > 1)
	 {
		 m_pMainLayout->AddWnd(&m_tabRunInfo, m_nVisionRunWidth, 0, CLayout::eSizePolicy_Fixed);
	 }

	 m_pMainLayout->SetParent(this);
	 //m_pMainLayout->OnSize();


	 // 类型2
	 m_pMainLayout2 = new CVerticalLayout;
	 m_pMainLayout2->SetAutoDeleteChild(false);
	 m_pMainLayout2->AddLayout(pView, 0, 0, CLayout::eSizePolicy_Expand);

	 if (m_vpDlgVisionRun.size() == 1)
	 {
		 m_pMainLayout2->AddWnd(m_pDlgVisionRun, 0, m_nVisionRunWidth, CLayout::eSizePolicy_Fixed);
	 }
	 else if(m_vpDlgVisionRun.size() > 1)
	 {
		 m_pMainLayout2->AddWnd(&m_tabRunInfo, 0, m_nVisionRunWidth, CLayout::eSizePolicy_Fixed);
	 }

	 m_pMainLayout2->SetParent(this);
	 //m_pMainLayout->OnSize();

	  
	 // 默认水平布局
	 m_pCurMainLayout = m_pMainLayout;
	 m_nCurMainLayout = 0;
 
}


CLayout* CDlgVisionView2::InitTitleLayout()
{
	// 位置布局
	CHorizontalLayout* pPosLayout1 = new CHorizontalLayout;
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS0), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS1), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS4), 80, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS7), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS8), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS9), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout1->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS10), 60, 0, CLayout::eSizePolicy_Fixed);
	CHorizontalLayout* pSpring1 = new CHorizontalLayout;
	pPosLayout1->AddLayout(pSpring1, 0, 0, CLayout::eSizePolicy_Expand);

	CHorizontalLayout* pPosLayout2 = new CHorizontalLayout;
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS2), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS3), 60, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS5), 80, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS6), 100, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS11), 100, 0, CLayout::eSizePolicy_Fixed);
	pPosLayout2->AddWnd(GetDlgItem(IDC_RADIO_DISPLAY_POS12), 100, 0, CLayout::eSizePolicy_Fixed);
	CHorizontalLayout* pSpring2 = new CHorizontalLayout;
	pPosLayout2->AddLayout(pSpring2, 0, 0, CLayout::eSizePolicy_Expand);

	CVerticalLayout* pPosLayout = new CVerticalLayout;
	pPosLayout->AddLayout(pPosLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pPosLayout->AddLayout(pPosLayout2, 0, 0, CLayout::eSizePolicy_Preferred);

	CHorizontalLayout* pPosLayoutMode = new CHorizontalLayout;
	pPosLayoutMode->AddWnd(GetDlgItem(IDC_STATIC_DISPLAY_MODE), 90, 20, CLayout::eSizePolicy_Fixed, 20);
	pPosLayoutMode->AddLayout(pPosLayout, 0, 0, CLayout::eSizePolicy_Expand);

	m_pPosBtnLayout = pPosLayoutMode;


	// 标题布局
	CHorizontalLayout* pTitleLayout = new CHorizontalLayout;
	//pTitleLayout->AddWnd(GetDlgItem(IDC_STATIC_WS_ICON), 55, 0, CLayout::eSizePolicy_Fixed);
	pTitleLayout->AddWnd(GetDlgItem(IDC_STATIC_WS), 55, 0, CLayout::eSizePolicy_Fixed);
	pTitleLayout->AddLayout(pPosLayoutMode, 0, 0, CLayout::eSizePolicy_Expand);
	pTitleLayout->AddWnd(GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH), 120, 40, CLayout::eSizePolicy_Fixed);
	pTitleLayout->AddWnd(GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH), 120, 40, CLayout::eSizePolicy_Fixed);
	pTitleLayout->AddWnd(GetDlgItem(IDC_BTN_DISPLAY_VISION), 50, 40, CLayout::eSizePolicy_Fixed);

	m_pTitleLayout = pTitleLayout;
	return pTitleLayout;
}

void CDlgVisionView2::SetAllPosBtnHide()
{
	int nCtrlIdArr[] = {IDC_STATIC_DISPLAY_MODE, IDC_RADIO_DISPLAY_POS0, IDC_RADIO_DISPLAY_POS1, IDC_RADIO_DISPLAY_POS2, IDC_RADIO_DISPLAY_POS3,
		IDC_RADIO_DISPLAY_POS4, IDC_RADIO_DISPLAY_POS5, IDC_RADIO_DISPLAY_POS6, IDC_RADIO_DISPLAY_POS7, IDC_RADIO_DISPLAY_POS8,
		IDC_RADIO_DISPLAY_POS9, IDC_RADIO_DISPLAY_POS10, IDC_RADIO_DISPLAY_POS11, IDC_RADIO_DISPLAY_POS12};
	for (int i = 0; i < sizeof(nCtrlIdArr)/sizeof(int); i++)
	{
		GetDlgItem(nCtrlIdArr[i])->ShowWindow(SW_HIDE);
	}
}

void CDlgVisionView2::SetSelPosModeBtnVisible()
{
	SetAllPosBtnHide();

	GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_SHOW);
	switch(m_nShowDisplayNum)
	{
	case 8:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_SHOW);  // 位置(5,6,7,8)
			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_SHOW);  // 位置8
			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_SHOW);   // 位置7
			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);   // 位置(1,2,3,4)
		}
	case 6:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);    // 位置6
			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);	// 位置5
		}
	case 4:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);  // 位置(3, 4)
			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);  // 位置(4)
			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);  // 位置(3)
			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);  // 位置(1 ,2)
		}
	case 3:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);   // 位置(2)
		}
	case 2:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);  // 全部位置
			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);   // 位置(2)
		}
	case 1:
		{
			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);  // 位置(1)
		}
	}

	if (m_pPosBtnLayout)
	{
		m_pPosBtnLayout->OnSize();
	}

	UpdateWindow();
}


void CDlgVisionView2::InitDisplayLayout()
{
	int nPosNum = m_nShowDisplayNum;

	switch(nPosNum)
	{
	case 1:
		InitDisplayLayoutPos1();
		break;
	case 2:
		InitDisplayLayoutPos2();
		break;
	case 3:
		InitDisplayLayoutPos3();
		break;
	case 4:
		InitDisplayLayoutPos4(); 
		break;
	case 5:
		InitDisplayLayoutPos5();
		break;
	case 6:
		InitDisplayLayoutPos6();
		break;
	case 7:
		InitDisplayLayoutPos7();
		break;
	case 8:
		InitDisplayLayoutPos8();
		break;
	}

	
}

void CDlgVisionView2::InitDisplayLayoutPos1()
{
	// 将Display窗口 按照配置信息排下序
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	CLayout* pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
	//pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

}


void CDlgVisionView2::InitDisplayLayoutPos2()
{
	// 将Display窗口 按照配置信息排下序
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种布局, 水平
	CLayout* pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pDisplayLayout->AddWnd(vpGuiDisplay[1]);

	pDisplayLayout->SetParentLayout(m_pViewDisplay);
	//pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);


	// 第二种布局, 垂直
	pDisplayLayout = new CVerticalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pDisplayLayout->AddWnd(vpGuiDisplay[1]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
	//pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

}


void CDlgVisionView2::InitDisplayLayoutPos3()
{
	// 将Display窗口 按照配置信息排下序
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种布局水平
	CLayout* pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pDisplayLayout->AddWnd(vpGuiDisplay[1]);
	pDisplayLayout->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);


	// 第二种布局垂直
	pDisplayLayout = new CVerticalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pDisplayLayout->AddWnd(vpGuiDisplay[1]);
	pDisplayLayout->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

	// 第三种左1右2
	pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	CLayout* pTemp = new CVerticalLayout;
	pTemp->AddWnd(vpGuiDisplay[1]);
	pTemp->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->AddLayout(pTemp);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

	// 第四种左2右1
	pDisplayLayout = new CHorizontalLayout;
	pTemp = new CVerticalLayout;
	pTemp->AddWnd(vpGuiDisplay[0]);
	pTemp->AddWnd(vpGuiDisplay[1]);
	pDisplayLayout->AddLayout(pTemp);
	pDisplayLayout->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);


	// 第五种上1下2
	pDisplayLayout = new CVerticalLayout;
	pDisplayLayout->AddWnd(vpGuiDisplay[0]);
	pTemp = new CHorizontalLayout;
	pTemp->AddWnd(vpGuiDisplay[1]);
	pTemp->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->AddLayout(pTemp);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

	// 第六种上2下1
	pDisplayLayout = new CVerticalLayout;
	pTemp = new CHorizontalLayout;
	pTemp->AddWnd(vpGuiDisplay[0]);
	pTemp->AddWnd(vpGuiDisplay[1]);
	pDisplayLayout->AddLayout(pTemp);
	pDisplayLayout->AddWnd(vpGuiDisplay[2]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

}


void CDlgVisionView2::InitDisplayLayoutPos4()
{
	// 将Display窗口 按照配置信息排下序
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种布局水平	上2下2
	CLayout* pDisplayLayout = new CVerticalLayout;
	CLayout* pTmp1 = new CHorizontalLayout;
	CLayout* pTmp2 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[0]);
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pTmp2->AddWnd(vpGuiDisplay[2]);
	pTmp2->AddWnd(vpGuiDisplay[3]);
	pDisplayLayout->AddLayout(pTmp1);
	pDisplayLayout->AddLayout(pTmp2);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);
}

void CDlgVisionView2::InitDisplayLayoutPos5()
{
	// 将Display窗口 按照配置信息排下序
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种 上2下3
	CLayout* pDisplayLayout = new CVerticalLayout;
	CLayout* pTmp1 = new CHorizontalLayout;
	CLayout* pTmp2 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[0]);
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pTmp2->AddWnd(vpGuiDisplay[2]);
	pTmp2->AddWnd(vpGuiDisplay[3]);
	pTmp2->AddWnd(vpGuiDisplay[4]);
	pDisplayLayout->AddLayout(pTmp1);
	pDisplayLayout->AddLayout(pTmp2);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);

	// 第2种 上3下2
	pDisplayLayout = new CVerticalLayout;
	pTmp1 = new CHorizontalLayout;
	pTmp2 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[0]);
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pTmp1->AddWnd(vpGuiDisplay[2]);
	pTmp2->AddWnd(vpGuiDisplay[3]);
	pTmp2->AddWnd(vpGuiDisplay[4]);
	pDisplayLayout->AddLayout(pTmp1);
	pDisplayLayout->AddLayout(pTmp2);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);


}

void CDlgVisionView2::InitDisplayLayoutPos6()
{
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种 对象目标水平排列 对像上1下2 目标上1下2
	CLayout* pDisplayLayout = new CHorizontalLayout;
	CLayout* pObject = new CVerticalLayout;
	pObject->AddWnd(vpGuiDisplay[0]);
	CLayout* pTmp1 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pTmp1->AddWnd(vpGuiDisplay[2]);
	pObject->AddLayout(pTmp1);

	CLayout* pTarget = new CVerticalLayout;
	pTarget->AddWnd(vpGuiDisplay[3]);
	pTmp1 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[4]);
	pTmp1->AddWnd(vpGuiDisplay[5]);
	pTarget->AddLayout(pTmp1);

	pDisplayLayout->AddLayout(pObject);
	pDisplayLayout->AddLayout(pTarget);

	pDisplayLayout->SetParentLayout(m_pViewDisplay);
	//pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);


	// 第二种 对象目标水平排列 对像上2下1 目标上2下1
	pDisplayLayout = new CHorizontalLayout;
	pObject = new CVerticalLayout;
	pTmp1 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[0]);
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pObject->AddLayout(pTmp1);
	pObject->AddWnd(vpGuiDisplay[2]);


	pTarget = new CVerticalLayout;
	pTmp1 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[3]);
	pTmp1->AddWnd(vpGuiDisplay[4]);
	pTarget->AddLayout(pTmp1);
	pTarget->AddWnd(vpGuiDisplay[5]);

	pDisplayLayout->AddLayout(pObject);
	pDisplayLayout->AddLayout(pTarget);

	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);
}


void CDlgVisionView2::InitDisplayLayoutPos7()
{
	// 暂时没有需求
}


void CDlgVisionView2::InitDisplayLayoutPos8()
{
	std::vector<scGuiDisplay*> vpGuiDisplay;
	for (int i = 0; i < MAXPOSNUM && i < m_vpGuiDisplay.size(); i++)
	{
		vpGuiDisplay.push_back(m_vpGuiDisplay[m_nArrayMode[i]]);
	}

	// 第一种 对象目标水平排列 
	CLayout* pDisplayLayout = new CHorizontalLayout;
	CLayout* pObject = new CVerticalLayout;
	CLayout* pTmp1 = new CHorizontalLayout;
	CLayout* pTmp2 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[0]);
	pTmp1->AddWnd(vpGuiDisplay[1]);
	pTmp2->AddWnd(vpGuiDisplay[2]);
	pTmp2->AddWnd(vpGuiDisplay[3]);
	pObject->AddLayout(pTmp1);
	pObject->AddLayout(pTmp2);


	CLayout* pTarget = new CVerticalLayout;
	pTmp1 = new CHorizontalLayout;
	pTmp2 = new CHorizontalLayout;
	pTmp1->AddWnd(vpGuiDisplay[4]);
	pTmp1->AddWnd(vpGuiDisplay[5]);
	pTmp2->AddWnd(vpGuiDisplay[6]);
	pTmp2->AddWnd(vpGuiDisplay[7]);
	pTarget->AddLayout(pTmp1);
	pTarget->AddLayout(pTmp2);

	pDisplayLayout->AddLayout(pObject);
	pDisplayLayout->AddLayout(pTarget);

	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpDisplayLayout.push_back(pDisplayLayout);
}


void CDlgVisionView2::InitPosShowMode()
{
	//只显示位置1~8
	for(int i = 0; i < 8; i++)
	{
		CLayout* pDisplayLayout = new CHorizontalLayout;
		pDisplayLayout->AddWnd( i >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[i]);
		pDisplayLayout->SetParentLayout(m_pViewDisplay);
//		pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
		m_vpChkPosDisplayLayout.push_back(pDisplayLayout);
	}

	
	// 同时显示(1,2)
	CLayout* pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd( 0 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[0]);
	pDisplayLayout->AddWnd( 1 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[1]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpChkPosDisplayLayout.push_back(pDisplayLayout);


	// 同时显示(3,4)
	pDisplayLayout = new CHorizontalLayout;
	pDisplayLayout->AddWnd(2 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[2]);
	pDisplayLayout->AddWnd(3 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[3]);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpChkPosDisplayLayout.push_back(pDisplayLayout);


	// 同时显示(1,2,3,4)
	pDisplayLayout = new CVerticalLayout;
	CLayout* pH1 = new CHorizontalLayout;
	CLayout* pH2 = new CHorizontalLayout;
	pH1->AddWnd(0 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[0]);
	pH1->AddWnd(1 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[1]);
	pH2->AddWnd(2 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[2]);
	pH2->AddWnd(3 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[3]);
	pDisplayLayout->AddLayout(pH1);
	pDisplayLayout->AddLayout(pH2);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpChkPosDisplayLayout.push_back(pDisplayLayout); 
	

	
	// 同时显示(5,6,7,8)
	pDisplayLayout = new CVerticalLayout;
	pH1 = new CHorizontalLayout;
	pH2 = new CHorizontalLayout;
	pH1->AddWnd(4 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[4]);
	pH1->AddWnd(5 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[5]);
	pH2->AddWnd(6 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[6]);
	pH2->AddWnd(7 >= m_vpGuiDisplay.size() ? NULL : m_vpGuiDisplay[7]);
	pDisplayLayout->AddLayout(pH1);
	pDisplayLayout->AddLayout(pH2);
	pDisplayLayout->SetParentLayout(m_pViewDisplay);
//	pDisplayLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW_AREA));
	m_vpChkPosDisplayLayout.push_back(pDisplayLayout); 
	
	
	// 全部显示(使用全部显示布局)
}

int CDlgVisionView2::GetVisionRunWidth()
{
	return m_nVisionRunWidth;
}

void CDlgVisionView2::SetVisionRunWidth(int nWidth)
{
	m_nVisionRunWidth = nWidth;
	if (m_pMainLayout)
	{
		if (m_vpDlgVisionRun.size() == 1)
		{
			m_pMainLayout->ResetWnd(m_pDlgVisionRun, m_nVisionRunWidth, 0, CLayout::eSizePolicy_Fixed);
		}
		else
		{
			m_pMainLayout->ResetWnd(&m_tabRunInfo, m_nVisionRunWidth, 0, CLayout::eSizePolicy_Fixed);
		}
	}
	if (m_pMainLayout2)
	{
		if (m_vpDlgVisionRun.size() == 1)
		{
			m_pMainLayout2->ResetWnd(m_pDlgVisionRun, 0, m_nVisionRunWidth, CLayout::eSizePolicy_Fixed);
		}
		else
		{
			m_pMainLayout2->ResetWnd(&m_tabRunInfo, 0, m_nVisionRunWidth, CLayout::eSizePolicy_Fixed);
		}
	}
}

int CDlgVisionView2::GetViewLayoutType()
{
	return m_nDisplayShowMode;
}

void CDlgVisionView2::SetViewLayoutType(int nViewLayoutType)
{
	 if (nViewLayoutType< 0 || nViewLayoutType >= m_vpDisplayLayout.size())
	 {
		 nViewLayoutType = 0;
	 }

	 m_nDisplayShowMode= nViewLayoutType;
}

int CDlgVisionView2::GetCurMainLayoutIndex()
{
	return m_nCurMainLayout;
}

void CDlgVisionView2::SetCurMainLayoutIndex(int nIndex)
{
   if (m_nCurMainLayout == nIndex)
   {
	   return;
   }
	
   switch(nIndex)
   {
   case 0:
	   m_nCurMainLayout = 0;
	   m_pCurMainLayout = m_pMainLayout;
	   break;
   case 1:
	   m_nCurMainLayout = 1;
	   m_pCurMainLayout = m_pMainLayout2;
	   break;
   default:
	   m_nCurMainLayout = 0;
	   m_pCurMainLayout = m_pMainLayout;
	   break;
   }
}

void CDlgVisionView2::UpdateViewLayout()
{
	// 全部位置显示
	UpdatePosShowLayout(m_nPosShowMode);
	UpdateGuiPos();
}

void CDlgVisionView2::UpdateMainLayout()
{
   if (m_pCurMainLayout)
   {
	   m_pCurMainLayout->OnSize();
   }

   if (m_pCurDisplayLayout)
   {
	   m_pCurDisplayLayout->OnSize();
   }

   if (m_pTabLayout)
   {
	   m_pTabLayout->OnSize();
   }
}


void CDlgVisionView2::UpdatePosShowLayout(int nChkId)
{
	for (int i = 0; i < m_vpGuiDisplay.size(); i++)
	{
		m_vpGuiDisplay[i]->ShowWindow(SW_HIDE);
	}
	
	if (nChkId >= 0 && nChkId < 8)
	{
		m_vpGuiDisplay[nChkId]->ShowWindow(SW_SHOW);
	}

	if (nChkId == 8)
	{
		m_vpGuiDisplay[0]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[1]->ShowWindow(SW_SHOW);
	}
	if (nChkId == 9)
	{
		m_vpGuiDisplay[2]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[3]->ShowWindow(SW_SHOW);
	}
	if (nChkId == 10)
	{
		m_vpGuiDisplay[0]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[1]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[2]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[3]->ShowWindow(SW_SHOW);
	}
	if (nChkId == 11)
	{
		m_vpGuiDisplay[4]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[5]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[6]->ShowWindow(SW_SHOW);
		m_vpGuiDisplay[7]->ShowWindow(SW_SHOW);
	}
	if (nChkId == 12)
	{
		for (int i = 0; i < m_vpGuiDisplay.size(); i++)
		{
			m_vpGuiDisplay[i]->ShowWindow(SW_SHOW);
		}
	}

	if (nChkId == 12)
	{
		m_pCurDisplayLayout = m_vpDisplayLayout[m_nDisplayShowMode];
	}
	else
	{
		m_pCurDisplayLayout = m_vpChkPosDisplayLayout[nChkId];
	}
	m_pCurDisplayLayout->OnSize();
}

void CDlgVisionView2::UpdateGuiPos()
{
	if (m_pVisionASM)
	{
		for (int k = 0; k < m_pVisionASM->GetPosNum(); k++)
		{
			CVisionAlignMarkPosGui* pPosConstGui = m_pVisionASM->m_cVisionAlignGui.GetVisionAlignConstGui(k);
			if (pPosConstGui)
			{
				pPosConstGui->SetGuiTextPos();
			}
		}
	}
}

void CDlgVisionView2::SetVisionRunDlg(CWnd* pVisionRunDlg)
{
	m_pDlgVisionRun = (CDlgVisionRun*)pVisionRunDlg;
}


void CDlgVisionView2::AddVisionRunDlg(CWnd* pVisionRunDlg)
{
    m_vpDlgVisionRun.push_back((CDlgVisionRun*)pVisionRunDlg);;
}
	

BOOL CDlgVisionView2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_cfontWSName.CreateFont(14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("黑体"));					// lpszFacename)

	if (GetDlgItem(IDC_STATIC_WS)->GetSafeHwnd() != NULL)
	{
		GetDlgItem(IDC_STATIC_WS)->SetFont(&m_cfontWSName);
	}

	InitUIStyle();
	InitTabRunInfo();
	InitUILayout();
	SetSelPosModeBtnVisible();



	CRect rect;
	GetDlgItem(IDC_STATIC_STATUS)->GetWindowRect(rect);	
	ScreenToClient(rect);
	//m_nAlignerInfoHeight = 0;

	UpdateDialogLanguage();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
	}

	GetDlgItem(IDC_STATIC_CH0_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH1_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH2_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH3_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH4_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH5_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH6_DISPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CH7_DISPLAY)->ShowWindow(SW_HIDE);

	int nDisplayNum = m_nShowDisplayNum;
	m_vpGuiDisplay.resize(nDisplayNum, NULL);
	m_nFitImage = 0;
	for (int i=0; i<nDisplayNum; i++)
	{
		GetDlgItem(IDC_STATIC_CH0_DISPLAY+i)->ShowWindow(SW_SHOW);
		m_vpGuiDisplay.at(i) = new scGuiDisplay;
		m_vpGuiDisplay.at(i)->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		//m_vpGuiDisplay.at(i)->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		m_vpGuiDisplay.at(i)->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		COLORREF TmpColor = RGB(0,51,102);
		m_vpGuiDisplay.at(i)->SetBackgroundParam(bgDispBGPure,TmpColor);
		m_vpGuiDisplay.at(i)->SetAutoFit(FALSE);
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_vpGuiDisplay.at(i)->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_vpGuiDisplay.at(i)->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_vpGuiDisplay.at(i)->SetLanguage(1);
			break;
		}
	}

	InitDisplayLayout();
	InitPosShowMode();
	m_pCurDisplayLayout = m_vpDisplayLayout[m_nDisplayShowMode];

	m_vpGuiText.resize(m_nShowDisplayNum, NULL);
	m_vpGuiCross.resize(m_nShowDisplayNum, NULL);
	m_vbGuiCrossVisible.resize(m_nShowDisplayNum, FALSE);
	CString strKey;
	CString strInfo;
	
	for (int k = 0; k < m_nShowDisplayNum; k++)
	{
		scGuiStaticContainer* pSContainer = m_vpGuiDisplay.at(k)->GetStaticContainer();
		scGuiStaticContainer* pCContainer = m_vpGuiDisplay.at(k)->GetConstContainer();


		m_vpGuiText.at(k) = new scGuiText;
		m_vpGuiText.at(k)->SetXYRotation(sc2Vector(10,10),scRadian(0));
		m_vpGuiText.at(k)->SetLineColor(RGB(0,255,0));
		//m_vpGuiText.at(k)->SetVisible(TRUE);
		m_vpGuiText.at(k)->SetVisible(FALSE);//不显示
		strInfo.Format(GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_CPFAST_STR_POS), k+1);

		m_vpGuiText.at(k)->SetText(strInfo);
		strKey.Format(_T("ViewGuiText%d"),k);
		//pSContainer->AddItem(m_vpGuiText.at(k), strKey);
		pCContainer->AddItem(m_vpGuiText.at(k), strKey);


		m_vpGuiCross.at(k) = new scGuiCross;
		m_vpGuiCross.at(k)->SetLineColor(RGB(0,255,0));
		strKey.Format(_T("ViewGuiCross%d"),k);
		pSContainer->AddItem(m_vpGuiCross.at(k), strKey);		

		m_vpGuiDisplay.at(k)->SetStaticContainer(pSContainer);
		m_vpGuiDisplay.at(k)->SetConstContainer(pCContainer);
	}
	


	m_labelStatus.SetBkColor(RGB(255,255,0));
	m_labelStatus.SetTextColor(RGB(0,0,0));
	m_labelStatus.SetFontBold(TRUE);
	m_labelStatus.SetFontSize(15);
	m_labelStatus.SetText(_T(""));


	m_labelOtherStatus.SetBkColor(RGB(255,255,0));
	m_labelOtherStatus.SetTextColor(RGB(0,0,0));
	m_labelOtherStatus.SetFontBold(TRUE);
	m_labelOtherStatus.SetFontSize(15);
	m_labelOtherStatus.SetText(_T(""));

	UpdateManualSearchButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CDlgVisionView2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CDlgVisionView2::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
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

	int i=0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay.at(i);
		m_vpGuiDisplay.at(i) = NULL;

// 		if ( m_vpGuiText.at(i))
// 		{
// 			delete m_vpGuiText.at(i);
// 			m_vpGuiText.at(i) = NULL;
// 		}
// 
// 		if ( m_vpGuiCross.at(i))
// 		{
// 			delete m_vpGuiCross.at(i);
// 			m_vpGuiCross.at(i) = NULL;
// 		}

	}

	for (i=0; i<m_vpGuiText.size(); i++)
	{
		if ( m_vpGuiText.at(i))
		{
			delete m_vpGuiText.at(i);
			m_vpGuiText.at(i) = NULL;
		}
	}

	for (i=0; i<m_vpGuiCross.size(); i++)
	{
		if ( m_vpGuiCross.at(i))
		{
			delete m_vpGuiCross.at(i);
			m_vpGuiCross.at(i) = NULL;
		}
	}

	m_vpGuiDisplay.clear();
	return CDialog::DestroyWindow();
}


BOOL CDlgVisionView2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	CWnd* pHwnd = GetDlgItem(IDC_STATIC_STATUS);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == pHwnd->GetSafeHwnd())
	{
		::PostMessage(m_pParent->GetOperationDlgHwnd(), WM_SHOW_OPERATION_DLG, m_nPlatformID, 0);
	}
	return CDialog::PreTranslateMessage(pMsg);
}


LRESULT CDlgVisionView2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_nFitImage<100)
	{
		m_nFitImage++;
		//m_nFitImageBool = TRUE;
	}
	if (m_pVisionASM != NULL)
	{
		switch (message)
		{
		case WM_UPDATE_DISPLAY_IMAGE:
			{

				
				

				SysPlatformInfo sysPlatformInfo;
				m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

				// 如果按照相机数显示、判断当前更新图片位置是不是第一个，如果不是没有必要再次更新
				if (sysPlatformInfo.m_nShowMode == 1)  
				{
					int nCameraIndex = m_pVisionASM->GetPosCameraIndex((int)wParam);
					int nFirstPosIndex =  m_pVisionASM->GetCameraFirstPosIndex(nCameraIndex);
					if (nFirstPosIndex != wParam)
					{
						if (sysPlatformInfo.m_bTargetInDifferentComputer && sysPlatformInfo.m_bTargetObjectCamSeparate && (int)wParam >= m_pVisionASM->GetPosNum()/2)
						{

						}
						else
						{
							break;
						}
						
					}
				}

				// 共享工位，不刷新图像
				if (sysPlatformInfo.m_bPlatformShareShow)
				{
					return 0;
				}

				

				m_pVisionASM->UpdateDisplayImage((int)wParam);

				int nIndex = (int)wParam;
				if (m_vpGuiDisplay.size()==1)
				{
					nIndex=0;
				}
				if (m_nShowMode == 0)
				{

					if (nIndex < m_vpGuiDisplay.size() && m_vpGuiDisplay.at(nIndex)!=NULL && m_vpGuiDisplay.at(nIndex)->GetSafeHwnd())
					{

						if (m_nFitImage<100)
						{
							m_vpGuiDisplay.at(nIndex)->FitImage();
							//m_vpGuiCross.at(nIndex)->FitImage();
						}
						if (m_vpGuiDisplay.at(nIndex)->HasImage()  && m_vbGuiCrossVisible.at(nIndex) == TRUE)
						{
							m_vpGuiCross.at(nIndex)->SetVisible(TRUE);
						}
						else
						{
							
							m_vpGuiCross.at(nIndex)->SetVisible(FALSE);	//
						}
					}
				}
				else
				{
					int nCamIndex = m_pVisionASM->GetPosCameraIndex(nIndex);
					if (nCamIndex < m_vpGuiDisplay.size() && m_vpGuiDisplay.at(nCamIndex)!=NULL && m_vpGuiDisplay.at(nCamIndex)->GetSafeHwnd())
					{
						if (m_nFitImage<100)
						{
							m_vpGuiDisplay.at(nIndex)->FitImage();
							//m_vpGuiCross.at(nIndex)->FitImage();
						}
						if (m_vpGuiDisplay.at(nCamIndex)->HasImage()  && m_vbGuiCrossVisible.at(nCamIndex) == TRUE)
						{
							m_vpGuiCross.at(nCamIndex)->SetVisible(TRUE);
						}
						else
						{
							m_vpGuiCross.at(nCamIndex)->SetVisible(FALSE);
						}
					}
				}
			}
			break;

		case WM_UPDATE_STATUSBAR_INFO:
			{				
					m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
					UpdateStatusBar();
			}
			break;

// 		case WM_UPDATE_OTHER_PLATFORM_STATUSBAR_INFO:
// 			{
// 				int nOtherPlatformId = wParam;
// 				if (nOtherPlatformId>=0 && nOtherPlatformId <m_pParent->m_vpVisionASM.size())
// 				{
// 
// 					m_pParent->m_vpVisionASM[nOtherPlatformId]->GetStatusBarInfo(m_bOtherPlatformStatusBarFlag, m_strOtherPlatformStatusBarInfo1, m_strOtherPlatformStatusBarInfo2);
// 					UpdateOtherPlatformStatusBar();
// 				}
// 			}
// 			break;

		case WM_UPDATE_CALIBRATE_TARGET_GRAPHIC:
			{
				//m_pVisionASM->UpdateAlnObjectGraphics((int)wParam, (BOOL)lParam);
				m_pVisionASM->ShowGuiCalibrateGraphics((int)wParam, (BOOL)lParam);
				if (m_pParent!=NULL && m_pParent->m_SysOptionConfig.m_nDisplayMode == 1)
				{
					int nIndex = (int)wParam;
					if (m_vpGuiDisplay.size()==1)
					{
						nIndex=0;
					}
					nIndex = m_pVisionASM->GetPosCameraIndex((int)wParam);
					if (nIndex < m_vpGuiDisplay.size() && m_vpGuiDisplay.at(nIndex)!=NULL && m_vpGuiDisplay.at(nIndex)->GetSafeHwnd())
					{

						m_vpGuiDisplay.at(nIndex)->Invalidate();
					}
				}
			}
			break;

		case WM_CAMERA_REMOVAL:
			{
				scGuiDisplay* pDisplay = m_pVisionASM->GetCameraDisplay((int)wParam);
				if ((pDisplay != NULL) && ::IsWindow(pDisplay->GetSafeHwnd()))
				{
					pDisplay->ClearScreen();
				}

				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
			break;

		case WM_CAMERA_ARRIVAL:
			{
				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
			break;

		case WM_UPDATE_CHANGE_PRODUCT_SET_DISPLAY:
			{
				m_pVisionASM->UpdateChangeProductSetDisplay((int)wParam, (int)lParam);
			}
			break;

		case WM_SAVE_VDB_FILE:
			{

				SysPlatformInfo sysPlatformInfo;
				m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
				int nProcessType = wParam / 100;
				wParam = wParam % 100;
				int nCamIndex = (int)wParam;

				if (sysPlatformInfo.m_nShowMode ==1 && sysPlatformInfo.m_eAlignerTargetMoveType != AlignerTargetMoveType::eTargetMoveByPlatform) // 修复移动相机拍照时，保存VDB Mark错位bug
				{
					nCamIndex = m_pVisionASM->GetPosCameraIndex((int)wParam);
				}

				if (nCamIndex < m_vpGuiDisplay.size() && m_vpGuiDisplay.at(nCamIndex)!=NULL && m_vpGuiDisplay.at(nCamIndex)->GetSafeHwnd())
				{
					m_pVisionASM->SaveSearchVDBFile(m_vpGuiDisplay.at(nCamIndex),(int)wParam,(SearchMode)lParam,nProcessType);
				}
				
			}
			break;

		case WM_UPDATE_ALIGNER_GUI_SHOW:
			{
				int nPosIndex = LOWORD(wParam);
				BOOL bVisible = HIWORD(wParam);
				int nSearchMode = lParam;
				if (bVisible)
				{
					m_pVisionASM->m_cVisionAlignGui.UpdatePosSearchResult(nPosIndex, nSearchMode);
				}
				else
				{
					m_pVisionASM->m_cVisionAlignGui.UpdatePosGuiInvisible(nPosIndex, nSearchMode);
				}
			}
			break;

		case WM_UPDATE_ALIGNER_COMBINE_GUI_SHOW:
			{
				int nPosIndex = LOWORD(wParam);
				BOOL bVisible = HIWORD(wParam);
				int nSearchMode = lParam;
				if (bVisible)
				{
					m_pVisionASM->m_cVisionAlignGui.UpdatePosObjectToTargetResultLine(nPosIndex);
				}
				else
				{
					m_pVisionASM->m_cVisionAlignGui.UpdatePosObjectToTargetLineGuiInvisible(nPosIndex);
				}
			}
			break;
		case WM_UPDATE_ALINGER_OFFSET_AREA_GUI_SHOW:
			{
				int nPosIndex = LOWORD(wParam);
				BOOL bVisible = HIWORD(wParam);
				int nSearchMode = lParam;
				if (bVisible)
				{
					m_pVisionASM->m_cVisionAlignGui.UpdateAllPosAlignOffsetByAreaShow(nPosIndex);
				}
				else
				{
					m_pVisionASM->m_cVisionAlignGui.UpdateAllPosAlignOffsetByAreaInvisible(nPosIndex);
				}
			}
			break;
		default:
			break;
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


void CDlgVisionView2::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pCurMainLayout)
	{
		m_pCurMainLayout->OnSize();
	}

	if (m_pCurDisplayLayout)
	{
		m_pCurDisplayLayout->OnSize();
	}

	if (m_pTabLayout)
	{
		m_pTabLayout->OnSize();
	}

	UpdateGuiPos();


	//MoveDlgItem();
}





void CDlgVisionView2::SetCameraNumber(int nCamNum, int nPositionNum, int nShowMode,/* int nPositionShowMode,*/ int nArrayMode[MAXPOSNUM])
{
	m_nCamNum = nCamNum;
	m_nPositionNum = nPositionNum;
	m_nShowMode = nShowMode;
	m_nShowDisplayNum = m_nShowMode == 0 ? m_nPositionNum : m_nCamNum;
	
	for (int i=0; i<MAXPOSNUM; i++)
	{
		m_nArrayMode[i] = nArrayMode[i];
	}
}

CWnd* CDlgVisionView2::SetParentWnd( CWnd* pParent )
{
	CWnd* pPrevParent = m_pParent;
	m_pParent = (CVisionASMDlg*)pParent;
	return pPrevParent;
}



void CDlgVisionView2::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	m_pVisionASM = pVisionASM;
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo spi;
		std::vector<scGuiDisplay*>  vpGuiDisplay;		// GUI显示控件

		m_pVisionASM->GetSysPlatformInfo(spi);
		if (spi.m_bPlatformShareShow)
		{
		    vpGuiDisplay = m_pParent->m_pDlgVisionView[spi.m_nShareShowPlatformIndex].m_vpGuiDisplay;
		}
		else
		{
			vpGuiDisplay = m_vpGuiDisplay;
		}


		if (m_nShowMode == 0) // 按位置数 
		{
			int nPosNum = m_pVisionASM->GetPosNum();
			for (int i=0; i<nPosNum; i++)
			{
				m_pVisionASM->SetPosDisplay(i, vpGuiDisplay.at(i));
			}
		}
		else
		{
			int nCamNum = m_pVisionASM->GetCameraNum();
			int nPosNum = m_pVisionASM->GetPosNum();
			for (int i=0; i<nCamNum; i++)
			{
				for (int j = 0; j < nPosNum; j++)
				{
					CPosInfo pos;
					m_pVisionASM->GetPosInfo(j, pos);
					if (pos.m_nCamIndex == i)
					{
						m_pVisionASM->SetPosDisplay(j, vpGuiDisplay.at(i));
					}
				}
			}
		}
	}
	UpdateCrossInfo();
}

void CDlgVisionView2::SetPlatformID( LPCTSTR strID )
{
	m_strPlatformID = strID;
	UpdatePlatformID();
}

CString CDlgVisionView2::GetPlatformIDString()
{
	return m_strPlatformID;
}

void CDlgVisionView2::UpdatePlatformID()
{
	if (GetDlgItem(IDC_STATIC_WS)->GetSafeHwnd() != NULL)
	{
		GetDlgItem(IDC_STATIC_WS)->SetWindowText(m_strPlatformID);
	}
}


void CDlgVisionView2::SetPlatformID(int nID)
{
	m_nPlatformID = nID;
}

int CDlgVisionView2::GetPlatformID()
{
	return m_nPlatformID;
}


// 更新手动按钮搜索按钮显示
void  CDlgVisionView2::UpdateManualSearchButton()
{
	CPlatformOptionInfo optionInfo;
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	}
	//if (m_nAlignerInfoRow == 5)
	{

		if (optionInfo.m_bManualAfTarSearchFailShowButton)
		{
			//GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(TRUE);
			//GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_HIDE);
		}

		if (optionInfo.m_bManualAfObjSearchFailShowButton)
		{
			//GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(TRUE);
			//GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_HIDE);
		}
	}

	if (m_pTitleLayout)
	{
		m_pTitleLayout->OnSize();
	}

	//else if (m_nAlignerInfoRow == 6)
// 	{
// 		if (m_pParent->m_SysOptionConfig.m_bManualAfTarSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (m_pParent->m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 	}
	//else if (m_nAlignerInfoRow == 8)
// 	{
// 		if (m_pParent->m_SysOptionConfig.m_bManualAfTarSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (m_pParent->m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH)->ShowWindow(SW_HIDE);
// 		}
// 	}

	
}
void CDlgVisionView2::UpdateStatusBar()
{
// 	if (m_pStatusBar != NULL)
// 	{
// 		m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
// 		//m_pStatusBar->SetText(m_strPlatformID, 1, 0);
// 		m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
// 		m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
// 	}

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);


	if (spi.m_bPlatformShareShow)
	{
		m_pParent->m_pDlgVisionView[spi.m_nShareShowPlatformIndex].UpdateOtherPlatformStatusBar(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
	}
	else
	{
		if (GetDlgItem(IDC_STATIC_STATUS)->GetSafeHwnd() != NULL)
		{
			if (m_bStatusBarFlag)
			{
				//m_labelStatus.SetBkColor(RGB(0,127,197));
				m_labelStatus.SetBkColor(RGB(54,164,186));

			}
			else
			{
				/*m_labelStatus.SetBkColor(RGB(255,128,58));*/
				m_labelStatus.SetBkColor(RGB(201,91,69));
			}
			m_labelStatus.SetText(m_strStatusBarInfo1 + _T(":") + m_strStatusBarInfo2);

		}
	}

	
}


void CDlgVisionView2::UpdateOtherPlatformStatusBar()
{
	if (GetDlgItem(IDC_STATIC_STATUS_OTHER_PLATFORM)->GetSafeHwnd() != NULL)
	{
		if (m_bOtherPlatformStatusBarFlag)
		{
			//m_labelStatus.SetBkColor(RGB(0,127,197));
			m_labelOtherStatus.SetBkColor(RGB(54,164,186));

		}
		else
		{
			/*m_labelStatus.SetBkColor(RGB(255,128,58));*/
			m_labelOtherStatus.SetBkColor(RGB(201,91,69));
		}
		m_labelOtherStatus.SetText(m_strOtherPlatformStatusBarInfo1 + _T(":") + m_strOtherPlatformStatusBarInfo2);

	}
}

void CDlgVisionView2::UpdateStatusBar( BOOL bFlag, LPCTSTR lpszInfo1, LPCTSTR lpszInfo2 )
{
	m_bStatusBarFlag = bFlag;
	m_strStatusBarInfo1 = lpszInfo1;
	m_strStatusBarInfo2 = lpszInfo2;
	UpdateStatusBar();
}

void CDlgVisionView2::UpdateOtherPlatformStatusBar( BOOL bFlag, LPCTSTR lpszInfo1, LPCTSTR lpszInfo2 )
{
	m_bOtherPlatformStatusBarFlag = bFlag;
	m_strOtherPlatformStatusBarInfo1 = lpszInfo1;
	m_strOtherPlatformStatusBarInfo2 = lpszInfo2;
	UpdateOtherPlatformStatusBar();
}



void CDlgVisionView2::UpdateCrossInfo()
{
	if (m_pVisionASM != NULL)
	{
		ImageRoi roi;

		if (m_nShowMode == 0) // 按位置数 
		{
			for(int i = 0; i < m_nShowDisplayNum; i++)
			{
	//			m_pVisionASM->GetImageROIParam(i, roi);
				m_pVisionASM->GetPosImageROIParam(i,roi);
				m_vpGuiCross.at(i)->SetCenterXYWidthHeight(roi.m_dCrossX, roi.m_dCrossY,30000,30000);
				m_vbGuiCrossVisible.at(i) = roi.m_bCrossVisible;
			}
		}
		else // 按相机数
		{
			int nPosIndex = 0;
			for(int i = 0; i < m_nShowDisplayNum; i++)
			{
				//			m_pVisionASM->GetImageROIParam(i, roi);
				nPosIndex = m_pVisionASM->GetCameraFirstPosIndex(i);
				m_pVisionASM->GetPosImageROIParam(nPosIndex,roi);
				m_vpGuiCross.at(i)->SetCenterXYWidthHeight(roi.m_dCrossX, roi.m_dCrossY,30000,30000);
				m_vbGuiCrossVisible.at(i) = roi.m_bCrossVisible;
			}
		}
	}
}

void CDlgVisionView2::UpdateDialogLanguage()
{
//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_VISION_VIEW2") == strDlgID)
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

void CDlgVisionView2::UpdateShowLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
	}

	for (int i=0; i<m_nShowDisplayNum; i++)
	{
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			if(m_vpGuiDisplay.at(i)!=NULL) m_vpGuiDisplay.at(i)->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			if(m_vpGuiDisplay.at(i)!=NULL) m_vpGuiDisplay.at(i)->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			if(m_vpGuiDisplay.at(i)!=NULL) m_vpGuiDisplay.at(i)->SetLanguage(1);
			break;
		}
		CString strInfo;
		strInfo.Format(GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_CPFAST_STR_POS), i+1);

		if (m_vpGuiText.at(i)!=NULL) m_vpGuiText.at(i)->SetText(strInfo);


		m_vpGuiDisplay.at(i)->MyInvalidate();
	}
}


void CDlgVisionView2::OnBnClickedBtnDisplayVision()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面显示窗口[显示][工位%d]\n"),m_nPlatformID+1);
	g_opLogFile.SaveLogText(strLog,1);
// 	m_pParent->ShowVisionView(m_nPlatformID);
// 	bool bVisible = m_pParent->GetViewRunVisible(m_nPlatformID);
// 	if (bVisible)
// 	{
// 		GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T(">>"));
// 		m_pParent->SetViewRunVisible(m_nPlatformID, false);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T("<<"));
// 		m_pParent->SetViewRunVisible(m_nPlatformID, true);
// 	}

	if (m_vpDlgVisionRun.size() == 1)
	{
		if (m_pDlgVisionRun->IsWindowVisible())
		{
			GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T(">>"));
			m_pDlgVisionRun->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T("<<"));
			m_pDlgVisionRun->ShowWindow(SW_SHOW);
		}
		m_pCurMainLayout->OnSize();
		m_vpDisplayLayout[m_nDisplayShowMode]->OnSize();
	}
	else
	{
		if (m_tabRunInfo.IsWindowVisible())
		{
			GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T(">>"));
			m_tabRunInfo.ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_BTN_DISPLAY_VISION)->SetWindowText(_T("<<"));
			m_tabRunInfo.ShowWindow(SW_SHOW);
		}

		m_pCurMainLayout->OnSize();
		m_vpDisplayLayout[m_nDisplayShowMode]->OnSize();
		m_pTabLayout->OnSize();
	}
	
// 	m_pCurMainLayout->OnSize();
	UpdateWindow();
}


BOOL CDlgVisionView2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}


void CDlgVisionView2::OnBnClickedBtnManualTargetSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面显示窗口[手动目标搜索]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	m_pVisionASM->ExecuteManualSearchMark(eTargetSearch);
}


void CDlgVisionView2::OnBnClickedBtnManualObjectSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面显示窗口[手动对象搜索]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	m_pVisionASM->ExecuteManualSearchMark(eObjectSearch);
}


void CDlgVisionView2::OnBnClickedRadioDisplayPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nPrePosShowMode == m_nPosShowMode)
	{
		return;
	}

	//m_nPosShowMode 按照排序
	UpdatePosShowLayout(m_nPosShowMode);
	UpdateGuiPos();

	m_nPrePosShowMode = m_nPosShowMode;
}

void CDlgVisionView2::SetSearchTool(int nPos,BOOL bObjectSearchTool)
{
	if(NULL== m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	int nProductIdx= 0;
	CString strTmpTitle = _T("定位模板设置");
	if(m_pVisionASM->GetTotalProuductData()!= NULL)
	{
		nProductIdx = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	}
	else
	{
		m_pVisionASM->SetStatusBarInfo(FALSE,_T("产品列表为空"),_T("")); 
		m_pVisionASM->PostMessageUpdateStatusBarInfo();
		return;
	}

	if(m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIdx)== NULL)
	{
		m_pVisionASM->SetStatusBarInfo(FALSE,_T("当前产品为空"),_T("")); 
		m_pVisionASM->PostMessageUpdateStatusBarInfo();
		return;
	}

	// 权限控制,没有启用模板时，无法设置模板；
	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);
	bool bShowTargetTemplateSet = false;
	bool bEnableTargetTemplateSet = false;
	bool bShowObjectTemplateSet = false;
	bool bEnableObjectTemplateSet = false;
	if (NULL !=vsConfig)
	{
		CString strNodeName = _T("");
		vsConfig->GetCheckedByItem(eProductAuthorize_TargetTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShowTargetTemplateSet,bEnableTargetTemplateSet);
		vsConfig->GetCheckedByItem(eProductAuthorize_ObjectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShowObjectTemplateSet,bEnableObjectTemplateSet);
	}

	CBaseSearchTool *pSearchTool = NULL;

	if((sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum/2) 
		|| (!sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum && bObjectSearchTool==TRUE))
	{
		if(bEnableObjectTemplateSet==false) 
		{
			m_pVisionASM->SetStatusBarInfo(FALSE,_T("权限管理-对象模板设置未启用"),_T("")); 
			m_pVisionASM->PostMessageUpdateStatusBarInfo();
			return;
		}
		pSearchTool = m_pVisionASM->GetRepoAlnObjectSearchTool(nProductIdx,nPos,0);
		strTmpTitle.Format(_T("对象位置%d定位模板设置"),nPos+1);
	}
	else if((sysPlatformInfo.m_bTargetObjectCamSeparate && nPos>=sysPlatformInfo.m_nPositionNum/2)
		|| (!sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum && bObjectSearchTool==FALSE))
	{
		if(bEnableTargetTemplateSet==false) 
		{
			m_pVisionASM->SetStatusBarInfo(FALSE,_T("权限管理-模板模板设置未启用"),_T("")); 
			m_pVisionASM->PostMessageUpdateStatusBarInfo();
			return;
		}

		pSearchTool = m_pVisionASM->GetRepoAlnTargetSearchTool(nProductIdx,nPos,0);
		strTmpTitle.Format(_T("目标位置%d定位模板设置"),nPos+1);
	}
	else
	{
		return;
	}

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
			CString strStatusBarInfo1 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_STR_ATTENTION);
			CString strStatusBarInfo2 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_STR_DISK_OUT);
			m_pVisionASM->SetStatusBarInfo(FALSE, strStatusBarInfo1, strStatusBarInfo2 + _T("\n无法新建模板！"));
			m_pVisionASM->PostMessageUpdateStatusBarInfo();
			
			CString strTempWarningType = GetSysInfoString(m_pParent->m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
			m_pVisionASM->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
			// 避免后续无法保存的情况
			return;
		}
	}

	

	

	CDlgSearch dlgSearch(this, pSearchTool);
	dlgSearch.m_strTitleName = strTmpTitle;
	dlgSearch.m_nCurPositionIdx = nPos;
	dlgSearch.SetVisionASMRef(m_pVisionASM, nPos);
	dlgSearch.SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
	dlgSearch.EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);

	if (dlgSearch.DoModal() == IDOK)
	{
		pSearchTool = dlgSearch.GetSearchTool();
		if(pSearchTool)
		{
			if((sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum/2) 
				|| (!sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum && bObjectSearchTool==TRUE))
			{
				m_pVisionASM->SetRepoAlnObjectSearchTool(nProductIdx, nPos, 0, pSearchTool);
				m_pVisionASM->SaveAlnObjectSearchToolRepository(nProductIdx);
			}
			else if((sysPlatformInfo.m_bTargetObjectCamSeparate && nPos>=sysPlatformInfo.m_nPositionNum/2)
				|| (!sysPlatformInfo.m_bTargetObjectCamSeparate && nPos<sysPlatformInfo.m_nPositionNum && bObjectSearchTool==FALSE))
			{
				m_pVisionASM->SetRepoAlnTargetSearchTool(nProductIdx, nPos, 0, pSearchTool);
				m_pVisionASM->SaveAlnTargetSearchToolRepository(nProductIdx);
			}

			m_pVisionASM->SetStatusBarInfo(TRUE,_T("主界面"),strTmpTitle + _T("修改模板完成")); 
			m_pVisionASM->PostMessageUpdateStatusBarInfo();
		}
	}
}

LRESULT CDlgVisionView2::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	if(m_pParent == NULL)
	{
		return S_OK;
	}

	if(NULL== m_pVisionASM)
	{
		return S_OK;
	}

	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	
	// 启用拍照时切换曝光时，主界面不可进行曝光设置和模板设置
	if(optionInfo.m_bEnableSearchShutter)
	{
		m_pVisionASM->SetStatusBarInfo(FALSE,GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDC_CHECK_ENABLE_SEARCH_SHUTTER),_T("启用时主界面不可进行曝光设置和模板设置")); 
		m_pVisionASM->PostMessageUpdateStatusBarInfo();
		return S_OK;
	}

	for(int it=0;it<m_vpGuiDisplay.size();it++)
	{
		if(m_vpGuiDisplay.at(it) && m_vpGuiDisplay.at(it)->GetID()==wParam)
		{
			int nPos = it;
			if (m_vpGuiDisplay.at(it)->GetImageCP() != NULL)
			{
				if (m_vpGuiDisplay.at(it)->GetImageCP()->IsValid())
				{
					long nWidth  = m_vpGuiDisplay.at(it)->GetImageCP()->Width();
					long nHeight = m_vpGuiDisplay.at(it)->GetImageCP()->Height();

					// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);

					// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
					double dImageX, dImageY;
					m_vpGuiDisplay.at(it)->TransformPos(x,y,dImageX,dImageY);

					if (dImageX<nWidth/2)
					{
						// 帧有效时，不进行曝光调整，无法实时观察到调整效果
						if(m_pParent->m_SysOptionConfig.m_nDisplayMode == 1 || !m_pParent->m_bIsRunning == TRUE)
						{
							m_pVisionASM->SetStatusBarInfo(FALSE,GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_STR_FRAME_VALID),_T("或停止运行时主界面不可进行曝光调整")); 
							m_pVisionASM->PostMessageUpdateStatusBarInfo();
							return S_OK;
						}
						SetCameraShutter(nPos);
					}
					else
					{
						// 软件运行时运行状态，以及没有登录状态 无法在主界面上修改模板
						if(m_pParent->m_bIsRunning == TRUE || m_pParent->m_bAuthorized== FALSE)
						{
							m_pVisionASM->SetStatusBarInfo(FALSE,_T(""),_T("未登录或停止运行时主界面不可进行模板设置")); 
							m_pVisionASM->PostMessageUpdateStatusBarInfo();
							return S_OK;
						}

						//按相机数目显示时 无法在主界面上修改模板
						if(sysPlatformInfo.m_nShowMode == 1)
						{
							m_pVisionASM->SetStatusBarInfo(FALSE,_T(""),_T("按相机数目显示时无法在主界面上修改模板")); 
							m_pVisionASM->PostMessageUpdateStatusBarInfo();
							return S_OK;
						}

						//对象目标不分离时,
						SetSearchTool(nPos,dImageY<nHeight/2);

					}
				}
			}


		}
	}

	return S_OK;

}

void CDlgVisionView2::SetCameraShutter(int nPosIndex)
{
	if(m_pParent == NULL)
	{
		return;
	}

	if(m_pVisionASM == NULL)
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
		return;
	}

	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);

	CDlgCalibTargetShutterSetting dlg;
	dlg.m_nPosIndex = nPosIndex;
	dlg.SetVisionAlignerRef(m_pVisionASM);
	dlg.SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);

	dlg.m_dEnvironmentShutter = dShutter;
	double dCurShutter = dShutter;
	int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
	if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
	{
		dCurShutter = vsConfig->m_vCameraParam.at(nCam).m_dShutter;
	}
	m_pVisionASM->m_bIsSetShutterInMain = TRUE;
	if (dlg.DoModal() == IDOK)
	{
		int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
		if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
		{
			vsConfig->m_vCameraParam.at(nCam).m_dShutter = dlg.m_dDmCodeShutter;
			m_pVisionASM->SaveCameraInfo();
			m_pVisionASM->SaveJobCameraInfo();
			m_pVisionASM->SetStatusBarInfo(TRUE,_T("主界面"),_T("调整曝光完成")); 
			m_pVisionASM->PostMessageUpdateStatusBarInfo();
		}
	}
	else
	{
		m_pVisionASM->SwitchToSearchShutter(nPosIndex, dShutter);
	}
	m_pVisionASM->m_bIsSetShutterInMain = FALSE;
}

void CDlgVisionView2::OnTcnSelchangeTabRunInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int nTabSel = m_tabRunInfo.GetCurSel();
	if (nTabSel >=0 && nTabSel < m_vpDlgVisionRun.size())
	{
		for (int i = 0; i < m_vpDlgVisionRun.size(); i++)
		{		 
			m_vpDlgVisionRun[i]->ShowWindow(SW_HIDE);
		}
		m_vpDlgVisionRun[nTabSel]->ShowWindow(SW_SHOW);
		m_pTabLayout->OnSize();
	}
	*pResult = 0;
}
