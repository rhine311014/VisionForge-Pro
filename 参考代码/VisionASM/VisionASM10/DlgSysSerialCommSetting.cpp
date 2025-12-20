// DlgSysSerialCommTest.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysSerialCommSetting.h"
#include "DlgSysCommSetting.h"

#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysSerialCommSetting dialog


CDlgSysSerialCommSetting::CDlgSysSerialCommSetting(CWnd* pParent /*=NULL*/)
	: CDlgSysBaseCommSetting(CDlgSysSerialCommSetting::IDD, pParent)
	, m_bRadioWordMode(TRUE)
	, m_nEditWriteRegValue(0)
	, m_nEditReadRegValue(0)
	, m_lWriteRegMaxNum(100)
	, m_lReadRegMaxNum(100)
{
	//{{AFX_DATA_INIT(CDlgSysSerialCommSetting)
	m_strDataReceived = _T("");
	m_strDataSending = _T("");
	//}}AFX_DATA_INIT
	//	m_pParent = (CDlgSysCommSetting *)pParent;
	m_pParent = (CDlgSysEnvironSetting *)pParent;
	m_pCommInfo = NULL;
	m_strRegAddress = _T("100");
	m_strTempRegAddress = _T("200");
	m_nRegType = 0;
	m_nEditReadRegAddrValue = _ttoi(m_strRegAddress);
    m_nEditWriteRegAddrValue = _ttoi(m_strRegAddress);

	m_bshowCommParamSet = true;
	m_bEnableCommParamSet= true;

	m_bshowCommTestSet = true;
	m_bEnableCommTestSet = true;
}


void CDlgSysSerialCommSetting::InitDlgItem()
{
	UpdateDlgByAuthorize();
	int i = 0;
	long n = 300;
	CString strInfo;
	for (i=0; i<10; i++)
	{
		strInfo.Format(_T("COM%d"), i+1);
		m_comboComPort.AddString(strInfo);
	}
	for (i=0; i<6; i++)
	{
		strInfo.Format(_T("%d"), n);
		m_comboBaudRate.AddString(strInfo);
		n *= 2;
	}
	
	n = 14400;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);
	
	n = 19200;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);
	
	n = 38400;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);
	
	n = 56000;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);
	
	n = 57600;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);
	
	n = 115200;
	strInfo.Format(_T("%d"), n);
	m_comboBaudRate.AddString(strInfo);


	m_comboParity.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_NO_PARITY));		//_T("?TD￡?é")
	m_comboParity.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ODD_PARITY));	//_T("??D￡?é")
	m_comboParity.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_EVEN_PARITY));	//_T("??D￡?é")
	m_comboParity.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MARK_PARITY));	//_T("±ê??D￡?é")
	m_comboParity.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPACE_PARITY));	//_T("????D￡?é")
	m_comboDataBits.AddString(_T("6"));
	m_comboDataBits.AddString(_T("7"));
	m_comboDataBits.AddString(_T("8"));
	m_comboStopBits.AddString(_T("1"));
	m_comboStopBits.AddString(_T("1.5"));
	m_comboStopBits.AddString(_T("2"));
	m_comboCmdStopChar.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_NO_STOP_CHAR));	//_T("?T")
	m_comboCmdStopChar.AddString(_T("\\"));
	m_comboCommStopChar.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_NO_STOP_CHAR));	//_T("?T")
	m_comboCommStopChar.AddString(_T("CR"));
	m_comboCommStopChar.AddString(_T("CRLF"));

	m_comboCmdStopChar.SetCurSel(0);
	m_comboCommStopChar.SetCurSel(0);
//	m_comboCmdStopChar.EnableWindow(FALSE);
// 	m_comboCommStopChar.EnableWindow(FALSE);

	if (m_pVisionASM->m_XYDCommProtocol.IsPlcProtocolType())
	{

		GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);

		GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);

		GetDlgItem(IDC_STATIC_READ_WRITE_REG_TEST)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_RADIO_DOUBLE_TYPE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_READ_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_READ_REG_ADDR_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_BUTTON_READ_REG)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_READ_REG_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_READ_REG_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_WRITE_REG_ADDR_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_BUTTON_WRITE_REG)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WRITE_REG_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_WRITE_REG_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(m_bshowCommTestSet);
	}
	else
	{
		GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_READ_WRITE_REG_TEST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_DOUBLE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_READ_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_READ_REG_ADDR_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_READ_REG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_READ_REG_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_READ_REG_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WRITE_REG_ADDR_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_WRITE_REG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_REG_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WRITE_REG_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(SW_HIDE);
	}


	

	if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
	{
		GetDlgItem(IDC_COMBO_COMM_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_CMD_STOP_CHAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_COMM_STOP_CHAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE_COMM)->EnableWindow(FALSE);
	}

	// 打开\关闭通信连接
	m_btnCommSwitch.SetThemeHelper(&m_themeHelper);
	m_btnCommSwitch.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsCommOpen())
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//			m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
			m_ctrlCommLamp.SetIcon(m_hIconOpen);
		}
		else
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//			m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
			m_ctrlCommLamp.SetIcon(m_hIconClosed);
		}
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	
	m_btnRDClear.SetThemeHelper(&m_themeHelper);
//	m_btnRDClear.SetIcon(IDI_ICON_CLEAR_COMM_DATA_16);
	m_btnRDClear.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnSDClear.SetThemeHelper(&m_themeHelper);
//	m_btnSDClear.SetIcon(IDI_ICON_CLEAR_COMM_DATA_16);
	m_btnSDClear.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnSend.SetThemeHelper(&m_themeHelper);
//	m_btnSend.SetIcon(IDI_ICON_SEND_COMM_DATA_16);
	m_btnSend.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnReadReg.SetThemeHelper(&m_themeHelper);
	m_btnReadReg.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnWriteReg.SetThemeHelper(&m_themeHelper);
	m_btnWriteReg.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

UINT CDlgSysSerialCommSetting::ThreadReceiveData( LPVOID pParam )
{
	CDlgSysSerialCommSetting *pOwner = (CDlgSysSerialCommSetting*)pParam;
	CString strReceived;
	pOwner->m_strDataReceivedOnce.Empty();
	while (!pOwner->m_bExitThread)
	{
		if ((pOwner->IsVisible()) && (pOwner->m_pVisionASM != NULL) && (pOwner->m_pVisionASM->IsCommOpen()))
		{
			if (pOwner->m_pVisionASM->ReceiveCommInfo(pOwner->m_strDataReceivedOnce))
			{
				pOwner->m_strDataReceived += pOwner->m_strDataReceivedOnce;
				pOwner->UpdateReceiveData();
			}
		}		
		Sleep(10);
	}
	
	SetEvent(pOwner->m_hEventReceiveThreadExit);
	return 0;
}

void CDlgSysSerialCommSetting::UpdateReceiveData()
{
	SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);

// 	if (m_pVisionASM->m_XYDCommProtocol.GetProtocolType() > 10)
// 	{
// 		if (0 == m_nRegType)		// 写寄存器（WORD）
// 		{
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(m_strDataReceived);
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 		else if (1 == m_nRegType)	 // 写寄存器（DWORD）
// 		{
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(m_strDataReceived);
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 		else if (2 == m_nRegType)	// 读寄存器（WORD）
// 		{
// 			CString strInfo;
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			std::vector<int> vReadData;
// 			vReadData.resize(5);
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(m_strDataReceived, nReadNum,vReadData,FALSE);
// 
// 			CString strTemp;
// 			CString strTempData;
// 			for (int i = 0; i < vReadData.size(); i++)
// 			{
// 				int nData = vReadData.at(i);	
// 				strTemp.Format(_T("%d,"),nData);
// 				strTempData = strTempData + strTemp;
// 			}
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, strTempData);
// 		}
// 		else if (3 == m_nRegType)	// 读寄存器（DWORD）
// 		{
// 			CString strInfo;
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			std::vector<int> vReadData;
// 			vReadData.resize(5);
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(m_strDataReceived, nReadNum,vReadData,TRUE);
// 
// 			CString strTemp;
// 			CString strTempData;
// 			for (int i = 0; i < vReadData.size(); i++)
// 			{
// 				int nData = vReadData.at(i);	
// 				strTemp.Format(_T("%d,"),nData);
// 				strTempData = strTempData + strTemp;
// 			}
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, strTempData);
// 		}
// 		else if (4 == m_nRegType)	// 无协议
// 		{
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 		else
// 		{
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 	}
}

void CDlgSysSerialCommSetting::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	CString strInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_uiComPort = (unsigned int)m_comboComPort.GetCurSel();
		m_comboBaudRate.GetLBText(m_comboBaudRate.GetCurSel(), strInfo);
		m_pCommInfo->m_uiBaudRate = _tcstoul(strInfo, NULL, 10);
		m_pCommInfo->m_Parity = (SerialParity)m_comboParity.GetCurSel();
		m_comboDataBits.GetLBText(m_comboDataBits.GetCurSel(), strInfo);
		m_pCommInfo->m_uiDataBits = _tcstoul(strInfo, NULL, 10);
		m_pCommInfo->m_StopBits = (SerialStopBits)m_comboStopBits.GetCurSel();
		m_pCommInfo->m_lWriteRegMaxNum = max(m_lWriteRegMaxNum,2);
		m_pCommInfo->m_lReadRegMaxNum = max(m_lReadRegMaxNum,2);
		
		// 命令停止符
		if (m_comboCmdStopChar.GetCurSel() < 1)
		{
			m_pCommInfo->m_strCmdStop = _T("");
		}
		else
		{
			m_comboCmdStopChar.GetLBText(m_comboCmdStopChar.GetCurSel(), strInfo);
			m_pCommInfo->m_strCmdStop = strInfo;
		}
		// 通信停止符
		if (m_comboCommStopChar.GetCurSel() < 1)
		{
			m_pCommInfo->m_strCommStop = _T("");
		}
		else
		{
			m_comboCommStopChar.GetLBText(m_comboCommStopChar.GetCurSel(), strInfo);
//			strInfo = (strInfo == _T("CR")) ? _T("\r") : _T("");
			m_pCommInfo->m_strCommStop = strInfo;
		}

		m_pCommInfo->m_lRegAddress = (long)_tcstoul(m_strRegAddress, NULL, 10);
		m_pCommInfo->m_lTempRegAddress = (long)_tcstoul(m_strTempRegAddress, NULL, 10);

		m_pVisionASM->SetCommParam(m_pCommInfo);

		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			m_pVisionASM->UpdateCommTool();
		}
	}
	else
	{
		vcCommInfo *pCommInfo = m_pVisionASM->GetCommParam();
		if ((pCommInfo == NULL) || (pCommInfo->GetCommType() != eSerial))
		{
			return;
		}
		m_pCommInfo = (vcSerialCommInfo *)pCommInfo;

		// 更新IP控件显示
		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			if ((m_pVisionASM->GetVisionAlignOther() != NULL))
			{
				vcCommInfo *pOtherCommInfo = m_pVisionASM->GetVisionAlignOther()->GetCommParam();
				if ((pOtherCommInfo != NULL) && (pCommInfo->GetCommType() == pOtherCommInfo->GetCommType()))
				{
					vcSerialCommInfo *pSrcCommInfo = (vcSerialCommInfo *)pOtherCommInfo;
					m_lWriteRegMaxNum = abs(pSrcCommInfo->m_lWriteRegMaxNum);
					m_lReadRegMaxNum = abs(pSrcCommInfo->m_lReadRegMaxNum);
					pSrcCommInfo->m_uiComPort = (pSrcCommInfo->m_uiComPort > 10) ? 0 : pSrcCommInfo->m_uiComPort;
					m_comboComPort.SetCurSel(pSrcCommInfo->m_uiComPort);
					strInfo.Format(_T("%d"), pSrcCommInfo->m_uiBaudRate);
					if (m_comboBaudRate.SelectString(-1, strInfo) == CB_ERR)
					{
						m_comboBaudRate.SelectString(-1, _T("9600"));
					}
					m_comboParity.SetCurSel((int)pSrcCommInfo->m_Parity);
					switch (pSrcCommInfo->m_uiDataBits)
					{
					case 6:
						m_comboDataBits.SetCurSel(0); break;
					case 7:
						m_comboDataBits.SetCurSel(1); break;
					case 8:
					default:
						m_comboDataBits.SetCurSel(2); break;
					}
					m_comboStopBits.SetCurSel((int)pSrcCommInfo->m_StopBits);
					// 命令停止符
					if (pSrcCommInfo->m_strCmdStop == _T("\\"))
					{
						m_comboCmdStopChar.SetCurSel(1);
					}
					else
					{
						m_comboCmdStopChar.SetCurSel(0);
					}
					// 通信停止符
					if (pSrcCommInfo->m_strCommStop == _T("CR"))
					{
						m_comboCommStopChar.SetCurSel(1);
					}
					else if (pSrcCommInfo->m_strCommStop == _T("CRLF"))
					{
						m_comboCommStopChar.SetCurSel(2);
					}
					else
					{
						m_comboCommStopChar.SetCurSel(0);
					}
				}
			}
		}
		else
		{
			m_lWriteRegMaxNum = abs(m_pCommInfo->m_lWriteRegMaxNum);
			m_lReadRegMaxNum = abs(m_pCommInfo->m_lReadRegMaxNum);
			m_pCommInfo->m_uiComPort = (m_pCommInfo->m_uiComPort > 10) ? 0 : m_pCommInfo->m_uiComPort;
			m_comboComPort.SetCurSel(m_pCommInfo->m_uiComPort);
			strInfo.Format(_T("%d"), m_pCommInfo->m_uiBaudRate);
			if (m_comboBaudRate.SelectString(-1, strInfo) == CB_ERR)
			{
				m_comboBaudRate.SelectString(-1, _T("9600"));
			}
			m_comboParity.SetCurSel((int)m_pCommInfo->m_Parity);
			switch (m_pCommInfo->m_uiDataBits)
			{
			case 6:
				m_comboDataBits.SetCurSel(0); break;
			case 7:
				m_comboDataBits.SetCurSel(1); break;
			case 8:
			default:
				m_comboDataBits.SetCurSel(2); break;
			}
			m_comboStopBits.SetCurSel((int)m_pCommInfo->m_StopBits);
			// 命令停止符
			if (m_pCommInfo->m_strCmdStop == _T("\\"))
			{
				m_comboCmdStopChar.SetCurSel(1);
			}
			else
			{
				m_comboCmdStopChar.SetCurSel(0);
			}
			// 通信停止符
			if (m_pCommInfo->m_strCommStop == _T("CR"))
			{
				m_comboCommStopChar.SetCurSel(1);
			}
			else if (m_pCommInfo->m_strCommStop == _T("CRLF"))
			{
				m_comboCommStopChar.SetCurSel(2);
			}
			else
			{
				m_comboCommStopChar.SetCurSel(0);
			}
		}


		m_strRegAddress.Format(_T("%d"), m_pCommInfo->m_lRegAddress);
		m_strTempRegAddress.Format(_T("%d"), m_pCommInfo->m_lTempRegAddress);

		UpdateData(FALSE);
	}
}

void CDlgSysSerialCommSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_SERIAL_COMM_CONFIG") == strDlgID)
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

void CDlgSysSerialCommSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysSerialCommSetting)
	DDX_Control(pDX, IDC_BUTTON_MANUALSEND, m_btnSend);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_COMM, m_btnCommSwitch);
	DDX_Control(pDX, IDC_BUTTON_CLEAN_TXDATA, m_btnSDClear);
	DDX_Control(pDX, IDC_BUTTON_CLEAN_RXDATA, m_btnRDClear);
	DDX_Control(pDX, IDC_COMBO_COMM_COMM_STOP_CHAR, m_comboCommStopChar);
	DDX_Control(pDX, IDC_COMBO_COMM_CMD_STOP_CHAR, m_comboCmdStopChar);
	DDX_Control(pDX, IDC_STATIC_LAMP, m_ctrlCommLamp);
	DDX_Control(pDX, IDC_COMBO_COMM_STOP, m_comboStopBits);
	DDX_Control(pDX, IDC_COMBO_COMM_PORT, m_comboComPort);
	DDX_Control(pDX, IDC_COMBO_COMM_DATA, m_comboDataBits);
	DDX_Control(pDX, IDC_COMBO_COMM_CHECK, m_comboParity);
	DDX_Control(pDX, IDC_COMBO_COMM_BAUD, m_comboBaudRate);
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_strDataReceived);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_strDataSending);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDT_REG_ADDRESS, m_strRegAddress);
	//DDX_Radio(pDX, IDC_RADIO_REG_WRITE_WORD, m_nRegType);
	DDX_Radio(pDX, IDC_RADIO_SINGLE_WORD, m_bRadioWordMode);
	DDX_Text(pDX, IDC_EDIT_READ_REG_ADDR_VALUE, m_nEditReadRegAddrValue);
	DDX_Text(pDX, IDC_EDIT_WRITE_REG_ADDR_VALUE, m_nEditWriteRegAddrValue);
	DDX_Text(pDX, IDC_EDIT_WRITE_REG_VALUE, m_nEditWriteRegValue);
	DDX_Text(pDX, IDC_EDIT_READ_REG_VALUE, m_nEditReadRegValue);
	DDX_Control(pDX, IDC_BUTTON_READ_REG, m_btnReadReg);
	DDX_Control(pDX, IDC_BUTTON_WRITE_REG, m_btnWriteReg);
	DDX_Text(pDX, IDC_EDT_TEMP_REG_ADDRESS, m_strTempRegAddress);
	DDX_Text(pDX, IDC_EDT_WRITE_REG_MAXNUM, m_lWriteRegMaxNum);
	DDX_Text(pDX, IDC_EDT_READ_REG_MAXNUM, m_lReadRegMaxNum);
}


BEGIN_MESSAGE_MAP(CDlgSysSerialCommSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysSerialCommSetting)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_RXDATA, OnButtonCleanRxdata)
	ON_BN_CLICKED(IDC_BUTTON_MANUALSEND, OnButtonManualsend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_TXDATA, OnButtonCleanTxdata)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COMM, OnButtonCloseComm)
	ON_EN_SETFOCUS(IDC_EDIT_TXDATA, OnSetfocusEditTxdata)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_PORT, OnSelchangeComboCommPort)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_DATA, OnSelchangeComboCommData)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_COMM_STOP_CHAR, OnSelchangeComboCommCommStopChar)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_CMD_STOP_CHAR, OnSelchangeComboCommCmdStopChar)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_CHECK, OnSelchangeComboCommCheck)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_BAUD, OnSelchangeComboCommBaud)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_STOP, OnSelchangeComboCommStop)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDT_REG_ADDRESS, &CDlgSysSerialCommSetting::OnKillfocusEdtSerialRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_REG_ADDRESS, &CDlgSysSerialCommSetting::OnSetfocusEdtSerialRegAddress)
	//ON_BN_CLICKED(IDC_RADIO_REG_WRITE_WORD, &CDlgSysSerialCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_WRITE_DWORD, &CDlgSysSerialCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_READ_WORD, &CDlgSysSerialCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_READ_DWORD, &CDlgSysSerialCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_FAST, &CDlgSysSerialCommSetting::OnBnClickedRadioRegChange)
	ON_EN_SETFOCUS(IDC_EDIT_READ_REG_ADDR_VALUE, &CDlgSysSerialCommSetting::OnEnSetfocusEditReadRegAddrValue)
	ON_EN_SETFOCUS(IDC_EDIT_WRITE_REG_ADDR_VALUE, &CDlgSysSerialCommSetting::OnEnSetfocusEditWriteRegAddrValue)
	ON_EN_SETFOCUS(IDC_EDIT_WRITE_REG_VALUE, &CDlgSysSerialCommSetting::OnEnSetfocusEditWriteRegValue)
	ON_BN_CLICKED(IDC_BUTTON_READ_REG, &CDlgSysSerialCommSetting::OnBnClickedButtonReadReg)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_REG, &CDlgSysSerialCommSetting::OnBnClickedButtonWriteReg)
	ON_EN_KILLFOCUS(IDC_EDIT_READ_REG_ADDR_VALUE, &CDlgSysSerialCommSetting::OnEnKillfocusEditReadRegAddrValue)
	ON_EN_KILLFOCUS(IDC_EDIT_WRITE_REG_ADDR_VALUE, &CDlgSysSerialCommSetting::OnEnKillfocusEditWriteRegAddrValue)
	ON_EN_KILLFOCUS(IDC_EDT_TEMP_REG_ADDRESS, &CDlgSysSerialCommSetting::OnKillfocusEdtTempRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_TEMP_REG_ADDRESS, &CDlgSysSerialCommSetting::OnSetfocusEdtTempRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_WRITE_REG_MAXNUM, &CDlgSysSerialCommSetting::OnEnSetfocusEdtWriteRegMaxnum)
	ON_EN_SETFOCUS(IDC_EDT_READ_REG_MAXNUM, &CDlgSysSerialCommSetting::OnEnSetfocusEdtReadRegMaxnum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysSerialCommSetting message handlers

BOOL CDlgSysSerialCommSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_LAMP_OK_32), RT_GROUP_ICON);
	m_hIconOpen = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_OPEN_COMM_32), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	m_hIconClosed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_CLOSE_COMM_32), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);	
	UpdateDialogLanguage();
	InitDlgItem();	
	UpdateDlgData(FALSE);

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	CString strCommPortChar;
	m_comboComPort.GetWindowText(strCommPortChar);
	strCommPortOld =strCommPortChar;

	CString strCommBaud;
	m_comboBaudRate.GetWindowText(strCommBaud);
	strCommBaudOld = strCommBaud;

	CString strCommCheck;
	m_comboParity.GetWindowText(strCommCheck);
	strCommCheckOld = strCommCheck;

	CString strCommData;
	m_comboDataBits.GetWindowText(strCommData);
	strCommDataOld = strCommData;

	CString strCommStop;
	m_comboStopBits.GetWindowText(strCommStop);
	strCommStopOld = strCommStop;

	CString strCmdStopChar;
	m_comboCmdStopChar.GetWindowText(strCmdStopChar);
	strCmdStopCharOld = strCmdStopChar;

	CString strCommStopChar;
	m_comboCommStopChar.GetWindowText(strCommStopChar);
	strCommStopCharOld = strCommStopChar;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysSerialCommSetting::OnButtonCloseComm() 
{
	if (m_pVisionASM->IsCommOpen())
	{
		if (m_pVisionASM->CloseComm())
		{
			SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
			m_ctrlCommLamp.SetIcon(m_hIconClosed);
			
			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SERIAL_PORT_CLOSED);	//_T("串口已关闭")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_CLOSE_SERIAL_PORT);	//_T("串口关闭失败")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		EndWaitCursor();
		CString strLog;
		strLog.Format(_T("点击[关闭]"));
		g_opLogFile.SaveLogText(strLog,3);
	}
	else
	{
		UpdateDlgData(TRUE);
		if (m_pVisionASM->OpenComm())
		{
			SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
		    m_ctrlCommLamp.SetIcon(m_hIconOpen);	

			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SERIAL_PORT_OPENED);	//_T("串口已打开")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SERIAL_PORT);	//_T("串口打开失败")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		CString strLog;
		strLog.Format(_T("点击[打开]"));
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysSerialCommSetting::OnButtonManualsend() 
{
	UpdateData(TRUE);
	m_pVisionASM->SendCommInfo(m_strDataSending,TRUE);

	CString strLog;
	strLog.Format(_T("点击[发送数据][发送]"));
	g_opLogFile.SaveLogText(strLog,3);
// 	UpdateData(TRUE);
// 
// 	if (m_pVisionASM->m_XYDCommProtocol.GetProtocolType() > 10)
// 	{
// 		CString strInfo;
// 		if (0 == m_nRegType)		// 写寄存器（WORD）
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nWriteNum = 5;
// 			std::vector<int> vWriteData;
// 
// 			vWriteData.resize(5);
// 			for (int i = 0; i < vWriteData.size(); i++)
// 			{
// 				vWriteData.at(i) = 1 + i;
// 			}
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strInfo,nRegAdrr,nWriteNum,vWriteData,FALSE);
// 
// 			m_pVisionASM->SendCommInfo(strInfo);
// 		}
// 		else if (1 == m_nRegType)   // 写寄存器（DWORD）
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nWriteNum = 5;
// 			std::vector<int> vWriteData;
// 
// 			vWriteData.resize(5);
// 			for (int i = 0; i < vWriteData.size(); i++)
// 			{
// 				vWriteData.at(i) = -100 + i;
// 			}
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strInfo,nRegAdrr,nWriteNum,vWriteData,TRUE);
// 
// 			m_pVisionASM->SendCommInfo(strInfo);
// 		}
// 		else if (2 == m_nRegType)	// 读寄存器（WORD）
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr,nReadNum,FALSE);
// 
// 			m_pVisionASM->SendCommInfo(strInfo);
// 		}
// 		else if (3 == m_nRegType)	// 读寄存器（DWORD）
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr,nReadNum,TRUE);
// 
// 			m_pVisionASM->SendCommInfo(strInfo);
// 		}
// 		else if (4 == m_nRegType)	// 无协议
// 		{
// 			m_pVisionASM->SendCommInfo(m_strDataSending);
// 		}
// 		else
// 		{
// 			m_pVisionASM->SendCommInfo(m_strDataSending);
// 		}
// 	}
// 	else
// 	{
// 		m_pVisionASM->SendCommInfo(m_strDataSending);
// 	}

}

void CDlgSysSerialCommSetting::OnButtonCleanRxdata() 
{
	UpdateData(TRUE);
	m_strDataReceived.Empty();
	CString strLog;
	strLog.Format(_T("点击[接收数据][清空]"));
	g_opLogFile.SaveLogText(strLog,3);
	UpdateData(FALSE);	
}

void CDlgSysSerialCommSetting::OnButtonCleanTxdata() 
{
	UpdateData(TRUE);
	m_strDataSending.Empty();

	CString strLog;
	strLog.Format(_T("点击[发送数据][清空]"));
	g_opLogFile.SaveLogText(strLog,3);

	UpdateData(FALSE);	
}

void CDlgSysSerialCommSetting::OnSetfocusEditTxdata() 
{
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TXDATA)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TXDATA, strInput);
		
		if (TRUE == /*m_pParent->*/m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TXDATA, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[发送数据]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
	}
}

BOOL CDlgSysSerialCommSetting::DestroyWindow() 
{
	m_bExitThread = TRUE;// 使线程自动退出
	//m_pVisionASM->CloseComm();
	m_pVisionASM->StopReceiveComm();
	if (WaitForSingleObject(m_hEventReceiveThreadExit, WAIT_THREAD_QUIT_TIME) == WAIT_TIMEOUT)
	{
		TerminateThread(m_pThreadReceiveData->m_hThread, 0);
	}
	CloseHandle(m_hEventReceiveThreadExit);

	if (m_hIconOpen != NULL)
	{
		DestroyIcon(m_hIconOpen);
		m_hIconOpen = NULL;
	}
	if (m_hIconClosed != NULL)
	{
		DestroyIcon(m_hIconClosed);
		m_hIconClosed = NULL;
	}

	return CDialog::DestroyWindow();
}

// 串口号
void CDlgSysSerialCommSetting::OnSelchangeComboCommPort() 
{
	CString strInfo;
	m_pCommInfo->m_uiComPort = (unsigned int)m_comboComPort.GetCurSel();
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCommPortChar;
	m_comboComPort.GetWindowText(strCommPortChar);

	if (strCommPortOld != strCommPortChar)
	{
		CString strLog;
		strLog.Format(_T("修改[串口号]由%s到%s\n"),strCommPortOld,strCommPortChar);
		g_opLogFile.SaveLogText(strLog,3);
		strCommPortOld = strCommPortChar;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 波特率
void CDlgSysSerialCommSetting::OnSelchangeComboCommBaud() 
{
	CString strInfo;
	m_comboBaudRate.GetLBText(m_comboBaudRate.GetCurSel(), strInfo);
	m_pCommInfo->m_uiBaudRate = _tcstoul(strInfo, NULL, 10);
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCommBaud;
	m_comboBaudRate.GetWindowText(strCommBaud);

	if (strCommBaudOld != strCommBaud)
	{
		CString strLog;
		strLog.Format(_T("修改[波特率]由%s到%s\n"),strCommBaudOld,strCommBaud);
		g_opLogFile.SaveLogText(strLog,3);
		strCommBaudOld = strCommBaud;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 校验方式
void CDlgSysSerialCommSetting::OnSelchangeComboCommCheck() 
{
	m_pCommInfo->m_Parity = (SerialParity)m_comboParity.GetCurSel();
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCommCheck;
	m_comboParity.GetWindowText(strCommCheck);

	if (strCommCheckOld != strCommCheck)
	{
		CString strLog;
		strLog.Format(_T("修改[校验位]由%s到%s\n"),strCommCheckOld,strCommCheck);
		g_opLogFile.SaveLogText(strLog,3);
		strCommCheckOld = strCommCheck;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 数据位
void CDlgSysSerialCommSetting::OnSelchangeComboCommData() 
{
	CString strInfo;
	m_comboDataBits.GetLBText(m_comboDataBits.GetCurSel(), strInfo);
	m_pCommInfo->m_uiDataBits = (unsigned int)_ttoi(strInfo);
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCommData;
	m_comboDataBits.GetWindowText(strCommData);

	if (strCommDataOld != strCommData)
	{
		CString strLog;
		strLog.Format(_T("修改[数据位]由%s到%s\n"),strCommDataOld,strCommData);
		g_opLogFile.SaveLogText(strLog,3);
		strCommDataOld = strCommData;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 停止位
void CDlgSysSerialCommSetting::OnSelchangeComboCommStop() 
{
	m_pCommInfo->m_StopBits = (SerialStopBits)m_comboStopBits.GetCurSel();
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCommStop;
	m_comboStopBits.GetWindowText(strCommStop);

	if (strCommStopOld != strCommStop)
	{
		CString strLog;
		strLog.Format(_T("修改[停止位]由%s到%s\n"),strCommStopOld,strCommStop);
		g_opLogFile.SaveLogText(strLog,3);
		strCommStopOld = strCommStop;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 命令终止符
void CDlgSysSerialCommSetting::OnSelchangeComboCommCmdStopChar() 
{
	CString strInfo;
	if (m_comboCmdStopChar.GetCurSel() < 1)
	{
		m_pCommInfo->m_strCmdStop = _T("");
	}
	else
	{
		m_comboCmdStopChar.GetLBText(m_comboCmdStopChar.GetCurSel(), strInfo);
		m_pCommInfo->m_strCmdStop = strInfo;
	}
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	/////////////////////////////////////////////////////////////////////////////
	CString strCmdStopChar;
	m_comboCmdStopChar.GetWindowText(strCmdStopChar);

	if (strCmdStopCharOld != strCmdStopChar)
	{
		CString strLog;
		strLog.Format(_T("修改[命令终止符]由%s到%s\n"),strCmdStopCharOld,strCmdStopChar);
		g_opLogFile.SaveLogText(strLog,3);
		strCmdStopCharOld = strCmdStopChar;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// 通信终止符
void CDlgSysSerialCommSetting::OnSelchangeComboCommCommStopChar() 
{
	CString strInfo;
	if (m_comboCommStopChar.GetCurSel() < 1)
	{
		m_pCommInfo->m_strCommStop = _T("");
	}
	else
	{
		m_comboCommStopChar.GetLBText(m_comboCommStopChar.GetCurSel(), strInfo);
//		strInfo = (strInfo == _T("CR")) ? _T("\r") : _T("");
		m_pCommInfo->m_strCommStop = strInfo;
	}
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
	}
	///////////////////////////////////////////////////////////////////////////// 
	CString strCommStopChar;
	m_comboCommStopChar.GetWindowText(strCommStopChar);

	if (strCommStopCharOld != strCommStopChar)
	{
		CString strLog;
		strLog.Format(_T("修改[通信终止符]由%s到%s\n"),strCommStopCharOld,strCommStopChar);
		g_opLogFile.SaveLogText(strLog,3);
		strCommStopCharOld = strCommStopChar;
	}
	//////////////////////////////////////////////////////////////////////////////////
}

BOOL CDlgSysSerialCommSetting::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSysSerialCommSetting::ReportCommStatus(BOOL bStatusConnected)
{
	
}

BOOL CDlgSysSerialCommSetting::TestReceiveCommRepeat(CString& strInfo, CommStatus& nCommStatus)
{
	scTimer tTimer;
	while (m_strDataReceivedOnce.IsEmpty())
	{
		if (tTimer.GetTimeMilli(FALSE) >= 3000 /*|| m_pVisionASM->m_CommStatus != eCommSucceed*/)
		{
			nCommStatus = eCommTimeOut;
			//报警信息 _T("通信接收指令停止或超时！");
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_COMM_STOP_OR_TIMEOUT));
			return FALSE;
		}
		Sleep(2);
	}
	strInfo = m_strDataReceivedOnce;
	return TRUE;
}

BOOL CDlgSysSerialCommSetting::TestIsSuccessReceived(CString& strReceivedInfo,CString strReadInfo)
{
	CommStatus nCommStatus = eCommSucceed;
	BOOL bReceived = FALSE;
	m_strDataReceivedOnce.Empty();
	m_strDataSending.Empty();
	if(FALSE == m_pVisionASM->SendCommInfo(strReadInfo,TRUE))      //发送读寄存器指令
	{
		//AfxMessageBox(_T("网络连接未打开，请先打开网络连接！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
		return FALSE;
	}
	m_strDataSending = strReadInfo;        //用于将发送的信息显示在发送数据框中
	SetDlgItemText(IDC_EDIT_TXDATA, m_strDataSending);
	bReceived = TestReceiveCommRepeat(strReceivedInfo,nCommStatus);

	if (bReceived == FALSE || nCommStatus != eCommSucceed)
	{
		////再发一次
		//if(FALSE == m_pVisionASM->SendCommInfo(strReadInfo,FALSE))      //发送读寄存器指令
		//{
		//	//AfxMessageBox(_T("网络连接未打开，请先打开网络连接！"));
		//	AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
		//	return FALSE;
		//}
		//m_strDataSending += strReadInfo;
		//SetDlgItemText(IDC_EDIT_TXDATA, m_strDataSending);

		//// 接收PLC回复
		//nCommStatus = eCommSucceed;
		//bReceived = TestReceiveCommRepeat(strReceivedInfo, nCommStatus);
		//if (bReceived == FALSE || nCommStatus != eCommSucceed)
		//{
		//	//报警 _T("未收到PLC回复！")
		//	//AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PLC_REPLY));
		//	return FALSE;
		//}

		return FALSE;
	}
	return TRUE;
}


void CDlgSysSerialCommSetting::OnKillfocusEdtSerialRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSysSerialCommSetting::OnSetfocusEdtSerialRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_REG_ADDRESS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_REG_ADDRESS, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_REG_ADDRESS, strOutput);

			CString strLog;
			strLog.Format(_T("修改[寄存器首地址]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
	}
}

//void CDlgSysSerialCommSetting::OnBnClickedRadioRegChange()
//{
//	// TODO: 在此添加控件通知处理程序代码m_nRegType
//	int nRegTypeOld = m_nRegType;
//	UpdateData(TRUE);
//	CString strVal[] = {_T("写寄存器(WORD)"), _T("写寄存器(DWORD)"),_T("读寄存器(WORD)"), _T("读寄存器(DWORD)"), _T("无协议")};
//	if (nRegTypeOld != m_nRegType)
//	{
//		CString strLog;
//		strLog.Format(_T("修改[对象到目标Mark角度]由%s到%s\n"),strVal[nRegTypeOld],strVal[m_nRegType]);
//		g_opLogFile.SaveLogText(strLog,3);
//	}
//}

void CDlgSysSerialCommSetting::OnEnSetfocusEditReadRegAddrValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_READ_REG_ADDR_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_READ_REG_ADDR_VALUE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_READ_REG_ADDR_VALUE, strOutput);
		}
	}
	OnEnKillfocusEditReadRegAddrValue();
}


void CDlgSysSerialCommSetting::OnEnSetfocusEditWriteRegAddrValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WRITE_REG_ADDR_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WRITE_REG_ADDR_VALUE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WRITE_REG_ADDR_VALUE, strOutput);
		}
	}
	OnEnKillfocusEditWriteRegAddrValue();
}


void CDlgSysSerialCommSetting::OnEnSetfocusEditWriteRegValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WRITE_REG_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WRITE_REG_VALUE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WRITE_REG_VALUE, strOutput);
		}
	}


}


void CDlgSysSerialCommSetting::OnBnClickedButtonReadReg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strReadInfo;
	CString strReceivedInfo;
	BOOL bReceived = FALSE;
	CommStatus nCommStatus = eCommSucceed;
	std::vector<int> vnReceivedData;
	int	 nDataNum = 1;								//数据量
	vnReceivedData.resize(nDataNum);
	if (m_bRadioWordMode)    //双字节
	{

		long nRegAdrr = m_nEditReadRegAddrValue + atoi(RREG_PLC_CMD_TRIGGER);     //寄存器地址
		m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strReadInfo,nRegAdrr,nDataNum,TRUE);  //打包读寄存器信息
		bReceived = TestIsSuccessReceived(strReceivedInfo,strReadInfo);
		//解析收到的字符串
		if (bReceived)
		{
			if (!m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(strReceivedInfo,nDataNum,vnReceivedData))
			{
				//报错 _T("解析PLC回复的字符串失败！")
				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_ERROR_PLC_REPLY));
			}
			else
			{
				m_nEditReadRegValue = vnReceivedData[0];
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_READ_PLC);	//_T("读取寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}


		}


	}
	else												//单字节
	{
		long nRegAdrr = m_nEditReadRegAddrValue + atoi(RREG_PLC_CMD_TRIGGER_WORD);     //寄存器地址
		m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strReadInfo,nRegAdrr,nDataNum,FALSE);
		bReceived = TestIsSuccessReceived(strReceivedInfo,strReadInfo);

		//解析收到的字符串
		if (bReceived)
		{
			if (!m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(strReceivedInfo,nDataNum,vnReceivedData,FALSE))
			{
				//报错 _T("解析PLC回复的字符串失败！")
				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_ERROR_PLC_REPLY));
			}
			else
			{
				m_nEditReadRegValue = vnReceivedData[0];
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_READ_PLC);	//_T("读取寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}


		}
	}

	UpdateData(FALSE);
}


void CDlgSysSerialCommSetting::OnBnClickedButtonWriteReg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int  nDataNum = 1;												// 数据量
	std::vector<int> vnWriteData;
	vnWriteData.resize(nDataNum);
	vnWriteData.at(0) = m_nEditWriteRegValue;
	CString strWriteInfo;
	BOOL bReceived;
	CString strWriteReceiveInfo;
	CommStatus nCommStatus = eCommSucceed;
	if (m_bRadioWordMode)				   //双字节
	{
		long nRegAdrr = m_nEditWriteRegAddrValue + atoi(RREG_PLC_CMD_TRIGGER);     //寄存器地址
		m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strWriteInfo,nRegAdrr,nDataNum,vnWriteData);
		bReceived = TestIsSuccessReceived(strWriteReceiveInfo,strWriteInfo);
		if (bReceived)
		{
			if (!m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(strWriteReceiveInfo))
			{
				//报错 _T("解析PLC回复的字符串失败！")
				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_ERROR_PLC_REPLY));
			}
			else
			{
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_WRITE_PLC);	//_T("写入寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}
			
		}
	}
	else					//单字节
	{
		long nRegAdrr = m_nEditWriteRegAddrValue + atoi(RREG_PLC_CMD_TRIGGER_WORD);     //寄存器地址
		m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strWriteInfo,nRegAdrr,nDataNum,vnWriteData,FALSE);
		bReceived = TestIsSuccessReceived(strWriteReceiveInfo,strWriteInfo);
		if (bReceived)
		{
			if (!m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(strWriteReceiveInfo,FALSE))
			{
				//报错 _T("解析PLC回复的字符串失败！")
				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_ERROR_PLC_REPLY));
			}
			else
			{
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_WRITE_PLC);	//_T("写入寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}
			
		}

	}
}


void CDlgSysSerialCommSetting::OnEnKillfocusEditReadRegAddrValue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bRadioWordMode)
	{
		if (m_nEditReadRegAddrValue%2  || m_nEditReadRegAddrValue<0)
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_EVEN_NUMBER));    //_T("输入值必须为偶数！")
			SetDlgItemText(IDC_EDIT_READ_REG_ADDR_VALUE,m_strRegAddress);
			return;
		}
	}
	UpdateData(FALSE);
}


void CDlgSysSerialCommSetting::OnEnKillfocusEditWriteRegAddrValue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bRadioWordMode)
	{
		if (m_nEditWriteRegAddrValue%2 || m_nEditWriteRegAddrValue<0)
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_EVEN_NUMBER));    //_T("输入值必须为偶数！")
			SetDlgItemText(IDC_EDIT_WRITE_REG_ADDR_VALUE,m_strRegAddress);
			return;
		}
	}
	UpdateData(FALSE);
}

void CDlgSysSerialCommSetting::OnKillfocusEdtTempRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSysSerialCommSetting::OnSetfocusEdtTempRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TEMP_REG_ADDRESS, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TEMP_REG_ADDRESS, strOutput);
		}
	}
}


void CDlgSysSerialCommSetting::UpdateCommStatus()
{

}

void CDlgSysSerialCommSetting::OnEnSetfocusEdtWriteRegMaxnum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_WRITE_REG_MAXNUM, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_WRITE_REG_MAXNUM, strOutput);

		}

	}
}


void CDlgSysSerialCommSetting::OnEnSetfocusEdtReadRegMaxnum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_READ_REG_MAXNUM, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_READ_REG_MAXNUM, strOutput);

		}

	}
}


void CDlgSysSerialCommSetting::UpdateControlShow(BOOL bVisible)
{
	
	BOOL bShow = bVisible;

	SetGroupBoxStatus(IDC_STATIC_READ_WRITE_REG_TEST,m_bEnableCommTestSet,bShow && m_bshowCommTestSet);

	GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);

	/*int nShow = SW_HIDE;
	if (bVisible)
	{
		nShow = SW_SHOW;
	}

	GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(nShow);

	GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(nShow);

	GetDlgItem(IDC_STATIC_READ_WRITE_REG_TEST)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(nShow);
	GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(nShow);
	GetDlgItem(IDC_RADIO_DOUBLE_TYPE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_READ_REG_ADDR)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_READ_REG_ADDR_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_READ_REG)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_READ_REG_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_READ_REG_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_WRITE_REG_ADDR_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_WRITE_REG)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_REG_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_WRITE_REG_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(nShow);*/

}


//void CDlgSysSerialCommSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
//{
//	CRect CRectGroupBox;
//	// 检查是否为Group控件
//	if(NULL== GetDlgItem(nID)) return;
//	if (NULL== (CStatic*) GetDlgItem(nID)) return;
//	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
//	ScreenToClient(&CRectGroupBox);
//
//	int nShow = SW_SHOW;
//	if (bShow)
//	{
//		nShow = SW_SHOW;
//	}
//	else
//	{
//		nShow = SW_HIDE;
//	}
//
//	// Group 控件本身也进行显示或隐藏
//	GetDlgItem(nID)->EnableWindow(bEnable);
//	GetDlgItem(nID)->ShowWindow(nShow);
//
//	CWnd *pWnd;
//	pWnd = GetWindow(GW_CHILD);
//	while(pWnd!=NULL)
//	{
//		CRect CRect2;
//		pWnd->GetWindowRect(&CRect2);
//		ScreenToClient(&CRect2);
//		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) && CRectGroupBox.PtInRect(CRect2.BottomRight()))
//		{
//			pWnd->EnableWindow(bEnable);
//			pWnd->ShowWindow(nShow);
//		}
//		pWnd = pWnd->GetNextWindow();
//	}
//}

void CDlgSysSerialCommSetting::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			bool bShow = false;
			bool bEnable = false;
			CString strNodeName = _T("");
			// 通信参数
			bool bFind = vsConfig->GetCheckedByItem(eCommAuthorize_ParamSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCommParamSet,m_bEnableCommParamSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_SERIAL_CONFIG,m_bEnableCommParamSet,m_bshowCommParamSet);
			}

			
			// 连接读写测试
			bFind = vsConfig->GetCheckedByItem(eCommAuthorize_TestSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCommTestSet,m_bEnableCommTestSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_SEND_DATA,m_bEnableCommTestSet,m_bshowCommTestSet);
				SetGroupBoxStatus(IDC_STATIC_GRP_RECEIVE_DATA,m_bEnableCommTestSet,m_bshowCommTestSet);
				SetGroupBoxStatus(IDC_STATIC_READ_WRITE_REG_TEST,m_bEnableCommTestSet,m_bshowCommTestSet);
			}




		}
	}
}