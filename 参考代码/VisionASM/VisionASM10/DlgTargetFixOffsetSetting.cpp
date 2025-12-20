// DlgTargetFixOffsetSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTargetFixOffsetSetting.h"
#include "afxdialogex.h"
#include "LogFileSave.h"
#include "svXMLConfigurator.h"

// CDlgTargetFixOffsetSetting 对话框

IMPLEMENT_DYNAMIC(CDlgTargetFixOffsetSetting, CDialogEx)

CDlgTargetFixOffsetSetting::CDlgTargetFixOffsetSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTargetFixOffsetSetting::IDD, pParent)
	, m_bEnableTargetFixOffset(FALSE)
{
	m_bSysKeyboardEnabled = TRUE;
	m_psaSysInfoStrings = NULL;
	m_nPrevIndex=0;
	m_saTarFixOffsetX.SetSize(ALIGNOFFSETNUM);
	m_saTarFixOffsetY.SetSize(ALIGNOFFSETNUM);
	m_saTarFixOffsetD.SetSize(ALIGNOFFSETNUM);
	// 	m_dTargetFixOffsetX	= 0;
	// 	m_dTargetFixOffsetY	= 0;
	// 	m_dTargetFixOffsetD	= 0;
	m_strTargetFixOffsetX	= _T("");
	m_strTargetFixOffsetY	= _T("");
	m_strTargetFixOffsetD	= _T("");
	m_nTargetOffsetType = 0;
}

CDlgTargetFixOffsetSetting::~CDlgTargetFixOffsetSetting()
{
}

void CDlgTargetFixOffsetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_TARGET_FIX_OFFSET, m_bEnableTargetFixOffset);
	DDX_Text(pDX, IDC_EDIT_TARGET_FIX_OFFSET_X, m_strTargetFixOffsetX);
	DDX_Text(pDX, IDC_EDIT_TARGET_FIX_OFFSET_Y, m_strTargetFixOffsetY);
	DDX_Text(pDX, IDC_EDIT_TARGET_FIX_OFFSET_D, m_strTargetFixOffsetD);
	DDX_Control(pDX, IDC_CMB_TAR_FIX_OFFSET_INDEX, m_comboAlnTarFixOffsetIndex);
	DDX_Radio(pDX, IDC_RADIO_TARGET_FIX_OFFSET_PRODUCT, m_nTargetOffsetType);
}


BEGIN_MESSAGE_MAP(CDlgTargetFixOffsetSetting, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_X, &CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetX)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_X, &CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetX)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_Y, &CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetY)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_Y, &CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetY)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_D, &CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetD)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_FIX_OFFSET_D, &CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetD)
	ON_CBN_SELCHANGE(IDC_CMB_TAR_FIX_OFFSET_INDEX, &CDlgTargetFixOffsetSetting::OnCbnSelchangeCmbTarFixOffsetIndex)
	ON_BN_CLICKED(IDOK, &CDlgTargetFixOffsetSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTargetFixOffsetSetting 消息处理程序


void CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_FIX_OFFSET_X)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标固定补偿][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}

		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditTargetFixOffsetX();
	}
}


void CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetX()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_X, m_strTargetFixOffsetX);
		}
		else
		{
			m_strTargetFixOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_X, m_strTargetFixOffsetX);
		}
	}
}


void CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_FIX_OFFSET_Y)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标固定补偿][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}

		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditTargetFixOffsetY();
	}
}


void CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetY()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_Y, m_strTargetFixOffsetY);
		}
		else
		{
			m_strTargetFixOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_Y, m_strTargetFixOffsetY);
		}
	}
}


void CDlgTargetFixOffsetSetting::OnEnSetfocusEditTargetFixOffsetD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_FIX_OFFSET_D)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_D, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标固定补偿][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}

		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditTargetFixOffsetD();
	}
}


void CDlgTargetFixOffsetSetting::OnEnKillfocusEditTargetFixOffsetD()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_D, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_D, m_strTargetFixOffsetX);
		}
		else
		{
			m_strTargetFixOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_FIX_OFFSET_D, m_strTargetFixOffsetX);
		}
	}
}


BOOL CDlgTargetFixOffsetSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDialogLanguage();
	CString strCombItem;
	for(int i = 0; i < ALIGNOFFSETNUM; i++)
	{
		strCombItem.Format(_T("%d"),i+1);
		m_comboAlnTarFixOffsetIndex.InsertString(i, strCombItem);
	}
	m_comboAlnTarFixOffsetIndex.SetCurSel(0);

// 	m_dTargetFixOffsetX = atof(m_saTarFixOffsetX.GetAt(0));
// 	m_dTargetFixOffsetY = atof(m_saTarFixOffsetY.GetAt(0));
// 	m_dTargetFixOffsetD = atof(m_saTarFixOffsetD.GetAt(0));
	m_strTargetFixOffsetX	= m_saTarFixOffsetX.GetAt(0);
	m_strTargetFixOffsetY	= m_saTarFixOffsetY.GetAt(0);
	m_strTargetFixOffsetD	= m_saTarFixOffsetD.GetAt(0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgTargetFixOffsetSetting::IsStringNumerical(LPCTSTR lpszVal)
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

void CDlgTargetFixOffsetSetting::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_TARGET_FIX_OFFSET") == strDlgID)
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




void CDlgTargetFixOffsetSetting::OnCbnSelchangeCmbTarFixOffsetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nCurSel = m_comboAlnTarFixOffsetIndex.GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}
	CString str;
	//str.Format(_T("%.6f"),m_dTargetFixOffsetX);
	//m_saTarFixOffsetX.SetAt(m_nPrevIndex, str);
	//str.Format(_T("%.6f"),m_dTargetFixOffsetY);
	//m_saTarFixOffsetY.SetAt(m_nPrevIndex, str);
	//str.Format(_T("%.6f"),m_dTargetFixOffsetD);
	//m_saTarFixOffsetD.SetAt(m_nPrevIndex, str);
	m_saTarFixOffsetX.SetAt(m_nPrevIndex, m_strTargetFixOffsetX);
	m_saTarFixOffsetY.SetAt(m_nPrevIndex, m_strTargetFixOffsetY);
	m_saTarFixOffsetD.SetAt(m_nPrevIndex, m_strTargetFixOffsetD);
	if (nCurSel != m_nPrevIndex)
	{
// 		m_dTargetFixOffsetX = atof(m_saTarFixOffsetX.GetAt(nCurSel));
// 		m_dTargetFixOffsetY = atof(m_saTarFixOffsetY.GetAt(nCurSel));
// 		m_dTargetFixOffsetD = atof(m_saTarFixOffsetD.GetAt(nCurSel));
		m_strTargetFixOffsetX	= m_saTarFixOffsetX.GetAt(nCurSel);
		m_strTargetFixOffsetY	= m_saTarFixOffsetY.GetAt(nCurSel);
		m_strTargetFixOffsetD	= m_saTarFixOffsetD.GetAt(nCurSel);
		m_nPrevIndex= nCurSel;
		UpdateData(FALSE);
	}
}


void CDlgTargetFixOffsetSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//CString str;
	//str.Format(_T("%.6f"),m_dTargetFixOffsetX);
	//m_saTarFixOffsetX.SetAt(m_nPrevIndex, str);
	//str.Format(_T("%.6f"),m_dTargetFixOffsetY);
	//m_saTarFixOffsetY.SetAt(m_nPrevIndex, str);
	//str.Format(_T("%.6f"),m_dTargetFixOffsetD);
	//m_saTarFixOffsetD.SetAt(m_nPrevIndex, str);
	m_saTarFixOffsetX.SetAt(m_nPrevIndex, m_strTargetFixOffsetX);
	m_saTarFixOffsetY.SetAt(m_nPrevIndex, m_strTargetFixOffsetY);
	m_saTarFixOffsetD.SetAt(m_nPrevIndex, m_strTargetFixOffsetD);
	CDialogEx::OnOK();
}
