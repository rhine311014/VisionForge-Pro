// DlgProductRename.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgProductRename.h"
//#include "DlgSysAlignerParam.h"
#include "DlgSysKindAlignView.h"
#include "DlgSysKindCalibSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProductRename dialog


CDlgProductRename::CDlgProductRename(LPCTSTR lpszCurName, CWnd* pParent)
	: CDialog(CDlgProductRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProductRename)
	m_strNewName = _T("");
	m_strOldName = _T("");
	//}}AFX_DATA_INIT
	m_strOldName = lpszCurName;
	m_strNewName = m_strOldName;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_pParent = (void *)pParent;
	m_nParentType = 0;
}


void CDlgProductRename::UpdateDialogLanguage()
{
//	return;
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
		
		if(_T("IDD_DLG_PRODUCT_RENAME") == strDlgID)
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

void CDlgProductRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProductRename)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_strNewName);
	DDX_Text(pDX, IDC_EDIT_OLD_NAME, m_strOldName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProductRename, CDialog)
	//{{AFX_MSG_MAP(CDlgProductRename)
	ON_EN_SETFOCUS(IDC_EDIT_NEW_NAME, OnSetfocusEditNewName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProductRename message handlers

void CDlgProductRename::OnOK() 
{
	UpdateData(TRUE);
	if (m_strNewName.GetLength() > 32)
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NAME_TOO_LONG));	//_T("产品名称太长！")
		return;
	}
	if (m_strNewName.IsEmpty())
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NAME_CANNOT_BE_EMPTY));	//_T("产品名称不能为空！")
		return;
	}
	if (m_nParentType == 0)
	{
		if (!((CDlgSysKindAlignView*)m_pParent)->IsNameUnique(m_strNewName) && (m_strNewName != m_strOldName))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_WITH_SAME_NAME_ALREADY_EXISTED));	//_T("相同名称产品已经存在！")
			return;
		}
	}
// 	else if (m_nParentType == 1)
// 	{
// 		if (!((CDlgSysKindAlignView*)m_pParent)->IsNameUnique(m_strNewName) && (m_strNewName != m_strOldName))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_WITH_SAME_NAME_ALREADY_EXISTED));	//_T("相同名称产品已经存在！")
// 			return;
// 		}
// 	}
// 	else if (m_nParentType == 2)
// 	{
// 		if (!((CDlgSysKindCalibSetting*)m_pParent)->IsNameUnique(m_strNewName) && (m_strNewName != m_strOldName))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_WITH_SAME_NAME_ALREADY_EXISTED));	//_T("相同名称产品已经存在！")
// 			return;
// 		}
// 	}
// 	else
// 	{
// 		return;
// 	}
// 	if (!m_pParent->IsNameUnique(m_strNewName) && (m_strNewName != m_strOldName))
// 	{
// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_WITH_SAME_NAME_ALREADY_EXISTED));	//_T("相同名称产品已经存在！")
// 		return;
// 	}
	CDialog::OnOK();
}

BOOL CDlgProductRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 设置按钮
	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_16);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_16);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	
	UpdateDialogLanguage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProductRename::OnSetfocusEditNewName() 
{
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_NEW_NAME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_NEW_NAME, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput, FALSE))
		{
			SetDlgItemText(IDC_EDIT_NEW_NAME, strOutput);
			CString strLog;
			if (strInput != strOutput)
			{
				strLog.Format(_T("修改产品[新名称]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
	}	
}
