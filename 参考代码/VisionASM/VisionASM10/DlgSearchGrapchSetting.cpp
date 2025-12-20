// DlgSearchGrapchSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSearchGrapchSetting.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"

// CDlgSearchGrapchSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSearchGrapchSetting, CDialog)

CDlgSearchGrapchSetting::CDlgSearchGrapchSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchGrapchSetting::IDD, pParent)
{
    m_nPrevIndex = 0;
}

CDlgSearchGrapchSetting::~CDlgSearchGrapchSetting()
{
    //delete m_pDlgObjCross;
    //delete m_pDlgTarCross;
}

void CDlgSearchGrapchSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CDlgSearchGrapchSetting, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgSearchGrapchSetting::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &CDlgSearchGrapchSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgSearchGrapchSetting::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDlgSearchGrapchSetting 消息处理程序


BOOL CDlgSearchGrapchSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateDialogLanguage();

	m_tab.InsertItem(0,GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_TARGET_CROSS));
	m_tab.InsertItem(1,GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_OBJECT_CROSS));
	
	InitDlgItem();
	
	m_tab.SetCurSel(0);
	



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSearchGrapchSetting::InitDlgItem()
{
	CRect rcTabSheet;
	m_tab.GetClientRect(rcTabSheet);
	rcTabSheet.top	  += 22;   
	rcTabSheet.bottom -= 4;
	rcTabSheet.left	  += 2;
	rcTabSheet.right  -= 4;

	//目标十字
	m_pDlgTarCross = new CDlgCrossSetting;
	m_pDlgTarCross->SetCrossSize(m_TarCross);
	m_pDlgTarCross->m_psaSysInfoStrings = m_psaSysInfoStrings;
	m_pDlgTarCross->m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;
	m_pDlgTarCross->Create(IDD_DLG_CROSS_SETTING, GetDlgItem(IDC_TAB1));
	m_pDlgTarCross->m_pParent = this;
	m_pDlgTarCross->MoveWindow(&rcTabSheet);
	m_pDlgTarCross->ShowWindow(SW_SHOW);
    CString strLog;
    strLog.Format(_T("点击[目标十字]进入[目标十字页面]"));
    g_opLogFile.SaveLogText(strLog,3);
	
	//对象十字
	m_pDlgObjCross = new CDlgCrossSetting;
	m_pDlgObjCross->SetCrossSize(m_ObjCross);
	m_pDlgObjCross->m_psaSysInfoStrings = m_psaSysInfoStrings;
	m_pDlgObjCross->m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;
	m_pDlgObjCross->Create(IDD_DLG_CROSS_SETTING, GetDlgItem(IDC_TAB1));
	m_pDlgObjCross->m_pParent = this;
	m_pDlgObjCross->MoveWindow(&rcTabSheet);
	m_pDlgObjCross->ShowWindow(SW_HIDE);
	
}


void CDlgSearchGrapchSetting::UpdataDlgdata(BOOL flag)
{
	
}

void CDlgSearchGrapchSetting::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_SEARCH_GRAPH_SETTING") == strDlgID)
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

void CDlgSearchGrapchSetting::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int CurIndex = m_tab.GetCurSel();
	if(CurIndex == 0)//目标十字
	{
		m_pDlgTarCross->ShowWindow(SW_SHOW);
		m_pDlgObjCross->ShowWindow(SW_HIDE);
	}
	else if(CurIndex == 1)
	{
		m_pDlgTarCross->ShowWindow(SW_HIDE);
		m_pDlgObjCross->ShowWindow(SW_SHOW);
	}
    if (m_nPrevIndex!= CurIndex)
    {
        CString strLog;
        CString strVal[] = {_T("目标十字"),_T("对象十字")};
        strLog.Format(_T("退出[%s页面]\n"),strVal[m_nPrevIndex]);
        g_opLogFile.SaveLogText(strLog,3);
        strLog.Format(_T("点击[%s]进入[%s页面]\n"),strVal[CurIndex],strVal[CurIndex]);
        g_opLogFile.SaveLogText(strLog,3);
    }
    m_nPrevIndex = CurIndex;
}


void CDlgSearchGrapchSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pDlgTarCross->UpdateDlgdata(TRUE);
	m_pDlgObjCross->UpdateDlgdata(TRUE);
	m_TarCross = m_pDlgTarCross->m_Cross;
	m_ObjCross = m_pDlgObjCross->m_Cross;
    CString strLog;
    CString strVal[] = {_T("目标十字"),_T("对象十字")};
    strLog.Format(_T("退出[%s页面]\n"),strVal[m_nPrevIndex]);
    g_opLogFile.SaveLogText(strLog,3);
    strLog.Format(_T("[保存]退出"));
    g_opLogFile.SaveLogText(strLog,2);
	CDialog::OnOK();
}


void CDlgSearchGrapchSetting::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	if(lgidLanguage == MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT))//英文
	{
		str = "Are you sure to restore default ?";
	}
	else
		str = "确定要恢复默认设置吗？";

	if(AfxMessageBox(str,MB_YESNO ) == IDYES)
	{
		SearchResultCrossSize tmp_Cross;
        
        tmp_Cross.nLineColor = RGB(255,0,0);
        tmp_Cross.nLineLength = 3000;
        tmp_Cross.nRotation = 0;
        m_pDlgTarCross->SetCrossSize(tmp_Cross);

        tmp_Cross.nLineColor = RGB(0,255,0);
        tmp_Cross.nLineLength = 100;
        tmp_Cross.nRotation = 45;
		m_pDlgObjCross->SetCrossSize(tmp_Cross);
		
		m_pDlgObjCross->UpdateDlgdata(false);
		m_pDlgTarCross->UpdateDlgdata(false);

        CString strLog;
        strLog.Format(_T("点击[恢复默认]"));
        g_opLogFile.SaveLogText(strLog,3);
	}
}


BOOL CDlgSearchGrapchSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CDlgSearchGrapchSetting::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_pDlgTarCross != NULL)
	{
		delete m_pDlgTarCross;
		m_pDlgTarCross = NULL;
	}


	if (m_pDlgObjCross != NULL)
	{
		delete m_pDlgObjCross;
		m_pDlgObjCross = NULL;
	}

	return CDialog::DestroyWindow();
}


void CDlgSearchGrapchSetting::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    CString strVal[] = {_T("目标十字"),_T("对象十字")};
    strLog.Format(_T("退出[%s页面]\n"),strVal[m_nPrevIndex]);
    g_opLogFile.SaveLogText(strLog,3);
    strLog.Format(_T("[不保存]退出"));
    g_opLogFile.SaveLogText(strLog,2);
    CDialog::OnCancel();
}
