// DlgRobotTeachPos.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgRobotTeachPos.h"
#include "afxdialogex.h"
#include "DlgRobotTeach.h"

// CDlgRobotTeachPos 对话框

IMPLEMENT_DYNAMIC(CDlgRobotTeachPos, CDialog)

CDlgRobotTeachPos::CDlgRobotTeachPos(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRobotTeachPos::IDD, pParent)
{
	m_pParent = NULL;
	m_nPageIndex = 0;
	m_psaSysInfoStrings = NULL;
}

CDlgRobotTeachPos::~CDlgRobotTeachPos()
{
	m_MyFont.DeleteObject();
	m_MyBtnFont.DeleteObject();
}

void CDlgRobotTeachPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS_X1, m_strPosX1);
	DDX_Text(pDX, IDC_EDIT_POS_Y1, m_strPosY1);
	DDX_Text(pDX, IDC_EDIT_POS_R1, m_strPosR1);
	DDX_Text(pDX, IDC_EDIT_POS_Z1, m_strPosZ1);
	DDX_Text(pDX, IDC_EDIT_POS_X2, m_strPosX2);
	DDX_Text(pDX, IDC_EDIT_POS_Y2, m_strPosY2);
	DDX_Text(pDX, IDC_EDIT_POS_R2, m_strPosR2);
	DDX_Text(pDX, IDC_EDIT_POS_Z2, m_strPosZ2);
	DDX_Text(pDX, IDC_EDIT_POS_X3, m_strPosX3);
	DDX_Text(pDX, IDC_EDIT_POS_Y3, m_strPosY3);
	DDX_Text(pDX, IDC_EDIT_POS_R3, m_strPosR3);
	DDX_Text(pDX, IDC_EDIT_POS_Z3, m_strPosZ3);
	DDX_Text(pDX, IDC_EDIT_POS_X4, m_strPosX4);
	DDX_Text(pDX, IDC_EDIT_POS_Y4, m_strPosY4);
	DDX_Text(pDX, IDC_EDIT_POS_R4, m_strPosR4);
	DDX_Text(pDX, IDC_EDIT_POS_Z4, m_strPosZ4);
	DDX_Text(pDX, IDC_EDIT_POS_X5, m_strPosX5);
	DDX_Text(pDX, IDC_EDIT_POS_Y5, m_strPosY5);
	DDX_Text(pDX, IDC_EDIT_POS_R5, m_strPosR5);
	DDX_Text(pDX, IDC_EDIT_POS_Z5, m_strPosZ5);
}


BEGIN_MESSAGE_MAP(CDlgRobotTeachPos, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS1, &CDlgRobotTeachPos::OnBnClickedBtnSavePos1)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS2, &CDlgRobotTeachPos::OnBnClickedBtnSavePos2)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS3, &CDlgRobotTeachPos::OnBnClickedBtnSavePos3)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS4, &CDlgRobotTeachPos::OnBnClickedBtnSavePos4)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS5, &CDlgRobotTeachPos::OnBnClickedBtnSavePos5)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS1, &CDlgRobotTeachPos::OnBnClickedBtnMovePos1)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS2, &CDlgRobotTeachPos::OnBnClickedBtnMovePos2)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS3, &CDlgRobotTeachPos::OnBnClickedBtnMovePos3)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS4, &CDlgRobotTeachPos::OnBnClickedBtnMovePos4)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS5, &CDlgRobotTeachPos::OnBnClickedBtnMovePos5)
END_MESSAGE_MAP()


// CDlgRobotTeachPos 消息处理程序


BOOL CDlgRobotTeachPos::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetItemFont();


	m_vstrPosX.push_back(&m_strPosX1);
	m_vstrPosY.push_back(&m_strPosY1);
	m_vstrPosR.push_back(&m_strPosR1);
	m_vstrPosZ.push_back(&m_strPosZ1);

	m_vstrPosX.push_back(&m_strPosX2);
	m_vstrPosY.push_back(&m_strPosY2);
	m_vstrPosR.push_back(&m_strPosR2);
	m_vstrPosZ.push_back(&m_strPosZ2);

	m_vstrPosX.push_back(&m_strPosX3);
	m_vstrPosY.push_back(&m_strPosY3);
	m_vstrPosR.push_back(&m_strPosR3);
	m_vstrPosZ.push_back(&m_strPosZ3);

	m_vstrPosX.push_back(&m_strPosX4);
	m_vstrPosY.push_back(&m_strPosY4);
	m_vstrPosR.push_back(&m_strPosR4);
	m_vstrPosZ.push_back(&m_strPosZ4);

	m_vstrPosX.push_back(&m_strPosX5);
	m_vstrPosY.push_back(&m_strPosY5);
	m_vstrPosR.push_back(&m_strPosR5);
	m_vstrPosZ.push_back(&m_strPosZ5);


	UpdateDlgData(FALSE);
	UpdateDialogLanguage();

	int i =0;
	int nBaseIndex = m_nPageIndex*RobotTeachPosMaxNumPerPage;

	i = 0;
	GetDlgItem(IDC_STATIC_POS1)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachPosName[i + nBaseIndex]);

	i = 1;
	GetDlgItem(IDC_STATIC_POS2)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachPosName[i + nBaseIndex]);

	i = 2;
	GetDlgItem(IDC_STATIC_POS3)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachPosName[i + nBaseIndex]);

	i = 3;
	GetDlgItem(IDC_STATIC_POS4)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachPosName[i + nBaseIndex]);

	i = 4;
	GetDlgItem(IDC_STATIC_POS5)->SetWindowText(
		m_pParent->m_RobotTeachRangeInfo.m_vstrRobotTeachPosName[i + nBaseIndex]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgRobotTeachPos::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strLog;
	strLog.Format(_T("点击[保存退出]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	return;
	CDialog::OnOK();
}


void CDlgRobotTeachPos::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strLog;
	strLog.Format(_T("点击[退出]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	return;
	CDialog::OnCancel();
}


BOOL CDlgRobotTeachPos::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgRobotTeachPos::SetItemFont()
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
	
	GetDlgItem(IDC_EDIT_POS_X1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z5)->SetFont(&m_MyFont);
}


void CDlgRobotTeachPos::EnableDlgItem(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_SAVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS4)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS5)->EnableWindow(bEnable);

}

void CDlgRobotTeachPos::UpdateDlgData(BOOL bSave)
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
		int nBaseIndex = m_nPageIndex*RobotTeachPosMaxNumPerPage;
		for(i = 0; i < RobotTeachPosMaxNumPerPage; i++)
		{
			if (m_pParent->m_vbPosValid[i + nBaseIndex] == TRUE)
			{
				strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[i + nBaseIndex].m_dPosX);
				*m_vstrPosX.at(i) = strInfo;
				strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[i + nBaseIndex].m_dPosY);
				*m_vstrPosY.at(i) = strInfo;
				strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[i + nBaseIndex].m_dPosR);
				*m_vstrPosR.at(i) = strInfo;
				strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[i + nBaseIndex].m_dPosZ);
				*m_vstrPosZ.at(i) = strInfo;

			}
			else
			{
				strInfo = _T("");
				*m_vstrPosX.at(i) = strInfo;
				strInfo = _T("");
				*m_vstrPosY.at(i) = strInfo;
				strInfo = _T("");
				*m_vstrPosR.at(i) = strInfo;
				strInfo = _T("");
				*m_vstrPosZ.at(i) = strInfo;
			}
		}

		UpdateData(FALSE);
	}
}

// 更新示教点位置
void CDlgRobotTeachPos::UpdateDlgDataTeachPosPos(BOOL bSave, int nPosIndex)
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
		i = nPosIndex - m_nPageIndex*RobotTeachPosMaxNumPerPage;

		if (m_pParent->m_vbPosValid[nPosIndex] == TRUE)
		{
			strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent-> m_vrpPosPos[nPosIndex].m_dPosX);
			*m_vstrPosX.at(i) = strInfo;
			strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[nPosIndex].m_dPosY);
			*m_vstrPosY.at(i) = strInfo;
			strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[nPosIndex].m_dPosR);
			*m_vstrPosR.at(i) = strInfo;
			strInfo.Format(m_pParent->m_strFormatStringFloat, m_pParent->m_vrpPosPos[nPosIndex].m_dPosZ);
			*m_vstrPosZ.at(i) = strInfo;
		}
		else
		{
			strInfo = _T("");
			*m_vstrPosX.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosY.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosR.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosZ.at(i) = strInfo;
		}

		UpdateData(FALSE);
	}
}


void CDlgRobotTeachPos::OnBnClickedBtnSavePos1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strInfo;
	strInfo.Format(_T("确定将当前机器人位置示教到该点并保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}

	m_pParent->m_nCurPosIndex = 0 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotTeachPos;

	CString strLog;
	strLog.Format(_T("点击位置1[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnSavePos2()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strInfo;
	strInfo.Format(_T("确定将当前机器人位置示教到该点并保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}
	m_pParent->m_nCurPosIndex = 1 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotTeachPos;

	CString strLog;
	strLog.Format(_T("点击位置2[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnSavePos3()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strInfo;
	strInfo.Format(_T("确定将当前机器人位置示教到该点并保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}
	m_pParent->m_nCurPosIndex = 2 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotTeachPos;

	CString strLog;
	strLog.Format(_T("点击位置3[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnSavePos4()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strInfo;
	strInfo.Format(_T("确定将当前机器人位置示教到该点并保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}
	m_pParent->m_nCurPosIndex = 3 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotTeachPos;

	CString strLog;
	strLog.Format(_T("点击位置4[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnSavePos5()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strInfo;
	strInfo.Format(_T("确定将当前机器人位置示教到该点并保存到机器人吗？"));
	if (AfxMessageBox(strInfo, MB_YESNO) != IDYES)
	{
		return;
	}
	m_pParent->m_nCurPosIndex = 4 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotTeachPos;

	CString strLog;
	strLog.Format(_T("点击位置5[示教保存到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnMovePos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParent->m_nCurPosIndex = 0 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotMovePos;

	CString strLog;
	strLog.Format(_T("点击位置1[移动]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnMovePos2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParent->m_nCurPosIndex = 1 + m_nPageIndex*RobotTeachPosMaxNumPerPage;
	m_pParent->m_nEvent = eRobotMovePos;

	CString strLog;
	strLog.Format(_T("点击位置2[移动]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnMovePos3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParent->m_nCurPosIndex = 2;
	m_pParent->m_nEvent = eRobotMovePos;

	CString strLog;
	strLog.Format(_T("点击位置3[移动]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnMovePos4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParent->m_nCurPosIndex = 3;
	m_pParent->m_nEvent = eRobotMovePos;

	CString strLog;
	strLog.Format(_T("点击位置4[移动]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeachPos::OnBnClickedBtnMovePos5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pParent->m_nCurPosIndex = 4;
	m_pParent->m_nEvent = eRobotMovePos;

	CString strLog;
	strLog.Format(_T("点击位置5[移动]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}

void CDlgRobotTeachPos::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_ROBOT_TEACH_POS") == strDlgID)
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