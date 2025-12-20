// DlgSysSocketCommTest.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysSocketCommSetting.h"
#include "DlgSysCommSetting.h"

#include "DlgSysEnvironSetting.h"

#include "CommProtocolProcess.h"

#include <fstream>
#include <string>  
#include <iostream> 
#include <sstream>
#include <vector>


#include "MOTIONDLL.H"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysSocketCommSetting dialog


CDlgSysSocketCommSetting::CDlgSysSocketCommSetting(CWnd* pParent)
	: CDlgSysBaseCommSetting(CDlgSysSocketCommSetting::IDD, pParent)
	, m_nEditRegReadValue(0)
	, m_nEditRegWriteValue(0)
	, m_nWordTypeMode(1)
	, m_lReadRegMaxNum(100)
	, m_lWriteRegMaxNum(100)
	, m_dWZW(1)
	, m_dWZR(1)
	, m_dWZY1(0)
	, m_dWZZ1(0)
	, m_dWZY2(0)
	, m_dWZZ2(0)
	, m_dWZY3(0)
	, m_dWZZ3(0)
	, m_dWZG(1000)
	, m_strResult(_T(""))
	, m_szGetName(_T(""))
	, m_nYZRSendType(0)
	, m_lYZRSendAddr(0)
	, m_nIDLength(17)
	, m_bEnableIDRaed(FALSE)
	, m_bIDInvert(FALSE)
{
	//{{AFX_DATA_INIT(CDlgSysSocketCommSetting)
	m_strDataReceived = _T("");
	m_strDataReceivedOnce = _T("");
	m_strDataSending = _T("");
	m_strPort = _T("5000");
	m_nServerType = 1;
	//}}AFX_DATA_INIT
	//m_pParent = (CDlgSysCommSetting *)pParent;
	m_pParent = (CDlgSysEnvironSetting *)pParent;
	m_pCommInfo = NULL;
	m_strRegAddress = _T("100");
	m_strWZRegAddress = _T("500");
	m_strRegAddress3 = _T("200");
	m_nEditTestReadRegAddr = _ttoi(m_strRegAddress);
	m_nEditTestWriteRegAddr = _ttoi(m_strRegAddress);
	m_strTempRegAddress = _T("200");
	m_nRegType = 0;

	m_bshowCommParamSet = true;
	m_bEnableCommParamSet= true;

	m_bshowCommTestSet = true;
	m_bEnableCommTestSet = true;
}

void CDlgSysSocketCommSetting::InitDlgItem()
{
	UpdateDlgByAuthorize();

	m_comboCmdStopChar.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_NO_STOP_CHAR));		//_T("无")
	m_comboCmdStopChar.AddString(_T("\\"));
	m_comboCommStopChar.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_NO_STOP_CHAR));	//_T("无")
	m_comboCommStopChar.AddString(_T("CR"));
	m_comboCommStopChar.AddString(_T("CRLF"));

	m_comboCmdStopChar.SetCurSel(1);
	m_comboCommStopChar.SetCurSel(1);
//	m_comboCmdStopChar.EnableWindow(FALSE);
// 	m_comboCommStopChar.EnableWindow(FALSE);


	if (m_pVisionASM->m_XYDCommProtocol.IsPlcProtocolType())
	{
		GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);

		GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(m_bshowCommParamSet);

		GetDlgItem(IDC_STATIC_TEST_REG)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_BUTTON_REG_READ)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_READ_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_REG_READ_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_TEST_RED_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_BUTTON_REG_WRITE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WRITE_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_REG_WRITE_VALUE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_RADIO_DOUBLE_WORD)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDIT_TEST_WRITE_REG_ADDR)->ShowWindow(m_bshowCommTestSet);
		GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(m_bshowCommParamSet);
		GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(m_bshowCommParamSet);
	}
	else
	{

		GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TEST_REG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_REG_READ)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_READ_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_REG_READ_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TEST_RED_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_REG_WRITE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_REG_WRITE_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_DOUBLE_WORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TEST_WRITE_REG_ADDR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(SW_HIDE);
	}

	

	if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
	{
		GetDlgItem(IDC_RAD_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RAD_CLIENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPA_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_CMD_STOP_CHAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COMM_COMM_STOP_CHAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE_COMM)->EnableWindow(FALSE);
	}

	// 打开\关闭通信连接
	m_btnCommSwitch.SetThemeHelper(&m_themeHelper);
	m_btnCommSwitch.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	if (m_pVisionASM != NULL)
	{
		m_pCommInfo = (vcSocketCommInfo *)m_pVisionASM->GetCommParam();
		if (m_pVisionASM->IsCommOpen())
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//			m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
			m_ctrlCommLamp.SetIcon(m_hIconOpen);

			m_pParent->m_bStatusBarFlag = TRUE;
			CString str = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//			m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
			m_ctrlCommLamp.SetIcon(m_hIconClosed);

			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
			if (m_pCommInfo->m_socketType == eSTServer)
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);		//_T("端口被占用")
			}
			else
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
			}
			m_pParent->UpdateStatusBar();
		}
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		ShowBendPathCalc(m_bshowCommTestSet && sysPlatformInfo.m_bShowBendPathCalc);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);

		m_pParent->m_bStatusBarFlag = FALSE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_VISION_ALIGN_TOOL);
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
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

	m_btnRegRead.SetThemeHelper(&m_themeHelper);
	m_btnRegRead.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnRegWrite.SetThemeHelper(&m_themeHelper);
	m_btnRegWrite.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	WZInitDlgItem() ;
}

void CDlgSysSocketCommSetting ::WZInitDlgItem() 
{
	// List_Simple
	CRect rcListRectSimple;
	m_ListWZ.GetClientRect(rcListRectSimple);
	m_ListWZ.ClientToScreen(rcListRectSimple);
	ScreenToClient(rcListRectSimple);

	int nSimpleColumn = 0;
	// 0序号、1 YN、2 ZN

	m_ListWZ.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rcListRectSimple.Width()/5);
	m_ListWZ.InsertColumn(1, _T(" YN"), LVCFMT_CENTER, rcListRectSimple.Width()/4);
	m_ListWZ.InsertColumn(2, _T(" ZN"), LVCFMT_CENTER, rcListRectSimple.Width()/4);
	m_ListWZ.InsertColumn(3, _T(" RN"), LVCFMT_CENTER, rcListRectSimple.Width()/4);
	// 0X差值、1Y差值、2距离、3目标距离、4对象距离、5Y向偏差、6X向偏差
	int nSimpleItem = 0;
	for(int i = 0; i < 100; i++)
	{
		CString str ;
		str.Format(_T("%d"),i+1);
		m_ListWZ.InsertItem(i,str);
		//nSimpleItem++;
	}

	//m_nListSimpleItemNum = nSimpleItem;
	m_ListWZ.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);


}

void CDlgSysSocketCommSetting::UpdateList()
{

	CString strTmp;
	for(int i=0;i<100;i++)
	{
		strTmp.Format(_T("%.4f"),m_vdYN.at(i));
		m_strListWZ[i][1] = strTmp;

		strTmp.Format(_T("%.4f"),m_vdZN.at(i));
		m_strListWZ[i][2] = strTmp;


		strTmp.Format(_T("%.4f"),m_vdRN.at(i));
		m_strListWZ[i][3] = strTmp;
	}
	// 刷新整个表格
	for(int i = 1; i < 4; i++)
	{
		for(int j = 0; j<100; j++)
		{
			m_ListWZ.SetItemText(j,i,m_strListWZ[j][i]);
		}
	}


	Invalidate(TRUE);
};

UINT CDlgSysSocketCommSetting::ThreadReceiveData( LPVOID pParam )
{
	CDlgSysSocketCommSetting *pOwner = (CDlgSysSocketCommSetting*)pParam;
	CString strReceived;
	pOwner->m_strDataReceivedOnce.Empty();
	while (!pOwner->m_bExitThread)
	{
		if ((pOwner->IsVisible()) && (pOwner->m_pVisionASM != NULL) && (pOwner->m_pVisionASM->IsCommOpen()))
		{
			//if (pOwner->m_pVisionASM->ReceiveCommInfo(strReceived))
			if (pOwner->m_pVisionASM->ReceiveCommInfo(pOwner->m_strDataReceivedOnce))
			{
				//pOwner->m_strDataReceived += strReceived;
				pOwner->m_strDataReceived += pOwner->m_strDataReceivedOnce;
				pOwner->UpdateReceiveData();				
			}
		}
		Sleep(10);
	}
	
	SetEvent(pOwner->m_hEventReceiveThreadExit);
	return 0;
}

void CDlgSysSocketCommSetting::UpdateReceiveData()
{
	SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);

// 	if (m_pVisionASM->m_XYDCommProtocol.GetProtocolType() > 10)
// 	{
// 		if (0 == m_nRegType)		// 写寄存器(WORD)
// 		{
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(m_strDataReceived);
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 		else if (1 == m_nRegType)	// 写寄存器(DWORD)
// 		{
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswWriteReg(m_strDataReceived);
// 
// 			SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 		}
// 		else if (2 == m_nRegType)	// 读寄存器(WORD)
// 		{
// 			CString strInfo;
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			std::vector<int> vReadData;
// 			vReadData.resize(5);
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(m_strDataReceived, nReadNum,vReadData, FALSE);
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
// 		else if (3 == m_nRegType)	// 读寄存器(DWORD)
// 		{
// 			CString strInfo;
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 			std::vector<int> vReadData;
// 
// 			vReadData.resize(5);
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->AnalyzeAswReadReg(m_strDataReceived, nReadNum,vReadData, TRUE);
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
// 
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_EDIT_RXDATA, m_strDataReceived);
// 	}

}

void CDlgSysSocketCommSetting::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	CString strInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_socketType = (SocketType)m_nServerType;
		m_ctrlIPAddress.GetAddress(m_pCommInfo->m_byAddr[0], m_pCommInfo->m_byAddr[1], m_pCommInfo->m_byAddr[2], m_pCommInfo->m_byAddr[3]);
		m_pCommInfo->m_uiPort = (unsigned int)_tcstoul(m_strPort, NULL, 10);
		m_pCommInfo->m_lRegAddress = (long)_tcstoul(m_strRegAddress, NULL, 10);
		m_pCommInfo->m_lTempRegAddress = (long)_tcstoul(m_strTempRegAddress, NULL, 10);
		m_pCommInfo->m_lWriteRegMaxNum = max(m_lWriteRegMaxNum,2);
		m_pCommInfo->m_lReadRegMaxNum = max(m_lReadRegMaxNum,2);

		m_pCommInfo->m_lWZRegAddress = (long)_tcstoul(m_strWZRegAddress, NULL, 10);
		m_pCommInfo->m_lRegAddress3 = (long)_tcstoul(m_strRegAddress3, NULL, 10);
		m_pCommInfo->m_szGetName = m_szGetName;//龙哥这个地方是不是没保存
		//m_pCommInfo->m_dWZW = m_dWZW;//龙哥这个地方是不是没保存
		//m_pCommInfo->m_dWZR = m_dWZR;
		//m_pCommInfo->m_dWZY1 = m_dWZY1;
		//m_pCommInfo->m_dWZY2 = m_dWZY2;
		//m_pCommInfo->m_dWZY3 = m_dWZY3;
		//m_pCommInfo->m_dWZZ1 = m_dWZZ1;
		//m_pCommInfo->m_dWZZ2 = m_dWZZ2;
		//m_pCommInfo->m_dWZZ3 = m_dWZZ3;
		//现在将折弯的参数保存在产品中

		
		
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL != vsConfig)
		{
			if(vsConfig->m_TotalProductData.GetCount() > 0)
			{
				int nCurProductIndex = vsConfig->m_TotalProductData.GetCurProductIndex();
				CProductData* pCurProductData = vsConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
				CBendInfo BendInfo = pCurProductData->m_pAlignerParam->GetBendInfo();
				if(fabs( (BendInfo.m_dBendSendAdsorptionSite-m_dWZW))>0.0001||
					fabs(BendInfo.m_dBendSendRAngle-m_dWZR)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateY1-m_dWZY1)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateY2-m_dWZY2)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateY3-m_dWZY3)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateZ1-m_dWZZ1)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateZ2-m_dWZZ2)>0.0001||
					fabs(BendInfo.m_dBendSendCompensateZ3-m_dWZZ3)>0.0001)
				{
					BendInfo.m_dBendSendszGetName = m_szGetName;
					BendInfo.m_dBendSendAdsorptionSite=m_dWZW;
					BendInfo.m_dBendSendRAngle=m_dWZR;
					BendInfo.m_dBendSendCompensateY1=m_dWZY1;
					BendInfo.m_dBendSendCompensateY2=m_dWZY2;
					BendInfo.m_dBendSendCompensateY3=m_dWZY3;
					BendInfo.m_dBendSendCompensateZ1=m_dWZZ1;
					BendInfo.m_dBendSendCompensateZ2=m_dWZZ2;
					BendInfo.m_dBendSendCompensateZ3=m_dWZZ3;

					pCurProductData->m_pAlignerParam->SetBendInfo(BendInfo);
					m_pVisionASM->SaveProductDataInfo(nCurProductIndex);
				}
			}
		}

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

		m_pCommInfo->m_bEnableIDRaed = m_bEnableIDRaed;
		m_pCommInfo->m_nIDLength = m_nIDLength;
		m_pCommInfo->m_bIDInvert = m_bIDInvert;

		m_pVisionASM->SetCommParam(m_pCommInfo);

		
	

		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			m_pVisionASM->UpdateCommTool();
		}
	} 
	else
	{		
		vcCommInfo *pCommInfo = m_pVisionASM->GetCommParam();
		if ((pCommInfo == NULL) || (pCommInfo->GetCommType() != eSocket))
		{
			return;
		}
		m_pCommInfo = (vcSocketCommInfo *)pCommInfo;

		// 更新IP控件显示
		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			if ((m_pVisionASM->GetVisionAlignOther() != NULL))
			{
				vcCommInfo *pOtherCommInfo = m_pVisionASM->GetVisionAlignOther()->GetCommParam();
				if ((pOtherCommInfo != NULL) && (pCommInfo->GetCommType() == pOtherCommInfo->GetCommType()))
				{
					vcSocketCommInfo *pSrcCommInfo = (vcSocketCommInfo *)pOtherCommInfo;
					m_lWriteRegMaxNum = abs(pSrcCommInfo->m_lWriteRegMaxNum);
					m_lReadRegMaxNum = abs(pSrcCommInfo->m_lReadRegMaxNum);
					m_nServerType = (int)((pSrcCommInfo->m_socketType == eSTUnknown) ? eSTServer : pSrcCommInfo->m_socketType);
					m_ctrlIPAddress.SetAddress(pSrcCommInfo->m_byAddr[0], pSrcCommInfo->m_byAddr[1], pSrcCommInfo->m_byAddr[2], pSrcCommInfo->m_byAddr[3]);
					m_strPort.Format(_T("%d"), pSrcCommInfo->m_uiPort);
					
					
	/*				m_strRegAddress3.Format(_T("%d"), pSrcCommInfo->m_lRegAddress3);
					m_strWZRegAddress.Format(_T("%d"), pSrcCommInfo->m_lWZRegAddress);;
					m_dWZW = pSrcCommInfo->m_dWZW;
					m_dWZR = pSrcCommInfo->m_dWZR;
					m_dWZY1 = pSrcCommInfo->m_dWZY1;
					m_dWZY2 = pSrcCommInfo->m_dWZY2;
					m_dWZY3 = pSrcCommInfo->m_dWZY3;
					m_dWZZ1 = pSrcCommInfo->m_dWZZ1;
					m_dWZZ2 = pSrcCommInfo->m_dWZZ2;
					m_dWZZ3 = pSrcCommInfo->m_dWZZ3;*/


					//现在将折弯的参数保存在产品中
					CVisionASMConfig *vsConfig;
					m_pVisionASM->GetVSconfig(vsConfig);
					if (NULL != vsConfig)
					{
						if(vsConfig->m_TotalProductData.GetCount() > 0)
						{
							int nCurProductIndex = vsConfig->m_TotalProductData.GetCurProductIndex();
							CProductData* pCurProductData = vsConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
							CBendInfo BendInfo = pCurProductData->m_pAlignerParam->GetBendInfo();

							m_szGetName=BendInfo.m_dBendSendszGetName;
							m_dWZW=BendInfo.m_dBendSendAdsorptionSite;
							m_dWZR=BendInfo.m_dBendSendRAngle;
							m_dWZY1=BendInfo.m_dBendSendCompensateY1;
							m_dWZY2=BendInfo.m_dBendSendCompensateY2;
							m_dWZY3=BendInfo.m_dBendSendCompensateY3;
							m_dWZZ1=BendInfo.m_dBendSendCompensateZ1;
							m_dWZZ2=BendInfo.m_dBendSendCompensateZ2;
							m_dWZZ3=BendInfo.m_dBendSendCompensateZ3;
						}
						
					}

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
			m_nServerType = (int)((m_pCommInfo->m_socketType == eSTUnknown) ? eSTServer : m_pCommInfo->m_socketType);
			m_ctrlIPAddress.SetAddress(m_pCommInfo->m_byAddr[0], m_pCommInfo->m_byAddr[1], m_pCommInfo->m_byAddr[2], m_pCommInfo->m_byAddr[3]);
			m_strPort.Format(_T("%d"), m_pCommInfo->m_uiPort);
			m_lWriteRegMaxNum = abs(m_pCommInfo->m_lWriteRegMaxNum);
			m_lReadRegMaxNum = abs(m_pCommInfo->m_lReadRegMaxNum);
			if (m_nServerType == 0)
			{
				GetDlgItem(IDC_IPA_IPADDRESS)->EnableWindow(FALSE);
			}
			else if (m_nServerType == 1)
			{
				GetDlgItem(IDC_IPA_IPADDRESS)->EnableWindow(m_bEnableCommParamSet);
			}
			//m_strRegAddress3.Format(_T("%d"), m_pCommInfo->m_lRegAddress3);
			//m_strWZRegAddress.Format(_T("%d"), m_pCommInfo->m_lWZRegAddress);
			//m_dWZW = m_pCommInfo->m_dWZW;
			//m_dWZR = m_pCommInfo->m_dWZR;
			//m_dWZY1 = m_pCommInfo->m_dWZY1;
			//m_dWZY2 = m_pCommInfo->m_dWZY2;
			//m_dWZY3 = m_pCommInfo->m_dWZY3;
			//m_dWZZ1 = m_pCommInfo->m_dWZZ1;
			//m_dWZZ2 = m_pCommInfo->m_dWZZ2;
			//m_dWZZ3 = m_pCommInfo->m_dWZZ3;

			CVisionASMConfig *vsConfig;
			m_pVisionASM->GetVSconfig(vsConfig);
			if (NULL != vsConfig)
			{
				if(vsConfig->m_TotalProductData.GetCount() > 0)
				{
					int nCurProductIndex = vsConfig->m_TotalProductData.GetCurProductIndex();
					CProductData* pCurProductData = vsConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
					CBendInfo BendInfo = pCurProductData->m_pAlignerParam->GetBendInfo();

					m_szGetName=BendInfo.m_dBendSendszGetName;
					m_dWZW=BendInfo.m_dBendSendAdsorptionSite;
					m_dWZR=BendInfo.m_dBendSendRAngle;
					m_dWZY1=BendInfo.m_dBendSendCompensateY1;
					m_dWZY2=BendInfo.m_dBendSendCompensateY2;
					m_dWZY3=BendInfo.m_dBendSendCompensateY3;
					m_dWZZ1=BendInfo.m_dBendSendCompensateZ1;
					m_dWZZ2=BendInfo.m_dBendSendCompensateZ2;
					m_dWZZ3=BendInfo.m_dBendSendCompensateZ3;
				}
			}

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
		m_strWZRegAddress.Format(_T("%d"), m_pCommInfo->m_lWZRegAddress);
		m_szGetName =	m_pCommInfo->m_szGetName;
		m_strRegAddress3.Format(_T("%d"), m_pCommInfo->m_lRegAddress3);

		//m_dWZW = m_pCommInfo->m_dWZW;
		//m_dWZR = m_pCommInfo->m_dWZR;
		//m_dWZY1 = m_pCommInfo->m_dWZY1;
		//m_dWZY2 = m_pCommInfo->m_dWZY2;
		//m_dWZY3 = m_pCommInfo->m_dWZY3;
		//m_dWZZ1 = m_pCommInfo->m_dWZZ1;
		//m_dWZZ2 = m_pCommInfo->m_dWZZ2;
		//m_dWZZ3 = m_pCommInfo->m_dWZZ3;

		m_bEnableIDRaed = m_pCommInfo->m_bEnableIDRaed;
		m_nIDLength = m_pCommInfo->m_nIDLength;
		m_bIDInvert = m_pCommInfo->m_bIDInvert;

		UpdateData(FALSE);
	}
}

void CDlgSysSocketCommSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_SOCKET_COMM_CONFIG") == strDlgID)
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

void CDlgSysSocketCommSetting::ReportCommStatus(BOOL bStatusConnected)
{
	if (bStatusConnected)
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
		
		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);
		
		m_pParent->m_bStatusBarFlag = FALSE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
		if (m_pCommInfo->m_socketType == eSTServer)
		{
			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
		}
		else
		{
			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
		}
		m_pParent->UpdateStatusBar();
	}
}

BOOL CDlgSysSocketCommSetting::TestReceiveCommRepeat(CString& strInfo, CommStatus& nCommStatus)
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
	strInfo = m_strDataReceivedOnce;    //ThreadReceiveData()中接收的信息

	return TRUE;
}

BOOL CDlgSysSocketCommSetting::TestIsSuccessReceived(CString& strReceivedInfo,CString strReadInfo)
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
	m_strDataSending = strReadInfo;     //用于将发送的信息显示在发送数据框中
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

void CDlgSysSocketCommSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysSocketCommSetting)
	DDX_Control(pDX, IDC_BUTTON_MANUALSEND, m_btnSend);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_COMM, m_btnCommSwitch);
	DDX_Control(pDX, IDC_BUTTON_CLEAN_TXDATA, m_btnSDClear);
	DDX_Control(pDX, IDC_BUTTON_CLEAN_RXDATA, m_btnRDClear);
	DDX_Control(pDX, IDC_COMBO_COMM_COMM_STOP_CHAR, m_comboCommStopChar);
	DDX_Control(pDX, IDC_COMBO_COMM_CMD_STOP_CHAR, m_comboCmdStopChar);
	DDX_Control(pDX, IDC_STATIC_LAMP, m_ctrlCommLamp);
	DDX_Control(pDX, IDC_IPA_IPADDRESS, m_ctrlIPAddress);
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_strDataReceived);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_strDataSending);
	DDX_Text(pDX, IDC_EDT_PORT, m_strPort);
	DDX_Radio(pDX, IDC_RAD_SERVER, m_nServerType);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDT_REG_ADDRESS, m_strRegAddress);
	//DDX_Radio(pDX, IDC_RADIO_REG_WRITE_WORD, m_nRegType);
	DDX_Text(pDX, IDC_EDIT_REG_READ_VALUE, m_nEditRegReadValue);
	DDX_Text(pDX, IDC_EDIT_REG_WRITE_VALUE, m_nEditRegWriteValue);
	DDX_Control(pDX, IDC_BUTTON_REG_READ, m_btnRegRead);
	DDX_Control(pDX, IDC_BUTTON_REG_WRITE, m_btnRegWrite);
	DDX_Text(pDX, IDC_EDIT_TEST_RED_REG_ADDR, m_nEditTestReadRegAddr);
	DDX_Radio(pDX, IDC_RADIO_SINGLE_WORD, m_nWordTypeMode);
	DDX_Text(pDX, IDC_EDIT_TEST_WRITE_REG_ADDR, m_nEditTestWriteRegAddr);
	DDX_Text(pDX, IDC_EDT_TEMP_REG_ADDRESS, m_strTempRegAddress);
	DDX_Text(pDX, IDC_EDT_READ_REG_MAXNUM, m_lReadRegMaxNum);
	DDX_Text(pDX, IDC_EDT_WRITE_REG_MAXNUM, m_lWriteRegMaxNum);

	DDX_Text(pDX, IDC_EDT_WZ_REG_ADDRESS, m_strWZRegAddress);
	DDX_Text(pDX, IDC_EDT_REG_ADDRESS3, m_strRegAddress3);
	DDX_Text(pDX, IDC_EDIT_WZ_W, m_dWZW);
	DDX_Text(pDX, IDC_EDIT_WZ_R, m_dWZR);
	DDX_Text(pDX, IDC_EDIT_WZ_Y1, m_dWZY1);
	DDX_Text(pDX, IDC_EDIT_WZ_Z1, m_dWZZ1);
	DDX_Text(pDX, IDC_EDIT_WZ_Y2, m_dWZY2);
	DDX_Text(pDX, IDC_EDIT_WZ_Z2, m_dWZZ2);
	DDX_Text(pDX, IDC_EDIT_WZ_Y3, m_dWZY3);
	DDX_Text(pDX, IDC_EDIT_WZ_Z3, m_dWZZ3);
	DDX_Text(pDX, IDC_EDIT_WZ_G, m_dWZG);
	DDX_Text(pDX, IDC_EDIT_WZ_OKNG, m_strResult);
	DDX_Text(pDX, IDC_EDIT_WZ_TEXT, m_strResultShow);
	DDX_Control(pDX, IDC_WZ_LIST, m_ListWZ);
	DDX_Text(pDX, IDC_EDIT1, m_szGetName);
	DDX_Radio(pDX, IDC_RADIO_YZR1, m_nYZRSendType);
	DDX_Text(pDX, IDC_EDIT_YZR_SEND_ADDR, m_lYZRSendAddr);
	DDX_Text(pDX, IDC_EDT_ID_LENGTH, m_nIDLength);
	DDX_Check(pDX, IDC_CHECK_ID_READ, m_bEnableIDRaed);
	DDX_Check(pDX, IDC_CHECK_ID_INVERT, m_bIDInvert);
}


BEGIN_MESSAGE_MAP(CDlgSysSocketCommSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysSocketCommSetting)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COMM, OnButtonCloseComm)
	ON_BN_CLICKED(IDC_BUTTON_MANUALSEND, OnButtonManualsend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_TXDATA, OnButtonCleanTxdata)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_RXDATA, OnButtonCleanRxdata)
	ON_EN_SETFOCUS(IDC_EDIT_TXDATA, OnSetfocusEditTxdata)
	ON_BN_CLICKED(IDC_RAD_SERVER, OnSocketTypeChange)
	ON_EN_KILLFOCUS(IDC_EDT_PORT, OnKillfocusEdtPort)
	ON_EN_KILLFOCUS(IDC_IPA_IPADDRESS, OnKillfocusIpaIpaddress)
	ON_EN_SETFOCUS(IDC_EDT_PORT, OnSetfocusEdtPort)
	ON_EN_SETFOCUS(IDC_IPA_IPADDRESS, OnSetfocusIpaIpaddress)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_CMD_STOP_CHAR, OnSelchangeComboCommCmdStopChar)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_COMM_STOP_CHAR, OnSelchangeComboCommCommStopChar)
	ON_BN_CLICKED(IDC_RAD_CLIENT, OnSocketTypeChange)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDT_REG_ADDRESS, &CDlgSysSocketCommSetting::OnKillfocusEdtRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_REG_ADDRESS, &CDlgSysSocketCommSetting::OnSetfocusEdtRegAddress)
	//ON_BN_CLICKED(IDC_RADIO_REG_WRITE_WORD, &CDlgSysSocketCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_WRITE_DWORD, &CDlgSysSocketCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_READ_WORD, &CDlgSysSocketCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_REG_READ_DWORD, &CDlgSysSocketCommSetting::OnBnClickedRadioRegChange)
	//ON_BN_CLICKED(IDC_RADIO_FAST, &CDlgSysSocketCommSetting::OnBnClickedRadioRegChange)
	ON_EN_SETFOCUS(IDC_EDIT_REG_WRITE_VALUE, &CDlgSysSocketCommSetting::OnEnSetfocusEditRegWriteValue)
	ON_BN_CLICKED(IDC_BUTTON_REG_READ, &CDlgSysSocketCommSetting::OnBnClickedButtonRegRead)
	ON_BN_CLICKED(IDC_BUTTON_REG_WRITE, &CDlgSysSocketCommSetting::OnBnClickedButtonRegWrite)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_RED_REG_ADDR, &CDlgSysSocketCommSetting::OnEnSetfocusEditTestReadRegAddr)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_WRITE_REG_ADDR, &CDlgSysSocketCommSetting::OnEnSetfocusEditTestWriteRegAddr)
	ON_EN_KILLFOCUS(IDC_EDIT_TEST_RED_REG_ADDR, &CDlgSysSocketCommSetting::OnEnKillfocusEditTestReadRegAddr)
	ON_EN_KILLFOCUS(IDC_EDIT_TEST_WRITE_REG_ADDR, &CDlgSysSocketCommSetting::OnEnKillfocusEditTestWriteRegAddr)
	ON_EN_KILLFOCUS(IDC_EDT_TEMP_REG_ADDRESS, &CDlgSysSocketCommSetting::OnKillfocusEdtTempRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_TEMP_REG_ADDRESS, &CDlgSysSocketCommSetting::OnSetfocusEdtTempRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_WRITE_REG_MAXNUM, &CDlgSysSocketCommSetting::OnEnSetfocusEdtWriteRegMaxnum)
	ON_EN_SETFOCUS(IDC_EDT_READ_REG_MAXNUM, &CDlgSysSocketCommSetting::OnEnSetfocusEdtReadRegMaxnum)

	ON_EN_KILLFOCUS(IDC_EDT_REG_ADDRESS3, &CDlgSysSocketCommSetting::OnKillfocusEdtRegAddress3)
	ON_EN_SETFOCUS(IDC_EDT_REG_ADDRESS3, &CDlgSysSocketCommSetting::OnSetfocusEdtRegAddress3)

	ON_EN_KILLFOCUS(IDC_EDT_WZ_REG_ADDRESS, &CDlgSysSocketCommSetting::OnKillfocusEdtWZRegAddress)
	ON_EN_SETFOCUS(IDC_EDT_WZ_REG_ADDRESS, &CDlgSysSocketCommSetting::OnSetfocusEdtWZRegAddress)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_W, &CDlgSysSocketCommSetting::OnKillfocusEdtWZW)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_W, &CDlgSysSocketCommSetting::OnSetfocusEdtWZW)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_R, &CDlgSysSocketCommSetting::OnKillfocusEdtWZR)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_R, &CDlgSysSocketCommSetting::OnSetfocusEdtWZR)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Y1, &CDlgSysSocketCommSetting::OnKillfocusEdtWZY1)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Y1, &CDlgSysSocketCommSetting::OnSetfocusEdtWZY1)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Z1, &CDlgSysSocketCommSetting::OnKillfocusEdtWZZ1)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Z1, &CDlgSysSocketCommSetting::OnSetfocusEdtWZZ1)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Y2, &CDlgSysSocketCommSetting::OnKillfocusEdtWZY2)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Y2, &CDlgSysSocketCommSetting::OnSetfocusEdtWZY2)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Z2, &CDlgSysSocketCommSetting::OnKillfocusEdtWZZ2)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Z2, &CDlgSysSocketCommSetting::OnSetfocusEdtWZZ2)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Y3, &CDlgSysSocketCommSetting::OnKillfocusEdtWZY3)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Y3, &CDlgSysSocketCommSetting::OnSetfocusEdtWZY3)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_Z3, &CDlgSysSocketCommSetting::OnKillfocusEdtWZZ3)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_Z3, &CDlgSysSocketCommSetting::OnSetfocusEdtWZZ3)
	ON_EN_KILLFOCUS(IDC_EDIT_WZ_G, &CDlgSysSocketCommSetting::OnKillfocusEdtWZG)
	ON_EN_SETFOCUS(IDC_EDIT_WZ_G, &CDlgSysSocketCommSetting::OnSetfocusEdtWZG)

	ON_BN_CLICKED(IDC_BUTTON_WZ_CALCULATE, &CDlgSysSocketCommSetting::OnBnClickedButtonWzCalculate)
	ON_BN_CLICKED(IDC_BUTTON_WZ_SENT, &CDlgSysSocketCommSetting::OnBnClickedButtonWzSent)

	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSysSocketCommSetting::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgSysSocketCommSetting::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgSysSocketCommSetting::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_YZR_ONE_SEND, &CDlgSysSocketCommSetting::OnBnClickedButtonYzrOneSend)
	ON_EN_SETFOCUS(IDC_EDIT_YZR_SEND_ADDR, &CDlgSysSocketCommSetting::OnEnSetfocusEditYzrSendAddr)
	ON_EN_SETFOCUS(IDC_EDT_ID_LENGTH, &CDlgSysSocketCommSetting::OnEnSetfocusEdtIdLength)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysSocketCommSetting message handlers


BOOL CDlgSysSocketCommSetting::OnInitDialog() 
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

	CString strCmdStopChar;
	m_comboCmdStopChar.GetWindowText(strCmdStopChar);
	strCmdStopCharOld = strCmdStopChar;
	CString strCommStopChar;
	m_comboCommStopChar.GetWindowText(strCommStopChar);
	strCommStopCharOld = strCommStopChar;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysSocketCommSetting::OnButtonCloseComm() 
{
	if (m_pVisionASM->IsCommOpen())
	{
		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CLOSING_SOCKET_CONNECTION);	//_T("正在关闭网络连接...")
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
		BeginWaitCursor();

		if (m_pVisionASM->CloseComm())
		{
			SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
			m_ctrlCommLamp.SetIcon(m_hIconClosed);
			
			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_CLOSEED);	//_T("网络连接已关闭")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_CLOSE_SOCKET_CONNECTION);	//_T("网络连接关闭失败")
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
		UpdateDlgData(TRUE);	// 读入所有参数设置
		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPENING_SOCKET_CONNECTION);	//_T("正在打开网络连接...")
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
		BeginWaitCursor();

		if (m_pVisionASM->OpenComm())
		{
			SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
			m_ctrlCommLamp.SetIcon(m_hIconOpen);
			
			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
			if (m_pCommInfo->m_socketType == eSTServer)
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
			}
			else
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
			}
			m_pParent->UpdateStatusBar();
		}
		EndWaitCursor();
		CString strLog;
		strLog.Format(_T("点击[打开]"));
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysSocketCommSetting::OnButtonManualsend() 
{
	UpdateData(TRUE);
	if (!m_pVisionASM->SendCommInfo(m_strDataSending,TRUE))
	{
		//AfxMessageBox(_T("网络连接未打开，请先打开网络连接！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
	}
	CString strLog;
	strLog.Format(_T("点击[发送数据][发送]"));
	g_opLogFile.SaveLogText(strLog,3);
// 	UpdateData(TRUE);
// 
// 	if (m_pVisionASM->m_XYDCommProtocol.GetProtocolType() > 10)
// 	{
// 		CString strInfo;
// 		if (0 == m_nRegType)		// 写寄存器(WORD)
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nWriteNum = 5;
// 			std::vector<int> vWriteData;
// 
// 			vWriteData.resize(5);
// 			for (int i = 0; i < vWriteData.size(); i++)
// 			{
// 				vWriteData.at(i) = 10 + i;
// 			}
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strInfo,nRegAdrr,nWriteNum,vWriteData, FALSE);
// 		}
// 		else if (1 == m_nRegType)	// 写寄存器(DWORD)
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nWriteNum = 5;
// 			std::vector<int> vWriteData;
// 
// 			vWriteData.resize(5);
// 			for (int i = 0; i < vWriteData.size(); i++)
// 			{
// 				vWriteData.at(i) = -10 + i;
// 			}
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackWriteRegInfo(strInfo,nRegAdrr,nWriteNum,vWriteData, TRUE);
// 		}
// 		else if (2 == m_nRegType)	// 读寄存器(WORD)
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr,nReadNum,FALSE);
// 		}
// 		else if (3 == m_nRegType)	// 读寄存器(DWORD)
// 		{
// 			long nRegAdrr = m_pVisionASM->m_XYDCommProtocol.m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);		// 寄存器地址
// 			int nReadNum = 5;
// 
// 			m_pVisionASM->m_XYDCommProtocol.m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr,nReadNum,TRUE);
// 		}
// 		else if (4 == m_nRegType)	// 无协议
// 		{
// 			// 			if (!m_pVisionASM->SendCommInfo(m_strDataSending))
// 			// 			{
// 			// 				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
// 			// 			}
// 		}
// 		else
// 		{
// 			// 			if (!m_pVisionASM->SendCommInfo(m_strDataSending))
// 			// 			{
// 			// 				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
// 			// 			}
// 		}
// 
// 		if (!m_pVisionASM->SendCommInfo(strInfo))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
// 		}
// 	}
// 	else
// 	{
// 		if (!m_pVisionASM->SendCommInfo(m_strDataSending))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_CONNECTION_PLEASE_OPEN));
// 		}
// 	}
}

void CDlgSysSocketCommSetting::OnButtonCleanTxdata() 
{
	UpdateData(TRUE);
	m_strDataSending.Empty();

	CString strLog;
	strLog.Format(_T("点击[发送数据][清空]"));
	g_opLogFile.SaveLogText(strLog,3);

	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnButtonCleanRxdata() 
{
	UpdateData(TRUE);
	m_strDataReceived.Empty();
	CString strLog;
	strLog.Format(_T("点击[接收数据][清空]"));
	g_opLogFile.SaveLogText(strLog,3);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEditTxdata() 
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



void CDlgSysSocketCommSetting::OnSetfocusEdtPort() 
{
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_PORT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_PORT, strInput);

		if (TRUE == /*m_pParent->*/m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_PORT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[端口]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
	}
}

void CDlgSysSocketCommSetting::OnSetfocusIpaIpaddress() 
{
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		unsigned char  m_byAddr[4];
		m_byAddr[0] = 0;
		m_byAddr[1] = 0; 
		m_byAddr[2] = 0; 
		m_byAddr[3] = 0;
		m_ctrlIPAddress.GetAddress(m_byAddr[0], m_byAddr[1], m_byAddr[2], m_byAddr[3]);

		CString strLog,strPre,strCur;
		strPre.Format(_T("%d %d %d %d"), m_byAddr[0],m_byAddr[1],m_byAddr[2],m_byAddr[3]);
		CRect rcWnd;
		CString strInput, strOutput;

		GetDlgItem(IDC_IPA_IPADDRESS)->GetWindowRect(&rcWnd);
		unsigned char byAddr = 0;
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			strInput.Format("%d", m_byAddr[i]);
			if (TRUE == /*m_pParent->*/m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
			{
				byAddr = _tcstoul(strOutput, NULL, 10);
				if (byAddr >=0 || byAddr <= 255)
				{
					m_byAddr[i] = byAddr;
				}
				m_ctrlIPAddress.SetAddress(m_byAddr[0], m_byAddr[1], m_byAddr[2], m_byAddr[3]);
			}
			else
			{
				return;
			}
		}

		strCur.Format(_T("%d %d %d %d"), m_byAddr[0],m_byAddr[1],m_byAddr[2],m_byAddr[3]);
		if (strPre != strCur)
		{
			CString strLog;
			strLog.Format(_T("修改[IP]%s到%s"),strPre,strCur);
			g_opLogFile.SaveLogText(strLog,3);
		}
	}
}



BOOL CDlgSysSocketCommSetting::DestroyWindow() 
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

void CDlgSysSocketCommSetting::OnSocketTypeChange() 
{
	int nServerTypeOld;
	nServerTypeOld = m_pCommInfo->m_socketType;
	UpdateData(TRUE);
	if ((SocketType)m_nServerType == m_pCommInfo->m_socketType)
	{
		return;
	}
	m_pCommInfo->m_socketType = (SocketType)m_nServerType;
	m_pVisionASM->SetCommParam(m_pCommInfo);
	if (nServerTypeOld != m_nServerType)
	{
		CString strLog,strVal;
		strVal = m_nServerType ? _T("客户端"):_T("服务端");
		strLog.Format(_T("修改网络[类型]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}

	BeginWaitCursor();
	m_pParent->m_bStatusBarFlag = TRUE;
	m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_REOPEN_SOCKET_CONNECTION);	//_T("正在重新打开网络连接...")
	m_pParent->m_strStatusBarInfo2 = _T("");
	m_pParent->UpdateStatusBar();
	
	if (m_pVisionASM->UpdateCommTool())
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
//		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
		m_ctrlCommLamp.SetIcon(m_hIconOpen);
		
		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
	}
	else
	{
		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);

		m_pParent->m_bStatusBarFlag = FALSE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
		if (m_pCommInfo->m_socketType == eSTServer)
		{
			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
		}
		else
		{
			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
		}
		m_pParent->UpdateStatusBar();
	}
	EndWaitCursor();

	// 更新IP控件显示
	if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
	{

	}
	else
	{
		if (m_nServerType == 0)
		{
			GetDlgItem(IDC_IPA_IPADDRESS)->EnableWindow(FALSE);
		}
		else if (m_nServerType == 1)
		{
			GetDlgItem(IDC_IPA_IPADDRESS)->EnableWindow(m_bEnableCommParamSet);
		}
	}

}

void CDlgSysSocketCommSetting::OnKillfocusEdtPort() 
{
// 	if (m_pVisionASM == NULL)
// 	{
// 		return;
// 	}
// 	CString strNewPort;
// 	GetDlgItemText(IDC_EDT_PORT, strNewPort);
// 	if (strNewPort == m_strPort)
// 	{
// 		return;
// 	}
// 	m_strPort = strNewPort;
// 	m_ctrlIPAddress.GetAddress(m_pCommInfo->m_byAddr[0], m_pCommInfo->m_byAddr[1], m_pCommInfo->m_byAddr[2], m_pCommInfo->m_byAddr[3]);
// 	m_pCommInfo->m_uiPort = (unsigned int)_tcstoul(m_strPort, NULL, 10);
// 	m_pVisionASM->SetCommParam(m_pCommInfo);
// 
// 	BeginWaitCursor();
// 	m_pParent->m_bStatusBarFlag = TRUE;
// 	m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_REOPEN_SOCKET_CONNECTION);	//_T("正在重新打开网络连接...")
// 	m_pParent->m_strStatusBarInfo2 = _T("");
// 	m_pParent->UpdateStatusBar();
// 
// 	if (m_pVisionASM->UpdateCommTool())
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconOpen);
// 
// 		m_pParent->m_bStatusBarFlag = TRUE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
// 		m_pParent->m_strStatusBarInfo2 = _T("");
// 		m_pParent->UpdateStatusBar();
// 	}
// 	else
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconClosed);
// 
// 		m_pParent->m_bStatusBarFlag = FALSE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
// 		if (m_pCommInfo->m_socketType == eSTServer)
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
// 		}
// 		else
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
// 		}
// 		m_pParent->UpdateStatusBar();
// 	}
// 	EndWaitCursor();
}

void CDlgSysSocketCommSetting::OnKillfocusIpaIpaddress()
{
// 	if (m_pVisionASM == NULL)
// 	{
// 		return;
// 	}
// 	
// 	BYTE byAddr[4];
// 	m_ctrlIPAddress.GetAddress(byAddr[0], byAddr[1], byAddr[2], byAddr[3]);
// 	if ( (byAddr[0] = m_pCommInfo->m_byAddr[0])
// 		|| (byAddr[1] != m_pCommInfo->m_byAddr[1])
// 		|| (byAddr[2] != m_pCommInfo->m_byAddr[2])
// 		|| (byAddr[3] != m_pCommInfo->m_byAddr[3]) )
// 	{
// 		m_pCommInfo->m_byAddr[0] = byAddr[0];
// 		m_pCommInfo->m_byAddr[1] = byAddr[1];
// 		m_pCommInfo->m_byAddr[2] = byAddr[2];
// 		m_pCommInfo->m_byAddr[3] = byAddr[3];
// 		GetDlgItemText(IDC_EDT_PORT, m_strPort);
// 		m_pCommInfo->m_uiPort = (unsigned int)_tcstoul(m_strPort, NULL, 10);
// 		m_pVisionASM->SetCommParam(m_pCommInfo);
// 		
// 		BeginWaitCursor();
// 		m_pParent->m_bStatusBarFlag = TRUE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_REOPEN_SOCKET_CONNECTION);	//_T("正在重新打开网络连接...")
// 		m_pParent->m_strStatusBarInfo2 = _T("");
// 		m_pParent->UpdateStatusBar();
// 		
// 		if (m_pVisionASM->UpdateCommTool())
// 		{
// 			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
// 			m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
// 			m_ctrlCommLamp.SetIcon(m_hIconOpen);
// 			
// 			m_pParent->m_bStatusBarFlag = TRUE;
// 			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
// 			m_pParent->m_strStatusBarInfo2 = _T("");
// 			m_pParent->UpdateStatusBar();
// 		}
// 		else
// 		{
// 			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
// 			m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
// 			m_ctrlCommLamp.SetIcon(m_hIconClosed);
// 			
// 			m_pParent->m_bStatusBarFlag = FALSE;
// 			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
// 			if (m_pCommInfo->m_socketType == eSTServer)
// 			{
// 				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
// 			}
// 			else
// 			{
// 				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
// 			}
// 			m_pParent->UpdateStatusBar();
// 		}
// 		EndWaitCursor();
// 	}
// 	else
// 	{
// 		return;
// 	}
}

void CDlgSysSocketCommSetting::OnSelchangeComboCommCmdStopChar() 
{
// 	CString strInfo;
// 	if (m_comboCmdStopChar.GetCurSel() < 1)
// 	{
// 		m_pCommInfo->m_strCmdStop = _T("");
// 	}
// 	else
// 	{
// 		m_comboCmdStopChar.GetLBText(m_comboCmdStopChar.GetCurSel(), strInfo);
// 		m_pCommInfo->m_strCmdStop = strInfo;
// 	}
// 	m_pVisionASM->SetCommParam(m_pCommInfo);
// 
// 	BeginWaitCursor();
// 	m_pParent->m_bStatusBarFlag = TRUE;
// 	m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_REOPEN_SOCKET_CONNECTION);	//_T("正在重新打开网络连接...")
// 	m_pParent->m_strStatusBarInfo2 = _T("");
// 	m_pParent->UpdateStatusBar();
// 	
// 	if (m_pVisionASM->UpdateCommTool())
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconOpen);
// 
// 		m_pParent->m_bStatusBarFlag = TRUE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
// 		m_pParent->m_strStatusBarInfo2 = _T("");
// 		m_pParent->UpdateStatusBar();
// 	}
// 	else
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconClosed);
// 
// 		m_pParent->m_bStatusBarFlag = FALSE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
// 		if (m_pCommInfo->m_socketType == eSTServer)
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
// 		}
// 		else
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
// 		}
// 		m_pParent->UpdateStatusBar();
// 	}	
// 	EndWaitCursor();
	//m_comboCmdStopChar
	CString strCmmStopChar;
	m_comboCmdStopChar.GetWindowText(strCmmStopChar);

	if (strCmdStopCharOld != strCmmStopChar)
	{
		CString strLog;
		strLog.Format(_T("修改[命令]由%s到%s\n"),strCmdStopCharOld,strCmmStopChar);
		g_opLogFile.SaveLogText(strLog,3);
		strCmdStopCharOld = strCmmStopChar;
	}
	
}

void CDlgSysSocketCommSetting::OnSelchangeComboCommCommStopChar() 
{
// 	CString strInfo;
// 	if (m_comboCommStopChar.GetCurSel() < 1)
// 	{
// 		m_pCommInfo->m_strCommStop = _T("");
// 	}
// 	else
// 	{
// 		m_comboCommStopChar.GetLBText(m_comboCommStopChar.GetCurSel(), strInfo);
//		//strInfo = (strInfo == _T("CR")) ? _T("\r") : _T("");
// 		m_pCommInfo->m_strCommStop = strInfo;
// 	}
// 	m_pVisionASM->SetCommParam(m_pCommInfo);
// 
// 	BeginWaitCursor();
// 	m_pParent->m_bStatusBarFlag = TRUE;
// 	m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_REOPEN_SOCKET_CONNECTION);	//_T("正在重新打开网络连接...")
// 	m_pParent->m_strStatusBarInfo2 = _T("");
// 	m_pParent->UpdateStatusBar();
// 	
// 	if (m_pVisionASM->UpdateCommTool())
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconOpen);
// 
// 		m_pParent->m_bStatusBarFlag = TRUE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
// 		m_pParent->m_strStatusBarInfo2 = _T("");
// 		m_pParent->UpdateStatusBar();
// 	}
// 	else
// 	{
// 		m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
// 		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
// 		m_ctrlCommLamp.SetIcon(m_hIconClosed);
// 
// 		m_pParent->m_bStatusBarFlag = FALSE;
// 		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
// 		if (m_pCommInfo->m_socketType == eSTServer)
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);	//_T("端口被占用")
// 		}
// 		else
// 		{
// 			m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
// 		}
// 		m_pParent->UpdateStatusBar();
// 	}
// 	EndWaitCursor();
	
	CString strCommStopChar;
	m_comboCommStopChar.GetWindowText(strCommStopChar);

	if (strCommStopCharOld != strCommStopChar)
	{
		CString strLog;
		strLog.Format(_T("修改[通信]由%s到%s\n"),strCommStopCharOld,strCommStopChar);
		g_opLogFile.SaveLogText(strLog,3);
		strCommStopCharOld = strCommStopChar;
	}
}

BOOL CDlgSysSocketCommSetting::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysSocketCommSetting::OnKillfocusEdtRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_lRegAddress = (long)_tcstoul(m_strRegAddress, NULL, 10);
		m_pVisionASM->SetCommParam(m_pCommInfo);
		m_pVisionASM->UpdateCommTool();
	}

}


void CDlgSysSocketCommSetting::OnSetfocusEdtRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_REG_ADDRESS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_REG_ADDRESS, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_REG_ADDRESS, strOutput);

			CString strLog;
			strLog.Format(_T("修改[寄存器首地址]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtRegAddress();
		}

	}
}


//void CDlgSysSocketCommSetting::OnBnClickedRadioRegChange()
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

void CDlgSysSocketCommSetting::OnEnSetfocusEditRegWriteValue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REG_WRITE_VALUE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REG_WRITE_VALUE, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REG_WRITE_VALUE, strOutput);
		}

	}
}


void CDlgSysSocketCommSetting::OnBnClickedButtonRegRead()
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
	if (m_nWordTypeMode)    //双字节
	{
			
		long nRegAdrr = m_nEditTestReadRegAddr + atoi(RREG_PLC_CMD_TRIGGER);     //寄存器地址
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
				int nOldRegAddr = _ttoi(m_strRegAddress);
				m_nEditRegReadValue = vnReceivedData[0];
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_READ_PLC);	//_T("读取寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}		
		}		
	}
	else												//单字节
	{
		long nRegAdrr = m_nEditTestReadRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);     //寄存器地址
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
				int nOldRegAddr = _ttoi(m_strRegAddress);
				m_nEditRegReadValue = vnReceivedData[0];
				m_pParent->m_bStatusBarFlag = TRUE;
				m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SUCCESS_READ_PLC);	//_T("读取寄存器成功！")
				m_pParent->m_strStatusBarInfo2 = _T("");
				m_pParent->UpdateStatusBar();
			}		
		}
	}

	UpdateData(FALSE);

}


void CDlgSysSocketCommSetting::OnBnClickedButtonRegWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int  nDataNum = 1;												// 数据量
	std::vector<int> vnWriteData;
	vnWriteData.resize(nDataNum);
	vnWriteData.at(0) = m_nEditRegWriteValue;
	CString strWriteInfo;
	BOOL bReceived;
	CString strWriteReceiveInfo;
	CommStatus nCommStatus = eCommSucceed;
	if (m_nWordTypeMode)				   //双字
	{
		long nRegAdrr = m_nEditTestWriteRegAddr + atoi(RREG_PLC_CMD_TRIGGER);     //寄存器地址
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
	else					//单字
	{
		long nRegAdrr = m_nEditTestWriteRegAddr + atoi(RREG_PLC_CMD_TRIGGER_WORD);     //寄存器地址
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


void CDlgSysSocketCommSetting::OnEnSetfocusEditTestReadRegAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TEST_RED_REG_ADDR)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TEST_RED_REG_ADDR, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TEST_RED_REG_ADDR, strOutput);
		}

	}

	OnEnKillfocusEditTestReadRegAddr();
}


void CDlgSysSocketCommSetting::OnEnSetfocusEditTestWriteRegAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TEST_WRITE_REG_ADDR)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TEST_WRITE_REG_ADDR, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TEST_WRITE_REG_ADDR, strOutput);
		}

	}

	OnEnKillfocusEditTestWriteRegAddr();
}


void CDlgSysSocketCommSetting::OnEnKillfocusEditTestReadRegAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(TRUE);
	if (m_nWordTypeMode)
	{
		if (m_nEditTestReadRegAddr%2 || m_nEditTestReadRegAddr<0)
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_EVEN_NUMBER));    //_T("输入值必须为偶数！")
			SetDlgItemText(IDC_EDIT_TEST_RED_REG_ADDR,m_strRegAddress);
			return;
		}
	}

	UpdateData(FALSE);

}


void CDlgSysSocketCommSetting::OnEnKillfocusEditTestWriteRegAddr()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if (m_nWordTypeMode)
	{
		if (m_nEditTestWriteRegAddr%2 || m_nEditTestWriteRegAddr<0)
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_EVEN_NUMBER));    //_T("输入值必须为偶数！")
			SetDlgItemText(IDC_EDIT_TEST_WRITE_REG_ADDR,m_strRegAddress);
			return;
		}
	}

	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnKillfocusEdtTempRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_lTempRegAddress = (long)_tcstoul(m_strTempRegAddress, NULL, 10);
		m_pVisionASM->SetCommParam(m_pCommInfo);
		m_pVisionASM->UpdateCommTool();
	}
}


void CDlgSysSocketCommSetting::OnSetfocusEdtTempRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_TEMP_REG_ADDRESS, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_TEMP_REG_ADDRESS, strOutput);
			OnKillfocusEdtTempRegAddress();
		}

	}
}


void CDlgSysSocketCommSetting::UpdateCommStatus()
{
	if (m_pVisionASM != NULL)
	{
		m_pCommInfo = (vcSocketCommInfo *)m_pVisionASM->GetCommParam();
		if (m_pVisionASM->IsCommOpen())
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_CLOSE));	//_T("关闭")
			//			m_btnCommSwitch.SetIcon(IDI_ICON_OK_16);
			m_ctrlCommLamp.SetIcon(m_hIconOpen);

			m_pParent->m_bStatusBarFlag = TRUE;
			CString str = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SOCKET_CONNECTION_OPEN_SUCCEED);	//_T("网络连接打开成功")
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();
		}
		else
		{
			m_btnCommSwitch.SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
			//			m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
			m_ctrlCommLamp.SetIcon(m_hIconClosed);

			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_OPEN_SOCKET_CONNECTION);	//_T("网络连接打开失败")
			if (m_pCommInfo->m_socketType == eSTServer)
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PORT_NOT_AVAILABLE);		//_T("端口被占用")
			}
			else
			{
				m_pParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_NO_REPLY_FROM_SERVER);	//_T("服务器没有响应")
			}
			m_pParent->UpdateStatusBar();
		}
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_CLOSE_COMM, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_OPEN));	//_T("打开")
		//		m_btnCommSwitch.SetIcon(IDI_ICON_CANCEL_16);
		m_ctrlCommLamp.SetIcon(m_hIconClosed);

		m_pParent->m_bStatusBarFlag = FALSE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_VISION_ALIGN_TOOL);
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();
	}
}

void CDlgSysSocketCommSetting::OnEnSetfocusEdtWriteRegMaxnum()
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


void CDlgSysSocketCommSetting::OnEnSetfocusEdtReadRegMaxnum()
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


void CDlgSysSocketCommSetting::UpdateControlShow(BOOL bVisible)
{
	BOOL bShow = bVisible;

	SetGroupBoxStatus(IDC_STATIC_TEST_REG,m_bEnableCommParamSet,bShow && m_bshowCommTestSet);

	GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);
	GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(bShow && m_bshowCommParamSet);


	/*GetDlgItem(IDC_STATIC_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_TEMP_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_TEMP_REG_ADDRESS)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_TEST_REG)->ShowWindow(nShow);

	GetDlgItem(IDC_STATIC_REG_ADDR)->ShowWindow(nShow);


	GetDlgItem(IDC_BUTTON_REG_READ)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_REG_WRITE)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_REG_WRITE_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_READ_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_VALUE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_TEST_REG)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_REG_ADDR)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_TEST_RED_REG_ADDR)->ShowWindow(nShow);

	GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(nShow);
	GetDlgItem(IDC_RADIO_DOUBLE_WORD)->ShowWindow(nShow);

	GetDlgItem(IDC_STATIC_WORD_TYPE)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_REG_ADDR)->ShowWindow(nShow);

	GetDlgItem(IDC_EDIT_TEST_WRITE_REG_ADDR)->ShowWindow(nShow);

	GetDlgItem(IDC_EDT_WRITE_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_EDT_READ_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_WRITE_REG_MAXNUM)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_READ_REG_MAXNUM)->ShowWindow(nShow);

	GetDlgItem(IDC_EDIT_REG_READ_VALUE)->ShowWindow(nShow);*/


}

//void CDlgSysSocketCommSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgSysSocketCommSetting::UpdateDlgByAuthorize()
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
				SetGroupBoxStatus(IDC_STATIC_GRP_SOCKET_CONFIG,m_bEnableCommParamSet,m_bshowCommParamSet);
			}


			// 连接读写测试
			bFind = vsConfig->GetCheckedByItem(eCommAuthorize_TestSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCommTestSet,m_bEnableCommTestSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_SEND_DATA,m_bEnableCommTestSet,m_bshowCommTestSet);
				SetGroupBoxStatus(IDC_STATIC_GRP_RECEIVE_DATA,m_bEnableCommTestSet,m_bshowCommTestSet);
				SetGroupBoxStatus(IDC_STATIC_TEST_REG,m_bEnableCommTestSet,m_bshowCommTestSet);

				SysPlatformInfo sysPlatformInfo;
				m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

				SetGroupBoxStatus(IDC_STATIC_BEND_PATH_GROUP,m_bEnableCommTestSet && sysPlatformInfo.m_bShowBendPathCalc,m_bshowCommTestSet && sysPlatformInfo.m_bShowBendPathCalc);

				GetDlgItem(IDC_WZ_LIST)->ShowWindow(m_bshowCommTestSet && sysPlatformInfo.m_bShowBendPathCalc);
				ShowBendPathCalc(m_bshowCommTestSet && sysPlatformInfo.m_bShowBendPathCalc);
				GetDlgItem(IDC_EDIT_WZ_OKNG)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_WZ_TEXT)->EnableWindow(FALSE);
			}

		}
	}
}


void CDlgSysSocketCommSetting::OnKillfocusEdtWZRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_lWZRegAddress = (long)_tcstoul(m_strWZRegAddress, NULL, 10);
		m_pVisionASM->SetCommParam(m_pCommInfo);
		m_pVisionASM->UpdateCommTool();

	}

}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZRegAddress()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_WZ_REG_ADDRESS)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_WZ_REG_ADDRESS, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_WZ_REG_ADDRESS, strOutput);

			CString strLog;
			strLog.Format(_T("修改[寄存器WZ首地址]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZRegAddress();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZW()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZW()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_W)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_W, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_W, strOutput);

			CString strLog;
			strLog.Format(_T("修改[W]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZW();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZR()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZR()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_R)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_R, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_R, strOutput);

			CString strLog;
			strLog.Format(_T("修改[R]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZR();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZY1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZY1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Y1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Y1, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Y1, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Y1]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZY1();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZZ1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZZ1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Z1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Z1, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Z1, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Z1]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZZ1();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZG()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZG()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_G)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_G, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_G, strOutput);

			CString strLog;
			strLog.Format(_T("修改[G]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZG();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZY2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZY2()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Y2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Y2, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Y2, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Y2]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZY2();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZZ2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZZ2()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Z2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Z2, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Z2, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Z2]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZZ2();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZY3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZY3()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Y3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Y3, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Y3, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Y3]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZY3();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtWZZ3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	UpdateData(FALSE);
}

void CDlgSysSocketCommSetting::OnSetfocusEdtWZZ3()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_WZ_Z3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_WZ_Z3, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_WZ_Z3, strOutput);

			CString strLog;
			strLog.Format(_T("修改[Z3]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtWZZ3();
		}

	}
}

void CDlgSysSocketCommSetting::OnKillfocusEdtRegAddress3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		UpdateData(TRUE);
		m_pCommInfo->m_lRegAddress3 = (long)_tcstoul(m_strRegAddress3, NULL, 10);
		m_pVisionASM->SetCommParam(m_pCommInfo);
		m_pVisionASM->UpdateCommTool();
	}

}


void CDlgSysSocketCommSetting::OnSetfocusEdtRegAddress3()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_REG_ADDRESS3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_REG_ADDRESS3, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_REG_ADDRESS3, strOutput);

			CString strLog;
			strLog.Format(_T("修改[ID寄存器地址]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);

			OnKillfocusEdtRegAddress3();
		}

	}
}

void CDlgSysSocketCommSetting::OnBnClickedButtonWzCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	int m_bResultok=1;
	double YN=0;
	double ZN=0;
	m_strResultShow=_T("计算结果\n");
	CString str;
	CString m_strResultYN = _T("YN :");
	m_strResultYN = m_strResultYN+_T("\n");
	CString m_strResultZN = _T("ZN :");
	m_strResultZN = m_strResultZN +_T("\n");
	m_vdYN.clear();
	m_vdZN.clear();
	m_vdRN.clear();
	;
	for (int i=1;i<101;i++)
	{
		double R=1.8*i;
		m_bResultok=MotionData(R,m_dWZW, m_dWZR,YN,ZN);
		if (m_bResultok==-1)
		{
			m_strResult=_T("计算失败！");
			UpdateData(FALSE);
			return ;
		} 
		if (i==98)
		{
			YN= YN+m_dWZY1;
			ZN= ZN+m_dWZZ1;
		}
		if (i==99)
		{
			YN= YN+m_dWZY2;
			ZN= ZN+m_dWZZ2;
		}
		if (i==100)
		{
			YN= YN+m_dWZY3;
			ZN= ZN+m_dWZZ3;
		}
		m_vdRN.push_back(R);
		m_vdYN.push_back(YN);
		m_vdZN.push_back(ZN);
		str.Format(_T("%.4f "),YN);
		m_strResultYN = m_strResultYN+str;
		str.Format(_T("%.4f "),ZN);
		m_strResultZN = m_strResultZN+str;
	}
	m_strResult=_T("计算成功");
	m_strResultShow=m_strResultYN+m_strResultZN;
	UpdateData(FALSE);
	UpdateList();

	return ;
}


void CDlgSysSocketCommSetting::OnBnClickedButtonWzSent()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int  nDataNum = 100;			// 数据量
	std::vector<int> vnWriteData;
	std::vector<double> vdDate;
	vnWriteData.resize(nDataNum);
	vdDate.clear();
	//vnWriteData= m_vdYN;
	CString strWriteInfo;
	CString strWriteReceiveInfo;

	CString strSend;
	BOOL bReceived;
	CString strInfo;
	CommStatus nCommStatus = eCommSucceed;

	

	//if (0)// 如果是PLC直接读取存储寄存器协议
	//{
	//	CString strSend;

	//	//vdDate = m_vdYN +m_vdZN;
	//	m_vdYN.insert(m_vdYN.end(),m_vdZN.begin(),m_vdZN.end());
	//	m_vdYN.insert(m_vdYN.end(),m_vdRN.begin(),m_vdRN.end());
	//	vdDate=m_vdYN;
	//	m_vdYN.clear();
	//	m_vdZN.clear();
	//	m_vdRN.clear();
	//	if (vdDate.size() ==nDataNum)
	//	{
	//		// 打包写寄存器指令
	//		if (FALSE == m_pVisionASM->m_XYDCommProtocol.PackRePlcWZResult(strSend, vdDate))
	//		{
	//			return ;
	//		}
	//		bReceived = m_pVisionASM->SendCommInfo(strSend,TRUE);  
	//		m_pVisionASM->VisionAlignLogRecord(_T("D://折弯路径.txt"),strSend);
	//		if (bReceived == FALSE || nCommStatus != eCommSucceed)
	//		{
	//			AfxMessageBox(_T("发送失败！"));
	//			return ;
	//		}

	//	}

	//}


	
	//if (1)// 如果是PLC直接读取存储寄存器协议
	for(int it=0 ;it<3;it++)
	{
		CString strSend;
		if(it==0)
		{
			vdDate = m_vdYN;
		}
		else if(it==1)
		{
			vdDate = m_vdZN;
		}
		else if(it==2)
		{
			vdDate = m_vdRN;
		}

		if (vdDate.size() ==nDataNum)
		{
			// 打包写寄存器指令
			if (FALSE == m_pVisionASM->m_XYDCommProtocol.PackRePlcWZResult(strSend, vdDate,it*300))
			{
				return ;
			}
			bReceived = m_pVisionASM->SendCommInfo(strSend,TRUE);  
			//m_pVisionASM->VisionAlignLogRecord(_T("D://折弯路径.txt"),strSend);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				AfxMessageBox(_T("发送失败！"));
				return ;
			}

			Sleep(100); //延时100ms
		}

	}


}


void CDlgSysSocketCommSetting::ShowBendPathCalc(BOOL bShow)
{
	//GetDlgItem(IDC_STATIC_REG_ADDRESS3)->ShowWindow(bShow);
	//GetDlgItem(IDC_EDT_REG_ADDRESS3)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_REG_ADDRESS3)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDT_REG_ADDRESS3)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDT_WZ_REG_ADDRESS)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_OKNG)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_W)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_R)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Y1)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Z1)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_G)->ShowWindow(bShow);
	GetDlgItem(IDC_BUTTON_WZ_CALCULATE)->ShowWindow(bShow);
	GetDlgItem(IDC_BUTTON_WZ_SENT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_TEXT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Y2)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Z2)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Y3)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_WZ_Z3)->ShowWindow(bShow);
	GetDlgItem(IDC_WZ_LIST)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_REG_ADDRESS4)->ShowWindow(bShow);

	//单次写入控件
	GetDlgItem(IDC_STATIC_YZR_ONE_SEND)->ShowWindow(bShow);
	GetDlgItem(IDC_BUTTON_YZR_ONE_SEND)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_YZR1)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_YZR2)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_YZR3)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_YZR_SEND_ADDR)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_YZR_SEND_ADDR)->ShowWindow(bShow);
}

void CDlgSysSocketCommSetting::OnBnClickedButton1()
{

	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	TCHAR szFilters[]= _T("csv file(*.csv)|*.csv|All Files (*.*)|*.*||");

		//获取文件路径
		CFileDialog* lpszOpenFile;
		//CString szGetName;
		lpszOpenFile = new CFileDialog(TRUE,  
			_T("*.csv"),
			_T("data file"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			szFilters);
		
		if (lpszOpenFile->DoModal()==IDOK)
		{		
			m_szGetName = lpszOpenFile->GetPathName();	
			//SetWindowText(szGetName);
			CString strLog;
			strLog.Format(_T("加载表格成功\n"));
			g_opLogFile.SaveLogText(strLog);
			delete lpszOpenFile;
		}
		else
		{
			CString strLog;
			strLog.Format(_T("加载表格失败\n"));
			g_opLogFile.SaveLogText(strLog);
			return;
		}
		//m_pParent->m_bSysKeyboardEnabled = TRUE;
		//// TODO: 在此添加控件通知处理程序代码
		//if (m_pParent->m_bSysKeyboardEnabled)
		//{
		//	
		//	m_pCommInfo->m_lRegAddress3 = (long)_tcstoul(m_strRegAddress3, NULL, 10);
		//	m_pVisionASM->SetCommParam(m_pCommInfo);
		//	m_pVisionASM->UpdateCommTool();
		//}
		
	

		UpdateData(FALSE);

}


void CDlgSysSocketCommSetting::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	int m_bResultok=1;
	double YN=0;
	double ZN=0;
	m_strResultShow=_T("导入结果\n");
	CString str;
	CString m_strResultYN = _T("YN :");
	m_strResultYN = m_strResultYN+_T("\n");
	CString m_strResultZN = _T("ZN :");
	m_strResultZN = m_strResultZN +_T("\n");
	m_vdYN.clear();
	m_vdZN.clear();
	m_vdRN.clear();
	

	try
	{
		////////////////////////////////////
		ifstream in((char*)m_szGetName.GetBuffer(m_szGetName.GetLength()));  
		//string filename;  
		string line;
		string strr;
		//string str;
		vector<vector<string>> strArray;  //保存所有数据的二维数组
		if(in) // 有该文件  
		{  

			// 按照逗号分隔
			while (getline (in, line)) // line中不包括每行的换行符  
			{ 
				// 存成二维表结构

				stringstream ss(line);
				//stringstream ss(line);
				vector<string> lineArray;
				// 按照逗号分隔
				while (getline(ss, strr, ','))
				{
					lineArray.push_back(strr);
				}
				strArray.push_back(lineArray);
			}  
		}  
		else // 没有该文件  
		{  
			//cout <<"no such file" << endl;  
		}  
		in.close();

		///////////////////////////////////

		if (strArray.size() > 100)
		{
			for (int i=1;i<strArray.size();i++)
			{
				m_vdYN.push_back(atof(strArray[i][1].c_str()));
				m_vdZN.push_back(atof(strArray[i][2].c_str()));
				m_vdRN.push_back(atof(strArray[i][3].c_str()));


				str.Format(_T("%.4f "),atof(strArray[i][1].c_str()));
				m_strResultYN = m_strResultYN+str;
				str.Format(_T("%.4f "),atof(strArray[i][2].c_str()));
				m_strResultZN = m_strResultZN+str;
			}

			m_strResult=_T("导入成功");
			m_strResultShow=m_strResultYN+m_strResultZN;
			UpdateData(FALSE);
			UpdateList();
			CString strLog;
			strLog.Format(_T("导入成功\n"));
			g_opLogFile.SaveLogText(strLog);

		}
		else
		{
			m_strResult=_T("导入失败小于100");
			m_strResultShow=m_strResult;
			UpdateData(FALSE);

			CString strLog;
			strLog.Format(_T("导入失败小于100\n"));
			g_opLogFile.SaveLogText(strLog);
		}
	}
	catch (...)
	{


		m_strResult=_T("导入失败");
		m_strResultShow=m_strResult;
		g_opLogFile.SaveLogText(m_strResult);
		UpdateData(FALSE);
	}

	//CString strTemp1,strResult;
	////存储数据的文件
	//for (long j=0;j<strArray.size(); j++)
	//{
	//	for (long k=4;k<strArray[j].size(); k++)
	//	{

	//		strTemp1 += strArray[j][k].c_str();
	//		strTemp1 += _T(",");
	//	} 
	//	strTemp1 += _T("\n");
	//}


	//strResult += strTemp1;


	//
	////保存文件
	//bool bNewCreate = false;
	//CCsvFileSave csvFileSave;

	//if (csvFileSave.OpenCsvFile(m_szGetName,bNewCreate))
	//{
	//	if (bNewCreate)	// 新创建
	//	{
	//		strFileNewName.Format(_T("\\SN\\ACF.csv"));

	//		csvFileNew = strInspectResultDirecotryPath + strFileNewName;

	//		ifstream inNew((char*)csvFileNew.GetBuffer(csvFileNew.GetLength()));  
	//		string lineNew;
	//		if(inNew) // 有该文件  
	//		{  
	//			int q = 0 ;
	//			// 按照逗号分隔
	//			while (getline (inNew, lineNew)) // line中不包括每行的换行符  
	//			{ 
	//				q += 1 ;
	//				if (q==2)
	//				{
	//					break;
	//				}
	//				csvFileSave.SaveRowText(lineNew.c_str());
	//			} 
	//		}  
	//		else // 没有该文件  
	//		{  
	//			CString strTemp;
	//			CString strHeader = _T("时间");
	//			strHeader += _T(",SN码");
	//			strHeader += _T(",结果");
	//			for (long i=0; i<vStrResultDistanceGlu.size(); i++)
	//			{
	//				if (((COtherOptionConfigure*)g_pCustomOption)->m_GluInspectOption.m_bEnableDataUintUsedUm)
	//				{
	//					strTemp.Format(_T(",%s(um)"),vStrResultDistanceGlu[i]);
	//				}else
	//				{
	//					strTemp.Format(_T(",%s(mm)"),vStrResultDistanceGlu[i]);
	//				}

	//				strHeader += strTemp;
	//			}

	//			csvFileSave.SaveRowText(strHeader);

	//			// 最大值
	//			{
	//				CString strTemp;
	//				CString strMax = _T("Max");
	//				strMax += _T(",");
	//				strMax += _T(",");
	//				for (long i=0; i<vdResultDistanceGluMax.size(); i++)
	//				{
	//					strTemp.Format(_T(",%0.4lf"),vdResultDistanceGluMax[i]);
	//					strMax += strTemp;
	//				}
	//				csvFileSave.SaveRowText(strMax);
	//			}

	//			//最小值
	//			{
	//				CString strTemp;
	//				CString strMin = _T("Min");
	//				strMin += _T(",");
	//				strMin += _T(",");
	//				for (long i=0; i<vdResultDistanceGluMin.size(); i++)
	//				{
	//					strTemp.Format(_T(",%0.4lf"),vdResultDistanceGluMin[i]);
	//					strMin += strTemp;
	//				}
	//				csvFileSave.SaveRowText(strMin);
	//			}
	//			//cout <<"no such file" << endl;  
	//		}  
	//		in.close();

	//	}

	//	csvFileSave.SaveRowText(strResult);
	//	csvFileSave.CloseCsvFile();
	//}

	//保存一个数组中的元素;
	

	return ;

}


void CDlgSysSocketCommSetting::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	
	TCHAR szFilters[]= _T("csv file(*.csv)|*.csv|All Files (*.*)|*.*||");

	//获取文件路径
	CFileDialog* lpszOpenFile;
	//CString szGetName;
	lpszOpenFile = new CFileDialog(FALSE,  
		_T("*.csv"),
		_T("data file"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		szFilters);
	

	if (lpszOpenFile->DoModal() == IDOK)
	{
		CString strFilePath = lpszOpenFile->GetPathName();	
		//SetWindowText(szGetName);

		FILE *fileStream = NULL;
		//fileStream = fopen(strFilePath, _T("wb"));
		int iError = fopen_s(&fileStream, strFilePath, _T("wb"));

		if (iError != 0)
		{
			CString strLog;
			strLog.Format(_T("导出失败，创建文件失败!\n"));
			g_opLogFile.SaveLogText(strLog);
			//MessageBox(_T("创建文件失败!"));
			return;
		}
		else
		{
			//存储数据的文件
			fprintf(fileStream, "序号,");
			fprintf(fileStream, "Y,");
			fprintf(fileStream, "Z,");
			fprintf(fileStream, "R,");
			fprintf(fileStream, "\n");
			for (long j=0;j<m_vdRN.size(); j++)
			{
			  fprintf(fileStream, "%d,", j+1);
			  fprintf(fileStream, "%.4f,", m_vdYN[j]);
			  fprintf(fileStream, "%.4f,", m_vdZN[j]);
			  fprintf(fileStream, "%.4f,", m_vdRN[j]);
			  fprintf(fileStream, "\n");
			}
			CString strLog;
			strLog.Format(_T("导出成功\n"));
			g_opLogFile.SaveLogText(strLog);

		}

		fclose(fileStream);
		fileStream= NULL;

		delete lpszOpenFile;
	}
	else
	{
		return;
	}
}


void CDlgSysSocketCommSetting::OnBnClickedButtonYzrOneSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int  nDataNum = 100;			// 数据量
	std::vector<int> vnWriteData;
	std::vector<double> vdDate;
	vnWriteData.resize(nDataNum);
	vdDate.clear();

	CString strSend;
	BOOL bReceived;
	CommStatus nCommStatus = eCommSucceed;

	if(m_nYZRSendType ==0)
	{
		vdDate = m_vdYN;
	}
	else if(m_nYZRSendType ==1)
	{
		vdDate = m_vdZN;
	}
	else if(m_nYZRSendType ==2)
	{
		vdDate = m_vdRN;
	}

	if (m_lYZRSendAddr == 0) //等于0时按照地址WZ地址偏移去写入
	{
		m_lYZRSendAddr = (long)_tcstoul(m_strWZRegAddress, NULL, 10) + m_nYZRSendType * 300;
	}

	if (vdDate.size() ==nDataNum)
	{
		int nMaxSendNumSingleSend = 100; //单次最大可发送地址数量
		int nSizeSingleData = 2; //双字地址
		nMaxSendNumSingleSend = nMaxSendNumSingleSend/nSizeSingleData;  //双字地址对应单次发送数据的最大数据量
		int nNeedSendTimes = (vdDate.size()+nMaxSendNumSingleSend - 1)/nMaxSendNumSingleSend; //需要发送的次数

		for (int i=0; i<nNeedSendTimes;++i)
		{
			std::vector<double> vdSend; //每次发送的数据
			if(vdDate.size() - i * nMaxSendNumSingleSend > nMaxSendNumSingleSend)
			{
				//未发送数量超过单次最大可发送数量
				vdSend.insert(vdSend.begin(),vdDate.begin() + i * nMaxSendNumSingleSend,
					vdDate.begin() + (i + 1) * nMaxSendNumSingleSend);
			}
			else
			{
				vdSend.insert(vdSend.begin(),vdDate.begin() + i * nMaxSendNumSingleSend,
					vdDate.end());
			}
			int nTempRegAddr = m_lYZRSendAddr + nSizeSingleData * nMaxSendNumSingleSend * i; //每次发送的地址

			// 打包写寄存器指令
			if (FALSE == m_pVisionASM->m_XYDCommProtocol.PackRePlcYZRResult(strSend,vdSend,nTempRegAddr))
			{
				return ;
			}

			bReceived = m_pVisionASM->SendCommInfo(strSend,TRUE);  
			//m_pVisionASM->VisionAlignLogRecord(_T("D://折弯路径.txt"),strSend);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				AfxMessageBox(_T("发送失败！"));
				return ;
			}

			Sleep(30);
		}
	}
}


void CDlgSysSocketCommSetting::OnEnSetfocusEditYzrSendAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_YZR_SEND_ADDR)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_YZR_SEND_ADDR, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_YZR_SEND_ADDR, strOutput);
		}

	}
}


void CDlgSysSocketCommSetting::OnEnSetfocusEdtIdLength()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_bSysKeyboardEnabled)
	{
		m_pParent->m_bSysKeyboardEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ID_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ID_LENGTH, strInput);

		m_pParent->m_bSysKeyboardEnabled = TRUE;
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_ID_LENGTH, strOutput);
		}

	}
}
