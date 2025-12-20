// DlgAcfAlignSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAcfAlignSet.h"
#include "afxdialogex.h"


// CDlgAcfAlignSet 对话框

IMPLEMENT_DYNAMIC(CDlgAcfAlignSet, CDialogEx)

CDlgAcfAlignSet::CDlgAcfAlignSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAcfAlignSet::IDD, pParent)
	, m_dCurMinMean(0)
	, m_dCurMaxMean(255)
	, m_dCurMinStdDev(0)
	, m_dCurMaxStdDev(255)
	, m_bMeanInvalid(FALSE)
	, m_bMeanInvert(FALSE)
	, m_bStdDevInvalid(FALSE)
	, m_bStdDevInvert(FALSE)
	, m_nCurLogicIndex(0)
	, m_nInpectTool(FALSE)
{

}

CDlgAcfAlignSet::~CDlgAcfAlignSet()
{
}

void CDlgAcfAlignSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_MIN_MEAN, m_dCurMinMean);
	DDX_Text(pDX, IDC_EDT_MAX_MEAN, m_dCurMaxMean);
	DDX_Text(pDX, IDC_EDT_MIN_STDDEV, m_dCurMinStdDev);
	DDX_Text(pDX, IDC_EDT_MAX_STDDEV, m_dCurMaxStdDev);
	DDX_Check(pDX, IDC_CHK_MEAN_NOTINVALID, m_bMeanInvalid);
	DDX_Check(pDX, IDC_CHK_MEAN_INVERT, m_bMeanInvert);
	DDX_Check(pDX, IDC_CHK_STDDEV_NOTINVALID, m_bStdDevInvalid);
	DDX_Check(pDX, IDC_CHK_STDDEV_INVERT, m_bStdDevInvert);
	DDX_Radio(pDX, IDC_RADIO_LOGIC_AND, m_nCurLogicIndex);
	DDX_Check(pDX, IDC_CHK_INPECT_Tool_NOTINVALID, m_nInpectTool);
}


BEGIN_MESSAGE_MAP(CDlgAcfAlignSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAcfAlignSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAcfAlignSet::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgAcfAlignSet 消息处理程序


void CDlgAcfAlignSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgAcfAlignSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
