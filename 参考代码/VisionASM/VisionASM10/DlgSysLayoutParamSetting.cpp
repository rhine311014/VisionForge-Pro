// DlgLayoutParamSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysLayoutParamSetting.h"
#include "afxdialogex.h"
#include "DlgSysEnvironSetting.h"


// CDlgSysLayoutParamSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSysLayoutParamSetting, CDialogEx)

CDlgSysLayoutParamSetting::CDlgSysLayoutParamSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysLayoutParamSetting::IDD, pParent)
	, m_nRdViewLayoutType(0)
	, m_strVisionRunDlgWidth(_T(""))
	, m_nRdViewRunShowType(0)
{
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled	= TRUE;
	m_pDlgViewPosLayoutMode = NULL;
	m_psaSysInfoStrings		= NULL;
	m_pParent = NULL;
}

CDlgSysLayoutParamSetting::~CDlgSysLayoutParamSetting()
{
	for (int i = 0; i < m_vpViewPosLayout.size(); i++)
	{
		delete m_vpViewPosLayout[i];
		m_pDlgViewPosLayoutMode[i].DestroyWindow();
	}
	delete[] m_pDlgViewPosLayoutMode;
	m_pDlgViewPosLayoutMode = NULL;
	m_vpViewPosLayout.clear();
}

void CDlgSysLayoutParamSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RD_VIEW_LAYOUT_TYPE1, m_nRdViewLayoutType);
	DDX_Text(pDX, IDC_EDIT_VISION_RUN_DLG_WIDTH, m_strVisionRunDlgWidth);
	DDX_Radio(pDX, IDC_RD_VIEW_RUN_SHOW_TYPE1, m_nRdViewRunShowType);
}


BEGIN_MESSAGE_MAP(CDlgSysLayoutParamSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSysLayoutParamSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSysLayoutParamSetting::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT_VISION_RUN_DLG_WIDTH, &CDlgSysLayoutParamSetting::OnEnSetfocusEditVisionRunDlgWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_VISION_RUN_DLG_WIDTH, &CDlgSysLayoutParamSetting::OnEnKillfocusEditVisionRunDlgWidth)
END_MESSAGE_MAP()


// CDlgSysLayoutParamSetting 消息处理程序


void CDlgSysLayoutParamSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgSysLayoutParamSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgSysLayoutParamSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDialogLanguage();
	InitViewPosLayoutMode();
	UpdateDlgData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgSysLayoutParamSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSysLayoutParamSetting::SetPlatformLayoutInfo(CPlatformLayoutInfo layoutInfo)
{
	m_PlatformLayoutInfo = layoutInfo;
}

void CDlgSysLayoutParamSetting::SetVisionASMRef(vcBaseVisionAlign* pVisionASM)
{
	m_pVisionASM = pVisionASM;
}

CPlatformLayoutInfo CDlgSysLayoutParamSetting::GetPlatformLayoutInfo()
{
    return m_PlatformLayoutInfo;
}

void CDlgSysLayoutParamSetting::UpdateDlgData(BOOL bValidUpdate/*=TRUE*/)
{
	 if (bValidUpdate)
	 {
		 UpdateData(TRUE);
		 m_PlatformLayoutInfo.m_nViewLayoutType = m_nRdViewLayoutType;
		 m_PlatformLayoutInfo.m_nViewVisionRunWidth = atoi(m_strVisionRunDlgWidth);
		 m_PlatformLayoutInfo.m_nViewMainLayoutType	= m_nRdViewRunShowType;
	 }
	 else
	 {
		 SysPlatformInfo platformInfo;
		 m_pVisionASM->GetSysPlatformInfo(platformInfo);

		 m_nRdViewLayoutType = m_PlatformLayoutInfo.m_nViewLayoutType;
		 m_strVisionRunDlgWidth.Format(_T("%d"), m_PlatformLayoutInfo.m_nViewVisionRunWidth);
		 m_nRdViewRunShowType =  m_PlatformLayoutInfo.m_nViewMainLayoutType;

		 // 更新布局模式显示
		 int nRdId[] = {IDC_RD_VIEW_LAYOUT_TYPE1, IDC_RD_VIEW_LAYOUT_TYPE2, IDC_RD_VIEW_LAYOUT_TYPE3, IDC_RD_VIEW_LAYOUT_TYPE4, IDC_RD_VIEW_LAYOUT_TYPE5, IDC_RD_VIEW_LAYOUT_TYPE6};
		 for (int i = 0; i < 6; i++)
		 {
			 int nPosNum = 0;
			 if (platformInfo.m_nShowMode == 0)
			 {
				 nPosNum = platformInfo.m_nPositionNum;
			 }

			 if(platformInfo.m_nShowMode == 1)
			 {
				 nPosNum	= platformInfo.m_nCamNum;
			 }

// 			 if (platformInfo.m_nPositionShowMode == 1)
// 			 {
// 				 nPosNum = 1;
// 			 }
			 m_pDlgViewPosLayoutMode[i].UpdateLayout(i, nPosNum);
			 GetDlgItem(nRdId[i])->EnableWindow(m_pDlgViewPosLayoutMode[i].LayoutIsValid());
		 }

		 UpdateData(FALSE);
	 }
}


BOOL CDlgSysLayoutParamSetting::IsStringPositiveNumerical(LPCTSTR lpszVal)
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if (((ch > '9') || (ch < '0')) && (ch != '.'))
		{
			if ((i == 0) && ((ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}


void CDlgSysLayoutParamSetting::InitViewPosLayoutMode()
{
	SysPlatformInfo platformInfo;
	int nCtrlID[] = {IDC_ST_LAYOUT_TYPE1, IDC_ST_LAYOUT_TYPE2, IDC_ST_LAYOUT_TYPE3, IDC_ST_LAYOUT_TYPE4, IDC_ST_LAYOUT_TYPE5, IDC_ST_LAYOUT_TYPE6};
	m_pVisionASM->GetSysPlatformInfo(platformInfo);
	m_vpViewPosLayout.resize(6);
	m_pDlgViewPosLayoutMode = new CDlgViewPosLayoutMode[6];
	for (int i = 0; i < 6; i++)
	{
		int nPosNum = 0;
		if (platformInfo.m_nShowMode == 0)
		{
			nPosNum	= platformInfo.m_nCamNum;
		}

		if(platformInfo.m_nShowMode == 1)
		{
			nPosNum = platformInfo.m_nPositionNum;
		}

// 		if (platformInfo.m_nPositionShowMode == 1)
// 		{
// 			nPosNum = 1;
// 		}

		m_pDlgViewPosLayoutMode[i].SetPosNum(nPosNum);
		m_pDlgViewPosLayoutMode[i].SetLayoutType(i);
		m_pDlgViewPosLayoutMode[i].Create(IDD_DLG_VIEW_POS_LAYOUT_MODE, GetDlgItem(nCtrlID[i]));
		m_pDlgViewPosLayoutMode[i].SetSysInfoStringsRef(m_psaSysInfoStrings);
		m_pDlgViewPosLayoutMode[i].UpdateDialogLanguage();
		m_pDlgViewPosLayoutMode[i].ShowWindow(SW_SHOW);
		m_vpViewPosLayout[i] = new CHorizontalLayout;
		m_vpViewPosLayout[i]->AddWnd(&m_pDlgViewPosLayoutMode[i]);
		m_vpViewPosLayout[i]->SetParent(GetDlgItem(nCtrlID[i]));
		m_vpViewPosLayout[i]->OnSize();
	}
}

void CDlgSysLayoutParamSetting::OnEnSetfocusEditVisionRunDlgWidth()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VISION_RUN_DLG_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VISION_RUN_DLG_WIDTH, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VISION_RUN_DLG_WIDTH, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditVisionRunDlgWidth();
	}	
}


void CDlgSysLayoutParamSetting::OnEnKillfocusEditVisionRunDlgWidth()
{

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VISION_RUN_DLG_WIDTH, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VISION_RUN_DLG_WIDTH, m_strVisionRunDlgWidth);
		}
		else
		{
			m_strVisionRunDlgWidth.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_VISION_RUN_DLG_WIDTH, m_strVisionRunDlgWidth);
		}
	}
}

void CDlgSysLayoutParamSetting::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_SYS_LAYOUT_PARAM_SETTING") == strDlgID)
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