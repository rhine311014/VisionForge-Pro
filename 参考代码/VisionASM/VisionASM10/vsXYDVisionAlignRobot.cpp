// vsXYDVisionAlignRobot.cpp
// 机器人控制

#include "stdafx.h"
#include "VisionASM.h"
#include "vsXYDVisionAlign.h"
#include "svXMLConfigurator.h"
#include "DlgSearchFailWarning.h"
#include "DlgManualSearch.h"
#include "DlgManualSearchPos1.h"
#include "DlgManualSearchPos2.h"

#include "VisionASMDlg.h"
#include <algorithm>




// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
BOOL vcXYDVisionAlign:: CommunicateRobotGetMod(RobotMod& rmRobotMod)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}

	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();



		CString strInfo;
		m_XYDCommProtocol.PackRobotGetMod(strInfo);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_ROBOT_SEND_GET_MODE_FAILE);//_T("发送获取工作模式出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_ROBOT_GET_MODE_TIMEOUT);// _T("获取工作模式通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMod(strReceived, rmRobotMod);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{
				CString strInfo;
				m_XYDCommProtocol.PackRobotGetMod(strInfo);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_ROBOT_SEND_GET_MODE_FAILE);//__T("发送获取工作模式出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取工作模式通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMod(strReceived, rmRobotMod);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_WORK_MODE_ERROR);;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
		// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
		// 	AlignLogRecord(str);
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		// 默认是手动模式
		rmRobotMod = eRmManual;
		return TRUE;
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}
BOOL vcXYDVisionAlign:: CommunicateRobotSetMod(RobotMod rmRobotMod)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotSetMod(strInfo, rmRobotMod);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WORK_MODE_ERROR);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取设置工作模式响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetMod(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{
				CString strInfo;
				m_XYDCommProtocol.PackRobotSetMod(strInfo, rmRobotMod);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WORK_MODE_ERROR);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取设置工作模式响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetMod(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WORK_MODE_RESPOND_ANALYSIS_ERROR);;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
		// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
		// 	AlignLogRecord(str);
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		// 默认是成功
		return TRUE;
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；
BOOL vcXYDVisionAlign:: CommunicateRobotAutoControl(RobotAutoControl racAutoControl)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotAutoControl(strInfo, racAutoControl);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_AUTO_WORK_MODE_EXECUTE_ERROR);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("获取设置自动模式下程序执行操作响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotAutoControl(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{
			CString strInfo;
			m_XYDCommProtocol.PackRobotAutoControl(strInfo, racAutoControl);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_AUTO_WORK_MODE_EXECUTE_ERROR);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("获取设置自动模式下程序执行操作响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotAutoControl(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_AUTO_WORK_MODE_EXECUTE__RESPOND_ANALYSIS_ERROR);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 3）机器人手动移动速度查询、设定（mspeed）；
BOOL vcXYDVisionAlign:: CommunicateRobotGetMSpeed(int& nSpeed)
{

	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotGetMSpeed(strInfo);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送手动移动速度查询出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取手动移动速度查询响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeed(strReceived, nSpeed);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotGetMSpeed(strInfo);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("发送手动移动速度查询出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取手动移动速度查询响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeed(strReceived, nSpeed);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("手动移动速度查询响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotGetMSpeed340(strInfo);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送手动移动速度查询出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取手动移动速度查询响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeed340(strFrontReceived, nSpeed);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("手动移动速度查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeedOK340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("手动移动速度查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeed340(strReceived, nSpeed);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("手动移动速度查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取手动移动速度查询响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetMSpeedOK340(strReceived);
				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("手动移动速度查询响应解析出错！");;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}

			}
		}		
	}
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}
BOOL vcXYDVisionAlign:: CommunicateRobotSetMSpeed(int nSpeed)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotSetMSpeed(strInfo, nSpeed);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("发送手动移动速度设置出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("获取手动移动速度设置响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetMSpeed(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{


			CString strInfo;
			m_XYDCommProtocol.PackRobotSetMSpeed(strInfo, nSpeed);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("发送手动移动速度设置出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("获取手动移动速度设置响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetMSpeed(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("手动移动速度设置响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 4）机器人移动命令（inch X、Y、Z等)；
BOOL vcXYDVisionAlign:: CommunicateRobotInch(RobotInchAxis nAxis, RobotInchType nType)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotInch(strInfo, nAxis, nType);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人点动移动出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人点动移动响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotInch(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{


				CString strInfo;
				m_XYDCommProtocol.PackRobotInch(strInfo, nAxis, nType);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("发送机器人点动移动出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人点动移动响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotInch(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人点动移动响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotInch340(strInfo, nAxis, nType);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人点动移动出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人点动移动响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotInchStart340(strFrontReceived);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人点动移动响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotInchEnd340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人点动移动响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotInchStart340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				// 成功后，还需要接收响应"END"
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人点动移动响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotInchEnd340(strReceived);

				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人点动移动响应解析出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}
			}
		}
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 5）机器人当前位置查询（WHRXY）；
BOOL vcXYDVisionAlign:: CommunicateRobotGetPosWHRXY(RobotPos& rpPos)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotGetPosWHRXY(strInfo);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人当前位置查询出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人当前位置查询响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXY(strReceived, rpPos);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotGetPosWHRXY(strInfo);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("发送机器人当前位置查询出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人当前位置查询响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXY(strReceived, rpPos);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotGetPosWHRXY340(strInfo);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人当前位置查询出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人当前位置查询响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXY340(strFrontReceived, rpPos);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXYOK340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXY340(strReceived, rpPos);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人当前位置查询响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotGetPosWHRXYOK340(strReceived);
				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人当前位置查询响应解析出错！");;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}

			}
		}		
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 6）设置机器人示教点位（TEACH 200）；
BOOL vcXYDVisionAlign:: CommunicateRobotTeachPos( int nPosIndex)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{

		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotTeachPos(strInfo, nPosIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人示教点位出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人示教点位响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotTeachPos(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotTeachPos(strInfo, nPosIndex);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("发送机器人示教点位出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人示教点位响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotTeachPos(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人示教点位响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotTeachPos340(strInfo, nPosIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("发送机器人示教点位出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人示教点位响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotTeachPos340(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotTeachPos340(strInfo, nPosIndex);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("发送机器人示教点位出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人示教点位响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotTeachPos340(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人示教点位响应解析出错！");
			PostMessageUpdateStatusBarInfo();

			return FALSE;
		}
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	return TRUE;
}

// 7）机器人点位查询（Read P200）；
BOOL vcXYDVisionAlign:: CommunicateRobotReadPos(int nPosIndex, RobotPos& rpPos)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadPos(strInfo, nPosIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人点位信息出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人点位信息响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPos(strReceived, rpPos);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{


				CString strInfo;
				m_XYDCommProtocol.PackRobotReadPos(strInfo, nPosIndex);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("获取机器人点位信息出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人点位信息响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPos(strReceived, rpPos);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人点位信息响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadPos340(strInfo, nPosIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人点位信息出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("获取机器人点位信息响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPos340(strFrontReceived, rpPos);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取机器人点位信息响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPosOK340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取机器人点位信息响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPos340(strReceived, rpPos);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取机器人点位信息响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("获取机器人点位信息响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadPosOK340(strReceived);
				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("获取机器人点位信息响应解析出错！");;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}

			}
		}
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
BOOL vcXYDVisionAlign::CommunicateRobotMovePos(int nPosIndex, int nSpeed)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotMovePos(strInfo, nPosIndex, nSpeed);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("机器人移动出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("机器人移动响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotMovePos(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{

			CString strInfo;
			m_XYDCommProtocol.PackRobotMovePos(strInfo, nPosIndex, nSpeed);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人移动出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("机器人移动响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotMovePos(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("获取机器人移动响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}


// 9）执行中断处理（ETX(03)）
BOOL vcXYDVisionAlign::CommunicateRobotETX()
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotETX(strInfo);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("机器人中断执行出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("机器人中断执行响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotETX(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{

			CString strInfo;
			m_XYDCommProtocol.PackRobotETX(strInfo);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人中断执行出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("机器人中断执行响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotETX(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("获取机器人中断执行响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}
// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
BOOL vcXYDVisionAlign::CommunicateRobotSetPosAixs(int nPosIndex, RobotInchAxis nAxis, double dVal)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotSetPosAixs(strInfo, nPosIndex, nAxis, dVal);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("机器人设置点的轴位置出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("机器人设置点的轴位置响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPosAixs(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{

			CString strInfo;
			m_XYDCommProtocol.PackRobotSetPosAixs(strInfo, nPosIndex, nAxis, dVal);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人设置点的轴位置出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("机器人设置点的轴位置响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPosAixs(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("获取机器人设置点的轴位置响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}
// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
BOOL  vcXYDVisionAlign::CommunicateRobotSetPos(int nPosIndex, RobotPos rpPos)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotSetPos(strInfo, nPosIndex, rpPos);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置点位置出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置点位置响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPos(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotSetPos(strInfo, nPosIndex, rpPos);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置点位置出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置点位置响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPos(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置点位置响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotSetPos340(strInfo, nPosIndex, rpPos);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置点位置出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置点位置响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPos340(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotSetPos340(strInfo, nPosIndex, rpPos);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置点位置出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置点位置响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotSetPos340(strReceived);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置点位置响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
// 12) 设置某个变量的值
// 13）设置某个变量的值
// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X-)
BOOL  vcXYDVisionAlign::CommunicateRobotABSADJ(RobotInchAxis nAxis, RobotInchType nType)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotABSADJ(strInfo, nAxis, nType);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("机器人运动到原点位置附近通信出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("机器人运动到原点位置附近响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotABSADJ(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{

			CString strInfo;
			m_XYDCommProtocol.PackRobotABSADJ(strInfo, nAxis, nType);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人运动到原点位置附近通信出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("机器人运动到原点位置附近响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotABSADJ(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("获取机器人运动到原点位置附近响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 15）主机器人轴的绝对式原点复位(ABSRESET k)
BOOL  vcXYDVisionAlign::CommunicateRobotABSRESET(RobotInchAxis nAxis)
{
	scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
	stPackandSendTimer.Reset();


	CString strInfo;
	m_XYDCommProtocol.PackRobotABSRESET(strInfo, nAxis);
	if(FALSE == SendCommInfo(strInfo))
	{		
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("机器人绝对式原点复位通信出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}			

	CString str;
	dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
	// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
	// 	AlignLogRecord(str);
	BOOL bReceived;
	CString strReceived;

	m_tTimer.Reset();
	//	 Sleep(500);
	bReceived = ReceiveCommInfo(strReceived);


	while (FALSE == bReceived)
	{
		if (TRUE == m_bStopNow)
			return FALSE;

		bReceived = ReceiveCommInfo(strReceived);

		if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
		{			
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 =  _T("机器人绝对式原点复位响应通信超时！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}        

		Sleep(2);
	}	

	CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotABSRESET(strReceived);

	if (answeredStatus!=eAswSucceed)
	{		
		int i = 0;
		CCommOptionInfo commOptionInfo;
		GetCommOptionInfo(commOptionInfo);
		/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
		for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
		{

			CString strInfo;
			m_XYDCommProtocol.PackRobotABSRESET(strInfo, nAxis);
			if(FALSE == SendCommInfo(strInfo))
			{		
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("机器人绝对式原点复位通信出错！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}			

			CString str;
			dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
			// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
			// 	AlignLogRecord(str);
			BOOL bReceived;
			CString strReceived;

			m_tTimer.Reset();
			//	 Sleep(500);
			bReceived = ReceiveCommInfo(strReceived);


			while (FALSE == bReceived)
			{
				if (TRUE == m_bStopNow)
					return FALSE;

				bReceived = ReceiveCommInfo(strReceived);

				if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
				{			
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 =  _T("机器人绝对式原点复位响应通信超时！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}        

				Sleep(2);
			}	

			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotABSRESET(strReceived);
			if (answeredStatus == eAswSucceed)
			{
				return TRUE;
			}

		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
		m_strStatusBarInfo2 = _T("获取机机器人绝对式原点复位响应解析出错！");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		return FALSE;
	}		
	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}



// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
BOOL  vcXYDVisionAlign::CommunicateRobotWriteSGI(int nIndex, int nVal)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotWriteSGI(strInfo, nIndex, nVal);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置整型常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置整型常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGI(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotWriteSGI(strInfo, nIndex, nVal);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置整型常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置整型常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGI(strReceived);

				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置整型常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotWriteSGI340(strInfo, nIndex, nVal);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置整型常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置整型常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGI340(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotWriteSGI340(strInfo, nIndex, nVal);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置整型常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置整型常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGI340(strReceived);

				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置整型常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 17) 查询整型常量的值（@“机器人语言”，如@READ SGI0[cr/lf]）
BOOL  vcXYDVisionAlign::CommunicateRobotReadSGI(int nIndex, int& nVal)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadSGI(strInfo, nIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("查询机器人整型常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("查询机器人整型常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGI(strReceived, nVal);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{


				CString strInfo;
				m_XYDCommProtocol.PackRobotReadSGI(strInfo, nIndex);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("查询机器人整型常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("查询机器人整型常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGI(strReceived, nVal);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取查询机器人整型常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}	
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadSGI340(strInfo, nIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("查询机器人整型常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("查询机器人整型常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGI340(strFrontReceived, nVal);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人整型常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGIOK340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人整型常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGI340(strReceived, nVal);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人整型常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("查询机器人整型常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGIOK340(strReceived);
				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("获取查询机器人整型常量响应解析出错！");;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}

			}
		}		
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 18) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
BOOL  vcXYDVisionAlign::CommunicateRobotWriteSGR(int nIndex, double dVal)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}

	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotWriteSGR(strInfo, nIndex, dVal);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置实数常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置实数常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGR(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotWriteSGR(strInfo, nIndex, dVal);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置实数常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置实数常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGR(strReceived);

				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置实数常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotWriteSGR340(strInfo, nIndex, dVal);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("机器人设置实数常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("机器人设置实数常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGR340(strReceived);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{

				CString strInfo;
				m_XYDCommProtocol.PackRobotWriteSGR340(strInfo, nIndex, dVal);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("机器人设置实数常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("机器人设置实数常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotWriteSGR340(strReceived);

				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取机器人设置实数常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}

// 18) 查询实数常量的值（@“机器人语言”，如@READ SGR0[cr/lf]）
BOOL  vcXYDVisionAlign::CommunicateRobotReadSGR(int nIndex, double& dVal)
{
	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}

	if (TmpMotionControlType == eMCYamahaRobot)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadSGR(strInfo, nIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("查询机器人实数常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo(strReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo(strReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("查询机器人实数常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	

		CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGR(strReceived, dVal);

		if (answeredStatus!=eAswSucceed)
		{		
			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 10; i++)
			{


				CString strInfo;
				m_XYDCommProtocol.PackRobotReadSGR(strInfo, nIndex);
				if(FALSE == SendCommInfo(strInfo))
				{		
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("查询机器人实数常量出错！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}			

				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("查询机器人实数常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGR(strReceived, dVal);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}

			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("获取查询机器人实数常量响应解析出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}		
	}
	else if (TmpMotionControlType == eMCYamahaRobot340)
	{
		scTimer stPackandSendTimer; double dPackandSendTime(0.0);	
		stPackandSendTimer.Reset();


		CString strInfo;
		m_XYDCommProtocol.PackRobotReadSGR340(strInfo, nIndex);
		if(FALSE == SendCommInfo(strInfo))
		{		
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("查询机器人实数常量出错！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return FALSE;
		}			

		CString str;
		dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
		// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
		// 	AlignLogRecord(str);
		BOOL bReceived;
		CString strReceived;
		CString strFrontReceived;

		m_tTimer.Reset();
		//	 Sleep(500);
		bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);


		while (FALSE == bReceived)
		{
			if (TRUE == m_bStopNow)
				return FALSE;

			bReceived = ReceiveCommInfo2(strReceived, strFrontReceived);

			if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 =  _T("查询机器人实数常量响应通信超时！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}        

			Sleep(2);
		}	
		CmdAnsweredStatus answeredStatus, answeredFrongtStatus;
		if (!strFrontReceived.IsEmpty()) // 有两条指令回复
		{
			answeredFrongtStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGR340(strFrontReceived, dVal);
			if (answeredFrongtStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人实数常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGROK340(strReceived);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人实数常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

		}
		else // 只有1条指令回复
		{
			answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGR340(strReceived, dVal);
			if (answeredStatus!=eAswSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("获取查询机器人实数常量响应解析出错！");;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}
			else
			{
				CString str;
				dPackandSendTime=stPackandSendTimer.GetTimeMilli(FALSE);
				// 	str.Format("Get Position Pack and send time= %f",dPackandSendTime);
				// 	AlignLogRecord(str);
				BOOL bReceived;
				CString strReceived;

				m_tTimer.Reset();
				//	 Sleep(500);
				bReceived = ReceiveCommInfo(strReceived);


				while (FALSE == bReceived)
				{
					if (TRUE == m_bStopNow)
						return FALSE;

					bReceived = ReceiveCommInfo(strReceived);

					if (m_tTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
					{			
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
						m_strStatusBarInfo2 =  _T("查询机器人实数常量响应通信超时！");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

						return FALSE;
					}        

					Sleep(2);
				}	

				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswRobotReadSGROK340(strReceived);
				if (answeredStatus!=eAswSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = _T("获取查询机器人实数常量响应解析出错！");;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return FALSE;
				}

			}
		}		
	}

	// 	str.Format(" Get position Communicate Get answered Status time= %f",m_tTimer.GetTimeMilli(FALSE));
	// 	AlignLogRecord(str);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_SUCCEED);
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	return TRUE;
}


BOOL vcXYDVisionAlign::LoadRobotTeachRangeInfo()
{
	ASSERT(m_pVisionASMConfig != NULL);

	if (m_pVisionASMConfig == NULL)
	{
		return FALSE;
	}
	if (m_pPlatformInfo == NULL)
	{
		return FALSE;
	}
	CString strInfo;
	double dValue = 0;
	int i = 0, j = 0, nData = 0;
	CRobotTeachRangeInfo robotTeachRangeInfo;
	CString strName;

	CString strDir = m_strEXEDir + m_pPlatformInfo->m_strPlatformDetailSavePath 
		+ _T("\\RobotTeachRangeInfo.xml");

	vcXMLConfigurator m_XMLConfigator;
	m_XMLConfigator.SetDoc(_T(""));
	if (!m_XMLConfigator.Load(strDir))
	{
		return FALSE;
	}


	if (!m_XMLConfigator.FindElem(_T("RobotTeachRangeInfo")))// (Root)
	{
		return FALSE;
	}
	m_XMLConfigator.EnterElem();


	// 密码
	strInfo.Format(_T("Password"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 密码
	{
		robotTeachRangeInfo.m_strPassword = m_XMLConfigator.GetElemData();
	}


	// 手动速度
	strInfo.Format(_T("MSpeed"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 手动速度
	{
		m_XMLConfigator.EnterElem();


		strInfo.Format(_T("MSpeedRangeEnable"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度范围是否启用
		{
			m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_bMSpeedRangeEnable);
		}

		strInfo.Format(_T("MSpeedMax"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度最大
		{
			m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_nMSpeedMax);
		}

		strInfo.Format(_T("MSpeedMin"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度最最小
		{
			m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_nMSpeedMin);
		}


		m_XMLConfigator.LeaveElem();// (Level-1)
	}



	// 当前位置
	strInfo.Format(_T("CurRobotPos"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
	{
		m_XMLConfigator.EnterElem();


		strInfo.Format(_T("CurRobotPosRangeEnable"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
		{
			m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_bCurRobotPosRangeEnable);
		}

		strInfo.Format(_T("CurRobotPosMax"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置最大
		{
			m_XMLConfigator.EnterElem();

			strInfo.Format(_T("X"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大X
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosX);
			}

			strInfo.Format(_T("Y"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大Y
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosY);
			}

			strInfo.Format(_T("R"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大R
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosR);
			}

			strInfo.Format(_T("Z"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大Z
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosZ);
			}

			strInfo.Format(_T("A"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大A
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosA);
			}

			strInfo.Format(_T("B"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大B
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosB);
			}

			strInfo.Format(_T("CT"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置手系
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_nCoordType);
			}

			m_XMLConfigator.LeaveElem();
		}

		strInfo.Format(_T("CurRobotPosMin"));
		if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置最小
		{
			m_XMLConfigator.EnterElem();

			strInfo.Format(_T("X"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小X
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosX);
			}

			strInfo.Format(_T("Y"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小Y
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosY);
			}

			strInfo.Format(_T("R"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小R
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosR);
			}

			strInfo.Format(_T("Z"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小Z
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosZ);
			}

			strInfo.Format(_T("A"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小A
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosA);
			}

			strInfo.Format(_T("B"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小B
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosB);
			}

			strInfo.Format(_T("CT"));
			if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置手系
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_nCoordType);
			}

			m_XMLConfigator.LeaveElem();
		}

		m_XMLConfigator.LeaveElem();// (Level-1)
	}

	// 常量信息

	// 常量页
	strInfo.Format(_T("RobotTeachSGPageName"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 常量页名称
	{
		robotTeachRangeInfo.m_strRobotTeachSGPageName = m_XMLConfigator.GetElemData();
	}

	//整型常量信息 

	// 示教整型常量起始点
	strInfo.Format(_T("RobotTeachSGIntStartPoint"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教整型常量起始点
	{
		m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_nRobotTeachSGIntStartPoint);
	}


	// 整型常量
	for (i = 0; i < RobotTeachSGIntMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachSGInt%d"), i + robotTeachRangeInfo.m_nRobotTeachSGIntStartPoint);
		if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.EnterElem();

			strInfo.Format(_T("RobotTeachSGIntName"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
			{
				robotTeachRangeInfo.m_vstrRobotTeachSGIntName[i] = m_XMLConfigator.GetElemData();
			}

			strInfo.Format(_T("RobotTeachSGIntRangeEnable"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vbRobotTeachSGIntRangeEnable[i]);
			}


			strInfo.Format(_T("RobotTeachSGIntMax"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最大
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vnRobotTeachSGIntMax[i]);
			}

			strInfo.Format(_T("RobotTeachSGIntMin"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最小
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vnRobotTeachSGIntMin[i]);
			}

			m_XMLConfigator.LeaveElem();// (Level-1)
		}
	}

	// 实数常量信息 

	// 示教实数常量起始点
	strInfo.Format(_T("RobotTeachSGRealStartPoint"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教实数常量起始点
	{
		m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_nRobotTeachSGRealStartPoint);
	}

	// 实数常量
	for (i = 0; i < RobotTeachSGRealMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachSGReal%d"), i + robotTeachRangeInfo.m_nRobotTeachSGRealStartPoint);
		if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.EnterElem();

			strInfo.Format(_T("RobotTeachSGRealName"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
			{
				robotTeachRangeInfo.m_vstrRobotTeachSGRealName[i] = m_XMLConfigator.GetElemData();
			}

			strInfo.Format(_T("RobotTeachSGRealRangeEnable"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vbRobotTeachSGRealRangeEnable[i]);
			}


			strInfo.Format(_T("RobotTeachSGRealMax"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最大
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vdRobotTeachSGRealMax[i]);
			}

			strInfo.Format(_T("RobotTeachSGRealMin"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最小
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vdRobotTeachSGRealMin[i]);
			}

			m_XMLConfigator.LeaveElem();// (Level-1)
		}
	}



	// 示教点信息
	// 密码
	strInfo.Format(_T("RobotTeachPosStartPoint"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教起始点
	{
		m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_nRobotTeachPosStartPoint);
	}

	// 示教点位置示教页名称
	strInfo.Format(_T("RobotTearchPosPageName"));
	if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教点位置示教页名称
	{
		m_XMLConfigator.EnterElem();
		for (i = 0; i < RobotTeachPosMaxPageNum; i++)
		{
			strInfo.Format(_T("Page%d"), i+1);
			if (m_XMLConfigator.FindElem(strInfo))	// (Level-2) 示教页名称
			{
				robotTeachRangeInfo.m_vstrRobotTeachPosPageName[i] = m_XMLConfigator.GetElemData();
			}
		}
		m_XMLConfigator.LeaveElem();// (Level-1)
	}

	// 示教点位置
	for (i = 0; i < RobotTeachPosTotalMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachPosP%d"), i + robotTeachRangeInfo.m_nRobotTeachPosStartPoint);
		if (m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.EnterElem();

			strInfo.Format(_T("RobotTeachPosName"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
			{
				robotTeachRangeInfo.m_vstrRobotTeachPosName[i] = m_XMLConfigator.GetElemData();
			}

			strInfo.Format(_T("RobotTeachPosRangeEnable"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
			{
				m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vbRobotTeachPosRangeEnable[i]);
			}


			strInfo.Format(_T("RobotTeachPosMax"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 示教点位置最大
			{
				m_XMLConfigator.EnterElem();

				strInfo.Format(_T("X"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大X
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosX);
				}

				strInfo.Format(_T("Y"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大Y
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosY);
				}

				strInfo.Format(_T("R"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大R
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosR);
				}

				strInfo.Format(_T("Z"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大Z
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosZ);
				}

				strInfo.Format(_T("A"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大A
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosA);
				}

				strInfo.Format(_T("B"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大B
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosB);
				}

				strInfo.Format(_T("CT"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置手系
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_nCoordType);
				}

				m_XMLConfigator.LeaveElem();
			}

			strInfo.Format(_T("RobotTeachPosMin"));
			if (m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 示教点位置最小
			{
				m_XMLConfigator.EnterElem();

				strInfo.Format(_T("X"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小X
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosX);
				}

				strInfo.Format(_T("Y"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小Y
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosY);
				}

				strInfo.Format(_T("R"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小R
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosR);
				}

				strInfo.Format(_T("Z"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小Z
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosZ);
				}

				strInfo.Format(_T("A"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小A
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosA);
				}

				strInfo.Format(_T("B"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小B
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosB);
				}

				strInfo.Format(_T("CT"));
				if (m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置手系
				{
					m_XMLConfigator.GetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_nCoordType);
				}

				m_XMLConfigator.LeaveElem();
			}


			m_XMLConfigator.LeaveElem();// (Level-1)
		}
	}

	m_XMLConfigator.LeaveElem();

	m_RobotTeachRangeInfo = robotTeachRangeInfo;

	return TRUE;
}

BOOL vcXYDVisionAlign::SaveRobotTeachRangeInfo()
{
	ASSERT(m_pVisionASMConfig != NULL);
	ASSERT(m_pPlatformInfo != NULL);

	if (m_pVisionASMConfig == NULL)
	{
		return FALSE;
	}
	if (m_pPlatformInfo == NULL)
	{
		return FALSE;
	}
	CString strInfo;
	double dValue = 0;
	int i = 0, j = 0, nData = 0;

	CString strName;
	CString strDir = m_strEXEDir + m_pPlatformInfo->m_strPlatformDetailSavePath 
		+ _T("\\RobotTeachRangeInfo.xml");

	vcXMLConfigurator m_XMLConfigator;
	m_XMLConfigator.SetDoc(_T(""));
	m_XMLConfigator.Load(strDir);	
	m_XMLConfigator.ResetDocElemPos();

	CRobotTeachRangeInfo robotTeachRangeInfo = m_RobotTeachRangeInfo;

	if (!m_XMLConfigator.FindElem(_T("RobotTeachRangeInfo")))// (Root)
	{
		m_XMLConfigator.AddElem(_T("RobotTeachRangeInfo"));
	}
	m_XMLConfigator.EnterElem();


	// 密码
	strInfo.Format(_T("Password"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 密码
	{
		m_XMLConfigator.AddElem(strInfo);		
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_strPassword);

	// 手动速度
	strInfo.Format(_T("MSpeed"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 手动速度
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.EnterElem();


	strInfo.Format(_T("MSpeedRangeEnable"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度范围是否启用
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_bMSpeedRangeEnable);

	strInfo.Format(_T("MSpeedMax"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度最大
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_nMSpeedMax);

	strInfo.Format(_T("MSpeedMin"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 手动速度最最小
	{
		m_XMLConfigator.AddElem(strInfo);		
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_nMSpeedMin);

	m_XMLConfigator.LeaveElem();// (Level-1)



	// 当前位置
	strInfo.Format(_T("CurRobotPos"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.EnterElem();


	strInfo.Format(_T("CurRobotPosRangeEnable"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_bCurRobotPosRangeEnable);


	strInfo.Format(_T("CurRobotPosMax"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置最大
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.EnterElem();

	strInfo.Format(_T("X"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大X
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosX);

	strInfo.Format(_T("Y"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大Y
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosY);

	strInfo.Format(_T("R"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大R
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosR);

	strInfo.Format(_T("Z"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大Z
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosZ);

	strInfo.Format(_T("A"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大A
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosA);

	strInfo.Format(_T("B"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大B
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_dPosB);

	strInfo.Format(_T("CT"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最大CT
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMax.m_nCoordType);

	m_XMLConfigator.LeaveElem();// (Level-2)


	strInfo.Format(_T("CurRobotPosMin"));
	if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置最小
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.EnterElem();

	strInfo.Format(_T("X"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小X
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosX);

	strInfo.Format(_T("Y"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小Y
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosY);

	strInfo.Format(_T("R"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小R
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosR);

	strInfo.Format(_T("Z"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小Z
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosZ);

	strInfo.Format(_T("A"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小A
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosA);

	strInfo.Format(_T("B"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小B
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_dPosB);

	strInfo.Format(_T("CT"));
	if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 当前位置最小CT
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_rpCurRobotPosMin.m_nCoordType);

	m_XMLConfigator.LeaveElem(); // (Level-2)

	m_XMLConfigator.LeaveElem();// (Level-1)

	// 常量页
	strInfo.Format(_T("RobotTeachSGPageName"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 常量页名称
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_strRobotTeachSGPageName);

	//整型常量信息 

	// 示教整型常量起始点
	strInfo.Format(_T("RobotTeachSGIntStartPoint"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教整型常量起始点
	{
		m_XMLConfigator.AddElem(strInfo);		
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_nRobotTeachSGIntStartPoint);


	// 整型常量
	for (i = 0; i < RobotTeachSGIntMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachSGInt%d"), i + robotTeachRangeInfo.m_nRobotTeachSGIntStartPoint);
		if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.AddElem(strInfo);
		}

		m_XMLConfigator.EnterElem();

		strInfo.Format(_T("RobotTeachSGIntName"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vstrRobotTeachSGIntName[i]);

		strInfo.Format(_T("RobotTeachSGIntRangeEnable"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vbRobotTeachSGIntRangeEnable[i]);

		strInfo.Format(_T("RobotTeachSGIntMax"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最大
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vnRobotTeachSGIntMax[i]);

		strInfo.Format(_T("RobotTeachSGIntMin"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最小
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vnRobotTeachSGIntMin[i]);

		m_XMLConfigator.LeaveElem();// (Level-1)

	}

	// 实数常量信息 

	// 示教实数常量起始点
	strInfo.Format(_T("RobotTeachSGRealStartPoint"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教实数常量起始点
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_nRobotTeachSGRealStartPoint);

	// 实数常量
	for (i = 0; i < RobotTeachSGRealMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachSGReal%d"), i + robotTeachRangeInfo.m_nRobotTeachSGRealStartPoint);
		if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.EnterElem();

		strInfo.Format(_T("RobotTeachSGRealName"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vstrRobotTeachSGRealName[i]);

		strInfo.Format(_T("RobotTeachSGRealRangeEnable"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vbRobotTeachSGRealRangeEnable[i]);


		strInfo.Format(_T("RobotTeachSGRealMax"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最大
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vdRobotTeachSGRealMax[i]);

		strInfo.Format(_T("RobotTeachSGRealMin"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 最小
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vdRobotTeachSGRealMin[i]);

		m_XMLConfigator.LeaveElem();// (Level-1)
	}


	//

	// 示教点信息
	strInfo.Format(_T("RobotTeachPosStartPoint"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教起始点
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_nRobotTeachPosStartPoint);

	// 示教点位置示教页名称
	strInfo.Format(_T("RobotTearchPosPageName"));
	if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 示教点位置示教页名称
	{
		m_XMLConfigator.AddElem(strInfo);
	}
	m_XMLConfigator.EnterElem();
	for (i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		strInfo.Format(_T("Page%d"), i+1);
		if (!m_XMLConfigator.FindElem(strInfo))	// (Level-2) 示教页名称
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vstrRobotTeachPosPageName[i]);

	}
	m_XMLConfigator.LeaveElem();// (Level-1)


	// 示教点位置
	for (i = 0; i < RobotTeachPosTotalMaxNum; i++)
	{
		// 当前位置
		strInfo.Format(_T("RobotTeachPosP%d"), i + robotTeachRangeInfo.m_nRobotTeachPosStartPoint);
		if (!m_XMLConfigator.FindElem(strInfo))// (Level-1) 当前位置
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.EnterElem();

		strInfo.Format(_T("RobotTeachPosName"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 名称
		{
			m_XMLConfigator.AddElem(strInfo);			
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vstrRobotTeachPosName[i]);


		strInfo.Format(_T("RobotTeachPosRangeEnable"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 当前位置范围是否启用
		{
			m_XMLConfigator.AddElem(strInfo);	

		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vbRobotTeachPosRangeEnable[i]);

		strInfo.Format(_T("RobotTeachPosMax"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 示教点位置最大
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.EnterElem();

		strInfo.Format(_T("X"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大X
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosX);

		strInfo.Format(_T("Y"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大Y
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosY);

		strInfo.Format(_T("R"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大R
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosR);

		strInfo.Format(_T("Z"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大Z
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosZ);

		strInfo.Format(_T("A"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大A
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosA);

		strInfo.Format(_T("B"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大B
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_dPosB);

		strInfo.Format(_T("CT"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最大CT
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMax[i].m_nCoordType);

		m_XMLConfigator.LeaveElem();// (Level-2)

		strInfo.Format(_T("RobotTeachPosMin"));
		if (!m_XMLConfigator.FindElem(strInfo))	 // (Level-2) 示教点位置最小
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.EnterElem();

		strInfo.Format(_T("X"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小X
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosX);

		strInfo.Format(_T("Y"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小Y
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosY);

		strInfo.Format(_T("R"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小R
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosR);

		strInfo.Format(_T("Z"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小Z
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosZ);

		strInfo.Format(_T("A"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小A
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosA);

		strInfo.Format(_T("B"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小B
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_dPosB);

		strInfo.Format(_T("CT"));
		if (!m_XMLConfigator.FindElem(strInfo)) // (Level-3) 示教点位置最小CT
		{
			m_XMLConfigator.AddElem(strInfo);
		}
		m_XMLConfigator.SetElemData(robotTeachRangeInfo.m_vrpRobotTeachPosMin[i].m_nCoordType);

		m_XMLConfigator.LeaveElem();// (Level-2)


		m_XMLConfigator.LeaveElem();// (Level-1)
	}

	m_XMLConfigator.LeaveElem();

	return m_XMLConfigator.Save(strDir);
}