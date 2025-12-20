// DLGEightShapeAlignSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DLGEightShapeAlignSetting.h"
#include "afxdialogex.h"
#include "LogFileSave.h"

// CDLGEightShapeAlignSetting 对话框

IMPLEMENT_DYNAMIC(CDLGEightShapeAlignSetting, CDialogEx)

CDLGEightShapeAlignSetting::CDLGEightShapeAlignSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGEightShapeAlignSetting::IDD, pParent)
	, m_bCheckEightShapeAlign(FALSE)
	, m_nEightShapeAlignMode(0)
	, m_dEightShapeAngle(0)
{
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;
}

CDLGEightShapeAlignSetting::~CDLGEightShapeAlignSetting()
{
}

void CDLGEightShapeAlignSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_EIGHT_SHAPE_ALIGN, m_bCheckEightShapeAlign);
	DDX_Radio(pDX, IDC_RADIO_POSITIVE_EIGHT, m_nEightShapeAlignMode);
	DDX_Text(pDX, IDC_EDIT_EIGHIT_ANGLE, m_dEightShapeAngle);
}


BEGIN_MESSAGE_MAP(CDLGEightShapeAlignSetting, CDialogEx)

	ON_BN_CLICKED(IDOK, &CDLGEightShapeAlignSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGEightShapeAlignSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_EIGHT_SHAPE_ALIGN, &CDLGEightShapeAlignSetting::OnBnClickedCheckEightShapeAlign)
	ON_BN_CLICKED(IDC_RADIO_POSITIVE_EIGHT, &CDLGEightShapeAlignSetting::OnBnClickedRadioPositiveEight)
	ON_BN_CLICKED(IDC_RADIO_NEGTIVE_EIGHT, &CDLGEightShapeAlignSetting::OnBnClickedRadioPositiveEight)

	ON_EN_SETFOCUS(IDC_EDIT_EIGHIT_ANGLE, &CDLGEightShapeAlignSetting::OnEnSetfocusEditEighitAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_EIGHIT_ANGLE, &CDLGEightShapeAlignSetting::OnEnKillfocusEditEighitAngle)


END_MESSAGE_MAP()


// CDLGEightShapeAlignSetting 消息处理程序


BOOL CDLGEightShapeAlignSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDLGEightShapeAlignSetting::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_EIGHT_SHAPE_ALIGN") == strDlgID)
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

void CDLGEightShapeAlignSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存]退出"));
	g_opLogFile.SaveLogText(strLog,4);
	CDialogEx::OnOK();
}


void CDLGEightShapeAlignSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[取消]退出"));
	g_opLogFile.SaveLogText(strLog,4);
	CDialog::OnCancel();
	CDialogEx::OnCancel();
}


void CDLGEightShapeAlignSetting::OnBnClickedCheckEightShapeAlign()
{
	// TODO: 在此添加控件通知处理程序代码
		BOOL bCheckEightShapeAlignOld = m_bCheckEightShapeAlign;
	
		UpdateData(TRUE);
		if (bCheckEightShapeAlignOld != m_bCheckEightShapeAlign)
		{
			CString strLog;
			CString strValue = m_bCheckEightShapeAlign ? _T("选中状态"):_T("未选中状态");
			strLog.Format(_T("修改[启用八字型对位]为%s\n"),strValue);
			g_opLogFile.SaveLogText(strLog,4);
		}
}


void CDLGEightShapeAlignSetting::OnBnClickedRadioPositiveEight()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	int nEightShapeAlignModeOld = m_nEightShapeAlignMode;
	UpdateData(TRUE);

	CString strVal[] = {_T("顺八字"), _T("逆八字")};
	if (nEightShapeAlignModeOld != m_nEightShapeAlignMode)
	{
		CString strLog;
		strLog.Format(_T("修改[启用八字对位]由%s到%s\n"),strVal[nEightShapeAlignModeOld],strVal[m_nEightShapeAlignMode]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDLGEightShapeAlignSetting::OnEnSetfocusEditEighitAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[八字型对位][八字角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditEighitAngle();
	}
}


void CDLGEightShapeAlignSetting::OnEnKillfocusEditEighitAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		CString strOldVal;
		GetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strVal);
		if (!IsStringNumerical(strVal))
		{

			strOldVal.Format("%f",m_dEightShapeAngle);
			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOldVal);
		}
		else
		{
			strOldVal.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOldVal);
		}
	}
}

BOOL CDLGEightShapeAlignSetting::IsStringNumerical( LPCTSTR lpszVal )
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
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}