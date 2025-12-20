// DlgCalibCopy.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibCopy.h"
#include "afxdialogex.h"

// CDlgCalibCopy 对话框

IMPLEMENT_DYNAMIC(CDlgCalibCopy, CDialogEx)

CDlgCalibCopy::CDlgCalibCopy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibCopy::IDD, pParent)
	, m_nSourcePosCalib(1)
	, m_nTargetPosCalib(2)
	, m_bEnableALLCalibCopy(TRUE)
{
	m_bSysKeyboardEnabled = false;
}

CDlgCalibCopy::~CDlgCalibCopy()
{
}

void CDlgCalibCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURCE_POS_CALIB, m_nSourcePosCalib);
	DDX_Text(pDX, IDC_EDIT_TARGET_POS_CALIB, m_nTargetPosCalib);
	DDX_Check(pDX, IDC_CHECK_ALL_CALIB_COPY, m_bEnableALLCalibCopy);
}


BEGIN_MESSAGE_MAP(CDlgCalibCopy, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCalibCopy::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT_SOURCE_POS_CALIB, &CDlgCalibCopy::OnEnSetfocusEditSourcePosCalib)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_POS_CALIB, &CDlgCalibCopy::OnEnSetfocusEditTargetPosCalib)
	ON_BN_CLICKED(IDC_CHECK_ALL_CALIB_COPY, &CDlgCalibCopy::OnBnClickedCheckAllCalibCopy)
END_MESSAGE_MAP()


// CDlgCalibCopy 消息处理程序


void CDlgCalibCopy::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgCalibCopy::OnEnSetfocusEditSourcePosCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SOURCE_POS_CALIB)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SOURCE_POS_CALIB, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SOURCE_POS_CALIB, strOutput);
		}
	}
}


void CDlgCalibCopy::OnEnSetfocusEditTargetPosCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_POS_CALIB)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_POS_CALIB, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_POS_CALIB, strOutput);
		}
	}
}


void CDlgCalibCopy::OnBnClickedCheckAllCalibCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableALLCalibCopy = !m_bEnableALLCalibCopy;

	if (m_bEnableALLCalibCopy)
	{
		SetDlgItemText(IDC_STATIC_SOURCE,_T("源扩展"));
		SetDlgItemText(IDC_STATIC_TARGET,_T("目标扩展"));
	} 
	else
	{
		SetDlgItemText(IDC_STATIC_SOURCE,_T("源位置"));
		SetDlgItemText(IDC_STATIC_TARGET,_T("目标位置"));
	}
}


BOOL CDlgCalibCopy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bEnableALLCalibCopy)
	{
		SetDlgItemText(IDC_STATIC_SOURCE,_T("源扩展"));
		SetDlgItemText(IDC_STATIC_TARGET,_T("目标扩展"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
