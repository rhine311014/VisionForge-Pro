// CDlgPCBSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "CDlgPCBSetting.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"

// CCDlgPCBSetting 对话框

IMPLEMENT_DYNAMIC(CCDlgPCBSetting, CDialog)

CCDlgPCBSetting::CCDlgPCBSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CCDlgPCBSetting::IDD, pParent)
	, m_bShowPCB4To3Align(FALSE)
{

	m_strPCBInsPreX = _T("0.000000");
	m_strPCBInsPreY = _T("0.000000");
	m_strPCBInsPreD = _T("0.000000");
	m_nPCBInsPreType = 0;
	m_bPCBShowInspectResult = TRUE;
	m_bChange2um = FALSE;
    m_nOldPCBInsPreType = m_nPCBInsPreType;
	m_bKillFocusEnabled = TRUE;
}

CCDlgPCBSetting::~CCDlgPCBSetting()
{
}

void CCDlgPCBSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PCB_INSPECT_X_PRECISION, m_strPCBInsPreX);
	DDX_Text(pDX, IDC_EDIT_PCB_INSPECT_Y_PRECISION, m_strPCBInsPreY);
	DDX_Text(pDX, IDC_EDIT_PCB_INSPECT_D_PRECISION, m_strPCBInsPreD);
	DDX_Radio(pDX, IDC_RADIO_CENTER_INSPECT, m_nPCBInsPreType);
	DDX_Check(pDX, IDC_CHECK_SHOW_INSPECT_RESULT, m_bPCBShowInspectResult);
	DDX_Check(pDX, IDC_CHECK_PCB_4TO3ALIGN, m_bShowPCB4To3Align);
}


BEGIN_MESSAGE_MAP(CCDlgPCBSetting, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_PCB_INSPECT_X_PRECISION, &CCDlgPCBSetting::OnKillfocusEditPcbInspectXPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_PCB_INSPECT_X_PRECISION, &CCDlgPCBSetting::OnSetfocusEditPcbInspectXPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_PCB_INSPECT_Y_PRECISION, &CCDlgPCBSetting::OnKillfocusEditPcbInspectYPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_PCB_INSPECT_Y_PRECISION, &CCDlgPCBSetting::OnSetfocusEditPcbInspectYPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_PCB_INSPECT_D_PRECISION, &CCDlgPCBSetting::OnKillfocusEditPcbInspectDPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_PCB_INSPECT_D_PRECISION, &CCDlgPCBSetting::OnSetfocusEditPcbInspectDPrecision)
    ON_BN_CLICKED(IDC_CHECK_PCB_4TO3ALIGN, &CCDlgPCBSetting::OnBnClickedCheckPcb4to3align)
    ON_BN_CLICKED(IDC_CHECK_SHOW_INSPECT_RESULT, &CCDlgPCBSetting::OnBnClickedCheckShowInspectResult)
    ON_BN_CLICKED(IDC_RADIO_CENTER_INSPECT, &CCDlgPCBSetting::OnBnClickedRadioCenterInspect)
    ON_BN_CLICKED(IDC_RADIO_FORE_POINT_INSPECT, &CCDlgPCBSetting::OnBnClickedRadioForePointInspect)
    ON_BN_CLICKED(IDC_RADIO_THREE_POINT_INSPECT, &CCDlgPCBSetting::OnBnClickedRadioThreePointInspect)
END_MESSAGE_MAP()


// CCDlgPCBSetting 消息处理程序


BOOL CCDlgPCBSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    UpdateDialogLanguage();
    LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
    if (m_psaSysInfoStrings != NULL)
    {
        lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
    }
	if (m_bChange2um)
	{
        switch (lgidLanguage)
        {
        case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
            GetDlgItem(IDC_STATIC_PCB_PRECISION_X)->SetWindowTextA(_T("Presicion X(um)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_Y)->SetWindowTextA(_T("Presicion Y(um)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_D)->SetWindowTextA(_T("Presicion θ(°)"));
            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
            
            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
        default:
            GetDlgItem(IDC_STATIC_PCB_PRECISION_X)->SetWindowTextA(_T("设定精度X(um)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_Y)->SetWindowTextA(_T("设定精度Y(um)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_D)->SetWindowTextA(_T("设定精度θ(°)"));
            break;
        }
		
	}
	else
	{
        switch (lgidLanguage)
        {
        case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
            GetDlgItem(IDC_STATIC_PCB_PRECISION_X)->SetWindowTextA(_T("Presicion X(mm)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_Y)->SetWindowTextA(_T("Presicion Y(mm)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_D)->SetWindowTextA(_T("Presicion θ(°)"));
            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

            break;
        case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
        default:
            GetDlgItem(IDC_STATIC_PCB_PRECISION_X)->SetWindowTextA(_T("设定精度X(mm)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_Y)->SetWindowTextA(_T("设定精度Y(mm)"));
            GetDlgItem(IDC_STATIC_PCB_PRECISION_D)->SetWindowTextA(_T("设定精度θ(°)"));
            break;
        }
	}
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CCDlgPCBSetting::UpdateDialogLanguage()
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

        if(_T("IDD_DLG_PCB_SETTING") == strDlgID)
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

void CCDlgPCBSetting::OnKillfocusEditPcbInspectXPrecision()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_X_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_X_PRECISION, m_strPCBInsPreX);
		}
		else
		{
			m_strPCBInsPreX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_X_PRECISION, m_strPCBInsPreX);
		}
	}	
}


void CCDlgPCBSetting::OnSetfocusEditPcbInspectXPrecision()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PCB_INSPECT_X_PRECISION)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_X_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_X_PRECISION, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[检测精度设置][设置精度X]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPcbInspectXPrecision();
	}

}


void CCDlgPCBSetting::OnKillfocusEditPcbInspectYPrecision()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_Y_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_Y_PRECISION, m_strPCBInsPreY);
		}
		else
		{
			m_strPCBInsPreY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_Y_PRECISION, m_strPCBInsPreY);
		}
	}	
}


void CCDlgPCBSetting::OnSetfocusEditPcbInspectYPrecision()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PCB_INSPECT_Y_PRECISION)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_Y_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_Y_PRECISION, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[检测精度设置][设置精度Y]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPcbInspectYPrecision();
	}
}


void CCDlgPCBSetting::OnKillfocusEditPcbInspectDPrecision()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_D_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_D_PRECISION, m_strPCBInsPreD);
		}
		else
		{
			m_strPCBInsPreD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_D_PRECISION, m_strPCBInsPreD);
		}
	}	
}


void CCDlgPCBSetting::OnSetfocusEditPcbInspectDPrecision()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PCB_INSPECT_D_PRECISION)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_PCB_INSPECT_D_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PCB_INSPECT_D_PRECISION, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[检测精度设置][设置精度D]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPcbInspectDPrecision();
	}
}


void CCDlgPCBSetting::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[保存]退出"));
    g_opLogFile.SaveLogText(strLog,4);
    CDialog::OnOK();
}


void CCDlgPCBSetting::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[取消]退出"));
    g_opLogFile.SaveLogText(strLog,4);
    CDialog::OnCancel();
}


void CCDlgPCBSetting::OnBnClickedCheckPcb4to3align()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowPCB4To3Align ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[启用兼容三点对位方式]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,5);
}


void CCDlgPCBSetting::OnBnClickedCheckShowInspectResult()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bPCBShowInspectResult ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[显示检测结果]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,5);
}
void CCDlgPCBSetting::OnInsPreTypeChange()
{
    UpdateData(TRUE);
    if (m_nOldPCBInsPreType != m_nPCBInsPreType)
    {
        CString strLog;
        CString strVal[] = {_T("中心对位检测模式"),_T("四点独立检测模式"),_T("三点独立检测模式")};
        strLog.Format(_T("修改[检测精度模式设置][%s]为[%s]\n"),strVal[m_nOldPCBInsPreType],strVal[m_nPCBInsPreType]);
        g_opLogFile.SaveLogText(strLog,5);
    }
    m_nOldPCBInsPreType = m_nPCBInsPreType;
}


void CCDlgPCBSetting::OnBnClickedRadioCenterInspect()
{
    // TODO: 在此添加控件通知处理程序代码
    OnInsPreTypeChange();
}


void CCDlgPCBSetting::OnBnClickedRadioForePointInspect()
{
    // TODO: 在此添加控件通知处理程序代码
    OnInsPreTypeChange();
}


void CCDlgPCBSetting::OnBnClickedRadioThreePointInspect()
{
    // TODO: 在此添加控件通知处理程序代码
    OnInsPreTypeChange();
}

BOOL CCDlgPCBSetting::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
			if ((i == 0) && (ch == '+'))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}
