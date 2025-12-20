// DlgHistogramForSpecicalPick.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgHistogramForSpecicalPick.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// DlgHistogramForSpecicalPick 对话框

IMPLEMENT_DYNAMIC(DlgHistogramForSpecicalPick, CDialogEx)

DlgHistogramForSpecicalPick::DlgHistogramForSpecicalPick(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgHistogramForSpecicalPick::IDD, pParent)
	, m_MeanHighHead(0)
	, m_nHeadGreyValue(0)
	, m_nTailGreyValue(0)
{
	m_dHeadXoffset = 0;
	m_dHeadYOffset = 0;
	m_nHeadLength = 50;
	m_nHeadWidth = 30;

	m_dTailXoffset = 0;
	m_dTailYOffset = 0;
	m_nTailLength = 50;
	m_nTailWidth = 30;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;
}

DlgHistogramForSpecicalPick::~DlgHistogramForSpecicalPick()
{
}

void DlgHistogramForSpecicalPick::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
// 	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_4, m_MeanHighHead);
// 	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_4, m_nHeadGreyValue);
// 	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_5, m_nTailGreyValue);
// 
// 	DDX_Text(pDX,IDC_EDT_LINE_CALIPERNUM_1,m_dHeadXoffset);
// 	DDX_Text(pDX,IDC_EDT_LINE_OUTLINE_NUM_1,m_dHeadYOffset);
// 	DDX_Text(pDX,IDC_RAD_LINE_POLARITY_MODE0_1,m_nHeadLength);
// 	DDX_Text(pDX,IDC_EDT_LINE_CONTRAST_THRE_1,m_nHeadWidth);
// 
// 	DDX_Text(pDX,IDC_EDT_LINE_CALIPERNUM_2,m_dTailXoffset);
// 	DDX_Text(pDX,IDC_EDT_LINE_OUTLINE_NUM_2,m_dTailYOffset);
// 	DDX_Text(pDX,IDC_RAD_LINE_POLARITY_MODE0_2,m_nTailLength);
// 	DDX_Text(pDX,IDC_EDT_LINE_CONTRAST_THRE_2,m_nTailWidth);

	DDX_Radio(pDX, IDC_RAD_HEAD_TAIL_MODE0, m_MeanHighHead);
	DDX_Text(pDX, IDC_EDT_HEAD_GREY_VALUE, m_nHeadGreyValue);
	DDX_Text(pDX, IDC_EDT_TAIL_GREY_VALUE, m_nTailGreyValue);

	DDX_Text(pDX,IDC_EDT_HEAD_OFFSET_X,m_dHeadXoffset);
	DDX_Text(pDX,IDC_EDT_HEAD_OFFSET_Y,m_dHeadYOffset);
	DDX_Text(pDX,IDC_EDT_HEAD_LENGTH,m_nHeadLength);
	DDX_Text(pDX,IDC_EDT_HEAD_WIDTH,m_nHeadWidth);

	DDX_Text(pDX,IDC_EDT_TAIL_OFFSET_X,m_dTailXoffset);
	DDX_Text(pDX,IDC_EDT_TAIL_OFFSET_Y,m_dTailYOffset);
	DDX_Text(pDX,IDC_EDT_TAIL_LENGTH,m_nTailLength);
	DDX_Text(pDX,IDC_EDT_TAIL_WIDTH,m_nTailWidth);
}


BEGIN_MESSAGE_MAP(DlgHistogramForSpecicalPick, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDT_HEAD_OFFSET_X, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadOffsetX)
	ON_EN_SETFOCUS(IDC_EDT_HEAD_OFFSET_Y, &DlgHistogramForSpecicalPick::OnSetfocusEdtLineHeadOffsetY)
	ON_EN_SETFOCUS(IDC_EDT_HEAD_LENGTH, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadLength)
	ON_EN_SETFOCUS(IDC_EDT_HEAD_WIDTH, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadWidth)
	ON_EN_SETFOCUS(IDC_EDT_TAIL_OFFSET_Y, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailOffsetY)
	ON_EN_SETFOCUS(IDC_EDT_TAIL_OFFSET_X, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailOffsetX)
	ON_EN_SETFOCUS(IDC_EDT_TAIL_LENGTH, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailLength)
	ON_EN_SETFOCUS(IDC_EDT_TAIL_WIDTH, &DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailWidth)
END_MESSAGE_MAP()


// DlgHistogramForSpecicalPick 消息处理程序


void DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadOffsetX()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_HEAD_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_HEAD_OFFSET_X, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_HEAD_OFFSET_X, strOutput);
		}
	}	
}


void DlgHistogramForSpecicalPick::OnSetfocusEdtLineHeadOffsetY()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_HEAD_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_HEAD_OFFSET_Y, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_HEAD_OFFSET_Y, strOutput);
		}
	}	
}


void DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadLength()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_HEAD_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_HEAD_LENGTH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_HEAD_LENGTH, strOutput);
		}
	}	
}

void DlgHistogramForSpecicalPick::OnEnSetfocusEdtHeadWidth()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_HEAD_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_HEAD_WIDTH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_HEAD_WIDTH, strOutput);
		}
	}	
}

void DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailOffsetY()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TAIL_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TAIL_OFFSET_Y, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TAIL_OFFSET_Y, strOutput);
		}
	}	
}


void DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailOffsetX()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TAIL_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TAIL_OFFSET_X, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TAIL_OFFSET_X, strOutput);
		}
	}	
}


void DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailLength()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TAIL_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TAIL_LENGTH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TAIL_LENGTH, strOutput);
		}
	}	
}


void DlgHistogramForSpecicalPick::OnEnSetfocusEdtTailWidth()
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TAIL_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TAIL_WIDTH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TAIL_WIDTH, strOutput);
		}
	}	
}


BOOL DlgHistogramForSpecicalPick::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void DlgHistogramForSpecicalPick::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_HISTOGRAM_FOR_SPECIAL_PICK") == strDlgID)
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

void DlgHistogramForSpecicalPick::CheckDlgDataValidation()
{

	UpdateData(TRUE);
	if (m_nHeadWidth < 1)
	{
		m_nHeadWidth = 1;
	}

	if (m_nHeadLength < 1)
	{
		m_nHeadLength = 1;
	}


	if (m_nTailWidth < 1)
	{
		m_nTailWidth = 1;
	}

	if (m_nTailLength< 1)
	{
		m_nTailLength = 1;
	}

	UpdateData(FALSE);

}

BOOL DlgHistogramForSpecicalPick::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_psaSysInfoStrings=m_pDlgParent->m_psaSysInfoStrings;
	this->UpdateDialogLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
