// DlgRobotTeachSG.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgRobotTeachSG.h"
#include "afxdialogex.h"
#include "DlgRobotTeach.h"


// CDlgRobotTeachSG 对话框

IMPLEMENT_DYNAMIC(CDlgRobotTeachSG, CDialog)

CDlgRobotTeachSG::CDlgRobotTeachSG(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRobotTeachSG::IDD, pParent)
	, m_bCheckEnableSGI1(FALSE)
	, m_bCheckEnableSGI2(FALSE)
	, m_bCheckEnableSGI3(FALSE)
	, m_bCheckEnableSGI4(FALSE)
	, m_bCheckEnableSGI5(FALSE)
{
	m_pParent = NULL;
	m_nPageIndex = 0;

	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = FALSE;
	m_psaSysInfoStrings = NULL;

}

CDlgRobotTeachSG::~CDlgRobotTeachSG()
{
	m_MyFont.DeleteObject();
	m_MyBtnFont.DeleteObject();
}

void CDlgRobotTeachSG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SGI1, m_strSGInt1);
	DDX_Text(pDX, IDC_EDIT_SGI2, m_strSGInt2);
	DDX_Text(pDX, IDC_EDIT_SGI3, m_strSGInt3);
	DDX_Text(pDX, IDC_EDIT_SGI4, m_strSGInt4);
	DDX_Text(pDX, IDC_EDIT_SGI5, m_strSGInt5);
	DDX_Text(pDX, IDC_EDIT_SGR1, m_strSGReal1);
	DDX_Text(pDX, IDC_EDIT_SGR2, m_strSGReal2);
	DDX_Text(pDX, IDC_EDIT_SGR3, m_strSGReal3);
	DDX_Text(pDX, IDC_EDIT_SGR4, m_strSGReal4);
	DDX_Text(pDX, IDC_EDIT_SGR5, m_strSGReal5);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SGI1, m_bCheckEnableSGI1);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SGI2, m_bCheckEnableSGI2);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SGI3, m_bCheckEnableSGI3);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SGI4, m_bCheckEnableSGI4);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SGI5, m_bCheckEnableSGI5);
}


BEGIN_MESSAGE_MAP(CDlgRobotTeachSG, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE_SG, &CDlgRobotTeachSG::OnBnClickedBtnSaveSG)
	ON_EN_SETFOCUS(IDC_EDIT_SGI1, &CDlgRobotTeachSG::OnEnSetfocusEditSgi1)
	ON_EN_KILLFOCUS(IDC_EDIT_SGI1, &CDlgRobotTeachSG::OnEnKillfocusEditSgi1)
	ON_EN_SETFOCUS(IDC_EDIT_SGI2, &CDlgRobotTeachSG::OnEnSetfocusEditSgi2)
	ON_EN_KILLFOCUS(IDC_EDIT_SGI2, &CDlgRobotTeachSG::OnEnKillfocusEditSgi2)
	ON_EN_SETFOCUS(IDC_EDIT_SGI3, &CDlgRobotTeachSG::OnEnSetfocusEditSgi3)
	ON_EN_KILLFOCUS(IDC_EDIT_SGI3, &CDlgRobotTeachSG::OnEnKillfocusEditSgi3)
	ON_EN_SETFOCUS(IDC_EDIT_SGI4, &CDlgRobotTeachSG::OnEnSetfocusEditSgi4)
	ON_EN_KILLFOCUS(IDC_EDIT_SGI4, &CDlgRobotTeachSG::OnEnKillfocusEditSgi4)
	ON_EN_SETFOCUS(IDC_EDIT_SGI5, &CDlgRobotTeachSG::OnEnSetfocusEditSgi5)
	ON_EN_KILLFOCUS(IDC_EDIT_SGI5, &CDlgRobotTeachSG::OnEnKillfocusEditSgi5)
	ON_EN_SETFOCUS(IDC_EDIT_SGR1, &CDlgRobotTeachSG::OnEnSetfocusEditSgr1)
	ON_EN_KILLFOCUS(IDC_EDIT_SGR1, &CDlgRobotTeachSG::OnEnKillfocusEditSgr1)
	ON_EN_SETFOCUS(IDC_EDIT_SGR2, &CDlgRobotTeachSG::OnEnSetfocusEditSgr2)
	ON_EN_KILLFOCUS(IDC_EDIT_SGR2, &CDlgRobotTeachSG::OnEnKillfocusEditSgr2)
	ON_EN_SETFOCUS(IDC_EDIT_SGR3, &CDlgRobotTeachSG::OnEnSetfocusEditSgr3)
	ON_EN_KILLFOCUS(IDC_EDIT_SGR3, &CDlgRobotTeachSG::OnEnKillfocusEditSgr3)
	ON_EN_SETFOCUS(IDC_EDIT_SGR4, &CDlgRobotTeachSG::OnEnSetfocusEditSgr4)
	ON_EN_KILLFOCUS(IDC_EDIT_SGR4, &CDlgRobotTeachSG::OnEnKillfocusEditSgr4)
	ON_EN_SETFOCUS(IDC_EDIT_SGR5, &CDlgRobotTeachSG::OnEnSetfocusEditSgr5)
	ON_EN_KILLFOCUS(IDC_EDIT_SGR5, &CDlgRobotTeachSG::OnEnKillfocusEditSgr5)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SGI1, &CDlgRobotTeachSG::OnBnClickedCheckEnableSgi1)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SGI2, &CDlgRobotTeachSG::OnBnClickedCheckEnableSgi2)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SGI3, &CDlgRobotTeachSG::OnBnClickedCheckEnableSgi3)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SGI4, &CDlgRobotTeachSG::OnBnClickedCheckEnableSgi4)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SGI5, &CDlgRobotTeachSG::OnBnClickedCheckEnableSgi5)
END_MESSAGE_MAP()


// CDlgRobotTeachSG 消息处理程序

BOOL CDlgRobotTeachSG::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetItemFont();


	m_vstrSGInt.push_back(&m_strSGInt1);
	m_vstrSGInt.push_back(&m_strSGInt2);
	m_vstrSGInt.push_back(&m_strSGInt3);
	m_vstrSGInt.push_back(&m_strSGInt4);
	m_vstrSGInt.push_back(&m_strSGInt5);

	m_vstrSGReal.push_back(&m_strSGReal1);
	m_vstrSGReal.push_back(&m_strSGReal2);
	m_vstrSGReal.push_back(&m_strSGReal3);
	m_vstrSGReal.push_back(&m_strSGReal4);
	m_vstrSGReal.push_back(&m_strSGReal5);



	UpdateDlgData(FALSE);
	UpdateDialogLanguage();


	int i =0;
//	int nBaseIndex = m_nPageIndex*RobotTeachPosMaxNumPerPage;

	i = 0;
	GetDlgItem(IDC_STATIC_SGI1)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);

	i = 1;
	GetDlgItem(IDC_STATIC_SGI2)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);

	i = 2;
	GetDlgItem(IDC_STATIC_SGI3)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);

	i = 3;
	GetDlgItem(IDC_STATIC_SGI4)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);

	i = 4;
	GetDlgItem(IDC_STATIC_SGI5)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);


	i = 0;
	GetDlgItem(IDC_STATIC_SGR1)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);

	i = 1;
	GetDlgItem(IDC_STATIC_SGR2)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);

	i = 2;
	GetDlgItem(IDC_STATIC_SGR3)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);

	i = 3;
	GetDlgItem(IDC_STATIC_SGR4)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);

	i = 4;
	GetDlgItem(IDC_STATIC_SGR5)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgRobotTeachSG::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strLog;
	strLog.Format(_T("点击[保存退出]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	return;
	CDialog::OnOK();
}


void CDlgRobotTeachSG::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strLog;
	strLog.Format(_T("点击[退出]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	return;
	CDialog::OnCancel();
}


BOOL CDlgRobotTeachSG::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgRobotTeachSG::SetItemFont()
{
	m_MyFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));					// lpszFacename


	m_MyBtnFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));					// lpszFacename

	GetDlgItem(IDC_EDIT_SGI1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGI2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGI3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGI4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGI5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGR1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGR2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGR3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGR4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_SGR5)->SetFont(&m_MyFont);
}


void CDlgRobotTeachSG::EnableDlgItem(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_SAVE_SG)->EnableWindow(bEnable);
}

void CDlgRobotTeachSG::UpdateDlgData(BOOL bSave)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);

		// 示教位置
		CString strInfo;
		int i = 0; 
		for(i = 0; i < RobotTeachSGIntMaxNum; i++)
		{
			if (m_pParent->m_vbSGIntValid[i] == TRUE)
			{
				m_pParent->m_vnCurTempSGInt[i] = atoi(*m_vstrSGInt.at(i));
			}
			else
			{
				m_pParent->m_vnCurTempSGInt[i] = 0;
			}
		}

		for(i = 0; i < RobotTeachSGRealMaxNum; i++)
		{
			if (m_pParent->m_vbSGRealValid[i] == TRUE)
			{
				m_pParent->m_vdCurTempSGReal[i] = atof(*m_vstrSGReal.at(i));
			}
			else
			{
				m_pParent->m_vdCurTempSGReal[i] = 0;
			}
		}
	}
	else
	{
		// 将产品数据上传到界面信息中


		// 示教位置
		CString strInfo;
		int i = 0; 
		for(i = 0; i < RobotTeachSGIntMaxNum; i++)
		{
			if (m_pParent->m_vbSGIntValid[i] == TRUE)
			{
				strInfo.Format(_T("%d"), m_pParent->m_vnSGInt[i]);
				*m_vstrSGInt.at(i) = strInfo;
			}
			else
			{
				strInfo = _T("");
				*m_vstrSGInt.at(i) = strInfo;
			}
		}

		if(atoi(*m_vstrSGInt.at(0)) == 0)
		{
			m_bCheckEnableSGI1 = FALSE;
		}
		else
		{
			m_bCheckEnableSGI1 = TRUE;
		}

		if(atoi(*m_vstrSGInt.at(1)) == 0)
		{
			m_bCheckEnableSGI2 = FALSE;
		}
		else
		{
			m_bCheckEnableSGI2 = TRUE;
		}

		if(atoi(*m_vstrSGInt.at(2)) == 0)
		{
			m_bCheckEnableSGI3 = FALSE;
		}
		else
		{
			m_bCheckEnableSGI3 = TRUE;
		}

		if(atoi(*m_vstrSGInt.at(3)) == 0)
		{
			m_bCheckEnableSGI4 = FALSE;
		}
		else
		{
			m_bCheckEnableSGI4 = TRUE;
		}
		
		if(atoi(*m_vstrSGInt.at(4)) == 0)
		{
			m_bCheckEnableSGI5 = FALSE;
		}
		else
		{
			m_bCheckEnableSGI5 = TRUE;
		}


		for(i = 0; i < RobotTeachSGRealMaxNum; i++)
		{
			if (m_pParent->m_vbSGRealValid[i] == TRUE)
			{
				strInfo.Format(_T("%.03f"), m_pParent->m_vdSGReal[i]);
				*m_vstrSGReal.at(i) = strInfo;
			}
			else
			{
				strInfo = _T("");
				*m_vstrSGReal.at(i) = strInfo;
			}
		}

		UpdateData(FALSE);
	}
}

// 更新示教点位置
void CDlgRobotTeachSG::UpdateDlgDataTeachSGI(BOOL bSave, int nPosIndex)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 示教位置
		CString strInfo;
		int i = 0; 
		i = nPosIndex;

		if (m_pParent->m_vbSGIntValid[i] == TRUE)
		{
			strInfo.Format(_T("%d"), m_pParent->m_vnSGInt[i]);
			*m_vstrSGInt.at(i) = strInfo;
		}
		else
		{
			strInfo = _T("");
			*m_vstrSGInt.at(i) = strInfo;
		}


		if (i == 0)
		{
			if(atoi(*m_vstrSGInt.at(0)) == 0)
			{
				m_bCheckEnableSGI1 = FALSE;
			}
			else
			{
				m_bCheckEnableSGI1 = TRUE;
			}
		}
		else if (i == 1)
		{
			if(atoi(*m_vstrSGInt.at(1)) == 0)
			{
				m_bCheckEnableSGI2 = FALSE;
			}
			else
			{
				m_bCheckEnableSGI2 = TRUE;
			}
		}
		else if (i == 2)
		{
			if(atoi(*m_vstrSGInt.at(2)) == 0)
			{
				m_bCheckEnableSGI3 = FALSE;
			}
			else
			{
				m_bCheckEnableSGI3 = TRUE;
			}
		}
		else if (i == 3)
		{
			if(atoi(*m_vstrSGInt.at(3)) == 0)
			{
				m_bCheckEnableSGI4 = FALSE;
			}
			else
			{
				m_bCheckEnableSGI4 = TRUE;
			}
		}
		else if (i == 4)
		{
			if(atoi(*m_vstrSGInt.at(4)) == 0)
			{
				m_bCheckEnableSGI5 = FALSE;
			}
			else
			{
				m_bCheckEnableSGI5 = TRUE;
			}
		}



		UpdateData(FALSE);
	}
}


// 更新示教点位置
void CDlgRobotTeachSG::UpdateDlgDataTeachSGR(BOOL bSave, int nPosIndex)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 示教位置
		CString strInfo;
		int i = 0; 
		i = nPosIndex;

		if (m_pParent->m_vbSGRealValid[i] == TRUE)
		{
			strInfo.Format(_T("%.03f"), m_pParent->m_vdSGReal[i]);
			*m_vstrSGReal.at(i) = strInfo;
		}
		else
		{
			strInfo = _T("");
			*m_vstrSGReal.at(i) = strInfo;
		}

		UpdateData(FALSE);
	}
}


void CDlgRobotTeachSG::OnBnClickedBtnSaveSG()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	UpdateDlgData(TRUE);

	CString strInfo;
	strInfo.Format(_T("确定将当前页数据保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}

	m_pParent->m_nEvent = eRobotSetSGISGR;
}


BOOL CDlgRobotTeachSG::IsStringNumerical( LPCTSTR lpszVal )
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

// 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围
BOOL CDlgRobotTeachSG::IsSGIOutRange(LPCTSTR lpszVal, int nPosIndex)
{
	if (m_pParent->m_RobotTeachRangeInfo.m_vbRobotTeachSGIntRangeEnable[nPosIndex] == FALSE)
	{
		return FALSE;
	}
	CString strVal = lpszVal;
	int nVal = atoi(strVal);
	int nMax = m_pParent->m_RobotTeachRangeInfo.m_vnRobotTeachSGIntMax[nPosIndex] ;
	int nMin = m_pParent->m_RobotTeachRangeInfo.m_vnRobotTeachSGIntMin[nPosIndex] ;
	if (nVal > nMax	|| nVal < nMin)
	{
		return TRUE;
	}

	return FALSE;
}

// 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围	
BOOL CDlgRobotTeachSG::IsSGROutRange(LPCTSTR lpszVal, int nPosIndex)
{
	if (m_pParent->m_RobotTeachRangeInfo.m_vbRobotTeachSGRealRangeEnable[nPosIndex] == FALSE)
	{
		return FALSE;
	}
	CString strVal = lpszVal;
	int dVal = atof(strVal);
	int dMax = m_pParent->m_RobotTeachRangeInfo.m_vdRobotTeachSGRealMax[nPosIndex] ;
	int dMin = m_pParent->m_RobotTeachRangeInfo.m_vdRobotTeachSGRealMin[nPosIndex] ;
	if (dVal > dMax	|| dVal < dMin)
	{
		return TRUE;
	}

	return FALSE;
}


void CDlgRobotTeachSG::OnEnSetfocusEditSgi1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGI1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGI1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGI1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][整数常量1]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgi1();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgi1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 0;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGI1, strVal);
		if (!IsStringNumerical(strVal) || IsSGIOutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGI1, *m_vstrSGInt.at(nIndex));
		}
		else
		{
			(*m_vstrSGInt.at(nIndex)).Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SGI1, *m_vstrSGInt.at(nIndex));
		}
	}	

	if(atoi(*m_vstrSGInt.at(nIndex)) == 0)
	{
		m_bCheckEnableSGI1 = FALSE;
	}
	else
	{
		m_bCheckEnableSGI1 = TRUE;
	}
	UpdateData(FALSE);
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgi2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGI2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGI2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGI2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][整数常量2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgi2();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgi2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 1;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGI2, strVal);
		if (!IsStringNumerical(strVal) || IsSGIOutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGI2, *m_vstrSGInt.at(nIndex));
		}
		else
		{
			(*m_vstrSGInt.at(nIndex)).Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SGI2, *m_vstrSGInt.at(nIndex));
		}
	}	

	if(atoi(*m_vstrSGInt.at(nIndex)) == 0)
	{
		m_bCheckEnableSGI2 = FALSE;
	}
	else
	{
		m_bCheckEnableSGI2 = TRUE;
	}
	UpdateData(FALSE);
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgi3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGI3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGI3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGI3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][整数常量3]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgi3();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgi3()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 2;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGI3, strVal);
		if (!IsStringNumerical(strVal) || IsSGIOutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGI3, *m_vstrSGInt.at(nIndex));
		}
		else
		{
			(*m_vstrSGInt.at(nIndex)).Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SGI3, *m_vstrSGInt.at(nIndex));
		}
	}	


	if(atoi(*m_vstrSGInt.at(nIndex)) == 0)
	{
		m_bCheckEnableSGI3 = FALSE;
	}
	else
	{
		m_bCheckEnableSGI3 = TRUE;
	}
	UpdateData(FALSE);
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgi4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGI4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGI4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGI4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][整数常量4]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgi4();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgi4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 3;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGI4, strVal);
		if (!IsStringNumerical(strVal) || IsSGIOutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGI4, *m_vstrSGInt.at(nIndex));
		}
		else
		{
			(*m_vstrSGInt.at(nIndex)).Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SGI4, *m_vstrSGInt.at(nIndex));
		}
	}


	if(atoi(*m_vstrSGInt.at(nIndex)) == 0)
	{
		m_bCheckEnableSGI4 = FALSE;
	}
	else
	{
		m_bCheckEnableSGI4 = TRUE;
	}
	UpdateData(FALSE);
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgi5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGI5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGI5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGI5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][整数常量5]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgi5();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgi5()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 4;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGI5, strVal);
		if (!IsStringNumerical(strVal) || IsSGIOutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGI5, *m_vstrSGInt.at(nIndex));
		}
		else
		{
			(*m_vstrSGInt.at(nIndex)).Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SGI5, *m_vstrSGInt.at(nIndex));
		}
	}	


	if(atoi(*m_vstrSGInt.at(nIndex)) == 0)
	{
		m_bCheckEnableSGI5 = FALSE;
	}
	else
	{
		m_bCheckEnableSGI5 = TRUE;
	}
	UpdateData(FALSE);
}





void CDlgRobotTeachSG::OnEnSetfocusEditSgr1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGR1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGR1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGR1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][实数常量1]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgr1();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgr1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 0;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGR1, strVal);
		if (!IsStringNumerical(strVal) || IsSGROutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGR1, *m_vstrSGReal.at(nIndex));
		}
		else
		{
			(*m_vstrSGReal.at(nIndex)).Format(_T("%.03f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SGR1, *m_vstrSGReal.at(nIndex));
		}
	}	
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgr2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGR2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGR2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGR2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][实数常量2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgr2();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgr2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 1;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGR2, strVal);
		if (!IsStringNumerical(strVal) || IsSGROutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGR2, *m_vstrSGReal.at(nIndex));
		}
		else
		{
			(*m_vstrSGReal.at(nIndex)).Format(_T("%.03f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SGR2, *m_vstrSGReal.at(nIndex));
		}
	}	
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgr3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGR3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGR3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGR3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][实数常量3]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgr3();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgr3()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 2;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGR3, strVal);
		if (!IsStringNumerical(strVal) || IsSGROutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGR3, *m_vstrSGReal.at(nIndex));
		}
		else
		{
			(*m_vstrSGReal.at(nIndex)).Format(_T("%.03f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SGR3, *m_vstrSGReal.at(nIndex));
		}
	}	
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgr4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGR4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGR4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGR4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][实数常量4]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgr4();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgr4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 3;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGR4, strVal);
		if (!IsStringNumerical(strVal) || IsSGROutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGR4, *m_vstrSGReal.at(nIndex));
		}
		else
		{
			(*m_vstrSGReal.at(nIndex)).Format(_T("%.03f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SGR4, *m_vstrSGReal.at(nIndex));
		}
	}	
}



void CDlgRobotTeachSG::OnEnSetfocusEditSgr5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SGR5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SGR5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SGR5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[整量页][实数常量5]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSgr5();
	}	
}


void CDlgRobotTeachSG::OnEnKillfocusEditSgr5()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = 4;
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SGR5, strVal);
		if (!IsStringNumerical(strVal) || IsSGROutRange(strVal, nIndex))
		{
			SetDlgItemText(IDC_EDIT_SGR5, *m_vstrSGReal.at(nIndex));
		}
		else
		{
			(*m_vstrSGReal.at(nIndex)).Format(_T("%.03f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SGR5, *m_vstrSGReal.at(nIndex));
		}
	}	
}



void CDlgRobotTeachSG::OnBnClickedCheckEnableSgi1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheckEnableSGI1Old = m_bCheckEnableSGI1;
	UpdateData(TRUE);
	if (bCheckEnableSGI1Old != m_bCheckEnableSGI1)
	{
		CString strVal = m_bCheckEnableSGI1 ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[整量页][整数常量][启动]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
	UpdateData(TRUE);

	int nIndex = 0;
	if (m_bCheckEnableSGI1)
	{
		(*m_vstrSGInt.at(nIndex)) = _T("1");
		SetDlgItemText(IDC_EDIT_SGI1, *m_vstrSGInt.at(nIndex));
	}
	else
	{
		(*m_vstrSGInt.at(nIndex)) = _T("0");
		SetDlgItemText(IDC_EDIT_SGI1, *m_vstrSGInt.at(nIndex));
	}
}



void CDlgRobotTeachSG::OnBnClickedCheckEnableSgi2()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheckEnableSGI2Old = m_bCheckEnableSGI2;
	UpdateData(TRUE);
	if (bCheckEnableSGI2Old != m_bCheckEnableSGI2)
	{
		CString strVal = m_bCheckEnableSGI2 ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[整量页][整数常量][启动]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}

	UpdateData(TRUE);

	int nIndex = 1;
	if (m_bCheckEnableSGI2)
	{
		(*m_vstrSGInt.at(nIndex)) = _T("1");
		SetDlgItemText(IDC_EDIT_SGI2, *m_vstrSGInt.at(nIndex));
	}
	else
	{
		(*m_vstrSGInt.at(nIndex)) = _T("0");
		SetDlgItemText(IDC_EDIT_SGI2, *m_vstrSGInt.at(nIndex));
	}
}



void CDlgRobotTeachSG::OnBnClickedCheckEnableSgi3()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheckEnableSGI3Old = m_bCheckEnableSGI3;
	UpdateData(TRUE);
	if (bCheckEnableSGI3Old != m_bCheckEnableSGI3)
	{
		CString strVal = m_bCheckEnableSGI3 ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[整量页][整数常量][启动]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}

	UpdateData(TRUE);

	int nIndex = 2;
	if (m_bCheckEnableSGI3)
	{
		(*m_vstrSGInt.at(nIndex)) = _T("1");
		SetDlgItemText(IDC_EDIT_SGI3, *m_vstrSGInt.at(nIndex));
	}
	else
	{
		(*m_vstrSGInt.at(nIndex)) = _T("0");
		SetDlgItemText(IDC_EDIT_SGI3, *m_vstrSGInt.at(nIndex));
	}
}



void CDlgRobotTeachSG::OnBnClickedCheckEnableSgi4()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheckEnableSGI4Old = m_bCheckEnableSGI4;
	UpdateData(TRUE);
	if (bCheckEnableSGI4Old != m_bCheckEnableSGI4)
	{
		CString strVal = m_bCheckEnableSGI4 ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[整量页][整数常量][启动]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}

	UpdateData(TRUE);

	int nIndex = 3;
	if (m_bCheckEnableSGI4)
	{
		(*m_vstrSGInt.at(nIndex)) = _T("1");
		SetDlgItemText(IDC_EDIT_SGI4, *m_vstrSGInt.at(nIndex));
	}
	else
	{
		(*m_vstrSGInt.at(nIndex)) = _T("0");
		SetDlgItemText(IDC_EDIT_SGI4, *m_vstrSGInt.at(nIndex));
	}
}


void CDlgRobotTeachSG::OnBnClickedCheckEnableSgi5()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheckEnableSGI5Old = m_bCheckEnableSGI5;
	UpdateData(TRUE);
	if (bCheckEnableSGI5Old != m_bCheckEnableSGI5)
	{
		CString strVal = m_bCheckEnableSGI5 ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[整量页][整数常量][启动]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}

	UpdateData(TRUE);

	int nIndex = 4;
	if (m_bCheckEnableSGI5)
	{
		(*m_vstrSGInt.at(nIndex)) = _T("1");
		SetDlgItemText(IDC_EDIT_SGI5, *m_vstrSGInt.at(nIndex));
	}
	else
	{
		(*m_vstrSGInt.at(nIndex)) = _T("0");
		SetDlgItemText(IDC_EDIT_SGI5, *m_vstrSGInt.at(nIndex));
	}
}

void CDlgRobotTeachSG::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_ROBOT_TEACH_SG") == strDlgID)
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
		}  
	}
	xml.LeaveElem();
}