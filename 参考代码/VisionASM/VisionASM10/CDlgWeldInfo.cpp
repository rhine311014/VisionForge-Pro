// CDlgWeldInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "CDlgWeldInfo.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"
// CCDlgWeldInfo 对话框

IMPLEMENT_DYNAMIC(CCDlgWeldInfo, CDialog)

CCDlgWeldInfo::CCDlgWeldInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CCDlgWeldInfo::IDD, pParent)
{

	m_strWeldMovementRatioX		= _T("1.000000");
	m_strWeldMovementRatioY		= _T("1.000000");
	m_strWeldMovementRatioD		= _T("1.000000");

	m_strWeldAmmendMoveRatioX	= _T("1.000000");
	m_strWeldAmmendMoveRatioY	= _T("1.000000");

	m_nWeldTargetSearchTimes	= 0;
	m_nWeldSearchBench			= 0;
	m_nWeldAngleBench			= 0;

    m_nOldWeldAngleBench = m_nWeldAngleBench;
    m_nOldWeldSearchBench = m_nWeldSearchBench;
    m_nOldWeldTargetSearchTimes = m_nWeldTargetSearchTimes;
	m_bKillFocusEnabled			= TRUE;
}

CCDlgWeldInfo::~CCDlgWeldInfo()
{
}

void CCDlgWeldInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENABLE_WELD, m_bEnableWeld);
	DDX_Text(pDX, IDC_EDIT_WELD_MOVEMENT_RATIO_X, m_strWeldMovementRatioX);
	DDX_Text(pDX, IDC_EDIT_WELD_MOVEMENT_RATIO_Y, m_strWeldMovementRatioY);
	DDX_Text(pDX, IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, m_strWeldAmmendMoveRatioX);
	DDX_Text(pDX, IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, m_strWeldAmmendMoveRatioY);
	DDX_Radio(pDX, IDC_RADIO_WELD_TARGET_MUL_SEARCH, m_nWeldTargetSearchTimes);
	DDX_Radio(pDX, IDC_RADIO_WELD_OBJECT_BENCH, m_nWeldSearchBench);
	DDX_Radio(pDX, IDC_RADIO_WELD_OBJECT_ANGLE_BENCH, m_nWeldAngleBench);
	DDX_Text(pDX, IDC_EDIT_WELD_MOVEMENT_RATIO_D, m_strWeldMovementRatioD);
}


BEGIN_MESSAGE_MAP(CCDlgWeldInfo, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_X, &CCDlgWeldInfo::OnKillfocusEditWeldMovementRatioX)
	ON_EN_SETFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_X, &CCDlgWeldInfo::OnSetfocusEditWeldMovementRatioX)
	ON_EN_KILLFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, &CCDlgWeldInfo::OnKillfocusEditWeldMovementRatioY)
	ON_EN_SETFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, &CCDlgWeldInfo::OnSetfocusEditWeldMovementRatioY)
	ON_EN_KILLFOCUS(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, &CCDlgWeldInfo::OnKillfocusEditWeldAmmendMovementRatioX)
	ON_EN_SETFOCUS(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, &CCDlgWeldInfo::OnSetfocusEditWeldAmmendMovementRatioX)
	ON_EN_KILLFOCUS(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, &CCDlgWeldInfo::OnKillfocusEditWeldAmmendMovementRatioY)
	ON_EN_SETFOCUS(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, &CCDlgWeldInfo::OnSetfocusEditWeldAmmendMovementRatioY)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_WELD, &CCDlgWeldInfo::OnBnClickedCheckEnableWeld)
    ON_BN_CLICKED(IDC_RADIO_WELD_TARGET_MUL_SEARCH, &CCDlgWeldInfo::OnBnClickedRadioWeldTargetMulSearch)
    ON_BN_CLICKED(IDC_RADIO_WELD_TARGET_ONE_SEARCH, &CCDlgWeldInfo::OnBnClickedRadioWeldTargetOneSearch)
    ON_BN_CLICKED(IDC_RADIO_WELD_TARGET_ZERO_SEARCH, &CCDlgWeldInfo::OnBnClickedRadioWeldTargetZeroSearch)
    ON_BN_CLICKED(IDC_RADIO_WELD_OBJECT_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldObjectBench)
    ON_BN_CLICKED(IDC_RADIO_WELD_TARGET_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldTargetBench)
    ON_BN_CLICKED(IDC_RADIO_WELD_NO_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldNoBench)
    ON_BN_CLICKED(IDC_RADIO_WELD_OBJECT_ANGLE_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldObjectAngleBench)
    ON_BN_CLICKED(IDC_RADIO_WELD_TARGET_ANGLE_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldTargetAngleBench)
    ON_BN_CLICKED(IDC_RADIO_WELD_NO_ANGLE_BENCH, &CCDlgWeldInfo::OnBnClickedRadioWeldNoAngleBench)
	ON_EN_KILLFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_D, &CCDlgWeldInfo::OnEnKillfocusEditWeldMovementRatioD)
	ON_EN_SETFOCUS(IDC_EDIT_WELD_MOVEMENT_RATIO_D, &CCDlgWeldInfo::OnEnSetfocusEditWeldMovementRatioD)
END_MESSAGE_MAP()


// CCDlgWeldInfo 消息处理程序


BOOL CCDlgWeldInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    UpdateDialogLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCDlgWeldInfo::OnKillfocusEditWeldMovementRatioX()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_X, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_X, m_strWeldMovementRatioX);
		}
		else
		{
			m_strWeldMovementRatioX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_X, m_strWeldMovementRatioX);
		}
	}	
}

void CCDlgWeldInfo::UpdateDialogLanguage()
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

        if(_T("IDD_DLG_WELD_SETTING") == strDlgID)
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

void CCDlgWeldInfo::OnSetfocusEditWeldMovementRatioX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WELD_MOVEMENT_RATIO_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_X, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[运动量系数X]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditWeldMovementRatioX();
	}
}





void CCDlgWeldInfo::OnKillfocusEditWeldMovementRatioY()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, m_strWeldMovementRatioY);
		}
		else
		{
			m_strWeldMovementRatioY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, m_strWeldMovementRatioY);
		}
	}
}


void CCDlgWeldInfo::OnSetfocusEditWeldMovementRatioY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WELD_MOVEMENT_RATIO_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_Y, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[运动量系数Y]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditWeldMovementRatioY();

	}
}




void CCDlgWeldInfo::OnKillfocusEditWeldAmmendMovementRatioX()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, m_strWeldAmmendMoveRatioX);
		}
		else
		{
			m_strWeldAmmendMoveRatioX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, m_strWeldAmmendMoveRatioX);
		}
	}
}


void CCDlgWeldInfo::OnSetfocusEditWeldAmmendMovementRatioX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_X, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[X轴补偿系数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditWeldAmmendMovementRatioX();

	}
}




void CCDlgWeldInfo::OnKillfocusEditWeldAmmendMovementRatioY()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, m_strWeldAmmendMoveRatioY);
		}
		else
		{
			m_strWeldAmmendMoveRatioY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, m_strWeldAmmendMoveRatioY);
		}
	}
}


void CCDlgWeldInfo::OnSetfocusEditWeldAmmendMovementRatioY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WELD_AMMEND_MOVEMENT_RATIO_Y, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[Y轴补偿系数]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,5);
            }
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditWeldAmmendMovementRatioY();

	}
}


void CCDlgWeldInfo::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[保存]退出"));
    g_opLogFile.SaveLogText(strLog,4);
    CDialog::OnOK();
}


void CCDlgWeldInfo::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString strLog;
    strLog.Format(_T("[取消]退出"));
    g_opLogFile.SaveLogText(strLog,4);
    CDialog::OnCancel();
}


void CCDlgWeldInfo::OnBnClickedCheckEnableWeld()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bEnableWeld ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[焊接机参数设置]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,5);
}
void CCDlgWeldInfo::OnTargetSearchTimesChange()
{
    UpdateData(TRUE);
    if (m_nOldWeldTargetSearchTimes != m_nWeldTargetSearchTimes)
    {
        CString strLog;
        CString strVal[] = {_T("目标多次对位"),_T("目标一次对位"),_T("目标零次对位")};
        strLog.Format(_T("修改[%s]为[%s]\n"),strVal[m_nOldWeldTargetSearchTimes],strVal[m_nWeldTargetSearchTimes]);
        g_opLogFile.SaveLogText(strLog,5);
    }
    m_nOldWeldTargetSearchTimes = m_nWeldTargetSearchTimes;

}
void CCDlgWeldInfo::OnWeldSearchBenchChange()
{
    UpdateData(TRUE);
    if (m_nOldWeldSearchBench!= m_nWeldSearchBench)
    {
        CString strLog;
        CString strVal[] = {_T("(X,Y)以对象为基准"),_T("(X,Y)以目标为基准"),_T("(X,Y)无基准")};
        strLog.Format(_T("修改[%s]为[%s]\n"),strVal[m_nOldWeldSearchBench],strVal[m_nWeldSearchBench]);
        g_opLogFile.SaveLogText(strLog,5);
    }
    m_nOldWeldSearchBench = m_nWeldSearchBench;
}
void CCDlgWeldInfo::OnWeldAngleBenchChange()
{
    UpdateData(TRUE);
    if (m_nOldWeldAngleBench != m_nWeldAngleBench)
    {
        CString strLog;
        CString strVal[] = {_T("角度以对象为基准"),_T("角度以目标为基准"),_T("角度无基准")};
        strLog.Format(_T("修改[%s]为[%s]\n"),strVal[m_nOldWeldAngleBench],strVal[m_nWeldAngleBench]);
        g_opLogFile.SaveLogText(strLog,5);
    }
    m_nOldWeldAngleBench = m_nWeldAngleBench;
}

void CCDlgWeldInfo::OnBnClickedRadioWeldTargetMulSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    OnTargetSearchTimesChange();
    
}


void CCDlgWeldInfo::OnBnClickedRadioWeldTargetOneSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    OnTargetSearchTimesChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldTargetZeroSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    OnTargetSearchTimesChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldObjectBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldSearchBenchChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldTargetBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldSearchBenchChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldNoBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldSearchBenchChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldObjectAngleBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldAngleBenchChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldTargetAngleBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldAngleBenchChange();
}


void CCDlgWeldInfo::OnBnClickedRadioWeldNoAngleBench()
{
    // TODO: 在此添加控件通知处理程序代码
    OnWeldAngleBenchChange();
}

BOOL CCDlgWeldInfo::IsStringPositiveNumerical(LPCTSTR lpszVal)
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

void CCDlgWeldInfo::OnEnKillfocusEditWeldMovementRatioD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_D, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_D, m_strWeldMovementRatioD);
		}
		else
		{
			m_strWeldMovementRatioD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_D, m_strWeldMovementRatioD);
		}
	}
}


void CCDlgWeldInfo::OnEnSetfocusEditWeldMovementRatioD()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WELD_MOVEMENT_RATIO_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WELD_MOVEMENT_RATIO_D, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[运动量系数D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditWeldMovementRatioD();

	}
}