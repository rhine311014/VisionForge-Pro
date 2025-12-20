// DlgCalibAxisDirection.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibAxisDirection.h"
#include "afxdialogex.h"


// DlgCalibAxisDirection 对话框

IMPLEMENT_DYNAMIC(CDlgCalibAxisDirection, CDialogEx)

CDlgCalibAxisDirection::CDlgCalibAxisDirection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibAxisDirection::IDD, pParent)
	, m_bReverseX(FALSE)
	, m_bReverseY(FALSE)
	, m_bReverseD(FALSE)
{
	m_psaSysInfoStrings = NULL;
}

CDlgCalibAxisDirection::~CDlgCalibAxisDirection()
{
}

void CDlgCalibAxisDirection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_REVERSE_X, m_bReverseX);
	DDX_Check(pDX, IDC_CHK_REVERSE_Y, m_bReverseY);
	DDX_Check(pDX, IDC_CHK_REVERSE_D, m_bReverseD);
}


BEGIN_MESSAGE_MAP(CDlgCalibAxisDirection, CDialogEx)
END_MESSAGE_MAP()


// DlgCalibAxisDirection 消息处理程序


BOOL CDlgCalibAxisDirection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDialogLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCalibAxisDirection::UpdateDialogLanguage()
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
		{
			strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		{
			strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		}	
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		{
			strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		}
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_DLG_CALIB_AXIS_DIRECTION") == strDlgID)
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

BOOL CDlgCalibAxisDirection::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
