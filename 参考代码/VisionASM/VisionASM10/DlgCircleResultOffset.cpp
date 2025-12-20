// DlgCircleResultOffset.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCircleResultOffset.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"
#include "DlgSearch.h"


// CDlgCircleResultOffset 对话框

IMPLEMENT_DYNAMIC(CDlgCircleResultOffset, CDialogEx)

CDlgCircleResultOffset::CDlgCircleResultOffset(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCircleResultOffset::IDD, pParent)
    , m_nCircleCenterMoveMode(0)
    , m_nCircleCenterMoveModeX(0)
    , m_nCircleCenterMoveModeY(0)
    , m_dCircleCenterMoveLengthX(0.0)
    , m_dCircleCenterMoveLengthY(0.0)
    , m_dCircleCenterMoveRadiusMulX(0.0)
    , m_dCircleCenterMoveRadiusMulY(0.0)
    , m_dCircleCenterMoveAngle(0.0)
    , m_dCircleCenterMoveRadiusMul(0.0)
{
    m_pDlgParent = NULL;
}

CDlgCircleResultOffset::~CDlgCircleResultOffset()
{
}

void CDlgCircleResultOffset::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RDO_C_COORD, m_nCircleCenterMoveMode);
    DDX_Radio(pDX, IDC_RDO_X_BY_OFFSET, m_nCircleCenterMoveModeX);
    DDX_Radio(pDX, IDC_RDO_Y_BY_OFFSET, m_nCircleCenterMoveModeY);
    DDX_Text(pDX, IDC_EDT_X_BY_OFFSET, m_dCircleCenterMoveLengthX);
    DDX_Text(pDX, IDC_EDT_Y_BY_OFFSET, m_dCircleCenterMoveLengthY);
    DDX_Text(pDX, IDC_EDT_X_BY_RADIUS, m_dCircleCenterMoveRadiusMulX);
    DDX_Text(pDX, IDC_EDT_Y_BY_RADIUS, m_dCircleCenterMoveRadiusMulY);
    DDX_Text(pDX, IDC_EDT_ANGLE, m_dCircleCenterMoveAngle);
    DDX_Text(pDX, IDC_EDT_RADIUS, m_dCircleCenterMoveRadiusMul);
}


BEGIN_MESSAGE_MAP(CDlgCircleResultOffset, CDialogEx)
    ON_EN_SETFOCUS(IDC_EDT_X_BY_OFFSET, &CDlgCircleResultOffset::OnEnSetfocusEdtXByOffset)
    ON_EN_SETFOCUS(IDC_EDT_X_BY_RADIUS, &CDlgCircleResultOffset::OnEnSetfocusEdtXByRadius)
    ON_EN_SETFOCUS(IDC_EDT_Y_BY_OFFSET, &CDlgCircleResultOffset::OnEnSetfocusEdtYByOffset)
    ON_EN_SETFOCUS(IDC_EDT_Y_BY_RADIUS, &CDlgCircleResultOffset::OnEnSetfocusEdtYByRadius)
    ON_EN_SETFOCUS(IDC_EDT_ANGLE, &CDlgCircleResultOffset::OnEnSetfocusEdtAngle)
    ON_EN_SETFOCUS(IDC_EDT_RADIUS, &CDlgCircleResultOffset::OnEnSetfocusEdtRadius)
END_MESSAGE_MAP()


// CDlgCircleResultOffset 消息处理程序


void CDlgCircleResultOffset::OnEnSetfocusEdtXByOffset()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_X_BY_OFFSET)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_X_BY_OFFSET, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_X_BY_OFFSET, strOutput);
        }
    }
}
void CDlgCircleResultOffset::UpdateDialogLanguage()
{
    //	return;
    LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
    if (m_psaSysInfoStrings != NULL)
    {
        lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
    }

    vcXMLConfigurator xml;
    CString strXMLPath;
    CString strCurWorkDir = m_pDlgParent->m_strCurWorkDir;

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

        if(_T("IDD_DLG_CIRCLE_RESULT_OFFSET") == strDlgID)
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

void CDlgCircleResultOffset::OnEnSetfocusEdtXByRadius()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_X_BY_RADIUS)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_X_BY_RADIUS, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_X_BY_RADIUS, strOutput);
        }
    }
}


void CDlgCircleResultOffset::OnEnSetfocusEdtYByOffset()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_Y_BY_OFFSET)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_Y_BY_OFFSET, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_Y_BY_OFFSET, strOutput);
        }
    }

}


void CDlgCircleResultOffset::OnEnSetfocusEdtYByRadius()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_Y_BY_RADIUS)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_Y_BY_RADIUS, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_Y_BY_RADIUS, strOutput);
        }
    }
}


void CDlgCircleResultOffset::OnEnSetfocusEdtAngle()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_ANGLE)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_ANGLE, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_ANGLE, strOutput);
        }
    }
}


void CDlgCircleResultOffset::OnEnSetfocusEdtRadius()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bSysKeyboardEnabled)
    {
        PostMessage(WM_SETFOCUS);

        CRect rcWnd;
        CString strInput, strOutput;
        GetDlgItem(IDC_EDT_RADIUS)->GetWindowRect(rcWnd);
        GetDlgItemText(IDC_EDT_RADIUS, strInput);

        if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
        {
            SetDlgItemText(IDC_EDT_RADIUS, strOutput);
        }
    }
}


BOOL CDlgCircleResultOffset::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    UpdateDialogLanguage();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
