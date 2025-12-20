// DlgFind4LineParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgFind4LineParamSet.h"
#include "afxdialogex.h"
#include "DlgSearch.h"



// CDlgFind4LineParamSet 对话框
class CDlgSearch;
IMPLEMENT_DYNAMIC(CDlgFind4LineParamSet, CDialogEx)

CDlgFind4LineParamSet::CDlgFind4LineParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFind4LineParamSet::IDD, pParent)
	, m_bSortByScore1(TRUE)
	, m_bSortByScore2(TRUE)
	, m_bSortByScore3(TRUE)
	, m_bSortByScore4(TRUE)
	, m_bShowCaliper_1(FALSE)
	, m_bShowCaliper_2(FALSE)
	, m_bShowCaliper_3(FALSE)
	, m_bShowCaliper_4(FALSE)
	, m_bEnableDualEdge_1(FALSE)
	, m_bEnableDualEdge_2(FALSE)
	, m_bEnableDualEdge_3(FALSE)
	, m_bEnableDualEdge_4(FALSE)
{
	m_nLineCaliperNum1 = 20;
	m_nLineCaliperNum2 = 20;
	m_nLineCaliperNum3 = 20;
	m_nLineCaliperNum4 = 20;

	m_dLineContrastThre1 = 10.0;
	m_dLineContrastThre2 = 10.0;
	m_dLineContrastThre3 = 10.0;
	m_dLineContrastThre4 = 10.0;

	m_lLineFilterHalfWidth1 = 1;
	m_lLineFilterHalfWidth2 = 1;
	m_lLineFilterHalfWidth3 = 1;
	m_lLineFilterHalfWidth4 = 1;

	m_nLineOutLineNum1 = 10;
	m_nLineOutLineNum2 = 10;
	m_nLineOutLineNum3 = 10;
	m_nLineOutLineNum4 = 10;

	m_nLinePolarityMode1 = (int)eLightToDark;
	m_nLinePolarityMode2 = (int)eLightToDark;
	m_nLinePolarityMode3 = (int)eLightToDark;
	m_nLinePolarityMode4 = (int)eLightToDark;

	m_bEnableLineMask_1 = FALSE;
	m_bShowFitPoint_1 = FALSE;

	m_bEnableLineMask_2 = FALSE;
	m_bShowFitPoint_2 = FALSE;

	m_bEnableLineMask_3 = FALSE;
	m_bShowFitPoint_3 = FALSE;

	m_bEnableLineMask_4 = FALSE;
	m_bShowFitPoint_4 = FALSE;

	//  m_nResultOutMode = 0;

	m_nResultOutMode1 = 0;
	m_nResultOutMode2 = 1;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgFind4LineParamSet::~CDlgFind4LineParamSet()
{

}

void CDlgFind4LineParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_1, m_nLineCaliperNum1);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_2, m_nLineCaliperNum2);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_4, m_nLineCaliperNum3);
	DDX_Text(pDX, IDC_EDT_LINE_CALIPERNUM_5, m_nLineCaliperNum4);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_1, m_dLineContrastThre1);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_2, m_dLineContrastThre2);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_4, m_dLineContrastThre3);
	DDX_Text(pDX, IDC_EDT_LINE_CONTRAST_THRE_5, m_dLineContrastThre4);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_1, m_nLineOutLineNum1);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_2, m_nLineOutLineNum2);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_4, m_nLineOutLineNum3);
	DDX_Text(pDX, IDC_EDT_LINE_OUTLINE_NUM_5, m_nLineOutLineNum4);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_1, m_nLinePolarityMode1);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_2, m_nLinePolarityMode2);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_4, m_nLinePolarityMode3);
	DDX_Radio(pDX, IDC_RAD_LINE_POLARITY_MODE0_5, m_nLinePolarityMode4);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE1, m_bSortByScore1);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE2, m_bSortByScore2);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE4, m_bSortByScore3);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_SCORE5, m_bSortByScore4);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE1, m_checkSortByScore1);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE2, m_checkSortByScore2);	
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE4, m_checkSortByScore3);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_SCORE5, m_checkSortByScore4);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_1, m_bEnableLineMask_1);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_2, m_bEnableLineMask_2);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_3, m_bEnableLineMask_3);
	DDX_Check(pDX, IDC_CHECK_LINE_MASK_4, m_bEnableLineMask_4);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_1, m_bShowFitPoint_1);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_2, m_bShowFitPoint_2);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_3, m_bShowFitPoint_3);
	DDX_Check(pDX, IDC_CHECK_SHOW_FITPOINT_4, m_bShowFitPoint_4);
	//DDX_Radio(pDX, IDC_RAD_OUT_CENTER,m_nResultOutMode);

	DDX_Control(pDX, IDC_COMBO_OUTPUTPOINT1, m_comboOuput1);
	DDX_Control(pDX, IDC_COMBO_OUTPUTPOINT2, m_comboOuput2);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUTPOINT1, m_nResultOutMode1);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUTPOINT2, m_nResultOutMode2);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_1, m_bShowCaliper_1);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_2, m_bShowCaliper_2);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_3, m_bShowCaliper_3);
	DDX_Check(pDX, IDC_CHK_SHOW_LINE_CALIPER_4, m_bShowCaliper_4);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_0, m_bEnableDualEdge_1);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_1, m_bEnableDualEdge_2);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_2, m_bEnableDualEdge_3);
	DDX_Check(pDX, IDC_CHK_ENABLE_DUAL_EDGE_3, m_bEnableDualEdge_4);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_1, m_lLineFilterHalfWidth1);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_2, m_lLineFilterHalfWidth2);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_3, m_lLineFilterHalfWidth3);
	DDX_Text(pDX, IDC_EDT_LINE_FILTER_HALFWIDTH_4, m_lLineFilterHalfWidth4);
}


BEGIN_MESSAGE_MAP(CDlgFind4LineParamSet, CDialogEx)
	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_1, OnChangeEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_1, OnSetfocusEdtLineCalipernum1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_1, OnSetfocusEdtLineContrastThre1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_1, OnSetfocusEdtLineOutlineNum1)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_2, OnChangeEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_2, OnSetfocusEdtLineCalipernum2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_2, OnSetfocusEdtLineContrastThre2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_2, OnSetfocusEdtLineOutlineNum2)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_4, OnChangeEdtLineCalipernum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_4, OnSetfocusEdtLineCalipernum3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_4, OnSetfocusEdtLineContrastThre3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_4, OnSetfocusEdtLineOutlineNum3)

	ON_EN_CHANGE(IDC_EDT_LINE_CALIPERNUM_5, OnChangeEdtLineCalipernum4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CALIPERNUM_5, OnSetfocusEdtLineCalipernum4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_CONTRAST_THRE_5, OnSetfocusEdtLineContrastThre4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_OUTLINE_NUM_5, OnSetfocusEdtLineOutlineNum4)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_1, OnEnSetfocusEditLineFilterHalfWidth1)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_2, OnEnSetfocusEditLineFilterHalfWidth2)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_3, OnEnSetfocusEditLineFilterHalfWidth3)
	ON_EN_SETFOCUS(IDC_EDT_LINE_FILTER_HALFWIDTH_4, OnEnSetfocusEditLineFilterHalfWidth4)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_1, &CDlgFind4LineParamSet::OnBnClickedCheckLineMask1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_1, &CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint1)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_2, &CDlgFind4LineParamSet::OnBnClickedCheckLineMask2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_2, &CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint2)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_3, &CDlgFind4LineParamSet::OnBnClickedCheckLineMask3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_3, &CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint3)
	ON_BN_CLICKED(IDC_CHECK_LINE_MASK_4, &CDlgFind4LineParamSet::OnBnClickedCheckLineMask4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FITPOINT_4, &CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint4)

	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_1, &CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper1)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_2, &CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper2)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_3, &CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper3)
	ON_BN_CLICKED(IDC_CHK_SHOW_LINE_CALIPER_4, &CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper4)
END_MESSAGE_MAP()


// CDlgFind4LineParamSet 消息处理程序


BOOL CDlgFind4LineParamSet::PreTranslateMessage(MSG* pMsg)
{
	
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgFind4LineParamSet::OnInitDialog()
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
		Stroutput[0] = _T("Up");
		Stroutput[1] = _T("Down");
		Stroutput[2] = _T("Left");
		Stroutput[3] = _T("Right");
		Stroutput[4] = _T("Upper Left");
		Stroutput[5] = _T("Upper Right");
		Stroutput[6] = _T("Bottom Left");
		Stroutput[7] = _T("Bottom Right");
		Stroutput[8] = _T("Center");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		Stroutput[0] = _T("上");
		Stroutput[1] = _T("下");
		Stroutput[2] = _T("左");
		Stroutput[3] = _T("右");
		Stroutput[4] = _T("左上");
		Stroutput[5] = _T("右上");
		Stroutput[6] = _T("左下");
		Stroutput[7] = _T("右下");
		Stroutput[8] = _T("中心");
		break;
	}
	for (int i= 0;i<9;i++)
	{
		m_comboOuput1.AddString(Stroutput[i]);
		m_comboOuput2.AddString(Stroutput[i]);
	}
	m_comboOuput1.SetCurSel(0);
	m_comboOuput2.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	
}




void CDlgFind4LineParamSet::OnBnClickedCheckLineMask1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckLineMask2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}

void CDlgFind4LineParamSet::OnBnClickedCheckLineMask3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckLineMask4()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateTrainData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}


void CDlgFind4LineParamSet::OnBnClickedCheckShowFitpoint4()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateSearchData(TRUE);

	m_pDlgParent->UpdateGuiDisplay();
}




void CDlgFind4LineParamSet::OnSetfocusEdtLineCalipernum1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_1, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineContrastThre1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_1, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineOutlineNum1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_1, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineCalipernum2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_2, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineContrastThre2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_2, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineOutlineNum2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_2, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineCalipernum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_4, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineContrastThre3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_4, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineOutlineNum3() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_4, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineCalipernum4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CALIPERNUM_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CALIPERNUM_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CALIPERNUM_5, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineContrastThre4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_CONTRAST_THRE_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_CONTRAST_THRE_5, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnSetfocusEdtLineOutlineNum4() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_OUTLINE_NUM_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_5, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_OUTLINE_NUM_5, strOutput);
		}
	}
}

void CDlgFind4LineParamSet::OnChangeEdtLineCalipernum1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum1 < 2) || (m_nLineCaliperNum1 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgFind4LineParamSet::OnChangeEdtLineCalipernum2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum2 < 2) || (m_nLineCaliperNum2 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgFind4LineParamSet::OnChangeEdtLineCalipernum3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum3 < 2) || (m_nLineCaliperNum3 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgFind4LineParamSet::OnChangeEdtLineCalipernum4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	if ((m_nLineCaliperNum4 < 2) || (m_nLineCaliperNum4 > 100))
	{
		return;
	}
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgFind4LineParamSet::CheckDlgDataValidation()
{

	UpdateData(TRUE);
	if (m_nLineCaliperNum1 < 2)
	{
		m_nLineCaliperNum1 = 3;
	}
	if (m_nLineCaliperNum1 > 100)
	{
		m_nLineCaliperNum1 = 100;
	}

	if ((m_nLineOutLineNum1 < 0) || (m_nLineOutLineNum1 > (m_nLineCaliperNum1 - 2)))
	{
		m_nLineOutLineNum1 = 0;
	}
	if ((m_dLineContrastThre1 < 0.0) || (m_dLineContrastThre1 > 255.0))
	{
		m_dLineContrastThre1 = 10;
	}
	if (m_lLineFilterHalfWidth1 < 1)
	{
		m_lLineFilterHalfWidth1 = 1;
	}

	if (m_nLineCaliperNum2 < 2)
	{
		m_nLineCaliperNum2 = 3;
	}
	if (m_nLineCaliperNum2 > 100)
	{
		m_nLineCaliperNum2 = 100;
	}

	if ((m_nLineOutLineNum2 < 0) || (m_nLineOutLineNum2 > (m_nLineCaliperNum2 - 2)))
	{
		m_nLineOutLineNum2 = 0;
	}
	if ((m_dLineContrastThre2 < 0.0) || (m_dLineContrastThre2 > 255.0))
	{
		m_dLineContrastThre2 = 10;
	}
	if (m_lLineFilterHalfWidth2 < 1)
	{
		m_lLineFilterHalfWidth2 = 1;
	}


	if (m_nLineCaliperNum3 < 2)
	{
		m_nLineCaliperNum3 = 3;
	}
	if (m_nLineCaliperNum3 > 100)
	{
		m_nLineCaliperNum3 = 100;
	}

	if ((m_nLineOutLineNum3 < 0) || (m_nLineOutLineNum3 > (m_nLineCaliperNum3 - 2)))
	{
		m_nLineOutLineNum3 = 0;
	}
	if ((m_dLineContrastThre3 < 0.0) || (m_dLineContrastThre3 > 255.0))
	{
		m_dLineContrastThre3 = 10;
	}
	if (m_lLineFilterHalfWidth3 < 1)
	{
		m_lLineFilterHalfWidth3 = 1;
	}


	if (m_nLineCaliperNum4 < 2)
	{
		m_nLineCaliperNum4 = 3;
	}
	if (m_nLineCaliperNum4 > 100)
	{
		m_nLineCaliperNum4 = 100;
	}

	if ((m_nLineOutLineNum4 < 0) || (m_nLineOutLineNum4 > (m_nLineCaliperNum4 - 2)))
	{
		m_nLineOutLineNum4 = 0;
	}
	if ((m_dLineContrastThre4 < 0.0) || (m_dLineContrastThre4 > 255.0))
	{
		m_dLineContrastThre4 = 10;
	}
	if (m_lLineFilterHalfWidth4 < 1)
	{
		m_lLineFilterHalfWidth4 = 1;
	}

	UpdateData(FALSE);

}

void CDlgFind4LineParamSet::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_RECT_FOR_SPECIAL_PICK") == strDlgID)
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



// 更新数据
void CDlgFind4LineParamSet::UpdateControlDisplay()
{
	UpdateData(TRUE);
}

void CDlgFind4LineParamSet::UpdateTrainData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);

		//找线1
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_nOutLineNum  = m_nLineOutLineNum1;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_dContrastThre = m_dLineContrastThre1;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_lFilterHalfWidth = m_lLineFilterHalfWidth1;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_edgeProperty = (EdgePolarity)m_nLinePolarityMode1;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bSortByScore = m_bSortByScore1;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_nCaliperNum = m_nLineCaliperNum1;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bEnableCornerLineMask = m_bEnableLineMask_1;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bCaliperVisible = m_bShowCaliper_1;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bEnableDualEdge = m_bEnableDualEdge_1;
		
		//找线2
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_nOutLineNum  = m_nLineOutLineNum2;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_dContrastThre = m_dLineContrastThre2;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_lFilterHalfWidth = m_lLineFilterHalfWidth2;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_edgeProperty = (EdgePolarity)m_nLinePolarityMode2;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bSortByScore = m_bSortByScore2;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_nCaliperNum = m_nLineCaliperNum2;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bEnableCornerLineMask = m_bEnableLineMask_2;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bCaliperVisible = m_bShowCaliper_2;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bEnableDualEdge = m_bEnableDualEdge_2;

		//找线3
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_nOutLineNum  = m_nLineOutLineNum3;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_dContrastThre = m_dLineContrastThre3;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_lFilterHalfWidth = m_lLineFilterHalfWidth3;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_edgeProperty = (EdgePolarity)m_nLinePolarityMode3;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bSortByScore = m_bSortByScore3;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_nCaliperNum = m_nLineCaliperNum3;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bEnableCornerLineMask = m_bEnableLineMask_3;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bCaliperVisible = m_bShowCaliper_3;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bEnableDualEdge = m_bEnableDualEdge_3;

		//找线4
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_nOutLineNum  = m_nLineOutLineNum4;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_dContrastThre = m_dLineContrastThre4;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_lFilterHalfWidth = m_lLineFilterHalfWidth4;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_edgeProperty = (EdgePolarity)m_nLinePolarityMode4;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bSortByScore = m_bSortByScore4;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_nCaliperNum = m_nLineCaliperNum4;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bEnableCornerLineMask = m_bEnableLineMask_4;
		((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bCaliperVisible = m_bShowCaliper_4;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bEnableDualEdge = m_bEnableDualEdge_4;
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		

		//找线1
		m_nLineOutLineNum1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_nOutLineNum;
		m_dLineContrastThre1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_dContrastThre;
		m_lLineFilterHalfWidth1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_lFilterHalfWidth;
		m_nLinePolarityMode1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_edgeProperty;
		m_bSortByScore1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bSortByScore;
		m_nLineCaliperNum1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_nCaliperNum;
		m_bEnableLineMask_1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bEnableCornerLineMask;
		m_bShowCaliper_1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bCaliperVisible;
		m_bEnableDualEdge_1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bEnableDualEdge;

		m_nLineOutLineNum2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_nOutLineNum;
		m_dLineContrastThre2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_dContrastThre;
		m_lLineFilterHalfWidth2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_lFilterHalfWidth;
		m_nLinePolarityMode2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_edgeProperty;
		m_bSortByScore2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bSortByScore;
		m_nLineCaliperNum2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_nCaliperNum;
		m_bEnableLineMask_2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bEnableCornerLineMask;
		m_bShowCaliper_2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bCaliperVisible;
		m_bEnableDualEdge_2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bEnableDualEdge;

		m_nLineOutLineNum3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_nOutLineNum;
		m_dLineContrastThre3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_dContrastThre;
		m_lLineFilterHalfWidth3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_lFilterHalfWidth;
		m_nLinePolarityMode3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_edgeProperty;
		m_bSortByScore3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bSortByScore;
		m_nLineCaliperNum3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_nCaliperNum;
		m_bEnableLineMask_3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bEnableCornerLineMask;
		m_bShowCaliper_3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bCaliperVisible;
		m_bEnableDualEdge_3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bEnableDualEdge;

		m_nLineOutLineNum4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_nOutLineNum;
		m_dLineContrastThre4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_dContrastThre;
		m_lLineFilterHalfWidth4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_lFilterHalfWidth;
		m_nLinePolarityMode4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_edgeProperty;
		m_bSortByScore4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bSortByScore;
		m_nLineCaliperNum4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_nCaliperNum;
		m_bEnableLineMask_4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bEnableCornerLineMask;
		m_bShowCaliper_4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bCaliperVisible;
		m_bEnableDualEdge_4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bEnableDualEdge;
		
		m_checkSortByScore1.SetCheck(m_bSortByScore1);
		m_checkSortByScore2.SetCheck(m_bSortByScore2);
		m_checkSortByScore3.SetCheck(m_bSortByScore3);
		m_checkSortByScore4.SetCheck(m_bSortByScore4);

		UpdateData(FALSE);
	}

}

void CDlgFind4LineParamSet::UpdateSearchData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);


		((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[0] = m_bShowFitPoint_1;
		((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[1]  = m_bShowFitPoint_2;
		((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[2]  = m_bShowFitPoint_3;
		((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[3]  = m_bShowFitPoint_4;

		//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode = m_nResultOutMode;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;

	}
	else // 设置界面值
	{

		m_bShowFitPoint_1 = ((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[0];
		m_bShowFitPoint_2 = ((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[1];
		m_bShowFitPoint_3 = ((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[2];
		m_bShowFitPoint_4 = ((CSpecialPickSearchSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bFindLineVisibleFitPoint[3];
		//m_nResultOutMode = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode;
		UpdateData(FALSE);
	}

}

void CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper1()
{
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper2()
{
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper3()
{
	UpdateTrainData(true);
	UpdateSearchData(true);
}


void CDlgFind4LineParamSet::OnBnClickedChkShowLineCaliper4()
{
	UpdateTrainData(true);
	UpdateSearchData(true);
}

void CDlgFind4LineParamSet::OnEnSetfocusEditLineFilterHalfWidth1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_1, strOutput);
		}
	}

}

void CDlgFind4LineParamSet::OnEnSetfocusEditLineFilterHalfWidth2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_2, strOutput);
		}
	}

}

void CDlgFind4LineParamSet::OnEnSetfocusEditLineFilterHalfWidth3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_3, strOutput);
		}
	}

}

void CDlgFind4LineParamSet::OnEnSetfocusEditLineFilterHalfWidth4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_LINE_FILTER_HALFWIDTH_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_LINE_FILTER_HALFWIDTH_4, strOutput);
		}
	}

}