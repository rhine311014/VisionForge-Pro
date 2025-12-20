// DlgCommParamSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCommParamSetting.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"
#include "DlgSysEnvironSetting.h"
// CDlgCommParamSetting 对话框

IMPLEMENT_DYNAMIC(CDlgCommParamSetting, CDialogEx)

CDlgCommParamSetting::CDlgCommParamSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommParamSetting::IDD, pParent)
	, m_nCommXUnitExp(0)
	, m_nCommYUnitExp(0)
	, m_nCommDUnitExp(0)
	, m_nCommDelay(0)
	, m_nCommTimeout(0)
	, m_nCommReSendTimes(0)
	, m_bUsePlatformComm(FALSE)
{
	m_bSysKeyboardEnabled = FALSE;
	m_nLanguage = 0;
	m_psaSysInfoStrings = NULL;
	m_bKillFocusEnabled = TRUE;
	m_pParentWnd = NULL;
	m_pVisionASM = NULL;

	m_bShowCommParam = true;
	m_bEnableCommParam = true;
}

CDlgCommParamSetting::~CDlgCommParamSetting()
{
}

void CDlgCommParamSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMM_X_UNIT_EXP, m_nCommXUnitExp);
	DDX_Text(pDX, IDC_EDIT_COMM_Y_UNIT_EXP, m_nCommYUnitExp);
	DDX_Text(pDX, IDC_EDIT_COMM_D_UNIT_EXP, m_nCommDUnitExp);
	DDX_Text(pDX, IDC_EDIT_COMM_DELAY, m_nCommDelay);
	DDX_Text(pDX, IDC_EDIT_COMM_TIMEOUT, m_nCommTimeout);
	DDX_Text(pDX, IDC_EDIT_COMM_RE_SEND_TIMES, m_nCommReSendTimes);
	DDX_Control(pDX, IDC_COMBO_COMM_DATA_LEN_TYPE, m_comboCommDataLenType);
	DDX_Check(pDX, IDC_CHK_USE_PLATFORM_COMM_SETTING, m_bUsePlatformComm);
}


BEGIN_MESSAGE_MAP(CDlgCommParamSetting, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_X_UNIT_EXP, &CDlgCommParamSetting::OnEnSetfocusEditCommXUnitExp)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_X_UNIT_EXP, &CDlgCommParamSetting::OnEnKillfocusEditCommXUnitExp)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_Y_UNIT_EXP, &CDlgCommParamSetting::OnEnSetfocusEditCommYUnitExp)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_Y_UNIT_EXP, &CDlgCommParamSetting::OnEnKillfocusEditCommYUnitExp)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_D_UNIT_EXP, &CDlgCommParamSetting::OnEnSetfocusEditCommDUnitExp)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_D_UNIT_EXP, &CDlgCommParamSetting::OnEnKillfocusEditCommDUnitExp)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_TIMEOUT, &CDlgCommParamSetting::OnEnSetfocusEditCommTimeout)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_TIMEOUT, &CDlgCommParamSetting::OnEnKillfocusEditCommTimeout)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_RE_SEND_TIMES, &CDlgCommParamSetting::OnEnSetfocusEditCommReSendTimes)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_RE_SEND_TIMES, &CDlgCommParamSetting::OnEnKillfocusEditCommReSendTimes)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_DELAY, &CDlgCommParamSetting::OnEnSetfocusEditCommDelay)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_DELAY, &CDlgCommParamSetting::OnEnKillfocusEditCommDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_DATA_LEN_TYPE, &CDlgCommParamSetting::OnCbnSelchangeComboCommDataLenType)
	ON_BN_CLICKED(IDC_CHK_USE_PLATFORM_COMM_SETTING, &CDlgCommParamSetting::OnBnClickedChkUsePlatformCommSetting)
END_MESSAGE_MAP()


// CDlgCommParamSetting 消息处理程序

BOOL CDlgCommParamSetting::IsStringNumerical( LPCTSTR lpszVal )
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

void CDlgCommParamSetting::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_COMM_PARAM_SETTING") == strDlgID)
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

void CDlgCommParamSetting::OnEnSetfocusEditCommXUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_X_UNIT_EXP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_X_UNIT_EXP, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_X_UNIT_EXP, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X单位幂]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommXUnitExp();
	}	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommXUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommXUnitExp = (m_nCommXUnitExp<1) ? 1 : (m_nCommXUnitExp);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


void CDlgCommParamSetting::OnEnSetfocusEditCommYUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_Y_UNIT_EXP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_Y_UNIT_EXP, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_Y_UNIT_EXP, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y单位幂]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommYUnitExp();
	}	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommYUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommYUnitExp = (m_nCommYUnitExp<1) ? 1 : (m_nCommYUnitExp);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


void CDlgCommParamSetting::OnEnSetfocusEditCommDUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_D_UNIT_EXP, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_D_UNIT_EXP, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[θ单位幂]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommDUnitExp();
	}
	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommDUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommDUnitExp = (m_nCommDUnitExp<1) ? 1 : (m_nCommDUnitExp);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


void CDlgCommParamSetting::OnEnSetfocusEditCommTimeout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_TIMEOUT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_TIMEOUT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[超时(毫秒)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommTimeout();
	}	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommTimeout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommTimeout = (m_nCommTimeout<0) ? 0 : (m_nCommTimeout);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


void CDlgCommParamSetting::OnEnSetfocusEditCommReSendTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_RE_SEND_TIMES, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_RE_SEND_TIMES, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[重发次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommReSendTimes();
	}	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommReSendTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommReSendTimes = (m_nCommReSendTimes<0) ? 0 : (m_nCommReSendTimes);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


void CDlgCommParamSetting::OnEnSetfocusEditCommDelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_DELAY)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_DELAY, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_DELAY, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[延时(毫秒)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommDelay();
	}	
}


void CDlgCommParamSetting::OnEnKillfocusEditCommDelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommDelay = (m_nCommDelay<0) ? 0 : (m_nCommDelay);
		UpdateData(FALSE);
		UpdateDlgData(TRUE);
	}
	
}


BOOL CDlgCommParamSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);
	GetDlgItem(IDC_STATIC_GRP_COMM)->SetFocus();
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCommParamSetting::InitDlgItem()
{
	m_comboCommDataLenType.ResetContent();
	m_comboCommDataLenType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COMM_DATA_LEN_CHANGE)/*_T("变化长度")*/);
	m_comboCommDataLenType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COMM_DATA_LEN_SAME)/*_T("固定长度")*/);
	m_comboCommDataLenType.SetCurSel(0);
	UpdateDlgByAuthorize();

}

CCommOptionInfo CDlgCommParamSetting::GetCommOptionInfo()const
{
	return m_commOptionInfo;
}
void CDlgCommParamSetting::SetCommOptionInfo(const CCommOptionInfo &commOptionInfo)
{
	m_commOptionInfo = commOptionInfo;
}

void CDlgCommParamSetting::UpdateDlgData(BOOL bSaveAndValidate /*= TRUE*/)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		m_commOptionInfo.m_bUsePlatformComm = m_bUsePlatformComm;
		m_commOptionInfo.m_nCommDelay = m_nCommDelay;
		m_commOptionInfo.m_nCommTimeOut = m_nCommTimeout;
		m_commOptionInfo.m_nCommReSendTimes = m_nCommReSendTimes;
		m_commOptionInfo.m_nCommXUnitExp = m_nCommXUnitExp;
		m_commOptionInfo.m_nCommYUnitExp = m_nCommYUnitExp;
		m_commOptionInfo.m_nCommDUnitExp = m_nCommDUnitExp;
		m_commOptionInfo.m_nCommDataLenType = m_comboCommDataLenType.GetCurSel();
		m_pVisionASM->SetCommOptionInfo(m_commOptionInfo);
	}
	else
	{
		m_bUsePlatformComm = m_commOptionInfo.m_bUsePlatformComm;
		m_nCommDelay = m_commOptionInfo.m_nCommDelay;
		m_nCommTimeout = m_commOptionInfo.m_nCommTimeOut;
		m_nCommReSendTimes = m_commOptionInfo.m_nCommReSendTimes;
		m_nCommXUnitExp = m_commOptionInfo.m_nCommXUnitExp;
		m_nCommYUnitExp = m_commOptionInfo.m_nCommYUnitExp;
		m_nCommDUnitExp = m_commOptionInfo.m_nCommDUnitExp;
		m_comboCommDataLenType.SetCurSel(m_commOptionInfo.m_nCommDataLenType);

		if (m_bUsePlatformComm)
		{
			GetDlgItem(IDC_EDIT_COMM_DELAY)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_EDIT_COMM_X_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_EDIT_COMM_Y_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
			GetDlgItem(IDC_COMBO_COMM_DATA_LEN_TYPE)->EnableWindow(m_bEnableCommParam);
		}
		else
		{
			GetDlgItem(IDC_EDIT_COMM_DELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COMM_X_UNIT_EXP)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COMM_Y_UNIT_EXP)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_COMM_DATA_LEN_TYPE)->EnableWindow(FALSE);
		}

		UpdateData(FALSE);
	}
}

void CDlgCommParamSetting::OnCbnSelchangeComboCommDataLenType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgData(TRUE);
}


void CDlgCommParamSetting::OnBnClickedChkUsePlatformCommSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bUsePlatformComm)
	{
		GetDlgItem(IDC_EDIT_COMM_DELAY)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_EDIT_COMM_X_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_EDIT_COMM_Y_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->EnableWindow(m_bEnableCommParam);
		GetDlgItem(IDC_COMBO_COMM_DATA_LEN_TYPE)->EnableWindow(m_bEnableCommParam);
	}
	else
	{
		GetDlgItem(IDC_EDIT_COMM_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMM_X_UNIT_EXP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMM_Y_UNIT_EXP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_DATA_LEN_TYPE)->EnableWindow(FALSE);
	}
}



void CDlgCommParamSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
{
	CRect CRectGroupBox;
	// 检查是否为Group控件
	if(NULL== GetDlgItem(nID)) return;
	if (NULL== (CStatic*) GetDlgItem(nID)) return;
	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	ScreenToClient(&CRectGroupBox);

	int nShow = SW_SHOW;
	if (bShow)
	{
		nShow = SW_SHOW;
	}
	else
	{
		nShow = SW_HIDE;
	}

	// Group 控件本身也进行显示或隐藏
	GetDlgItem(nID)->EnableWindow(bEnable);
	GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) || CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgCommParamSetting::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			CString strNodeName = _T("");
			// 通信延时超时设置
			bool bFind = vsConfig->GetCheckedByItem(eCommAuthorize_CommDelaySet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCommParam,m_bEnableCommParam);

			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_COMM,m_bEnableCommParam,m_bShowCommParam);
			}
		}
	}
}