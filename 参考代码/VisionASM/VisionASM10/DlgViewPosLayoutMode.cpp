// DlgViewPosLayoutMode.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgViewPosLayoutMode.h"
#include "afxdialogex.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"

// CDlgViewPosLayoutMode 对话框

IMPLEMENT_DYNAMIC(CDlgViewPosLayoutMode, CDialogEx)

CDlgViewPosLayoutMode::CDlgViewPosLayoutMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgViewPosLayoutMode::IDD, pParent)
{
	m_nLayoutType = 0;
	m_nPosNum = 0;
	m_pMainLayout = NULL;

	m_nSTWidth = 40;
	m_nSTHeigh = 40;
}

CDlgViewPosLayoutMode::~CDlgViewPosLayoutMode()
{
	FreeResource();
}

void CDlgViewPosLayoutMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgViewPosLayoutMode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgViewPosLayoutMode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgViewPosLayoutMode::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgViewPosLayoutMode 消息处理程序


void CDlgViewPosLayoutMode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgViewPosLayoutMode::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgViewPosLayoutMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetControlHide();
	InitLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgViewPosLayoutMode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
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



void CDlgViewPosLayoutMode::SetLayoutType(int nLayoutType)
{
	m_nLayoutType = nLayoutType;
}


void CDlgViewPosLayoutMode::SetPosNum(int nPosNum)
{
	m_nPosNum = nPosNum;
}


void CDlgViewPosLayoutMode::UpdateLayout(int nLayoutType, int nPosNum)
{
	SetLayoutType(nLayoutType);
	SetPosNum(nPosNum);
	FreeResource();
	SetControlHide();
	InitLayout();

}

bool CDlgViewPosLayoutMode::LayoutIsValid()
{
	if (m_pMainLayout== NULL)
	{
		return false;
	}

	return true;
}

void CDlgViewPosLayoutMode::FreeResource()
{
	if (m_pMainLayout)
	{
		delete m_pMainLayout;
	}
	m_pMainLayout = NULL;
}


void CDlgViewPosLayoutMode::SetControlHide()
{
	GetDlgItem(IDC_ST_POS1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_POS8)->ShowWindow(SW_HIDE);
}


void CDlgViewPosLayoutMode::SetControlVisible()
{
	int nCtrlID[] = {IDC_ST_POS1, IDC_ST_POS2, IDC_ST_POS3, IDC_ST_POS4, IDC_ST_POS5, IDC_ST_POS6, IDC_ST_POS7, IDC_ST_POS8};
	for (int i = 0; i< m_nPosNum; i++)
	{
		GetDlgItem(nCtrlID[i])->ShowWindow(SW_SHOW);
	}
}


void CDlgViewPosLayoutMode::InitLayout()
{
	switch(m_nPosNum)
	{
	case 1:
		InitLayoutPos1();
		break;
	case 2:
		InitLayoutPos2();
		break;
	case 3:
		InitLayoutPos3();
		break;
	case 4:
		InitLayoutPos4();
		break;
	case 6:
		InitLayoutPos6();
		break;
	case 8:
		InitLayoutPos8();
		break;
	}

	if (m_pMainLayout)
	{
		SetControlVisible();
		m_pMainLayout->OnSize();
	}
}
// 位置数1布局
void CDlgViewPosLayoutMode::InitLayoutPos1()
{
	// 类型1
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}
	
}

// 位置数2布局
void CDlgViewPosLayoutMode::InitLayoutPos2()
{
	// 类型1	水平排列
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

	// 类型2	垂直排列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

}

// 位置数3布局
void CDlgViewPosLayoutMode::InitLayoutPos3()
{
	// 类型1	水平排列
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型2	垂直排列
	if (m_nLayoutType == 1)
	{
		m_pMainLayout = new CVerticalLayout;
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

	// 类型3	左1右2
	if (m_nLayoutType == 2)
	{
		m_pMainLayout = new CHorizontalLayout;
		CLayout* pTmp = new CVerticalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp, m_nSTWidth, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

	// 类型4	左2右1
	if (m_nLayoutType == 3)
	{
		m_pMainLayout = new CHorizontalLayout;
		CLayout* pTmp = new CVerticalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddLayout(pTmp, m_nSTWidth, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}


	// 类型5	上1下2
	if (m_nLayoutType == 4)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp = new CHorizontalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	// 类型6	上2下1
	if (m_nLayoutType == 5)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp = new CHorizontalLayout;
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddLayout(pTmp, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}

	
}

// 位置数4布局
void CDlgViewPosLayoutMode::InitLayoutPos4()
{
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS4), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddLayout(pTmp1, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTmp2, m_nSTWidth*2, 35, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->SetParent(this);
	}

}

// 位置数6布局
void CDlgViewPosLayoutMode::InitLayoutPos6()
{
     // 上1下2
	 if (m_nLayoutType == 0)
	 {
		 m_pMainLayout = new CHorizontalLayout;

		 CLayout* pObj = new CVerticalLayout;
		 CLayout* pTmp1 = new CHorizontalLayout;
		 pTmp1->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTmp1->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pObj->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pObj->AddLayout(pTmp1, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);


		 CLayout* pTar = new CVerticalLayout;
		 CLayout* pTmp2 = new CHorizontalLayout;
		 pTmp2->AddWnd(GetDlgItem(IDC_ST_POS5), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTmp2->AddWnd(GetDlgItem(IDC_ST_POS6), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTar->AddWnd(GetDlgItem(IDC_ST_POS4), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTar->AddLayout(pTmp2, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		 m_pMainLayout->AddLayout(pObj, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		 m_pMainLayout->AddLayout(pTar, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);

		 m_pMainLayout->SetParent(this);

	 }


	 // 上2下1
	 if (m_nLayoutType == 1)
	 {
		 m_pMainLayout = new CHorizontalLayout;

		 CLayout* pObj = new CVerticalLayout;
		 CLayout* pTmp1 = new CHorizontalLayout;
		 pTmp1->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTmp1->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pObj->AddLayout(pTmp1, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pObj->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);


		 CLayout* pTar = new CVerticalLayout;
		 CLayout* pTmp2 = new CHorizontalLayout;
		 pTmp2->AddWnd(GetDlgItem(IDC_ST_POS4), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTmp2->AddWnd(GetDlgItem(IDC_ST_POS5), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTar->AddLayout(pTmp2, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		 pTar->AddWnd(GetDlgItem(IDC_ST_POS6), m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);


		 m_pMainLayout->AddLayout(pObj, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		 m_pMainLayout->AddLayout(pTar, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);

		 m_pMainLayout->SetParent(this);

	 }
}

// 位置数8布局
void CDlgViewPosLayoutMode::InitLayoutPos8()
{
	if (m_nLayoutType == 0)
	{
		m_pMainLayout = new CHorizontalLayout;

		CLayout* pObj = new CVerticalLayout;
		CLayout* pTmp1 = new CHorizontalLayout;
		CLayout* pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS1), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS2), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS3), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS4), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pObj->AddLayout(pTmp1, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pObj->AddLayout(pTmp2, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);


		CLayout* pTar = new CVerticalLayout;
		pTmp1 = new CHorizontalLayout;
		pTmp2 = new CHorizontalLayout;

		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS5), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp1->AddWnd(GetDlgItem(IDC_ST_POS6), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS7), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTmp2->AddWnd(GetDlgItem(IDC_ST_POS8), m_nSTWidth, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTar->AddLayout(pTmp1, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);
		pTar->AddLayout(pTmp2, m_nSTWidth*2, m_nSTHeigh, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->AddLayout(pObj, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);
		m_pMainLayout->AddLayout(pTar, m_nSTWidth*2, m_nSTHeigh*2, CLayout::eSizePolicy_Fixed);

		m_pMainLayout->SetParent(this);
	}
}

void CDlgViewPosLayoutMode::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}
}

void CDlgViewPosLayoutMode::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_VIEW_POS_LAYOUT_MODE") == strDlgID)
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