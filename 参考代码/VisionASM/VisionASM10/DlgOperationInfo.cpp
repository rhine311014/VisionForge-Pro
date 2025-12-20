// DlgWarningInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgOperationInfo.h"
#include "afxdialogex.h"


// CDlgOperationInfo 对话框

IMPLEMENT_DYNAMIC(CDlgOperationInfo, CDialogEx)

#define OPERATION_REFRESH_TIME 500

CDlgOperationInfo::CDlgOperationInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOperationInfo::IDD, pParent)
	, m_nInfoType(0)
{
	m_strWarningInfo			= _T("");
	m_nPlatformNum				= 1;
	m_nCurrentPlatformIndex		= 0;
	m_strCurWorkDir				= _T("");
	m_strWarningFilePath		= _T("");
	m_strOperationLogFilePath	= _T("");
	m_boIsCleaningInfo			= FALSE;
	m_saWarningInfo.clear();		
	m_saWarningInfoOld.clear();		
	m_saOperationLogInfo.clear();	
	m_saOperationLogInfoOld.clear();
	m_psaSysInfoStrings = NULL;
	m_nInfoTypeOld				= 0;
}

CDlgOperationInfo::~CDlgOperationInfo()
{
}

void CDlgOperationInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,	IDC_COMBO_PLATFORM_INDEX,	m_comboPlatformSel);
	DDX_Radio(pDX,		IDC_RADIO_WARNING_INFO,		m_nInfoType);
	DDX_Control(pDX,	IDC_LIST_SHOW_INFO,			m_listctrlInfo);
	DDX_CBIndex(pDX,	IDC_COMBO_PLATFORM_INDEX,	m_nCurrentPlatformIndex);
}

BEGIN_MESSAGE_MAP(CDlgOperationInfo, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR_INFO,			&CDlgOperationInfo::OnBnClickedBtnClearInfo)
	ON_BN_CLICKED(IDC_BTN_CLOSE,				&CDlgOperationInfo::OnBnClickedBtnClose)
	ON_CBN_SELCHANGE(IDC_COMBO_PLATFORM_INDEX,	&CDlgOperationInfo::OnCbnSelchangeComboPlatformIndex)
	ON_BN_CLICKED(IDC_RADIO_WARNING_INFO,		&CDlgOperationInfo::OnBnClickedRadioShowInfoType)
	ON_BN_CLICKED(IDC_RADIO_OPER_LOG,			&CDlgOperationInfo::OnBnClickedRadioShowInfoType)
END_MESSAGE_MAP()


// CDlgOperationInfo 消息处理程序


BOOL CDlgOperationInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDlgItem();
	return TRUE;
}

CWnd* CDlgOperationInfo::SetParentWndPointer(CVisionASMDlg* pParent)
{
	CWnd* pPrevParent = (CWnd*)m_pParentDlg;
	m_pParentDlg = pParent;
	return pPrevParent;
}

void CDlgOperationInfo::SetVisionASMRef(std::vector<vcBaseVisionAlign*> vpVisionASM)
{
	if (vpVisionASM.empty())
	{
		return;
	}
	m_vpVisionASM = vpVisionASM;
}

void CDlgOperationInfo::SetPlatformNum(int nPlatformNum)
{
	m_nPlatformNum = nPlatformNum;
}

void CDlgOperationInfo::InitDlgItem()
{
	// 1.设置当前显示为“报警信息”
	m_nInfoType		= 0;
	// 2.根据平台数量，初始化combobox的值
	for (int i = 0; i < m_nPlatformNum; i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("工位%d"), i);
		m_comboPlatformSel.AddString(strTemp);
	}
	m_comboPlatformSel.SetCurSel(m_nCurrentPlatformIndex);	//默认为平台1
	// 3.初始化listctrl控件
	InitInfoList();
	// 4.更新语言
	UpdateDialogLanguage();

	UpdateData(FALSE);
}

void CDlgOperationInfo::InitInfoList()
{
	//1.初始化控件的样式
	InitListType();
	//2.初始化控件内容
	InitListContent();
}

void CDlgOperationInfo::InitListType()
{
	CRect rc;
	GetDlgItem(IDC_LIST_SHOW_INFO)->GetWindowRect(rc);

	m_listctrlInfo.InsertColumn(0, _T("保留"), LVCFMT_CENTER);
	m_listctrlInfo.InsertColumn(1, _T("时间"), LVCFMT_CENTER);
	m_listctrlInfo.InsertColumn(2, _T("类型"), LVCFMT_CENTER);
	m_listctrlInfo.InsertColumn(3, _T("内容描述"), LVCFMT_LEFT);
	m_listctrlInfo.DeleteColumn(0);		//zzc微软规定第一列不能设置格式，为了好看点，就得这么整

	int nWidth[3] = { 0 };
	nWidth[0] = rc.Width()/4;
	nWidth[1] = rc.Width()/3;
	nWidth[2] = rc.Width() - (nWidth[0] + nWidth[1]) - 1;
	
	m_listctrlInfo.SetColumnWidth(0 , nWidth[0]);
	m_listctrlInfo.SetColumnWidth(1 , nWidth[1]);
	m_listctrlInfo.SetColumnWidth(2 , nWidth[2]);

	COLORREF crBkColor = ::GetSysColor(COLOR_3DFACE);
	m_listctrlInfo.SetBkColor(crBkColor);
	m_listctrlInfo.SetExtendedStyle(m_listctrlInfo.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	int nColumnCount = m_listctrlInfo.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < 20; i++)	//为了看着顺眼
	{
		CString strTemp = _T("");
		m_listctrlInfo.InsertItem(i, strTemp);
		for (int j = 1; j < nColumnCount; j++)
		{
			m_listctrlInfo.SetItemText(i, j, strTemp);
		}
	}
}

void CDlgOperationInfo::InitListContent()
{
	CStdioFile file;
	CString strText=_T("");
	int nCount = 0;

	SetWarningInfoFilePath(m_vpVisionASM[m_nCurrentPlatformIndex]->GetWarningInfoFilePath());	//设置当前工位的报警文件路径

	if (m_strWarningFilePath == _T(""))
	{
		return;
	}
	m_vstrWarningInfo.clear();
	if (file.Open(m_strWarningFilePath, CFile::modeReadWrite))
	{
		file.SeekToBegin();
		while (file.ReadString(strText))
		{
			if (strText == _T(""))
			{
				continue;
			}
			m_vstrWarningInfo.push_back(strText);	//每个元素都是一行报警信息
			nCount++;
		}
		TRACE(_T("有效报警信息行数：%d\n"), nCount);
		file.Close();
	}
	
}

void CDlgOperationInfo::UpdateDialogLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParentDlg->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParentDlg->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_OPERATION_INFO") == strDlgID)
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
	UpdateCurDlgLanguage();
}

void CDlgOperationInfo::UpdateDlgItem(CRect rcCommCommand, int nIntendX)
{
 	CRect rect;
 	ScreenToClient(rect);
// 	GetDlgItem(IDC_BTN_SHOW_DETAIL_WARNING)->MoveWindow(rcCommCommand.left + rcCommCommand.Width() / nIntendX * (nIntendX - 1) + 2, 
// 		rcCommCommand.top - rect.Height() - 2, rcCommCommand.Width() / nIntendX -2, rect.Height());

	GetDlgItem(IDC_BTN_CLEAR_INFO)->GetWindowRect(rect);
	ScreenToClient(rect);

	GetDlgItem(IDC_BTN_CLEAR_INFO)->ShowWindow(SW_HIDE);
}

void CDlgOperationInfo::UpdateCurDlgLanguage()
{
	if (m_pParentDlg->m_psaSysInfoStrings != NULL)
	{
		CString strTempLang = GetSysInfoString(m_pParentDlg->m_psaSysInfoStrings, IDS_OPERATION_DLG_PLATFORM_INDEX);
		m_comboPlatformSel.ResetContent();
		for (int i = 0; i < m_nPlatformNum; i++)
		{
			CString strTemp = _T("");
			strTemp.Format(strTempLang, i + 1);
			m_comboPlatformSel.AddString(strTemp);
		}
		m_comboPlatformSel.SetCurSel(m_nCurrentPlatformIndex);
		for (int i = 0; i < m_listctrlInfo.GetHeaderCtrl()->GetItemCount(); i++)	//注意注意！！必须保证xml文件表头内容连续
		{
			strTempLang = GetSysInfoString(m_pParentDlg->m_psaSysInfoStrings, IDS_OPERATION_DLG_COLUMN_0_TITLE + i);
			CHeaderCtrl *pHead = m_listctrlInfo.GetHeaderCtrl();  
			if(pHead && pHead->GetSafeHwnd())  
			{
				TCHAR szBuf[129] = {0};
				HDITEM hdItem ={0};  
				hdItem.mask = HDI_TEXT ;		//取字符掩码
				hdItem.pszText = szBuf;		//字符缓冲区
				hdItem.cchTextMax = 128;	//缓冲区大小
				hdItem.pszText = (LPSTR)(LPCSTR)strTempLang; //设置新字符
				pHead->SetItem(i, &hdItem); //设置题头
			}
		}

	}
}

void CDlgOperationInfo::UpdateCurShowInfo(deque<CString> saShowInfo)
{
	UpdateData(TRUE);
	int nColumnCount		= m_listctrlInfo.GetHeaderCtrl()->GetItemCount();
	int nCount				= m_listctrlInfo.GetItemCount();	//获取当前已经有了多少行内容
	int nWarningInfoSection = 3;				//当前记录的报警信息，有“时间-内容”组成，可分成两段
	CStringArray strArrayRes;					//每一行信息分解成两段，第一个元素为时间，第二个元素为报警内容
	char chSplit			= '*';				//每一行报警信息的分隔符
	int nOldSize			= 0;				//上一次的信息条数
	int nCurSize			= 0;				//这次的信息条数
	int nInsert				= 0;				//需要增加的信息条数
	if (m_nInfoType == 1)			//如果是操作记录，分隔符为'-'
	{
		nOldSize = m_saOperationLogInfoOld.size();	//上一次记录的操作信息条数
		nCurSize = m_saOperationLogInfo.size();		//当前获得的操作信息条数
		nInsert  = nCurSize - nOldSize;				//需要增加的条数，先不考虑值为负的情况
		chSplit = '-';
	}
	else
	{
		nOldSize = m_saWarningInfoOld.size();	//上一次记录的操作信息条数
		nCurSize = m_saWarningInfo.size();		//当前获得的操作信息条数
		nInsert  = nCurSize - nOldSize;				//需要增加的条数，先不考虑值为负的情况
	}

	if (saShowInfo.size() > nCount)			//当前要显示的内容比较多
	{
		for (int i = nCount; i < saShowInfo.size(); i++)
		{
			CString strTemp = _T("");
			m_listctrlInfo.InsertItem(i, strTemp);		//填充空行占位
			for (int j = 1; j < nColumnCount; j++)
			{
				m_listctrlInfo.SetItemText(i, j, strTemp);
			}
		}
	}
	else	//当前已创建的listctrl行数大于或等于要显示的内容，将要显示的内容更新，将多余行的内容清空
	{
		for (int i = saShowInfo.size(); i < nCount; i++)
		{
			CString strTemp = _T("");
			m_listctrlInfo.SetItemText(i, 0, strTemp);		//清空多余内容
			for (int j = 1; j < nColumnCount; j++)
			{
				m_listctrlInfo.SetItemText(i, j, strTemp);
			}
		}
	}

	for (int i = nOldSize; i < nCurSize; i++)
	{
		if (Split(saShowInfo.at(i), chSplit, strArrayRes) < nColumnCount /*!= nWarningInfoSection*/)	//如果分解的报警信息不是两段
		{
			continue;
		}
		//将解析的内容添加到listctrl
		m_listctrlInfo.SetItemText(i, 0, strArrayRes.GetAt(0));	
		for (int j = 1; j < nColumnCount; j++)
		{
			m_listctrlInfo.SetItemText(i, j, strArrayRes.GetAt(j));
		}
	}

	if ((nInsert == 0) && (nCurSize == nCount))	//列表已满，新旧报警size已经一致，滚动信息，删除头，添加尾
	{
		int nPos = 0;
		CheckChange(nPos);
		for (int nIndex = 0; nIndex < nPos; nIndex++)
		{
			m_listctrlInfo.DeleteItem(0);	//删除第一条信息

			if (Split(saShowInfo.at(nCurSize - (nPos -nIndex)), chSplit, strArrayRes)  < nColumnCount/*!= nWarningInfoSection*/)	//如果分解的报警信息不是两段
			{
				continue;
			}
			//将解析的内容添加到listctrl
			m_listctrlInfo.InsertItem(nCurSize - 1, strArrayRes.GetAt(0));	
			for (int j = 1; j < nColumnCount; j++)
			{
				m_listctrlInfo.SetItemText(nCurSize - 1, j, strArrayRes.GetAt(j));
			}
		}
		
	}

	if (nCount > 0)
		m_listctrlInfo.EnsureVisible(nCount-1, FALSE);

	UpdateData(FALSE);
}


void CDlgOperationInfo::UpdateCurShowInfoAll(deque<CString> saShowInfo)
{
	UpdateData(TRUE);
	int nColumnCount = m_listctrlInfo.GetHeaderCtrl()->GetItemCount();
	int nCount = m_listctrlInfo.GetItemCount();	//获取当前已经有了多少行内容
	int nWarningInfoSection = 3;	//当前记录的报警信息，有“时间-内容”组成，可分成两段
	CStringArray strArrayRes;		//每一行信息分解成两段，第一个元素为时间，第二个元素为报警内容
	char chSplit = '-';				//每一行报警信息的分隔符

	if (m_nInfoType == 0)
	{
		chSplit = '*';
	}
	else
	{
		chSplit = '-';

	}

	if (saShowInfo.size() > nCount)			//当前要显示的内容比较多
	{
		for (int i = 0; i < saShowInfo.size() - nCount; i++)
		{
			CString strTemp = _T("");
			m_listctrlInfo.InsertItem(i, strTemp);		//填充空行占位
			for (int j = 1; j < nColumnCount; j++)
			{
				m_listctrlInfo.SetItemText(i, j, strTemp);
			}
		}
	}
	else	//当前已创建的listctrl行数大于或等于要显示的内容，将要显示的内容更新，将多余行的内容清空
	{
		for (int i = saShowInfo.size(); i < nCount; i++)
		{
			CString strTemp = _T("");
			m_listctrlInfo.SetItemText(i, 0, strTemp);		//清空多余内容
			for (int j = 1; j < nColumnCount; j++)
			{
				m_listctrlInfo.SetItemText(i, j, strTemp);
			}
		}
	}

	deque<CString>::iterator it = saShowInfo.begin();	//取出每一行报警信息

	for (int i = 0; it != saShowInfo.end(); i++, it++)
	{
		if (Split(*it, chSplit, strArrayRes) < nColumnCount/*!=nWarningInfoSection*/)	//如果分解的报警信息不是两段
		{
			return;
		}
		//将解析的内容添加到listctrl
		m_listctrlInfo.SetItemText(i, 0, strArrayRes.GetAt(0));	
		for (int j = 1; j < nColumnCount; j++)
		{
			m_listctrlInfo.SetItemText(i, j, strArrayRes.GetAt(j));
		}
	}

	UpdateData(FALSE);
}

void CDlgOperationInfo::LoadWarningInfoFromFile()
{
	if (m_vstrWarningInfo.empty())
	{
		return;
	}
	std::vector<CString>::iterator it = m_vstrWarningInfo.begin();	//取出每一行报警信息
	int nWarningInfoSection = 2;	//当前记录的报警信息，有“时间-内容”组成，可分成两段
	CStringArray strArrayRes;		//每一行信息分解成两段，第一个元素为时间，第二个元素为报警内容
	char chSplit = '*';				//每一行报警信息的分隔符
	int i = 0;
	for (; it != m_vstrWarningInfo.end(); i++, it++)
	{
		int nColumnCount = m_listctrlInfo.GetHeaderCtrl()->GetItemCount();

		if (Split(*it, chSplit, strArrayRes) < nColumnCount /* !=nWarningInfoSection*/)	//如果分解的报警信息不是两段
		{
			return;
		}
		//将解析的内容添加到listctrl
		m_listctrlInfo.InsertItem(i, strArrayRes.GetAt(0));
		for (int j = 1; j < nColumnCount; j++)
		{
			m_listctrlInfo.SetItemText(i, j, strArrayRes.GetAt(j));
		}
	}
}

void CDlgOperationInfo::SetWarningInfoFilePath(CString strFilePath)
{
	if (strFilePath == _T(""))
	{
		return;
	}
	m_strWarningFilePath = strFilePath;
}

void CDlgOperationInfo::SetCurrentWorkPath(CString strFilePath)
{
	if (strFilePath != _T(""))
	{
		m_strCurWorkDir = strFilePath;
	}
}

void CDlgOperationInfo::SetOperationLogFilePath(CString strFilePath)
{
	if (strFilePath == _T(""))
	{
		return;
	}
	m_strOperationLogFilePath = strFilePath;
}

void CDlgOperationInfo::SetCurrentShowPlatform(int nCurrentShowPlatform)
{
	if (m_nCurrentPlatformIndex != nCurrentShowPlatform)
	{
		m_nCurrentPlatformIndex = nCurrentShowPlatform;
		m_comboPlatformSel.SetCurSel(m_nCurrentPlatformIndex);
		UpdateData(FALSE);
		OnCbnSelchangeComboPlatformIndex();
	}
}

int CDlgOperationInfo::GetCurrentShowPlatform()
{
	return m_nCurrentPlatformIndex;
}

int CDlgOperationInfo::GetCurrentShowInfoType()
{
	return m_nInfoType;
}

void CDlgOperationInfo::ClearItemContent()
{
	int nColumnCount = m_listctrlInfo.GetHeaderCtrl()->GetItemCount();	//列数
	int nCount = m_listctrlInfo.GetItemCount();							//获取当前已经有了多少行内容

	while (nCount > 20) //超20的部分直接删除
	{
		m_listctrlInfo.DeleteItem(nCount - 1);
		nCount = m_listctrlInfo.GetItemCount();
	}

	for (int i = 0; i < nCount; i++)
	{
		CString strTemp = _T("");
		m_listctrlInfo.SetItemText(i, 0, strTemp);		//填充空行占位
		for (int j = 1; j < nColumnCount; j++)
		{
			m_listctrlInfo.SetItemText(i, j, strTemp);
		}
	}
}

void CDlgOperationInfo::CheckChange(int& nPos)
{
	int nPosRes = 0;
	if (m_nInfoType == 0)	//报警
	{
		deque<CString>::iterator it = m_saWarningInfoOld.begin();
		for (;it != m_saWarningInfoOld.end(); nPosRes++, it++)
		{
			if (*it == m_saWarningInfo.at(0))
			{
				break;
			}
		}
	}
	else
	{
		deque<CString>::iterator it = m_saOperationLogInfoOld.begin();
		for (;it != m_saOperationLogInfoOld.end(); nPosRes++, it++)
		{
			if (*it == m_saOperationLogInfo.at(0))
			{
				break;
			}
		}
	}

	nPos = nPosRes;
}

int CDlgOperationInfo::Split(CString strSource, const char split, CStringArray &strArrayRes)
{
	strArrayRes.RemoveAll();
	CString temp = strSource;
	int tag = 0;
	while (1)
	{
		tag = temp.Find(split);
		if (tag >= 0)
		{
			strArrayRes.Add(temp.Left(tag));
			temp = temp.Right(temp.GetLength() - tag - 1);
		}
		else 
		{
			break; 
		}
	}
	strArrayRes.Add(temp);
	return strArrayRes.GetSize();
}

BOOL CDlgOperationInfo::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgOperationInfo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOW_OPERATION_DLG:
		{			
			int nIndex = (int)wParam;
			this->SetCurrentShowPlatform(nIndex);
			if (!this->IsWindowVisible())
			{
				CString strLog;
				strLog.Format(_T("显示运行信息窗口[显示运行信息窗口]\n"));
				g_opLogFile.SaveLogText(strLog, 1);
			}
			this->ShowWindow(SW_SHOW);
		}
		break;
	case WM_UPDATE_WARNING_INFO:
		{
			UpdateData(TRUE);
			int nIndex = (int)wParam;
			//如果当前窗口显示的信息的平台序号与当前产生报警要更新的平台序号一致，且没有勾选停止刷新，那么实时刷新当前报警
			if ((nIndex == m_nCurrentPlatformIndex) 
				&& (((CButton*)GetDlgItem(IDC_CHECK_STOP_UPDATEINFO))->GetCheck() != 1)
				&& (!m_boIsCleaningInfo)
				&& (m_nInfoType == 0))	
			{
				m_vpVisionASM[nIndex]->GetWarningInfo(m_saWarningInfo);	//获取当前的报警信息
				if(m_saWarningInfoOld != m_saWarningInfo)
				{
					UpdateCurShowInfo(m_saWarningInfo);
					m_saWarningInfoOld = m_saWarningInfo;
				}
			}
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void CDlgOperationInfo::OnBnClickedBtnClearInfo()
{
	UpdateData(TRUE);
	m_boIsCleaningInfo = TRUE;

	ClearItemContent();	//清空列表的内容

	if (m_nInfoType == 0)
	{
		m_vpVisionASM[m_nCurrentPlatformIndex]->ClearWarningInfo();
		m_saWarningInfoOld.clear();
	}
	else
	{
		g_opLogFile.ClearOperationLogInfo();
		m_saOperationLogInfoOld.clear();
	}	

	m_boIsCleaningInfo = FALSE;

	CString strLog;
	strLog.Format(_T("点击清空内容[清空内容]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	UpdateData(FALSE);
}


void CDlgOperationInfo::OnBnClickedBtnClose()
{
	this->ShowWindow(SW_HIDE);

	CString strLog;
	strLog.Format(_T("点击关闭运行信息窗口[关闭运行信息窗口]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}

BOOL CDlgOperationInfo::DestroyWindow()
{
	return CDialog::DestroyWindow();
}


void CDlgOperationInfo::OnCbnSelchangeComboPlatformIndex()
{
	UpdateData(TRUE);
	deque<CString>			saWarningInfo;
	m_vpVisionASM[m_nCurrentPlatformIndex]->GetWarningInfo(saWarningInfo);	//获取当前的报警信息
	if (m_nInfoType == 0)
	{
		UpdateCurShowInfoAll(saWarningInfo);
	}

	if (m_nInfoType != m_nInfoTypeOld)	//说明类型进行了切换
	{
		CString strLog;
		CString strTempNow = _T("");
		CString strTempOld = _T("");
		switch(m_nInfoType)
		{
		case 0:
			{
				strTempNow = _T("报警信息");
				strTempOld = _T("操作记录");
			}
			break;
		case 1:
			{
				strTempNow = _T("操作记录");
				strTempOld = _T("报警信息");
			}
			break;
		}
		strLog.Format(_T("显示信息类型[%s]切换到[%s]\n"), strTempOld, strTempNow);
		g_opLogFile.SaveLogText(strLog,3);
		m_nInfoTypeOld = m_nInfoType;
	}
}


void CDlgOperationInfo::OnBnClickedRadioShowInfoType()
{
	UpdateData(TRUE);

 	switch (m_nInfoType)
 	{
 	case 0:
 		{
			GetDlgItem(IDC_COMBO_PLATFORM_INDEX)->EnableWindow(TRUE);
			KillTimer(1);
			ClearItemContent();		//先清空当前列表控件里显示的内容
			m_vpVisionASM[m_nCurrentPlatformIndex]->GetWarningInfo(m_saWarningInfo);	//获取当前的报警信息
			UpdateCurShowInfoAll(m_saWarningInfo);
			m_saWarningInfoOld = m_saWarningInfo;
 		}
 		break;
 	case 1:
 		{
			GetDlgItem(IDC_COMBO_PLATFORM_INDEX)->EnableWindow(FALSE);	//操作信息与平台无关
			ClearItemContent();		//先清空当前列表控件里显示的内容
			g_opLogFile.GetOperationLogInfo(m_saOperationLogInfo);
			UpdateCurShowInfoAll(m_saOperationLogInfo);
			SetTimer(1, OPERATION_REFRESH_TIME, NULL);
 		}
 		break;
 	default:
 		break;
 	}

	UpdateData(FALSE);
}

void CDlgOperationInfo::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			g_opLogFile.GetOperationLogInfo(m_saOperationLogInfo);
			if ((m_saOperationLogInfoOld != m_saOperationLogInfo)
				&& (m_boIsCleaningInfo != TRUE)
				&& (((CButton*)GetDlgItem(IDC_CHECK_STOP_UPDATEINFO))->GetCheck() != 1))
			{
				UpdateCurShowInfo(m_saOperationLogInfo);
				m_saOperationLogInfoOld = m_saOperationLogInfo;
			}
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
