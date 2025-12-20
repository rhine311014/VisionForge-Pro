// DlgLightCtrlRename.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLightCtrlRename.h"
#include "afxdialogex.h"
#include "SystemOptionConfig.h"

// CDlgLightCtrlRename 对话框

IMPLEMENT_DYNAMIC(CDlgLightCtrlRename, CDialogEx)

CDlgLightCtrlRename::CDlgLightCtrlRename(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLightCtrlRename::IDD, pParent)
	, m_strLightCtrlName1(_T(""))
	, m_strLightCtrlName2(_T(""))
	, m_strLightCtrlName3(_T(""))
	, m_strLightCtrlName4(_T(""))
	, m_strLightCtrlName5(_T(""))
	, m_strLightCtrlName6(_T(""))
	, m_strLightCtrlName7(_T(""))
	, m_strLightCtrlName8(_T(""))
{

}

CDlgLightCtrlRename::~CDlgLightCtrlRename()
{

}

void CDlgLightCtrlRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME1, m_strLightCtrlName1);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME2, m_strLightCtrlName2);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME3, m_strLightCtrlName3);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME4, m_strLightCtrlName4);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME5, m_strLightCtrlName5);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME6, m_strLightCtrlName6);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME7, m_strLightCtrlName7);
	DDX_Text(pDX, IDC_EDIT_LIGHT_CONTROL_NAME8, m_strLightCtrlName8);
}


BEGIN_MESSAGE_MAP(CDlgLightCtrlRename, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLightCtrlRename::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLightCtrlRename 消息处理程序


void CDlgLightCtrlRename::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CDlgLightCtrlRename::GetSystemOptionLightName(SystemOptionLightConfig& systemOptionLightConfig)
{
	systemOptionLightConfig.m_strLightCtrlName[0] = m_strLightCtrlName1;
	systemOptionLightConfig.m_strLightCtrlName[1] = m_strLightCtrlName2;
	systemOptionLightConfig.m_strLightCtrlName[2] = m_strLightCtrlName3;
	systemOptionLightConfig.m_strLightCtrlName[3] = m_strLightCtrlName4;
	systemOptionLightConfig.m_strLightCtrlName[4] = m_strLightCtrlName5;
	systemOptionLightConfig.m_strLightCtrlName[5] = m_strLightCtrlName6;
	systemOptionLightConfig.m_strLightCtrlName[6] = m_strLightCtrlName7;
	systemOptionLightConfig.m_strLightCtrlName[7] = m_strLightCtrlName8;
}

void CDlgLightCtrlRename::SetSystemOptionLightName(SystemOptionLightConfig& systemOptionLightConfig)
{
	m_strLightCtrlName1 = systemOptionLightConfig.m_strLightCtrlName[0];
	m_strLightCtrlName2 = systemOptionLightConfig.m_strLightCtrlName[1];
	m_strLightCtrlName3 = systemOptionLightConfig.m_strLightCtrlName[2];
	m_strLightCtrlName4 = systemOptionLightConfig.m_strLightCtrlName[3];
	m_strLightCtrlName5 = systemOptionLightConfig.m_strLightCtrlName[4];
	m_strLightCtrlName6 = systemOptionLightConfig.m_strLightCtrlName[5];
	m_strLightCtrlName7 = systemOptionLightConfig.m_strLightCtrlName[6];
	m_strLightCtrlName8 = systemOptionLightConfig.m_strLightCtrlName[7];
}
