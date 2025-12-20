// DlgSearchToolCopyPos.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSearchToolCopyPos.h"
#include "afxdialogex.h"


// CDlgSearchToolCopyPos 对话框

IMPLEMENT_DYNAMIC(CDlgSearchToolCopyPos, CDialog)

CDlgSearchToolCopyPos::CDlgSearchToolCopyPos(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchToolCopyPos::IDD, pParent)
{
	m_bSysKeyboardEnabled = FALSE;
	m_psaSysInfoStrings = NULL;
	m_nPosNum = MAXPOSNUM;
	m_vbCopyPos.resize(MAXPOSNUM, FALSE);
	m_nPosCNum = 1;
	m_nCandidateIndex = 0;
	m_nSrcCandidateIndex = 0;
}

CDlgSearchToolCopyPos::~CDlgSearchToolCopyPos()
{
}

void CDlgSearchToolCopyPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_POS1, m_checkPos1);
	DDX_Control(pDX, IDC_CHECK_POS2, m_checkPos2);
	DDX_Control(pDX, IDC_CHECK_POS3, m_checkPos3);
	DDX_Control(pDX, IDC_CHECK_POS4, m_checkPos4);
	DDX_Control(pDX, IDC_CHECK_POS5, m_checkPos5);
	DDX_Control(pDX, IDC_CHECK_POS6, m_checkPos6);
	DDX_Control(pDX, IDC_CHECK_POS7, m_checkPos7);
	DDX_Control(pDX, IDC_CHECK_POS8, m_checkPos8);
	DDX_Control(pDX, IDC_COMBO_CANDIDATE_INDEX, m_comboCandidate);
}


BEGIN_MESSAGE_MAP(CDlgSearchToolCopyPos, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSearchToolCopyPos::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_POS1, &CDlgSearchToolCopyPos::OnBnClickedCheckPos1)
	ON_BN_CLICKED(IDC_CHECK_POS2, &CDlgSearchToolCopyPos::OnBnClickedCheckPos2)
	ON_BN_CLICKED(IDC_CHECK_POS3, &CDlgSearchToolCopyPos::OnBnClickedCheckPos3)
	ON_BN_CLICKED(IDC_CHECK_POS4, &CDlgSearchToolCopyPos::OnBnClickedCheckPos4)
	ON_BN_CLICKED(IDC_CHECK_POS5, &CDlgSearchToolCopyPos::OnBnClickedCheckPos5)
	ON_BN_CLICKED(IDC_CHECK_POS6, &CDlgSearchToolCopyPos::OnBnClickedCheckPos6)
	ON_BN_CLICKED(IDC_CHECK_POS7, &CDlgSearchToolCopyPos::OnBnClickedCheckPos7)
	ON_BN_CLICKED(IDC_CHECK_POS8, &CDlgSearchToolCopyPos::OnBnClickedCheckPos8)
	ON_BN_CLICKED(IDCANCEL, &CDlgSearchToolCopyPos::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_CANDIDATE_INDEX, &CDlgSearchToolCopyPos::OnCbnSelchangeComboCandidateIndex)
END_MESSAGE_MAP()


// CDlgSearchToolCopyPos 消息处理程序


BOOL CDlgSearchToolCopyPos::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_vpCheckPos.push_back(&m_checkPos1);
	m_vpCheckPos.push_back(&m_checkPos2);
	m_vpCheckPos.push_back(&m_checkPos3);
	m_vpCheckPos.push_back(&m_checkPos4);
	m_vpCheckPos.push_back(&m_checkPos5);
	m_vpCheckPos.push_back(&m_checkPos6);
	m_vpCheckPos.push_back(&m_checkPos7);
	m_vpCheckPos.push_back(&m_checkPos8);

	int i = 0;
	for (i=0; i<MAXPOSNUM; i++)
	{
		if (i < m_nPosNum)
		{	
			m_vpCheckPos.at(i)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_vpCheckPos.at(i)->ShowWindow(SW_HIDE);
		}
	}

	if (m_nPosCNum <= 1)
	{
		m_nPosCNum = 1;
		GetDlgItem(IDC_STATIC_CANDIDATE_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CANDIDATE_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CANDIDATE_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CANDIDATE_INDEX)->EnableWindow(FALSE);
	}
	if (m_nSrcCandidateIndex < 0)
	{
		m_nSrcCandidateIndex = 0;
	}

	std::vector<CString> vstrCandidate;
	vstrCandidate.push_back(GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_COPY_POS_CANDIDATE_0));//_T("标准"));
	vstrCandidate.push_back(GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_COPY_POS_CANDIDATE_1));//_T("候选1"));
	vstrCandidate.push_back(GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_COPY_POS_CANDIDATE_2));//_T("候选2"));
	vstrCandidate.push_back(GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_COPY_POS_CANDIDATE_3));//_T("候选3"));
	vstrCandidate.push_back(GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_COPY_POS_CANDIDATE_4));//_T("候选4"));
	for (i=0; i<m_nPosCNum && i<vstrCandidate.size(); i++)
	{
		m_comboCandidate.AddString(vstrCandidate[i]);
	}
	m_comboCandidate.SetCurSel(min(m_nSrcCandidateIndex,m_nPosCNum-1));
	m_nCandidateIndex = m_comboCandidate.GetCurSel();

	UpdateDialogLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSearchToolCopyPos::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i = 0;
	for (i=0; i<MAXPOSNUM && i<m_vpCheckPos.size(); i++)
	{
		if (m_vpCheckPos.at(i)->GetCheck() == BST_CHECKED)
		{
			m_vbCopyPos.at(i) = TRUE;
		}
		else
		{
			m_vbCopyPos.at(i) = FALSE;
		}
	}

	m_nCandidateIndex = m_comboCandidate.GetCurSel();

	CString strLog;
	strLog.Format(_T("[确定]退出，复制完成"));
	g_opLogFile.SaveLogText(strLog,4);

	CDialog::OnOK();
}

void CDlgSearchToolCopyPos::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_SEARCH_TOOL_COPY_POS") == strDlgID)
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

void CDlgSearchToolCopyPos::OnBnClickedCheckPos1()
{
	// TODO: 在此添加控件通知处理程序代码m_checkPos1
	CString strLog;
	CString strValue = m_checkPos1.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置1]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos2.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置2]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos3.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置3]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos4.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置4]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos5.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置5]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos6.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置6]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos7.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置7]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCheckPos8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	CString strValue = m_checkPos8.GetCheck() == BST_CHECKED ? _T("选中状态"):_T("未选中状态");
	strLog.Format(_T("修改[位置8]为%s\n"),strValue);
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgSearchToolCopyPos::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[取消]退出，未复制"));
	g_opLogFile.SaveLogText(strLog,4);
	CDialog::OnCancel();
}


void CDlgSearchToolCopyPos::OnCbnSelchangeComboCandidateIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strOld, strNew;

	int nComboOld = m_nCandidateIndex;
	m_comboCandidate.GetLBText(nComboOld,strOld);

	m_nCandidateIndex = m_comboCandidate.GetCurSel();
	m_comboCandidate.GetLBText(m_nCandidateIndex,strNew);

	if (nComboOld != m_comboCandidate.GetCurSel())
	{
		CString strLog;

		strLog.Format(_T("修改[候选设置]由%s到%s\n"),strOld,strNew);

		g_opLogFile.SaveLogText(strLog,5);
	}
}
