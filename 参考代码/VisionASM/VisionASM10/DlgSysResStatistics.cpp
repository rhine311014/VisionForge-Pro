// DlgSysResStatistics.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysResStatistics.h"
#include "afxdialogex.h"


// CDlgSysResStatistics 对话框

IMPLEMENT_DYNAMIC(CDlgSysResStatistics, CDialogEx)

CDlgSysResStatistics::CDlgSysResStatistics(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysResStatistics::IDD, pParent)
	, m_bGetSysResourceUse(TRUE)
	, m_bThreValid(TRUE)
{
	m_pParent = NULL;
	m_pSysStatusInfo = NULL;
	m_bGetSysResourceUse = TRUE;
	m_dMaxCpuUseThre = 0;
	m_dMaxDiskUseThre = 0;
	m_dMaxMemUseThre = 0;
	m_nMaxHandleUseThre = 0;
	m_nMaxGDIUseThre = 0;
	m_bExitCacuThread = FALSE;
	m_pCacuThread = NULL;
}

CDlgSysResStatistics::~CDlgSysResStatistics()
{
}

void CDlgSysResStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CPU_USED_RATE_VAL, m_labelCpuUse);
	DDX_Control(pDX, IDC_STATIC_MEM_USED_RATE_VAL, m_labelMemUse);
	/*DDX_Control(pDX, IDC_STATIC_DISK_USED_RATE_VAL, m_labelDiskUse);*/
	DDX_Check(pDX, IDC_CHECK_GET_SYS_RESOURCE_USE, m_bGetSysResourceUse);
	DDX_Control(pDX, IDC_STATIC_HANDLE_USED_COUNT_VAL, m_labelHandleUse);
	DDX_Control(pDX, IDC_STATIC_GDI_USED_COUNT_VAL, m_labelGDIUse);

	DDX_Control(pDX, IDC_COMBO_DISK_USE, m_cmbDiskUse);
	DDX_Check(pDX, IDC_CHK_THRE_VALID, m_bThreValid);
}


BEGIN_MESSAGE_MAP(CDlgSysResStatistics, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_GET_SYS_RESOURCE_USE, &CDlgSysResStatistics::OnBnClickedCheckGetSysResourceUse)
	ON_EN_SETFOCUS(IDC_EDIT_CPU_USE_THRE, &CDlgSysResStatistics::OnEnSetfocusEditCpuUseThre)
	ON_EN_KILLFOCUS(IDC_EDIT_CPU_USE_THRE, &CDlgSysResStatistics::OnEnKillfocusEditCpuUseThre)
	ON_EN_SETFOCUS(IDC_EDIT_MEM_USE_THRE, &CDlgSysResStatistics::OnEnSetfocusEditMemUseThre)
	ON_EN_KILLFOCUS(IDC_EDIT_MEM_USE_THRE, &CDlgSysResStatistics::OnEnKillfocusEditMemUseThre)
	ON_EN_SETFOCUS(IDC_EDIT_DISK_USE_THRE, &CDlgSysResStatistics::OnEnSetfocusEditDiskUseThre)
	ON_EN_KILLFOCUS(IDC_EDIT_DISK_USE_THRE, &CDlgSysResStatistics::OnEnKillfocusEditDiskUseThre)
	ON_EN_SETFOCUS(IDC_EDIT_HANDLE_USE_THRE, &CDlgSysResStatistics::OnEnSetfocusEditHandleUseThre)
	ON_EN_KILLFOCUS(IDC_EDIT_HANDLE_USE_THRE, &CDlgSysResStatistics::OnEnKillfocusEditHandleUseThre)
	ON_EN_SETFOCUS(IDC_EDIT_GDI_USE_THRE, &CDlgSysResStatistics::OnEnSetfocusEditGdiUseThre)
	ON_EN_KILLFOCUS(IDC_EDIT_GDI_USE_THRE, &CDlgSysResStatistics::OnEnKillfocusEditGdiUseThre)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHK_THRE_VALID, &CDlgSysResStatistics::OnBnClickedChkThreValid)
END_MESSAGE_MAP()

void CDlgSysResStatistics::SetParent(CDlgSysOptionSetting* pParent)
{
	if (pParent!=NULL)
	{
		m_pParent = pParent;
		m_pSysStatusInfo = &(m_pParent->m_pParent->m_SystemStatusInfo);
	}
}

// CDlgSysResStatistics 消息处理程序
BOOL CDlgSysResStatistics::IsStringNumerical( LPCTSTR lpszVal )
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
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgSysResStatistics::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_RES_STATISTICS") == strDlgID)
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

void CDlgSysResStatistics::InitDlgItem()
{
	UpdateDlgData(FALSE);
	//if (!m_pParent->m_TempSysOptionConfig.m_bGetSysResourceUse)
	//{
	//	m_bExitCacuThread = FALSE;
	//	m_pCacuThread = AfxBeginThread(CacuFunc,this);
	//}
	Sleep(500);
	UpdateSystemResourceInfoText();
	SetTimer(1,1000,NULL);
	

}

void CDlgSysResStatistics::UpdateDlgData(BOOL bSaveAndValidate/* = TRUE*/)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		m_pParent->m_SysOptionConfig.m_bGetSysResourceUse = m_bGetSysResourceUse;
		m_pParent->m_SysOptionConfig.m_bThreValid = m_bThreValid;
		m_pParent->m_SysOptionConfig.m_dMaxCpuUseThre = m_dMaxCpuUseThre;
		m_pParent->m_SysOptionConfig.m_dMaxDiskUseThre = m_dMaxDiskUseThre;
		m_pParent->m_SysOptionConfig.m_dMaxMemUseThre = m_dMaxMemUseThre;
		m_pParent->m_SysOptionConfig.m_nMaxHandleUseThre = m_nMaxHandleUseThre;
		m_pParent->m_SysOptionConfig.m_nMaxGDIUseThre = m_nMaxGDIUseThre;
	}
	else
	{

		m_bGetSysResourceUse = m_pParent->m_SysOptionConfig.m_bGetSysResourceUse;
		m_bThreValid = m_pParent->m_SysOptionConfig.m_bThreValid;
		m_dMaxCpuUseThre = m_pParent->m_SysOptionConfig.m_dMaxCpuUseThre;
		m_dMaxDiskUseThre = m_pParent->m_SysOptionConfig.m_dMaxDiskUseThre;
		m_dMaxMemUseThre = m_pParent->m_SysOptionConfig.m_dMaxMemUseThre;
		m_nMaxHandleUseThre = m_pParent->m_SysOptionConfig.m_nMaxHandleUseThre;
		m_nMaxGDIUseThre = m_pParent->m_SysOptionConfig.m_nMaxGDIUseThre;

		CString strMaxCpuUseThre,strMaxDiskUseThre,strMaxMemUseThre,strMaxHandleUseThre,strMaxGDIUseThre;
		strMaxCpuUseThre.Format(_T("%.2f"),m_dMaxCpuUseThre*100);
		strMaxDiskUseThre.Format(_T("%.2f"),m_dMaxDiskUseThre*100);
		strMaxMemUseThre.Format(_T("%.2f"),m_dMaxMemUseThre*100);
		strMaxHandleUseThre.Format(_T("%d"),m_nMaxHandleUseThre);
		strMaxGDIUseThre.Format(_T("%d"),m_nMaxGDIUseThre);
		GetDlgItem(IDC_EDIT_DISK_USE_THRE)->SetWindowText(strMaxDiskUseThre);
		GetDlgItem(IDC_EDIT_CPU_USE_THRE)->SetWindowText(strMaxCpuUseThre);
		GetDlgItem(IDC_EDIT_MEM_USE_THRE)->SetWindowText(strMaxMemUseThre);
		GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->SetWindowText(strMaxHandleUseThre);
		GetDlgItem(IDC_EDIT_GDI_USE_THRE)->SetWindowText(strMaxGDIUseThre);
		/*if (m_bGetSysResourceUse)*/
		if (m_bThreValid)
		{
			/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(TRUE);*/
			GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(TRUE);

			//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(TRUE);
			///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(TRUE);*/
			//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(TRUE);

			/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(TRUE);*/
			GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(TRUE);

			/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(TRUE);*/
			GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(TRUE);

			/*GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(TRUE);*/
			GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(TRUE);
			
		}
		else
		{
			/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(FALSE);*/
			GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(FALSE);

			//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(FALSE);
			///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(FALSE);*/
			//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(FALSE);

			/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(FALSE);*/
			GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(FALSE);

			/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(FALSE);*/
			GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(FALSE);

			//GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(FALSE);
			//GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(FALSE);
		}
		UpdateData(FALSE);
	}
}

BOOL CDlgSysResStatistics::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateDialogLanguage();
	InitDlgItem();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSysResStatistics::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CDlgSysResStatistics::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
	UpdateDlgData(TRUE);
}


void CDlgSysResStatistics::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(1);
	if (m_pCacuThread)
	{

		m_bExitCacuThread = TRUE;
		if(WaitForSingleObject(m_pCacuThread->m_hThread,5000)==WAIT_TIMEOUT)
		{
			TerminateThread(m_pCacuThread->m_hThread,0);
		}
		m_pCacuThread = NULL;
	}
	// TODO: 在此处添加消息处理程序代码
}


void CDlgSysResStatistics::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateSystemResourceInfoText();
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgSysResStatistics::UpdateSystemResourceInfoText()
{
	if(m_pSysStatusInfo == NULL)
	{
		return;
	}
	
	//if (!m_bGetSysResourceUse)
	//{
	//	GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(FALSE);
	//	/*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(FALSE);*/
	//	GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(FALSE);

	//	GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(FALSE);

	//	return;
	//}

	//if (m_pParent->m_TempSysOptionConfig.m_bGetSysResourceUse == FALSE)
	//{
	//	m_pSysStatusInfo->CacuDriveName();
	//	m_pSysStatusInfo->CacuDriveUsedInfo();
	//	m_pSysStatusInfo->CacuCpuUsedInfo();
	//	m_pSysStatusInfo->CacuMemoryUsedInfo();
	//	m_pSysStatusInfo->CacuGDIUsedCount();
	//	m_pSysStatusInfo->CacuHandleUsedCount();
	//	//return;
	//}

	BOOL bWarningCpu = FALSE;
	BOOL bWarningMem = FALSE;
	BOOL bWarningDisk = FALSE;
	BOOL bWarningHandle = FALSE;
	BOOL bWarningGDI = FALSE;
	CString strSysInfoCpu = _T("");
	CString strSysInfoMem = _T("");
	CString strSysInfoDisk = _T("");
	CString strSysInfoHandle = _T("");
	CString strSysInfoGDI = _T("");

	CString strTmp = _T("");

	double SysMemPercent = m_pSysStatusInfo->GetSysMemoryUsedRate();
	strSysInfoMem.Format(_T("%.0f%%"),SysMemPercent*100);
	if (m_bThreValid && SysMemPercent > m_dMaxMemUseThre)
	{
		bWarningMem = TRUE;

	}
	double CpuPercent = m_pSysStatusInfo->GetCpuUsedRate();
	strSysInfoCpu.Format(_T("%.0f%%"),CpuPercent*100);
	if (m_bThreValid && CpuPercent>m_dMaxCpuUseThre)
	{
		bWarningCpu = TRUE;
	}

	double DiskPercent = 0.0;
	CString strDiskWarning;
	m_cmbDiskUse.ResetContent();
	std::vector<CString> DiskName = m_pSysStatusInfo->GetDiskName();
	CString strDiskName;
	CString str;
	for (int i=0;i<DiskName.size();i++)
	{
		str = DiskName.at(i)+_T(" ");
		strDiskName+=str;
	}
	m_cmbDiskUse.AddString(strDiskName,RGB(255,255,255));

	for (int i=0;i<DiskName.size()/*&&i<3*/;i++)
	{
		m_pSysStatusInfo->GetDiskUsedRate(DiskName.at(i),DiskPercent);
		if (m_bThreValid && DiskPercent>m_dMaxDiskUseThre)
		{
			bWarningDisk = TRUE;
		}
		else
		{
			bWarningDisk = FALSE;
		}
		strTmp.Format(_T("%s%.0f%%  "),DiskName.at(i).Left(DiskName.at(i).Find('\\')),DiskPercent*100);
		/*strSysInfoDisk = strSysInfoDisk + strTmp;*/
		if (bWarningDisk)
		{
			m_cmbDiskUse.AddString(strTmp,RGB(255,0,0));
		}
		else
		{
			m_cmbDiskUse.AddString(strTmp,RGB(0,255,0));
		}	

	}
	m_cmbDiskUse.SetCurSel(0);
	//if (strSysInfoDisk == _T(""))
	//{
	//	strSysInfoDisk = _T("0%");
	//}

	int nHandleCount = m_pSysStatusInfo->GetHandleUsedCount();
	strSysInfoHandle.Format(_T("%d"),nHandleCount);
	if (m_bThreValid && nHandleCount > m_nMaxHandleUseThre)
	{
		bWarningHandle = TRUE;
	}

	int nGDICount = m_pSysStatusInfo->GetGDIUsedCount();
	strSysInfoGDI.Format(_T("%d"),nGDICount);
	if (m_bThreValid && nGDICount > m_nMaxGDIUseThre)
	{
		bWarningGDI = TRUE;
	}

	m_labelCpuUse.SetRedraw(FALSE);
	m_labelCpuUse.SetWindowText(strSysInfoCpu);
	m_labelCpuUse.SetRedraw(TRUE);
	if (bWarningCpu)
	{
		m_labelCpuUse.SetBkColor(RGB(255,0,0));
	}
	else
	{
		m_labelCpuUse.SetBkColor(RGB(0,255,0));
	}
	/*m_labelCpuUse.SetFontBold(TRUE);
	m_labelCpuUse.SetFontSize(10);*/

	m_labelMemUse.SetRedraw(FALSE);
	m_labelMemUse.SetWindowText(strSysInfoMem);
	m_labelMemUse.SetRedraw(TRUE);
	if (bWarningMem)
	{
		m_labelMemUse.SetBkColor(RGB(255,0,0));

	}
	else
	{
		m_labelMemUse.SetBkColor(RGB(0,255,0));
	}
	/*m_labelMemUse.SetFontBold(TRUE);
	m_labelMemUse.SetFontSize(10);*/

	//m_labelDiskUse.SetRedraw(FALSE);
	//m_labelDiskUse.SetWindowText(strSysInfoDisk);
	//m_labelDiskUse.SetRedraw(TRUE);
	//if (bWarningDisk)
	//{
	//	m_labelDiskUse.SetBkColor(RGB(255,0,0));
	//}	
	//else
	//{
	//	m_labelDiskUse.SetBkColor(RGB(0,255,0));
	//}
	/*m_labelDiskUse.SetFontBold(TRUE);
	m_labelDiskUse.SetFontSize(10);*/

	m_labelHandleUse.SetRedraw(FALSE);
	m_labelHandleUse.SetWindowText(strSysInfoHandle);
	m_labelHandleUse.SetRedraw(TRUE);
	if (bWarningHandle)
	{
		m_labelHandleUse.SetBkColor(RGB(255,0,0));
	}	
	else
	{
		m_labelHandleUse.SetBkColor(RGB(0,255,0));
	}

	m_labelGDIUse.SetRedraw(FALSE);
	m_labelGDIUse.SetWindowText(strSysInfoGDI);
	m_labelGDIUse.SetRedraw(TRUE);
	if (bWarningGDI)
	{
		m_labelGDIUse.SetBkColor(RGB(255,0,0));
	}	
	else
	{
		m_labelGDIUse.SetBkColor(RGB(0,255,0));
	}

}



void CDlgSysResStatistics::OnBnClickedCheckGetSysResourceUse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//if (m_bGetSysResourceUse)
	//{
	//	/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(TRUE);*/
	//	GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(TRUE);

	//	//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(TRUE);
	//	///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(TRUE);*/
	//	//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(TRUE);

	//	/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(TRUE);*/
	//	GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(TRUE);

	//	/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(TRUE);*/
	//	GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(TRUE);

	//	/*GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(TRUE);*/
	//	GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(TRUE);

	//}
	//else
	//{
	//	/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(FALSE);*/
	//	GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(FALSE);

	//	//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(FALSE);
	//	///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(FALSE);*/
	//	//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(FALSE);

	//	/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(FALSE);*/
	//	GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(FALSE);

	//	/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(FALSE);*/
	//	GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(FALSE);

	//	/*GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(FALSE);*/
	//	GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(FALSE);
	//}
}


void CDlgSysResStatistics::OnEnSetfocusEditCpuUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CPU_USE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CPU_USE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CPU_USE_THRE, strOutput);
			//if (strInput != strOutput)
			//{
			//	CString strLog;
			//	strLog.Format(_T("修改[CPU占用率警告阈值]由%s到%s\n"),strInput,strOutput);
			//	g_opLogFile.SaveLogText(strLog,3);
			//}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCpuUseThre();
	}	
}


void CDlgSysResStatistics::OnEnKillfocusEditCpuUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CPU_USE_THRE, strVal);
		if (!IsStringNumerical(strVal))
		{
			CString strMaxCpuUseThre;
			strMaxCpuUseThre.Format(_T("%.2f"), m_dMaxCpuUseThre*100);
			SetDlgItemText(IDC_EDIT_CPU_USE_THRE, strMaxCpuUseThre);
		}
		else
		{
			CString strMaxCpuUseThre;
			strMaxCpuUseThre.Format(_T("%.2f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CPU_USE_THRE, strMaxCpuUseThre);
		}
	}

	CString strMaxCpuUseThre;
	GetDlgItemText(IDC_EDIT_CPU_USE_THRE,strMaxCpuUseThre);
	if (atof(strMaxCpuUseThre)/100 < 0||atof(strMaxCpuUseThre)/100 > m_pParent->m_SysOptionConfig.m_dCpuThreMaxValue)
	{
		strMaxCpuUseThre.Format(_T("%.2f"), m_dMaxCpuUseThre*100);
		SetDlgItemText(IDC_EDIT_CPU_USE_THRE, strMaxCpuUseThre);
		CString strInfo;
		strInfo.Format(_T("超过阈值范围：%.2f-%.2f\n请重新设置"),0.00,m_pParent->m_SysOptionConfig.m_dCpuThreMaxValue*100);
		MessageBox(strInfo,_T("提示"));
	}
	m_dMaxCpuUseThre = atof(strMaxCpuUseThre)/100;
}


void CDlgSysResStatistics::OnEnSetfocusEditMemUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MEM_USE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MEM_USE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MEM_USE_THRE, strOutput);
			//if (strInput != strOutput)
			//{
			//	CString strLog;
			//	strLog.Format(_T("修改[内存占用率警告阈值]由%s到%s\n"),strInput,strOutput);
			//	g_opLogFile.SaveLogText(strLog,3);
			//}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditMemUseThre();
	}	
}


void CDlgSysResStatistics::OnEnKillfocusEditMemUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_MEM_USE_THRE, strVal);
		if (!IsStringNumerical(strVal))
		{
			CString strMaxMemUseThre;
			strMaxMemUseThre.Format(_T("%.2f"), m_dMaxMemUseThre*100);
			SetDlgItemText(IDC_EDIT_MEM_USE_THRE, strMaxMemUseThre);
		}
		else
		{
			CString strMaxMemUseThre;
			strMaxMemUseThre.Format(_T("%.2f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_MEM_USE_THRE, strMaxMemUseThre);
		}
	}
	CString strMaxMemUseThre;
	GetDlgItemText(IDC_EDIT_MEM_USE_THRE,strMaxMemUseThre);
	if (atof(strMaxMemUseThre)/100 < 0||atof(strMaxMemUseThre)/100 > m_pParent->m_SysOptionConfig.m_dMemThreMaxValue)
	{
		strMaxMemUseThre.Format(_T("%.2f"), m_dMaxMemUseThre*100);
		SetDlgItemText(IDC_EDIT_MEM_USE_THRE, strMaxMemUseThre);
		CString strInfo;
		strInfo.Format(_T("超过阈值范围：%.2f-%.2f\n请重新设置"),0.00,m_pParent->m_SysOptionConfig.m_dMemThreMaxValue*100);
		MessageBox(strInfo,_T("提示"));
	}
	m_dMaxMemUseThre = atof(strMaxMemUseThre)/100;
}


void CDlgSysResStatistics::OnEnSetfocusEditDiskUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISK_USE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISK_USE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISK_USE_THRE, strOutput);
			//if (strInput != strOutput)
			//{
			//	CString strLog;
			//	strLog.Format(_T("修改[磁盘占用率警告阈值]由%s到%s\n"),strInput,strOutput);
			//	g_opLogFile.SaveLogText(strLog,3);
			//}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDiskUseThre();
	}	
}


void CDlgSysResStatistics::OnEnKillfocusEditDiskUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_DISK_USE_THRE, strVal);
		if (!IsStringNumerical(strVal))
		{
			CString strMaxDiskUseThre;
			strMaxDiskUseThre.Format(_T("%.2f"), m_dMaxDiskUseThre*100);
			SetDlgItemText(IDC_EDIT_DISK_USE_THRE, strMaxDiskUseThre);
		}
		else
		{
			CString strMaxDiskUseThre;
			strMaxDiskUseThre.Format(_T("%.2f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_DISK_USE_THRE, strMaxDiskUseThre);
		}
	}
	CString strMaxDiskUseThre;
	GetDlgItemText(IDC_EDIT_DISK_USE_THRE,strMaxDiskUseThre);
	if (atof(strMaxDiskUseThre)/100 < 0||atof(strMaxDiskUseThre)/100 > m_pParent->m_SysOptionConfig.m_dDiskThreMaxValue)
	{
		strMaxDiskUseThre.Format(_T("%.2f"), m_dMaxDiskUseThre*100);
		SetDlgItemText(IDC_EDIT_DISK_USE_THRE, strMaxDiskUseThre);
		CString strInfo;
		strInfo.Format(_T("超过阈值范围：%.2f-%.2f\n请重新设置"),0.00,m_pParent->m_SysOptionConfig.m_dDiskThreMaxValue*100);
		MessageBox(strInfo,_T("提示"));
	}
	m_dMaxDiskUseThre = atof(strMaxDiskUseThre)/100;
}


void CDlgSysResStatistics::OnEnSetfocusEditHandleUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strOutput);
			//if (strInput != strOutput)
			//{
			//	CString strLog;
			//	strLog.Format(_T("修改[磁盘占用率警告阈值]由%s到%s\n"),strInput,strOutput);
			//	g_opLogFile.SaveLogText(strLog,3);
			//}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditHandleUseThre();
	}	
}


void CDlgSysResStatistics::OnEnKillfocusEditHandleUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strVal);
		if (!IsStringNumerical(strVal))
		{
			CString strMaxHandleUseThre;
			strMaxHandleUseThre.Format(_T("%d"), m_nMaxHandleUseThre);
			SetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strMaxHandleUseThre);
		}
		else
		{
			CString strMaxHandleUseThre;
			strMaxHandleUseThre.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strMaxHandleUseThre);
		}
	}
	CString strMaxHandleUseThre;
	GetDlgItemText(IDC_EDIT_HANDLE_USE_THRE,strMaxHandleUseThre);
	if (atoi(strMaxHandleUseThre) < 0||atoi(strMaxHandleUseThre) > m_pParent->m_SysOptionConfig.m_nHandleThreMaxValue)
	{
		strMaxHandleUseThre.Format(_T("%d"), m_nMaxHandleUseThre);
		SetDlgItemText(IDC_EDIT_HANDLE_USE_THRE, strMaxHandleUseThre);
		CString strInfo;
		strInfo.Format(_T("超过阈值范围：%d-%d\n请重新设置"),0,m_pParent->m_SysOptionConfig.m_nHandleThreMaxValue);
		MessageBox(strInfo,_T("提示"));
	}
	m_nMaxHandleUseThre = atoi(strMaxHandleUseThre);
}


void CDlgSysResStatistics::OnEnSetfocusEditGdiUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_GDI_USE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_GDI_USE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_GDI_USE_THRE, strOutput);
			//if (strInput != strOutput)
			//{
			//	CString strLog;
			//	strLog.Format(_T("修改[磁盘占用率警告阈值]由%s到%s\n"),strInput,strOutput);
			//	g_opLogFile.SaveLogText(strLog,3);
			//}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditGdiUseThre();
	}
}


void CDlgSysResStatistics::OnEnKillfocusEditGdiUseThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_GDI_USE_THRE, strVal);
		if (!IsStringNumerical(strVal))
		{
			CString strMaxGdiUseThre;
			strMaxGdiUseThre.Format(_T("%d"), m_nMaxGDIUseThre);
			SetDlgItemText(IDC_EDIT_GDI_USE_THRE, strMaxGdiUseThre);
		}
		else
		{
			CString strMaxGdiUseThre;
			strMaxGdiUseThre.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_GDI_USE_THRE, strMaxGdiUseThre);
		}
	}
	CString strMaxGdiUseThre;
	GetDlgItemText(IDC_EDIT_GDI_USE_THRE,strMaxGdiUseThre);
	if (atoi(strMaxGdiUseThre) < 0||atoi(strMaxGdiUseThre) > m_pParent->m_SysOptionConfig.m_nGDIThreMaxValue)
	{
		strMaxGdiUseThre.Format(_T("%d"), m_nMaxGDIUseThre);
		SetDlgItemText(IDC_EDIT_GDI_USE_THRE, strMaxGdiUseThre);
		CString strInfo;
		strInfo.Format(_T("超过阈值范围：%d-%d\n请重新设置"),0,m_pParent->m_SysOptionConfig.m_nGDIThreMaxValue);
		MessageBox(strInfo,_T("提示"));
	}
	m_nMaxGDIUseThre = atoi(strMaxGdiUseThre);
}


HBRUSH CDlgSysResStatistics::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//COLORREF colorRed = RGB(255,0,0);
	//HBRUSH brushRed = CreateSolidBrush(colorRed);
	//COLORREF colorGreen = RGB(0,255,0);
	//HBRUSH brushGreen = CreateSolidBrush(colorGreen);
	//switch(nCtlColor)
	//{
	//case CTLCOLOR_EDIT:
	//	{
	//		UINT nParentID = pWnd->GetParent()->GetDlgCtrlID();
	//		if (IDC_COMBO_DISK_USE == nParentID)
	//		{
	//			pDC->SetBkColor(colorRed);
	//			return brushRed;
	//		}
	//	}
	//	break;;
	//case CTLCOLOR_LISTBOX:
	//	{
	//		pDC->SetBkColor(colorGreen);
	//		return brushGreen;
	//	}
	//	break;
	//default:
	//	break;
	//}
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
UINT CDlgSysResStatistics::CacuFunc(LPVOID pParam)
{
	CDlgSysResStatistics* pOwner = (CDlgSysResStatistics*)pParam;
	if (pOwner!=NULL)
	{
		pOwner->m_pSysStatusInfo->CacuDriveName();
		while(!pOwner->m_bExitCacuThread)
		{
			pOwner->m_pSysStatusInfo->CacuDriveUsedInfo();
			pOwner->m_pSysStatusInfo->CacuMemoryUsedInfo();
			pOwner->m_pSysStatusInfo->CacuCpuUsedInfo();
			pOwner->m_pSysStatusInfo->CacuHandleUsedCount();
			pOwner->m_pSysStatusInfo->CacuGDIUsedCount();

			Sleep(1000);
		}
	}
	return 0;
}


void CDlgSysResStatistics::OnBnClickedChkThreValid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bThreValid)
	{
		/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(TRUE);*/
		GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(TRUE);

		//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(TRUE);
		///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(TRUE);*/
		//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(TRUE);

		/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(TRUE);*/
		GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(TRUE);

		/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(TRUE);*/
		GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(TRUE);

		/*GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(TRUE);*/
		GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(TRUE);

	}
	else
	{
		/*GetDlgItem(IDC_STATIC_CPU_USED_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CPU_USED_RATE_VAL)->EnableWindow(FALSE);*/
		GetDlgItem(IDC_STATIC_THRE_CPU)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PERCENT_CPU)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CPU_USE_THRE)->EnableWindow(FALSE);

		//GetDlgItem(IDC_STATIC_DISK_USED_RATE)->EnableWindow(FALSE);
		///*GetDlgItem(IDC_STATIC_DISK_USED_RATE_VAL)->EnableWindow(FALSE);*/
		//GetDlgItem(IDC_COMBO_DISK_USE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_THRE_DISK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PERCENT_DISK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DISK_USE_THRE)->EnableWindow(FALSE);

		/*GetDlgItem(IDC_STATIC_MEM_USED_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MEM_USED_RATE_VAL)->EnableWindow(FALSE);*/
		GetDlgItem(IDC_STATIC_THRE_MEM)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PERCENT_MEM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MEM_USE_THRE)->EnableWindow(FALSE);

		/*GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HANDLE_USED_COUNT_VAL)->EnableWindow(FALSE);*/
		GetDlgItem(IDC_STATIC_THRE_HANDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HANDLE_USE_THRE)->EnableWindow(FALSE);

		/*GetDlgItem(IDC_STATIC_GDI_USED_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_GDI_USED_COUNT_VAL)->EnableWindow(FALSE);*/
		GetDlgItem(IDC_STATIC_THRE_GDI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GDI_USE_THRE)->EnableWindow(FALSE);
	}
}
