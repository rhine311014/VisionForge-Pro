// DlgBlobSearchParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgBlobSearchParamSet.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
// CDlgBlobSearchParamSet 对话框

IMPLEMENT_DYNAMIC(CDlgBlobSearchParamSet, CDialogEx)

CDlgBlobSearchParamSet::CDlgBlobSearchParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBlobSearchParamSet::IDD, pParent)
	, m_nBlobSearchNums(1)
	, m_nOutPutPoint1(0)
	, m_nOutPutPoint2(0)
	, m_nImageChannel(0)
{
	m_nBlobMode = 0;
	m_lBlobGrayThre = 128;
	m_lConnectivityMinPels = 100;
	m_lBlobAreaMinThre = 100;
	m_lBlobAreaMaxThre = 1000;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;
}

CDlgBlobSearchParamSet::~CDlgBlobSearchParamSet()
{
}

void CDlgBlobSearchParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BLOB_MODE, m_nBlobMode);
	DDX_Text(pDX, IDC_EDT_THRESHOLD, m_lBlobGrayThre);
	DDX_Text(pDX, IDC_EDT_CONNECT_MIN_PELS, m_lConnectivityMinPels);
	DDX_Text(pDX, IDC_EDT_BLOB_MIN_AREA, m_lBlobAreaMinThre);
	DDX_Text(pDX, IDC_EDT_BLOB_MAX_AREA, m_lBlobAreaMaxThre);
	DDX_Text(pDX, IDC_EDT_BLOB_SEARCH_NUMS, m_nBlobSearchNums);
	DDX_Control(pDX, IDC_COMBO_OUTPUTPOINT1, m_ComboOutput1);
	DDX_Control(pDX, IDC_COMBO_OUTPUTPOINT2, m_ComboOutput2);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUTPOINT1, m_nOutPutPoint1);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUTPOINT2, m_nOutPutPoint2);
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgBlobSearchParamSet, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDT_BLOB_MIN_AREA, &CDlgBlobSearchParamSet::OnSetfocusEdtBlobMinArea)
	ON_EN_SETFOCUS(IDC_EDT_BLOB_MAX_AREA, &CDlgBlobSearchParamSet::OnSetfocusEdtBlobMaxArea)
	ON_EN_SETFOCUS(IDC_EDT_CONNECT_MIN_PELS, &CDlgBlobSearchParamSet::OnSetfocusEdtConnectMinPels)
	ON_EN_SETFOCUS(IDC_EDT_BLOB_SEARCH_NUMS, &CDlgBlobSearchParamSet::OnEnSetfocusEdtBlobSearchNums)
	ON_EN_SETFOCUS(IDC_EDT_THRESHOLD, &CDlgBlobSearchParamSet::OnEnSetfocusEdtThreshold)
END_MESSAGE_MAP()


// CDlgBlobSearchParamSet 消息处理程序


BOOL CDlgBlobSearchParamSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	CString Stroutput[9];
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		Stroutput[0] = _T("Center");
		Stroutput[1] = _T("The center of the outer rectangle");
		Stroutput[2] = _T("The UpperLeft of the outer rectangle");
		Stroutput[3] = _T("The UpperRight of the outer rectangle");
		Stroutput[4] = _T("The BottomLeft of the outer rectangle");
		Stroutput[5] = _T("The BottomRight of the outer rectangle");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		Stroutput[0] = _T("质心");
		Stroutput[1] = _T("外接矩形中心");
		Stroutput[2] = _T("外接矩形左上");
		Stroutput[3] = _T("外接矩形右上");
		Stroutput[4] = _T("外接矩形左下");
		Stroutput[5] = _T("外接矩形右下");
		break;
	}
	for (int i= 0;i<6;i++)
	{
		m_ComboOutput1.AddString(Stroutput[i]);
		m_ComboOutput2.AddString(Stroutput[i]);
	}
	m_ComboOutput1.SetCurSel(2);
	m_ComboOutput2.SetCurSel(3);
	m_nOutPutPoint1 = 2;
	m_nOutPutPoint2 = 3;
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CDlgBlobSearchParamSet::UpdateRadioButtonDisplay()
{
	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{
			if (m_pDlgParent->m_imageTrain.IsRGB24() || m_pDlgParent->m_imageTrain.IsRGB32())
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			cpImage imageTrain;
			m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
			if (imageTrain.IsValid() && 
				(imageTrain.IsRGB24() || imageTrain.IsRGB32()))
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
}
BOOL CDlgBlobSearchParamSet::PreTranslateMessage(MSG* pMsg)
{

	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CDlgBlobSearchParamSet::OnSetfocusEdtBlobMinArea()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BLOB_MIN_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BLOB_MIN_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BLOB_MIN_AREA, strOutput);
		}
	}
}


void CDlgBlobSearchParamSet::OnSetfocusEdtBlobMaxArea()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BLOB_MAX_AREA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BLOB_MAX_AREA, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BLOB_MAX_AREA, strOutput);
		}
	}
}


void CDlgBlobSearchParamSet::OnSetfocusEdtConnectMinPels()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_CONNECT_MIN_PELS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_CONNECT_MIN_PELS, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_CONNECT_MIN_PELS, strOutput);
		}
	}
}


void CDlgBlobSearchParamSet::OnEnSetfocusEdtBlobSearchNums()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BLOB_SEARCH_NUMS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BLOB_SEARCH_NUMS, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BLOB_SEARCH_NUMS, strOutput);
		}
	}
}


void CDlgBlobSearchParamSet::OnEnSetfocusEdtThreshold()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_THRESHOLD)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_THRESHOLD, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_THRESHOLD, strOutput);
		}
	}
}


void CDlgBlobSearchParamSet::UpdateDialogLanguage()
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
		if(_T("IDD_DLG_BLOB_SEARCH_TOOL_FOR_SPECIAL_PICK") == strDlgID)
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
