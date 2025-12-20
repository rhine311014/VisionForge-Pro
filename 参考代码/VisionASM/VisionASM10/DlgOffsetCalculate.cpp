// DlgOffsetCalculate.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgOffsetCalculate.h"
#include "afxdialogex.h"
#include "vsVisionAlignDef.h"
#include "svXMLConfigurator.h"
#include "LogFileSave.h"

// CDlgOffsetCalculate 对话框

IMPLEMENT_DYNAMIC(CDlgOffsetCalculate, CDialogEx)

CDlgOffsetCalculate::CDlgOffsetCalculate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOffsetCalculate::IDD, pParent)
	, m_nRadioPosMode(0)
	, m_bOffsetXYEnable(FALSE)
	, m_bEnableObjectPositionVirtualMode(FALSE)
	, m_bEnableTargetPositionVirtualMode(FALSE)
	, m_nObjectPositionVirtualMode(0)
	, m_nTargetPositionVirtualMode(0)
{
	m_psaSysInfoStrings = NULL;
	m_strObjectL1	= _T("0.0");
	m_strObjectW1	= _T("0.0");
	m_strTargetL2	= _T("0.0");
	m_strTargetW2	= _T("0.0");
	m_strF1			= _T("0.0");
	m_strF2			= _T("0.0");
	m_dObjectL1		= 0;
	m_dObjectW1		= 0;
	m_dTargetL2		= 0;
	m_dTargetW2		= 0;
	m_dF1			= 0;
	m_dF2			= 0;
	m_bKillFocusEnabled = TRUE;
}

CDlgOffsetCalculate::~CDlgOffsetCalculate()
{
}

void CDlgOffsetCalculate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE0, m_staticImage0);
	DDX_Control(pDX, IDC_STATIC_IMAGE1, m_staticImage1);
	DDX_Control(pDX, IDC_STATIC_IMAGE2, m_staticImage2);
	DDX_Control(pDX, IDC_STATIC_IMAGE3, m_staticImage3);
	DDX_Control(pDX, IDC_STATIC_IMAGE4, m_staticImage4);
	DDX_Radio(pDX, IDC_RADIO_POS_MODE1, m_nRadioPosMode);
	DDX_Text(pDX, IDC_EDIT_L1, m_strObjectL1);
	DDX_Text(pDX, IDC_EDIT_W1, m_strObjectW1);
	DDX_Text(pDX, IDC_EDIT_L2, m_strTargetL2);
	DDX_Text(pDX, IDC_EDIT_W2, m_strTargetW2);
	DDX_Text(pDX, IDC_EDIT_F1, m_strF1);
	DDX_Text(pDX, IDC_EDIT_F2, m_strF2);
	DDX_Check(pDX, IDC_CHECK_XY_ENABLE, m_bOffsetXYEnable);
	DDX_Control(pDX, IDC_EDIT_ANGLE_RESULT, m_editAngleResult);
	DDX_Check(pDX, IDC_CHECK_OBJECT_POSITION_VITUAL_MODE, m_bEnableObjectPositionVirtualMode);
	DDX_Check(pDX, IDC_CHECK_TARGET_POSITION_VITUAL_MODE, m_bEnableTargetPositionVirtualMode);
	DDX_Radio(pDX, IDC_RADIO_OBJECT_34POSITION_VIRTUAL, m_nObjectPositionVirtualMode);
	DDX_Radio(pDX, IDC_RADIO_TARGET_78POSITION_VIRTUAL, m_nTargetPositionVirtualMode);
}


BEGIN_MESSAGE_MAP(CDlgOffsetCalculate, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_POS_MODE1, &CDlgOffsetCalculate::OnBnClickedRadioPosMode1)
	ON_BN_CLICKED(IDC_RADIO_POS_MODE2, &CDlgOffsetCalculate::OnBnClickedRadioPosMode2)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_CALCULATE, &CDlgOffsetCalculate::OnBnClickedButtonOffsetCalculate)
	ON_BN_CLICKED(IDOK, &CDlgOffsetCalculate::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_XY_ENABLE, &CDlgOffsetCalculate::OnBnClickedCheckXyEnable)
	ON_EN_SETFOCUS(IDC_EDIT_L1, &CDlgOffsetCalculate::OnEnSetfocusEditL1)
	ON_EN_SETFOCUS(IDC_EDIT_W1, &CDlgOffsetCalculate::OnEnSetfocusEditW1)
	ON_EN_SETFOCUS(IDC_EDIT_L2, &CDlgOffsetCalculate::OnEnSetfocusEditL2)
	ON_EN_SETFOCUS(IDC_EDIT_W2, &CDlgOffsetCalculate::OnEnSetfocusEditW2)
	ON_EN_SETFOCUS(IDC_EDIT_F1, &CDlgOffsetCalculate::OnEnSetfocusEditF1)
	ON_EN_SETFOCUS(IDC_EDIT_F2, &CDlgOffsetCalculate::OnEnSetfocusEditF2)
	ON_BN_CLICKED(IDCANCEL, &CDlgOffsetCalculate::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_ANGLE_RESULT, &CDlgOffsetCalculate::OnEnChangeEditAngleResult)
	ON_EN_KILLFOCUS(IDC_EDIT_L1, &CDlgOffsetCalculate::OnEnKillfocusEditL1)
	ON_EN_KILLFOCUS(IDC_EDIT_W1, &CDlgOffsetCalculate::OnEnKillfocusEditW1)
	ON_EN_KILLFOCUS(IDC_EDIT_L2, &CDlgOffsetCalculate::OnEnKillfocusEditL2)
	ON_EN_KILLFOCUS(IDC_EDIT_W2, &CDlgOffsetCalculate::OnEnKillfocusEditW2)
	ON_EN_KILLFOCUS(IDC_EDIT_F1, &CDlgOffsetCalculate::OnEnKillfocusEditF1)
	ON_EN_KILLFOCUS(IDC_EDIT_F2, &CDlgOffsetCalculate::OnEnKillfocusEditF2)
	
	ON_BN_CLICKED(IDC_CHECK_OBJECT_POSITION_VITUAL_MODE, &CDlgOffsetCalculate::OnBnClickedCheckObjectPositionVitualMode)
	ON_BN_CLICKED(IDC_CHECK_TARGET_POSITION_VITUAL_MODE, &CDlgOffsetCalculate::OnBnClickedCheckTargetPositionVitualMode)
	ON_BN_CLICKED(IDC_RADIO_OBJECT_12POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioObject12positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_OBJECT_34POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioObject34positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_OBJECT_13POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioObject13positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_OBJECT_24POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioObject24positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_TARGET_56POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioTarget56positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_TARGET_78POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioTarget78positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_TARGET_57POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioTarget57positionVirtual)
	ON_BN_CLICKED(IDC_RADIO_TARGET_68POSITION_VIRTUAL, &CDlgOffsetCalculate::OnBnClickedRadioTarget68positionVirtual)
END_MESSAGE_MAP()


// CDlgOffsetCalculate 消息处理程序


BOOL CDlgOffsetCalculate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 修改新选择的平台的显示界面
//	if (m_pParent != NULL)
//	{
//// 		SysPlatformInfo platformInfo;
//// 		m_pParent->m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_pParent->m_nPlatformIndex, platformInfo);
//
//		if ((m_platformInfo.m_bEnableObjectPositionVirtualMode) || (m_platformInfo.m_bEnableTargetPositionVirtualMode))
//		{
//			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDC_RADIO_POS_MODE1)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDC_RADIO_POS_MODE2)->ShowWindow(SW_HIDE);
//// 			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//// 			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//// 			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//// 			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//// 			GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
//		}
//	}


	//lzk 0405 界面显示初始化
	if (m_optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
	{
		GetDlgItem(IDC_STATIC_GRP_POS_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_POS_MODE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_POS_MODE2)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_OBJECT_POSITION_VITUAL_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_TARGET_POSITION_VITUAL_MODE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RADIO_TARGET_56POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_TARGET_78POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_TARGET_57POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_TARGET_68POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OBJECT_POSITION_VITUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_OBJECT_12POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_OBJECT_34POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_OBJECT_13POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_OBJECT_24POSITION_VIRTUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TARGET_POSITION_VITUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OBJECT_POSITION_VITUAL)->ShowWindow(SW_HIDE);
	}

	ShowImage();

	GetDlgItem(IDC_EDIT_F1)->EnableWindow(m_bOffsetXYEnable);
	GetDlgItem(IDC_EDIT_F2)->EnableWindow(m_bOffsetXYEnable);

	m_strObjectL1.Format(_T("%.3f"), m_dObjectL1);
	m_strObjectW1.Format(_T("%.3f"), m_dObjectW1);
	m_strTargetL2.Format(_T("%.3f"), m_dTargetL2);
	m_strTargetW2.Format(_T("%.3f"), m_dTargetW2);
	m_strF1.Format(_T("%.4f"), m_dF1);
	m_strF2.Format(_T("%.4f"), m_dF2);

	UpdateDialogLanguage();//
	CString strLog;
	strLog.Format(_T("进入[对象补偿设置][产品尺寸设置]"));
	g_opLogFile.SaveLogText(strLog,4);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgOffsetCalculate::ShowImage()
{

	if (m_nRadioPosMode == 0)
	{
		m_staticImage0.ShowWindow(SW_SHOW);
		m_staticImage1.ShowWindow(SW_HIDE);
		m_staticImage2.ShowWindow(SW_HIDE);
		m_staticImage3.ShowWindow(SW_HIDE);
		m_staticImage4.ShowWindow(SW_HIDE);
	}
	else if (m_nRadioPosMode == 1)
	{
		m_staticImage0.ShowWindow(SW_HIDE);
		m_staticImage1.ShowWindow(SW_SHOW);
		m_staticImage2.ShowWindow(SW_HIDE);
		m_staticImage3.ShowWindow(SW_HIDE);
		m_staticImage4.ShowWindow(SW_HIDE);
	}
}


void CDlgOffsetCalculate::OnBnClickedRadioPosMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioPosMode = 0;
	CString strLog;
	strLog.Format(_T("修改[拍照位置模式]为拍照位置模式1\n"));
	g_opLogFile.SaveLogText(strLog,5);
	ShowImage();
}


void CDlgOffsetCalculate::OnBnClickedRadioPosMode2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioPosMode = 1;
	CString strLog;
	strLog.Format(_T("修改[拍照位置模式]为拍照位置模式2\n"));
	g_opLogFile.SaveLogText(strLog,5);
	ShowImage();
}


void CDlgOffsetCalculate::OnBnClickedButtonOffsetCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (!IsStringPositiveNumerical(m_strObjectL1) || !IsStringPositiveNumerical(m_strObjectW1) 
		|| !IsStringPositiveNumerical(m_strTargetL2) || !IsStringPositiveNumerical(m_strTargetW2))
	{
		AfxMessageBox(_T("请输入正确L1/W1/L2/W2数值！"));
		return;
	}

	scDegree angleOffset = scDegree(0);
	m_dObjectL1 = atof(m_strObjectL1);
	m_dObjectW1 = atof(m_strObjectW1);
	m_dTargetL2 = atof(m_strTargetL2);
	m_dTargetW2 = atof(m_strTargetW2);

	if (!AngleOffsetCalculate(m_dObjectL1, m_dObjectW1, m_dTargetL2, m_dTargetW2, m_nRadioPosMode, angleOffset))
	{
		AfxMessageBox(_T("计算角度补偿初值失败，可能是对象或目标尺寸设置不合理！"));
		return;
	}
	
	CString strResult;
	strResult.Format(_T("%.6lf"),angleOffset.ToDouble());
	m_editAngleResult.SetWindowText(strResult);
}


void CDlgOffsetCalculate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	scDegree angleOffset = scDegree(0);
	m_dObjectL1	= atof(m_strObjectL1);
	m_dObjectW1	= atof(m_strObjectW1);
	m_dTargetL2	= atof(m_strTargetL2);
	m_dTargetW2	= atof(m_strTargetW2);
	m_dF1			= atof(m_strF1);
	m_dF2			= atof(m_strF2);


	if (!IsStringPositiveNumerical(m_strObjectL1) || !IsStringPositiveNumerical(m_strObjectW1) 
		|| !IsStringPositiveNumerical(m_strTargetL2) || !IsStringPositiveNumerical(m_strTargetW2)
		|| !IsStringPositiveNumerical(m_strF1) || !IsStringPositiveNumerical(m_strF2))
	{
		AfxMessageBox(_T("请输入正确L1/W1/L2/W2/F1/F2数值！"));
		return;
	}
	
	//lzk 0405 保存退出
	if ((m_optionInfo.m_bEnableTargetAndObjectPositionVirtualMode))
	{
		CDialogEx::OnOK();
		return;
	}

	if (!AngleOffsetCalculate(m_dObjectL1, m_dObjectW1, m_dTargetL2, m_dTargetW2, m_nRadioPosMode, angleOffset))
	{
		AfxMessageBox(_T("计算角度补偿初值失败，可能是对象或目标尺寸设置不合理！"));
		return;
	}

	if (m_bOffsetXYEnable)
	{
		double dOffsetX = 0, dOffsetY = 0;
		if (!XYOffsetCalculate(m_dObjectL1, m_dObjectW1, m_dTargetL2, m_dTargetW2, m_dF1, m_dF2, m_nRadioPosMode, dOffsetX, dOffsetY))
		{
			AfxMessageBox(_T("计算平移补偿初值失败，可能是对象或目标尺寸设置不合理！"));
			return;
		}
	}
	CString strLog;
	strLog.Format(_T("[保存]退出"));
	g_opLogFile.SaveLogText(strLog,4);

	CDialogEx::OnOK();
}


void CDlgOffsetCalculate::OnBnClickedCheckXyEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOffsetXYEnable = !m_bOffsetXYEnable;

	GetDlgItem(IDC_EDIT_F1)->EnableWindow(m_bOffsetXYEnable);
	GetDlgItem(IDC_EDIT_F2)->EnableWindow(m_bOffsetXYEnable);
}


void CDlgOffsetCalculate::OnEnSetfocusEditL1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_L1)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_L1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_L1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象水平长度L1]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}

		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditL1();
	}
}


void CDlgOffsetCalculate::OnEnSetfocusEditW1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_W1)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_W1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_W1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象水平宽度W1]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditW1();
	}
}


void CDlgOffsetCalculate::OnEnSetfocusEditL2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_L2)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_L2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_L2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象水平长度L2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditL2();
	}
}


void CDlgOffsetCalculate::OnEnSetfocusEditW2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_W2)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_W2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_W2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象垂直长度W2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditW2();
	}
}


void CDlgOffsetCalculate::OnEnSetfocusEditF1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_F1)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_F1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_F1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[水平方向偏移F1]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditF1();
	}
}


void CDlgOffsetCalculate::OnEnSetfocusEditF2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);
		m_bKillFocusEnabled = FALSE;
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_F2)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_F2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_F2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[垂直方向偏移F2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditF2();
	}
}


void CDlgOffsetCalculate::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_OFFSET_CALCULATE") == strDlgID)
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


void CDlgOffsetCalculate::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[取消]退出"));
	g_opLogFile.SaveLogText(strLog,3);
	CDialogEx::OnCancel();
}


void CDlgOffsetCalculate::OnEnChangeEditAngleResult()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码	
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ANGLE_RESULT)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_ANGLE_RESULT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ANGLE_RESULT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[计算角度补偿初值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,5);
			}
		}
	}
}

BOOL CDlgOffsetCalculate::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
		if (((ch > '9') || ((ch < '0'))) && (ch != '.'))
		{
			if ((i == 0) && ((ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}


void CDlgOffsetCalculate::OnEnKillfocusEditL1()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_L1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_L1, m_strObjectL1);
		}
		else
		{
			m_strObjectL1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_L1, m_strObjectL1);
		}
	}
}


void CDlgOffsetCalculate::OnEnKillfocusEditW1()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_W1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_W1, m_strObjectW1);
		}
		else
		{
			m_strObjectW1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_W1, m_strObjectW1);
		}
	}
}


void CDlgOffsetCalculate::OnEnKillfocusEditL2()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_L2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_L2, m_strTargetL2);
		}
		else
		{
			m_strTargetL2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_L2, m_strTargetL2);
		}
	}
}


void CDlgOffsetCalculate::OnEnKillfocusEditW2()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_W2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_W2, m_strTargetW2);
		}
		else
		{
			m_strTargetW2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_W2, m_strTargetW2);
		}
	}
}


void CDlgOffsetCalculate::OnEnKillfocusEditF1()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_F1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_F1, m_strF1);
		}
		else
		{
			m_strF1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_F1, m_strF1);
		}
	}
}


void CDlgOffsetCalculate::OnEnKillfocusEditF2()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_F2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_F2, m_strF2);
		}
		else
		{
			m_strF1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_F2, m_strF2);
		}
	}
}

void CDlgOffsetCalculate::OnBnClickedCheckObjectPositionVitualMode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableObjectPositionVirtualMode = !m_bEnableObjectPositionVirtualMode;
	CString strLog;
	strLog.Format(_T("[启用对象位置虚拟]%d"),m_bEnableObjectPositionVirtualMode);
	g_opLogFile.SaveLogText(strLog,5);

}


void CDlgOffsetCalculate::OnBnClickedCheckTargetPositionVitualMode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEnableTargetPositionVirtualMode = !m_bEnableTargetPositionVirtualMode;
	CString strLog;
	strLog.Format(_T("[启用目标位置虚拟]%d"),m_bEnableTargetPositionVirtualMode);
	g_opLogFile.SaveLogText(strLog,5);

}


void CDlgOffsetCalculate::OnBnClickedRadioObject12positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nObjectPositionVirtualMode = 1;
	CString strLog;
	strLog.Format(_T("修改[对象位置虚拟]为12虚拟34拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioObject34positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nObjectPositionVirtualMode = 0;
	CString strLog;
	strLog.Format(_T("修改[对象位置虚拟]为34虚拟12拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioObject13positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nObjectPositionVirtualMode = 3;
	CString strLog;
	strLog.Format(_T("修改[对象位置虚拟]为13虚拟24拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioObject24positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nObjectPositionVirtualMode = 2;
	CString strLog;
	strLog.Format(_T("修改[对象位置虚拟]为24虚拟13拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioTarget56positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nTargetPositionVirtualMode = 1;
	CString strLog;
	strLog.Format(_T("修改[目标位置虚拟]为56虚拟78拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioTarget78positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nTargetPositionVirtualMode = 0;
	CString strLog;
	strLog.Format(_T("修改[目标位置虚拟]为78虚拟56拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioTarget57positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nTargetPositionVirtualMode = 3;
	CString strLog;
	strLog.Format(_T("修改[目标位置虚拟]为57虚拟68拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}


void CDlgOffsetCalculate::OnBnClickedRadioTarget68positionVirtual()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nTargetPositionVirtualMode = 2;
	CString strLog;
	strLog.Format(_T("修改[目标位置虚拟]为68虚拟57拍照\n"));
	g_opLogFile.SaveLogText(strLog,5);
}
