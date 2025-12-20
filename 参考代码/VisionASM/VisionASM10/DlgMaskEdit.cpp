// DlgMaskEdit.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgMaskEdit.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskEdit dialog


CDlgMaskEdit::CDlgMaskEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaskEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaskEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
}


void CDlgMaskEdit::UpdateDialogLanguage()
{
//	return;
	if (m_psaSysInfoStrings != NULL)
	{
		m_lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	
	TCHAR aModuleFileName[MAX_PATH];
	GetModuleFileName(NULL, aModuleFileName, MAX_PATH);
	CString strModuleFileName(aModuleFileName);
	int nIndex = strModuleFileName.ReverseFind('\\');
	CString strCurWorkDir = strModuleFileName.Left(nIndex);
	
	switch (m_lgidLanguage)
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
		
		if(_T("IDD_DLG_MASK_EDIT") == strDlgID)
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

void CDlgMaskEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaskEdit)
	DDX_Control(pDX, IDOK, m_btnApply);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaskEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgMaskEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskEdit message handlers

BOOL CDlgMaskEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STC_MASK_EDIT);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_GuiMaskEdit.Create(&rect, this);

	CString strCaption;

	if (m_psaSysInfoStrings != NULL)
	{
		m_lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	UpdateDialogLanguage();

	m_btnApply.SetThemeHelper(&m_themeHelper);
// 	m_btnApply.SetIcon(IDI_ICON_OK_16);
// 	m_btnApply.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	strCaption = (m_psaSysInfoStrings == NULL) ? _T("应用") : GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_APPLY);
	m_btnApply.SetWindowText(strCaption);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_16);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	strCaption = (m_psaSysInfoStrings == NULL) ? _T("取消") : GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CANCEL);
	m_btnCancel.SetWindowText(strCaption);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMaskEdit::SetSysInfoStringsRef( CStringArray* psaSysInfoStrings )
{
	m_psaSysInfoStrings = psaSysInfoStrings;
}


void CDlgMaskEdit::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[取消]退出\n"));
    g_opLogFile.SaveLogText(strLog,6);
    CDialog::OnCancel();
}


void CDlgMaskEdit::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[保存]退出\n"));
    g_opLogFile.SaveLogText(strLog,6);
    CDialog::OnOK();
}
