#include "stdafx.h"
#include "vsAuthorizeXMLTool.h"
//#include "HaspInfo.h"
#include "vsEn_Decrypt.h"
#include "base64.h"
#include "vsHaspInfo.h"

#include "VisionASMDLG.h"
#include "vsBaseVisionAlign.h"

vcAuthorizeXMLTool::vcAuthorizeXMLTool()
{
	m_pWndOwner = NULL;

	m_pAuthorizeCheckThread = NULL;
	m_bExitAuthorizeCheckThread = TRUE;

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath += _T("\\VSConfig\\Config.xml");

	m_strSysConfigFilePath = strPath;
	m_strSysExpireDate = _T("");
	m_strSysRegID = _T("");
	m_strRestDate = _T("");
	m_strRestSecond = _T("");
	m_strLastRunDate = _T("");

	m_nWarningDayNumber = 3;
}

vcAuthorizeXMLTool::~vcAuthorizeXMLTool()
{
	DisableAuthorizeCheck();
}

void vcAuthorizeXMLTool::SetOwner(CWnd* pWnd)
{
	m_pWndOwner = pWnd;
}

void vcAuthorizeXMLTool::SetWarningDayNumber(int nDayNumber)
{
	m_nWarningDayNumber = nDayNumber;
}

void vcAuthorizeXMLTool::EnableAuthorizeCheck()
{
	CreatAuthorizeCheckThread();
}

void vcAuthorizeXMLTool::DisableAuthorizeCheck()
{
	CloseAuthorizeCheckThread();
}

BOOL vcAuthorizeXMLTool::CreatAuthorizeCheckThread()
{
	if (m_pAuthorizeCheckThread != NULL)
	{
		CloseAuthorizeCheckThread();
	}

	m_bExitAuthorizeCheckThread = FALSE;

	m_pAuthorizeCheckThread = AfxBeginThread(AuthorizeCheckThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);	

	if (m_pAuthorizeCheckThread==NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL vcAuthorizeXMLTool::CloseAuthorizeCheckThread()
{
	if (m_pAuthorizeCheckThread)
	{
		m_bExitAuthorizeCheckThread = TRUE;

		if (WaitForSingleObject(m_pAuthorizeCheckThread->m_hThread, 5000) == WAIT_TIMEOUT)
		{
			TerminateThread(m_pAuthorizeCheckThread->m_hThread, 0);
		}

		m_pAuthorizeCheckThread = NULL;
	}

	return TRUE;
}

BOOL vcAuthorizeXMLTool::CheckDateTimeIsValid(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
	if ( (nYear >= 1900) && (nMonth >= 1 && nMonth <= 12) && (nDay >= 1 && nDay <= 31) && (nHour >= 0 && nHour <= 23) && (nMinute >= 0 && nMinute <= 59) && (nSecond >= 0 && nSecond <= 59) )
	{
		return TRUE;
	}

	return FALSE;
}

UINT vcAuthorizeXMLTool::AuthorizeCheckThread(LPVOID pParam)
{
	vcAuthorizeXMLTool* pAuthorizeXMLTool = (vcAuthorizeXMLTool*)pParam;
	if (pAuthorizeXMLTool == NULL)
	{
		return 0;
	}

	CVisionASMDlg* pOwner = (CVisionASMDlg*)(pAuthorizeXMLTool->m_pWndOwner);
	if (pOwner != NULL)
	{
		SysPlatformInfo locSysPlatformInfo;
		pOwner->m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);
		BOOL bfirstShow0 = TRUE;
		BOOL bfirstShow1 = TRUE;
		BOOL bfirstShow2 = TRUE;
		BOOL bfirstShow3 = TRUE;
		BOOL bfirstShow4 = TRUE;
		BOOL bfirstShow5 = TRUE;

		CString strRegisterCode;
		if (!pAuthorizeXMLTool->GetSystemRegisterCode(strRegisterCode))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		CString strRegID;
		if (!pAuthorizeXMLTool->GetSystemRegID(strRegID))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		CString strRestDate;
		if (!pAuthorizeXMLTool->GetSystemRestDate(strRestDate))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		CString strRestSecond;
		if (!pAuthorizeXMLTool->GetSystemRestSecond(strRestSecond))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		CString strLastRunDate;
		if (!pAuthorizeXMLTool->GetSystemLastRundate(strLastRunDate))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		CString strExpiredate;
		if (!pAuthorizeXMLTool->GetExpiredate(strExpiredate))
		{
			CString strmsg;
			strmsg= GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			pOwner->AddTitleWarningMsg(strmsg);
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		if (CheckIfDateInvalid(strExpiredate)==FALSE)
		{
			CString strmsg;
			strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_SOFT_NOT_REGISTER);//.Format("软件未注册，请联系厂商\n 联系电话：13923742436");
			pOwner->AddTitleWarningMsg(strmsg);		
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}


		if (CheckIfDateInvalid(strLastRunDate)==FALSE)
		{
			CString strmsg;
			strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_SOFT_NOT_REGISTER);//.Format("软件未注册，请联系厂商\n 联系电话：13923742436");
			pOwner->AddTitleWarningMsg(strmsg);		
			for (int i=0; i<pOwner->m_nPlatformNum; i++)
			{
				pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return 0;
		}

		while(FALSE == pAuthorizeXMLTool->m_bExitAuthorizeCheckThread)
		{
			EnterCriticalSection(&pOwner->m_CSLanguage);
			if (pAuthorizeXMLTool->m_bExitAuthorizeCheckThread)
			{
				LeaveCriticalSection(&pOwner->m_CSLanguage);
				break;
			}

			//当前日期时间
			SYSTEMTIME curTime;
			GetLocalTime(&curTime);
			CString strCurtime;
			strCurtime.Format("%d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay);


			int nddate = DaysBetween2Date(strLastRunDate,strCurtime);
			if (nddate>=1)  //系统时间被改变
			{
				CString strmsg;
				strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_SYSTEMTIME_ABNORMAL);//.Format("软件已到期，请联系厂商\n 联系电话：13923742436");
				if (bfirstShow5)
				{
					pOwner->AddTitleWarningMsg(strmsg);
					bfirstShow0 = TRUE;
					bfirstShow1 = TRUE;
					bfirstShow2 = TRUE;
					bfirstShow3 = TRUE;
					bfirstShow4 = TRUE;
					bfirstShow5 = FALSE;

					for (int i=0; i<pOwner->m_nPlatformNum; i++)
					{
						pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
						locSysPlatformInfo.m_bRegisterSuccess = FALSE;
						pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
						pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
					}
				}
				LeaveCriticalSection(&pOwner->m_CSLanguage);
				continue;

			}
			else if(nddate<0) //运行超过一天，记录最后时间
			{
				strLastRunDate = strCurtime;
				pAuthorizeXMLTool->m_strLastRunDate = base64_encode_str(strLastRunDate.GetBuffer(strLastRunDate.GetLength()),strLastRunDate.GetLength());
				pAuthorizeXMLTool->SaveSystemLastRundate();
			}

			int ndeltadate = DaysBetween2Date(strExpiredate,strCurtime);

			int ndletasecond = ndeltadate*24*3600 + (12 - curTime.wHour)*3600 + (0- curTime.wMinute)*60 + (0-curTime.wSecond);

			int nRestDate = atoi(strRestDate);
			int nRestSecond = atoi(strRestSecond);


			if (ndeltadate<nRestDate) //计算出的剩余天数比保存的剩余天数小
			{
				nRestDate = ndeltadate;
				strRestDate.Format("%d",nRestDate);
				pAuthorizeXMLTool->m_strRestDate = base64_encode_str(strRestDate.GetBuffer(100),strRestDate.GetLength());
				pAuthorizeXMLTool->SaveSystemRestDate();

			}
			else if (nRestDate<ndeltadate)//计算出的剩余天数比保存的剩余天数大（系统时间被改变)
			{
				ndeltadate = nRestDate;
			}

			if (nRestSecond<ndletasecond) //时间已经被改变
			{
				ndletasecond = nRestSecond;
			}


			if (ndeltadate<=pAuthorizeXMLTool->m_nWarningDayNumber && ndeltadate>0) //提前三天提示
			{	
				//m_bExpired = FALSE;
				CString strmsg;
				strmsg.Format(GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE_COMMING),ndeltadate);

				if (bfirstShow0)
				{
					pOwner->AddTitleWarningMsg(strmsg);
					bfirstShow0 = FALSE;
					bfirstShow1 = TRUE;
					bfirstShow2 = TRUE;
					bfirstShow3 = TRUE;
					bfirstShow4 = TRUE;
					bfirstShow5 = TRUE;

					for (int i=0; i<pOwner->m_nPlatformNum; i++)
					{
						pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
						locSysPlatformInfo.m_bRegisterSuccess = TRUE;
						pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
						pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
					}
				}


			}
			else if (ndeltadate <0)// 到期
			{		

				CString strmsg;
				strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE);//.Format("软件已到期，请联系厂商\n 联系电话：13923742436");
				if (bfirstShow1)
				{
					pOwner->AddTitleWarningMsg(strmsg);
					bfirstShow0 = TRUE;
					bfirstShow1 = FALSE;
					bfirstShow2 = TRUE;
					bfirstShow3 = TRUE;
					bfirstShow4 = TRUE;
					bfirstShow5 = TRUE;

					for (int i=0; i<pOwner->m_nPlatformNum; i++)
					{
						pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
						locSysPlatformInfo.m_bRegisterSuccess = FALSE;
						pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
						pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
					}
				}



			}
			else if (ndeltadate==0)//当天
			{
				CString strmsg;

				if (curTime.wHour>=12 || ndletasecond< 0)
				{
					strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE);//.Format("软件已到期，请联系厂商\n 联系电话：13923742436");	
					if (bfirstShow2)
					{
						nRestSecond = ndletasecond;
						strRestSecond.Format("%d",nRestSecond);
						pAuthorizeXMLTool->m_strRestSecond = base64_encode_str(strRestSecond.GetBuffer(100),strRestSecond.GetLength());
						pAuthorizeXMLTool->SaveSystemRestSecond();

						pOwner->AddTitleWarningMsg(strmsg);
						bfirstShow0 = TRUE;
						bfirstShow1 = TRUE;
						bfirstShow2 = FALSE;
						bfirstShow3 = TRUE;
						bfirstShow4 = TRUE;
						bfirstShow5 = TRUE;

						for (int i=0; i<pOwner->m_nPlatformNum; i++)
						{
							pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
							locSysPlatformInfo.m_bRegisterSuccess = FALSE;
							pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
							pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
						}
					}	



				}
				else
				{
					strmsg = GetSysInfoString(pOwner->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE_BEFORNOON);//.Format("软件将于当天12：00到期，请联系厂商\n 联系电话：13923742436");					
					if (bfirstShow3)
					{
						pOwner->AddTitleWarningMsg(strmsg);
						bfirstShow0 = TRUE;
						bfirstShow1 = TRUE;
						bfirstShow2 = TRUE;
						bfirstShow3 = FALSE;
						bfirstShow4 = TRUE;
						bfirstShow5 = TRUE;

						for (int i=0; i<pOwner->m_nPlatformNum; i++)
						{
							pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
							locSysPlatformInfo.m_bRegisterSuccess = TRUE;
							pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
							pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
						}
					}	

				}

			}
			else if (ndeltadate>pAuthorizeXMLTool->m_nWarningDayNumber) //正常使用
			{
				CString strmsg;
				strmsg.Empty();
				if (bfirstShow4)
				{
					pOwner->AddTitleWarningMsg(strmsg);
					bfirstShow0 = TRUE;
					bfirstShow1 = TRUE;
					bfirstShow2 = TRUE;
					bfirstShow3 = TRUE;
					bfirstShow4 = FALSE;

					for (int i=0; i<pOwner->m_nPlatformNum; i++)
					{
						pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
						locSysPlatformInfo.m_bRegisterSuccess = TRUE;
						pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
						pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
					}
				}


				//return 0;
			}
			else   //其他
			{
				for (int i=0; i<pOwner->m_nPlatformNum; i++)
				{
					pOwner->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
					locSysPlatformInfo.m_bRegisterSuccess = FALSE;
					pOwner->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
					pOwner->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
				}
			}




			LeaveCriticalSection(&pOwner->m_CSLanguage);

			Sleep(100);
		}
	}

	return 0;
}

AuthorizeXMLStatus vcAuthorizeXMLTool::CheckAuthorize(CString& strRegDate)
{
	if (m_pWndOwner == NULL)
	{
		return eAuthorizeXMLError;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	SysPlatformInfo locSysPlatformInfo;
	pDlg->m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);
	if (locSysPlatformInfo.m_bExpireWarningEnabled)
	{
		CString strRegisterCode;
		if (!GetSystemRegisterCode(strRegisterCode))
		{
			return eAuthorizeXMLError;
		}

		CString strRegID;
		if (!GetSystemRegID(strRegID))
		{
			return eAuthorizeXMLError;
		}

		CString strRestDate;
		if (!GetSystemRestDate(strRestDate))
		{
			return eAuthorizeXMLError;
		}

		CString strRestSecond;
		if (!GetSystemRestSecond(strRestSecond))
		{
			return eAuthorizeXMLError;
		}

		CString strLastRunDate;
		if (!GetSystemLastRundate(strLastRunDate))
		{
			return eAuthorizeXMLError;
		}

		CString strExpiredate;
		if (!GetExpiredate(strExpiredate))
		{
			AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST));
			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return eAuthorizeXMLError;
		}

		if (CheckIfDateInvalid(strExpiredate)==FALSE)
		{
			AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_SOFT_NOT_REGISTER));
			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return eAuthorizeXMLError;
		}

		//当前日期时间
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		CString strCurtime;
		strCurtime.Format("%d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay);

		int nddate = DaysBetween2Date(strLastRunDate,strCurtime);//最后运行日期要比当前日期小
		if (nddate>0)  //如果大，证明日期被改变
		{
			AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings, IDS_STR_SYSTEMTIME_ABNORMAL));
			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
			return eAuthorizeXMLError;
		}
		else if(nddate<0)//保存最后运行日期
		{
			strLastRunDate = strCurtime;
			m_strLastRunDate = base64_encode_str(strLastRunDate.GetBuffer(strLastRunDate.GetLength()),strLastRunDate.GetLength());
			SaveSystemLastRundate();
		}

		int ndeltadate = DaysBetween2Date(strExpiredate,strCurtime);
		int ndletasecond = ndeltadate*24*3600 + (12 - curTime.wHour)*3600 + (0- curTime.wMinute)*60 + (0-curTime.wSecond);

		int nRestDate = atoi(strRestDate);
		int nRestSecond = atoi(strRestSecond);

		if (ndeltadate<nRestDate) //计算出的剩余天数比保存的剩余天数小
		{
			nRestDate = ndeltadate;
			strRestDate.Format("%d",nRestDate);
			m_strRestDate = base64_encode_str(strRestDate.GetBuffer(100),strRestDate.GetLength());
			SaveSystemRestDate();
		}
		else if (nRestDate<ndeltadate) //计算出的剩余天数比保存的剩余天数大（系统时间被改变)
		{
			ndeltadate = nRestDate;

		}

		if (ndletasecond<nRestSecond)
		{
			nRestSecond = ndletasecond;
			strRestSecond.Format("%d",nRestSecond);
			m_strRestSecond = base64_encode_str(strRestSecond.GetBuffer(100),strRestSecond.GetLength());
			SaveSystemRestSecond();
		}
		else if (nRestSecond<ndletasecond) //时间已经被改变
		{
			ndletasecond = nRestSecond;
		}



		if (ndeltadate<=m_nWarningDayNumber && ndeltadate>0) //提前三天提示
		{	
			CString strmsg;
			strmsg.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE_COMMING),ndeltadate);
			AfxMessageBox(strmsg);

			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = TRUE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}

			return eAuthorizeXMLWarning;
		}
		else if (ndeltadate <0)// 到期
		{		

			CString strmsg;
			strmsg.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE));
			AfxMessageBox(strmsg);

			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}

			return eAuthorizeXMLExpire;
		}
		else if (ndeltadate==0)//当天
		{
			CString strmsg;
			if (curTime.wHour>=12)
			{
				strmsg= GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE);
				AfxMessageBox(strmsg);

				for (int i=0; i<pDlg->m_nPlatformNum; i++)
				{
					pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
					locSysPlatformInfo.m_bRegisterSuccess = FALSE;
					pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
					pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
				}

				nRestSecond = -1;
				strRestSecond.Format("%d",nRestSecond);
				m_strRestSecond = base64_encode_str(strRestSecond.GetBuffer(100),strRestSecond.GetLength());
				SaveSystemRestSecond();

				return eAuthorizeXMLExpire;
			}
			else
			{
				strmsg= GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE_BEFORNOON);//.Format("软件将于当天12：00到期，请联系厂商\n 联系电话：13923742436");
				AfxMessageBox(strmsg);

				for (int i=0; i<pDlg->m_nPlatformNum; i++)
				{
					pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
					locSysPlatformInfo.m_bRegisterSuccess = TRUE;
					pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
					pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
				}

				return eAuthorizeXMLWarning;
			}
		}
		else if (ndeltadate>m_nWarningDayNumber) //正常使用
		{
			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = TRUE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}

			return eAuthorizeXMLValid;
		}
		else   //其他
		{
			for (int i=0; i<pDlg->m_nPlatformNum; i++)
			{
				pDlg->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				pDlg->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				pDlg->m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}

			return eAuthorizeXMLError;
		}

		return eAuthorizeXMLError;
	}

	return eAuthorizeXMLError;
}

BOOL vcAuthorizeXMLTool::Register(CString strRegisterCode, CString strRegID, CString& strRegDate)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	LoadSystemExpireDate();
	LoadSystemRestDate();
	LoadSystemLastRundate();

	CString strExpiredate;
	if (Decryptstr(strRegisterCode, strRegID, strExpiredate)==FALSE)
	{
		AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_CODE_ERROR),MB_ICONSTOP);
		return FALSE;
	}

	if (CheckIfDateInvalid(strExpiredate)==FALSE)
	{
		AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_CODE_ERROR),MB_ICONSTOP);
		return FALSE;
	}
	else
	{
		int year,month,date;
		StringToDate(strExpiredate,year,month,date);
		CString strRegisterdate;

		//注册日期判断
		CString strCurtime;
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		strCurtime.Format("%d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay);

		//////////////////////////////////////////////////////////////////////////
		//判断注册时间，与当前时间问题
		CString strLastRunDate = base64_decode_str(pDlg->m_SysOptionConfig.m_strLastRunDate.GetBuffer(100),pDlg->m_SysOptionConfig.m_strLastRunDate.GetLength());
		int nddate = DaysBetween2Date(strLastRunDate,strCurtime);
		if (nddate>0) //最后一次运行时间要比注册晚，说明日期被改变
		{
			AfxMessageBox(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_SYSTEMTIME_ABNORMAL),MB_ICONSTOP);
			return FALSE;
		}
		else if(nddate<=0)
		{
			m_strLastRunDate = base64_encode_str(strCurtime.GetBuffer(strCurtime.GetLength()),strCurtime.GetLength());
			SaveSystemLastRundate();
		}

		//////////////////////////////////////////////////////////////////////////


		int ndeltadate = DaysBetween2Date(strExpiredate,strCurtime);


		CString strRestDate;	
		strRestDate.Format("%d",ndeltadate);
		m_strRestDate = base64_encode_str(strRestDate.GetBuffer(100),strRestDate.GetLength());	
		SaveSystemRestDate();

		int ndeltaSecond =0;
		ndeltaSecond = ndeltadate*24*3600 + (12 - curTime.wHour)*3600 + (0- curTime.wMinute)*60 + (0-curTime.wSecond);
		CString strRestSecond;
		strRestSecond.Format("%d",ndeltaSecond);
		m_strRestSecond = base64_encode_str(strRestSecond.GetBuffer(100),strRestSecond.GetLength());	
		SaveSystemRestSecond();


		if (ndeltadate<0)
		{		
			strRegisterdate.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_FAILED),year,month,date);
			AfxMessageBox(strRegisterdate,MB_ICONSTOP);

			return FALSE;
		}
		else if (ndeltadate==0)
		{
			if (curTime.wHour>=12)
			{
				strRegisterdate.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_FAILED),year,month,date);
				AfxMessageBox(strRegisterdate,MB_ICONSTOP);

				return FALSE;
			}
			else
			{
				strRegisterdate.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_SUCCESS),year,month,date);
				AfxMessageBox(strRegisterdate);

				return TRUE;
			}
		}
		else
		{
			strRegisterdate.Format(GetSysInfoString(pDlg->m_psaSysInfoStrings,IDS_STR_REGISTER_SUCCESS),year,month,date);
			AfxMessageBox(strRegisterdate);

			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

BOOL vcAuthorizeXMLTool::GetUseDongleID(CString& strDongleID)
{
// 	string stringID;
// 	FindDongleID(stringID);
// 
// 	strDongleID = stringID.c_str();

	if (!GetDongleID(strDongleID))
	{
		return FALSE;
	}	

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetSystemRegisterCode(CString& strRegisterCode)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	//获取注册码
	LoadSystemExpireDate();
	strRegisterCode = m_strSysExpireDate;

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetSystemRegID(CString& strRegID)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	SysPlatformInfo locSysPlatformInfo;
	pDlg->m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);
	if (locSysPlatformInfo.m_bUseDongleID)
	{
		GetUseDongleID(strRegID);
	}
	else
	{
		LoadSystemRegID();		
		strRegID = m_strSysRegID;
	}

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetSystemRestDate(CString& strRestDate)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	LoadSystemRestDate();
	strRestDate = base64_decode_str(m_strRestDate.GetBuffer(100),m_strRestDate.GetLength());

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetSystemRestSecond(CString& strRestSecond)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	LoadSystemRestSecond();
	strRestSecond = base64_decode_str(m_strRestSecond.GetBuffer(100),m_strRestSecond.GetLength());

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetSystemLastRundate(CString& strLastRunDate)
{
	if (m_pWndOwner == NULL)
	{
		return FALSE;
	}

	CVisionASMDlg* pDlg = (CVisionASMDlg*)m_pWndOwner;

	LoadSystemLastRundate();
	strLastRunDate = base64_decode_str(m_strLastRunDate.GetBuffer(m_strLastRunDate.GetLength()),m_strLastRunDate.GetLength());

	return TRUE;
}

BOOL vcAuthorizeXMLTool::GetExpiredate(CString& strExpiredate)
{
	CString strRegisterCode;
	if (!GetSystemRegisterCode(strRegisterCode))
	{
		return FALSE;
	}

	CString strRegID;
	if (!GetSystemRegID(strRegID))
	{
		return FALSE;
	}

	if (!Decryptstr(strRegisterCode,strRegID,strExpiredate))
	{
		return FALSE;
	}

	return TRUE;
}

CString vcAuthorizeXMLTool::GetCurrenttime()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	CString strCurtime = _T("");
	strCurtime.Format("%d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay);

	return strCurtime;
}

BOOL vcAuthorizeXMLTool::LoadSystemExpireDate()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();
			if (xml.FindElem(_T("ExpireDate")))
			{
				m_strSysExpireDate = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}


			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL vcAuthorizeXMLTool::SaveSystemExpireDate()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("ExpireDate")))
	{
		xml.AddElem(_T("ExpireDate"));
	}
	xml.SetElemData(m_strSysExpireDate);

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}

BOOL vcAuthorizeXMLTool::LoadSystemRegID()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();


			if (xml.FindElem(_T("RegisterID")))
			{
				m_strSysRegID = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL vcAuthorizeXMLTool::SaveSystemRegID()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("RegisterID")))
	{
		xml.AddElem(_T("RegisterID"));
	}
	xml.SetElemData(m_strSysRegID);

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}

BOOL vcAuthorizeXMLTool::LoadSystemRestDate()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();


			if (xml.FindElem(_T("RestDate")))
			{
				m_strRestDate = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL vcAuthorizeXMLTool::SaveSystemRestDate()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("RestDate")))
	{
		xml.AddElem(_T("RestDate"));
	}
	xml.SetElemData(m_strRestDate);

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}

BOOL vcAuthorizeXMLTool::LoadSystemRestSecond()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();


			if (xml.FindElem(_T("RestSecond")))
			{
				m_strRestSecond = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL vcAuthorizeXMLTool::SaveSystemRestSecond()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("RestSecond")))
	{
		xml.AddElem(_T("RestSecond"));
	}
	xml.SetElemData(m_strRestSecond);

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}

BOOL vcAuthorizeXMLTool::LoadSystemLastRundate()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();


			if (xml.FindElem(_T("LastRunDate")))
			{
				m_strLastRunDate = xml.GetElemData();
				CString strLastRundate = base64_decode_str(m_strLastRunDate.GetBuffer(m_strLastRunDate.GetLength()),m_strLastRunDate.GetLength());
				CString strLastOfDir = GetLastDateOfDir();

				int ndeltadate = DaysBetween2Date(strLastOfDir,strLastRundate);
				if (ndeltadate>0)
				{
					m_strLastRunDate = base64_encode_str(strLastOfDir.GetBuffer(strLastOfDir.GetLength()),strLastOfDir.GetLength());
				}

			}
			else
			{
				CString strLastOfDir = GetLastDateOfDir();
				m_strLastRunDate = base64_encode_str(strLastOfDir.GetBuffer(strLastOfDir.GetLength()),strLastOfDir.GetLength());

				//bSucceed = FALSE;
			}
			xml.LeaveElem();
		}
		else
		{
			CString strLastOfDir = GetLastDateOfDir();
			m_strLastRunDate = base64_encode_str(strLastOfDir.GetBuffer(strLastOfDir.GetLength()),strLastOfDir.GetLength());
		}
	}
	else
	{
		CString strLastOfDir = GetLastDateOfDir();
		m_strLastRunDate = base64_encode_str(strLastOfDir.GetBuffer(strLastOfDir.GetLength()),strLastOfDir.GetLength());
	}

	return bSucceed;
}

BOOL vcAuthorizeXMLTool::SaveSystemLastRundate()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("LastRunDate")))
	{
		xml.AddElem(_T("LastRunDate"));
	}
	xml.SetElemData(m_strLastRunDate);

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}

CString vcAuthorizeXMLTool::GetLastDateOfDir()
{
	FILETIME ftCreate={0},ftLastWrite={0},ftAccess={0};
	SYSTEMTIME systime;
	CString strLastWrite;
	// 获取当前EXE文件夹的路径
	CString strEXEDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\'));  

	HANDLE hFile = CreateFile(strEXEDir,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		GetFileTime(hFile,&ftCreate,&ftAccess,&ftLastWrite);
		FileTimeToSystemTime(&ftLastWrite,&systime);
		strLastWrite.Format("%d%02d%02d",systime.wYear,systime.wMonth,systime.wDay);
	//	m_strLastRunDate = base64_encode_str(strLastWrite.GetBuffer(strLastWrite.GetLength()),strLastWrite.GetLength());
	}
	else
	{
		strLastWrite.Format("30001231");
	//	m_strLastRunDate = base64_encode_str(strLastWrite.GetBuffer(strLastWrite.GetLength()),strLastWrite.GetLength());
	}

	CloseHandle(hFile);
	return strLastWrite;
}