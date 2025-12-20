// DlgPlatformLayoutMode.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPlatformLayoutMode.h"
#include "afxdialogex.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"


// CDlgPlatformLayoutMode 对话框

IMPLEMENT_DYNAMIC(CDlgPlatformLayoutMode, CDialogEx)

CDlgPlatformLayoutMode::CDlgPlatformLayoutMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPlatformLayoutMode::IDD, pParent)
{
	m_pMainLayout = NULL;
	m_nPlatformNum = 0;
	m_nLayoutType = 0;
	m_nStHeight = 40;
	m_nStWidth = 40;
}

CDlgPlatformLayoutMode::~CDlgPlatformLayoutMode()
{
	FreeResource();
}

void CDlgPlatformLayoutMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPlatformLayoutMode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgPlatformLayoutMode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPlatformLayoutMode::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgPlatformLayoutMode 消息处理程序


void CDlgPlatformLayoutMode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgPlatformLayoutMode::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgPlatformLayoutMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgPlatformLayoutMode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message  == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




void CDlgPlatformLayoutMode::UpdateShow(int nLayoutType, int nPlatformNum)
{
	  m_nPlatformNum = nPlatformNum;
	  m_nLayoutType = nLayoutType;
	  FreeResource();
	  InitLayout();
	  SetStVisible();
	  
	  if (m_pMainLayout)
	  {
		  m_pMainLayout->OnSize();
	  }
}

void CDlgPlatformLayoutMode::FreeResource()
{
	  if (m_pMainLayout)
	  {
		  delete m_pMainLayout;
		  m_pMainLayout = NULL;
	  }
}


// 初始化UI显示
void CDlgPlatformLayoutMode::InitLayout()
{
	switch(m_nPlatformNum)
	{
	case 1:
		InitLayoutPlatformNum1();
		break;
	case 2:
		InitLayoutPlatformNum2();
		break;
	case 3:
		InitLayoutPlatformNum3();
		break;
	case 4:
		InitLayoutPlatformNum4();
		break;
	case 5:
		InitLayoutPlatformNum5();
		break;
	case 6:
		InitLayoutPlatformNum6();
		break;
	}
}


// 初始化平台数为1的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum1()
{
	// 类型1
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}
}


// 初始化平台数为2的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum2()
{
	// 类型1 水平排列
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

	// 类型2 垂直排列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

}


// 初始化平台数为3的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum3()
{
	// 类型1 水平排列
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型2 垂直排列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

	// 类型3 左1右2
	if (m_nLayoutType == 2)
	{
		m_pMainLayout = new CHorizontalLayout;

		CLayout* pTmp = new CVerticalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp, m_nStWidth, m_nStHeight*2, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型4 左2右1
	if (m_nLayoutType == 3)
	{
		m_pMainLayout = new CHorizontalLayout;

		CLayout* pTmp = new CVerticalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp, m_nStWidth, m_nStHeight*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight*2, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型5 上2下1
	if (m_nLayoutType == 4)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp = new CHorizontalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}


	// 类型6 上1下2
	if (m_nLayoutType == 5)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp = new CHorizontalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}
}


// 初始化平台数为4的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum4()
{
	// 类型1 上2下2
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型2 水平排列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CHorizontalLayout;

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型3 垂直排列
	if (m_nLayoutType == 2)
	{
		m_pMainLayout = new CVerticalLayout;

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

}


// 初始化平台数为5的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum5()
{
	// 类型1 上3下2
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型2 上2下3
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型3 左2右3
	if (m_nLayoutType == 2)
	{
		m_pMainLayout = new CHorizontalLayout;
		CLayout* pTmp1 = new CVerticalLayout;
		CLayout* pTmp2 = new CVerticalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth, m_nStHeight*3, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth, m_nStHeight*3, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型4 左3右2
	if (m_nLayoutType == 4)
	{
		m_pMainLayout = new CHorizontalLayout;
		CLayout* pTmp1 = new CVerticalLayout;
		CLayout* pTmp2 = new CVerticalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth, m_nStHeight*3, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth, m_nStHeight*3, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}
}


// 初始化平台数为6的时候， 布局显示
void CDlgPlatformLayoutMode::InitLayoutPlatformNum6()
{
	// 类型1 2行3列
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS6), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth*3, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型2 3行2列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;
		CLayout* pTmp3 = new CHorizontalLayout;


		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS1), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_WS2), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS3), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_WS4), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		pTmp3->AddWnd(GetDlgItem(IDC_ST_WS5), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);
		pTmp3->AddWnd(GetDlgItem(IDC_ST_WS6), m_nStWidth, m_nStHeight, CLayout::eSizePolicy_Preferred);

		m_pMainLayout->AddLayout(pTmp1, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp3, m_nStWidth*2, m_nStHeight, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}
}


void CDlgPlatformLayoutMode::SetStVisible()
{
	int nCtrlID[] = {IDC_ST_WS1, IDC_ST_WS2, IDC_ST_WS3, IDC_ST_WS4, IDC_ST_WS5, IDC_ST_WS6};
	for (int i = 0; i < sizeof(nCtrlID)/sizeof(int); i++)
	{
		if (i < m_nPlatformNum && m_pMainLayout)
		{
			GetDlgItem(nCtrlID[i])->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(nCtrlID[i])->ShowWindow(SW_HIDE);
		}
	}
}


bool CDlgPlatformLayoutMode::LayoutIsValid()
{
	return m_pMainLayout==NULL ? false : true;
}

void CDlgPlatformLayoutMode::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PLATFORM_LAYOUT_MODE") == strDlgID)
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