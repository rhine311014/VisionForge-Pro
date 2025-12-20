// vsXYDVisionAlignCheck.cpp
// 检查模式

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

BOOL vcXYDVisionAlign::CheckTargetDistance()
{
	// 获取当前产品序号
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		return FALSE;
	}

	// 获取当前产品信息
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		return FALSE;
	}

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	if (pAlignerParam == NULL)
	{
		return FALSE;
	}

// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}


	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = GetPosNum();
	CString strInfo = _T("");
	CString strTemp = _T("");


	for(int i = 0; i < TARGETSIZEMAXNUM; i++)
	{
		m_AlignerTargetSizeInfo.m_vdTargetSize[i] = 0;
		m_AlignerTargetSizeInfo.m_vnCheckStatus[i] = eSCSUnCheck;
	}

	int nExProductIdx = 0;
	if (optionInfo.m_bTargetSearchExProduct)
	{
		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
	}
	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);

	if (m_pSystempOptionConfig->m_bCheckModeOnly)
	{
		checkInfo.m_nCheckMode = 1;
		checkInfo.m_bCheckTargetAndObjectDis = TRUE;
		checkInfo.m_bCheckTargetDis = TRUE;
	}

	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
	{
		return TRUE;
	}

// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam)
// 	{
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialTargetMarkPlatformPos();
// 
// 		if (nAlignnExProductIndex == 0)
// 		{
// 			double dDis;
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 			dDis = vMark[0].Distance(vMark[1]);
// 
// 			if (checkInfo.m_nCheckMode == 0)
// 			{
// 				// 输入模式
// 				checkInfo.m_dTargetDis0 = dDis;
// 
// 				EnterCriticalSection(&m_csAlignCheckInfo);
// 				pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 				SaveProductDataInfo(nCurProductIndex);
// 
// 				PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 				m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
// 				PostMessageUpdateAlignerTargetSizeInfo();
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台4 X:%.3f Y:%.3f,平台5 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式45 D:%.3f,"), checkInfo.m_dTargetDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式45 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果45 R:%d,"), TRUE);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				return TRUE;
// 			}
// 			else
// 			{
// 				// 检查模式
// 				m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;
// 
// 				BOOL bTargetDis0 = (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold);
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台4 X:%.3f Y:%.3f,平台5 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式45 D:%.3f,"), checkInfo.m_dTargetDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式45 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果45 R:%d,"), bTargetDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 	
// 
// 				// 目标位置4+位置5
// //				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
// 				if(!bTargetDis0)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 
// 					PostMessageUpdateAlignerTargetSizeInfo();
// 					return FALSE;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 
// 					PostMessageUpdateAlignerTargetSizeInfo();
// 					return TRUE;
// 				}
// 
// 			}
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis;
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 			dDis = vMark[0].Distance(vMark[1]);
// 
// 			if (checkInfo.m_nCheckMode == 0)
// 			{
// 				// 输入模式
// 				checkInfo.m_dTargetDis1 = dDis;
// 
// 				EnterCriticalSection(&m_csAlignCheckInfo);
// 				pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 				SaveProductDataInfo(nCurProductIndex);
// 
// 				PostMessageUpdateAlignerTargetStdSizeInfo();
// 				m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis;
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
// 				PostMessageUpdateAlignerTargetSizeInfo();
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台6 X:%.3f Y:%.3f,平台7 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式67 D:%.3f,"), checkInfo.m_dTargetDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式67 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T(" 结果67 R:%d,"), TRUE);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				return TRUE;
// 			}
// 			else
// 			{
// 				// 检查模式
// 				m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis;
// 
// 				BOOL bTargetDis1 = (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) <= checkInfo.m_dTargetDisThreshold);
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 目标产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台6 X:%.3f Y:%.3f,平台7 X:%.3f Y:%.3f"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式67 D:%.3f,"), checkInfo.m_dTargetDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式67 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果67 R:%d,"), bTargetDis1);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				// 目标位置6+位置7
// //				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
// 				if(!bTargetDis1)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 
// 					PostMessageUpdateAlignerTargetSizeInfo();
// 					return FALSE;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 
// 					PostMessageUpdateAlignerTargetSizeInfo();
// 					return TRUE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 	}

	if (!sysPlatformInfo.m_bTargetObjectCamSeparate)
	{
		// 目标、对象不分离
		switch(nPosNum)
		{
		case 2:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis = 0;
				sc2Vector vMark[2];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
				dDis = vMark[0].Distance(vMark[1]);

				if (checkInfo.m_nCheckMode == 0)
				{
					// 输入模式
					checkInfo.m_dTargetDis0 = dDis;
					checkInfo.m_dTargetDis1 = 0;
					checkInfo.m_dTargetDis2 = 0;
					checkInfo.m_dTargetDis3 = 0;
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();
					// 输入模式时，刷新当前值
					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;			// _T("TargetLen12");
					PostMessageUpdateAlignerTargetSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dTargetDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;			// _T("TargetLen12");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dTargetDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), bTargetDis0);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}



//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis0)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

						PostMessageUpdateAlignerTargetSizeInfo();

						return FALSE;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

						PostMessageUpdateAlignerTargetSizeInfo();
						
						return TRUE;
					}
				}
			}
			break;

		case 3:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis[3];
				sc2Vector vMark[3];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);


				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[0].Distance(vMark[2]);
				dDis[2] = vMark[1].Distance(vMark[2]);

				if (checkInfo.m_nCheckMode == 0)	
				{				
					// 输入模式
					checkInfo.m_dTargetDis0 = dDis[0];
					checkInfo.m_dTargetDis1 = dDis[1];
					checkInfo.m_dTargetDis2 = dDis[2];
					checkInfo.m_dTargetDis3 = 0;
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();

					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen12");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen13");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen23");
					PostMessageUpdateAlignerTargetSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式02 D:%.3f 输入模式12 D:%.3f,"), checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式02 D:%.3f 检查模式12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果01 R:%d 结果02 R:%d 结果12 R:%d,"), TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen12");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen13");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen23");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis1 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis2 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) <= checkInfo.m_dTargetDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式02 D:%.3f 输入模式12 D:%.3f,"), checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式02 D:%.3f 检查模式12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果01 R:%d 结果02 R:%d 结果12 R:%d,"), bTargetDis0, bTargetDis1, bTargetDis2);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis0)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}

//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis1)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis2)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if (bTargetDis0 && bTargetDis1 && bTargetDis2)
					{
						PostMessageUpdateAlignerTargetSizeInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerTargetSizeInfo();
						return FALSE;
					}


				}
			}
			break;

		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis[4];
				sc2Vector vMark[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);

				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[2].Distance(vMark[3]);
				dDis[2] = vMark[0].Distance(vMark[2]);
				dDis[3] = vMark[1].Distance(vMark[3]);


				if (checkInfo.m_nCheckMode == 0)	
				{	
					// 输入模式
					checkInfo.m_dTargetDis0 = dDis[0];
					checkInfo.m_dTargetDis1 = dDis[1];
					checkInfo.m_dTargetDis2 = dDis[2];
					checkInfo.m_dTargetDis3 = dDis[3];
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();

					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];				// _T("TargetLen12");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];				// _T("TargetLen34");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];				// _T("TargetLen13");
					m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];				// _T("TargetLen24");
					PostMessageUpdateAlignerTargetSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), 
							checkInfo.m_dTargetDis0, 
							checkInfo.m_dTargetDis1, 
							checkInfo.m_dTargetDis2, 
							checkInfo.m_dTargetDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];				// _T("TargetLen12");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];				// _T("TargetLen34");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];				// _T("TargetLen13");
					m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];				// _T("TargetLen24");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis1 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis2 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis3 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) <= checkInfo.m_dTargetDisThreshold );


					// 目标位置1+位置2
					if (m_nCheckTargetDistanceIndex == 19)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y%.3f,平台3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 0.000, 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), checkInfo.m_dTargetDis0, 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), bTargetDis0, TRUE, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis0)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

							PostMessageUpdateAlignerTargetSizeInfo();

							return FALSE;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

							PostMessageUpdateAlignerTargetSizeInfo();

							return TRUE;
						}
					}
					// 目标位置3+位置4
					else if (m_nCheckTargetDistanceIndex == 20)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 0.000, 0.000, 0.000, 0.000, vMark[2].GetX(), vMark[2].GetY(), vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), 0.000, checkInfo.m_dTargetDis1, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), 0.000, dDis[1], 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T(" 结果01:%d 结果23:%d 结果02:%d 结果13:%d,"), TRUE, bTargetDis1, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis1)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;

							PostMessageUpdateAlignerTargetSizeInfo();

							return FALSE;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;

							PostMessageUpdateAlignerTargetSizeInfo();
							return TRUE;
						}
					}
					else if (m_nCheckObjectDistanceIndex == 23 || m_nCheckObjectDistanceIndex == 24)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;

						PostMessageUpdateAlignerTargetSizeInfo();
						return TRUE;
					}
					else
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 
								vMark[0].GetX(), vMark[0].GetY(), 
								vMark[1].GetX(), vMark[1].GetY(), 
								vMark[2].GetX(), vMark[2].GetY(), 
								vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), 
								checkInfo.m_dTargetDis0, 
								checkInfo.m_dTargetDis1, 
								checkInfo.m_dTargetDis2, 
								checkInfo.m_dTargetDis3);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T(" 结果01 R:%d 结果23 R:%d 结果02: R%d 结果13 R:%d,"), bTargetDis0, bTargetDis1, bTargetDis2, bTargetDis3);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis0)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis1)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis2)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis3)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
						}

						if (bTargetDis0 && bTargetDis1 && bTargetDis2 && bTargetDis3)
						{
							PostMessageUpdateAlignerTargetSizeInfo();

							return TRUE;
						}
						else
						{
							PostMessageUpdateAlignerTargetSizeInfo();

							return FALSE;
						}

					}

				}
			}
			break;

		default:
			{

			}
			break;

		}
	}
	else
	{
		// 目标、对象分离
		switch(nPosNum)
		{

		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis = 0;
				sc2Vector vMark[2];
				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
				dDis = vMark[0].Distance(vMark[1]);

				if (checkInfo.m_nCheckMode == 0)	// 输入模式
				{
					checkInfo.m_dTargetDis0 = dDis;
					checkInfo.m_dTargetDis1 = 0;
					checkInfo.m_dTargetDis2 = 0;
					checkInfo.m_dTargetDis3 = 0;
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();
					
					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;		// _T("TargetLen34");
					PostMessageUpdateAlignerTargetSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标2 X:%.3f Y:%.3f,目标3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入23 D:%.3f,"), checkInfo.m_dTargetDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查23 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果23 R:%d,"), TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;		// _T("TargetLen34");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标2 X:%.3f Y:%.3f,目标3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入23 D:%.3f,"), checkInfo.m_dTargetDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查23 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果23 D:%.3f,"), bTargetDis0);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis0)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

						PostMessageUpdateAlignerTargetSizeInfo();
						return FALSE;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

						PostMessageUpdateAlignerTargetSizeInfo();
						return TRUE;
					}
				}
			}
			break;

		case 6:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis[3];
				sc2Vector vMark[3];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);

				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[0].Distance(vMark[2]);
				dDis[2] = vMark[1].Distance(vMark[2]);



				if (checkInfo.m_nCheckMode == 0)	
				{				
					// 输入模式
					checkInfo.m_dTargetDis0 = dDis[0];
					checkInfo.m_dTargetDis1 = dDis[1];
					checkInfo.m_dTargetDis2 = dDis[2];
					checkInfo.m_dTargetDis3 = 0;
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();

					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen45");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen46");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen56");

					PostMessageUpdateAlignerTargetSizeInfo();
					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标3 X:%.3f Y:%.3f,目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入34 D:%.3f 输入35 D:%.3f 输入45 D:%.3f,"), checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查34 D:%.3f 检查35 D:%.3f 检查45 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果34 R:%d 结果35 R:%d 结果45 R:%d,"), TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen45");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen46");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen56");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis1 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis2 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) <= checkInfo.m_dTargetDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标3 X:%.3f Y:%.3f,目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入34 D:%.3f 输入35 D:%.3f 输入45 D:%.3f,"), checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查34 D:%.3f 检查35 D:%.3f 检查45 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果34 R:%d 结果35 R:%d 结果45 R:%d,"), bTargetDis0, bTargetDis1, bTargetDis2);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis0)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}


//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis1)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}


//					if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dTargetDisThreshold)
					if(!bTargetDis2)
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if (bTargetDis0 && bTargetDis1 && bTargetDis2)
					{
						PostMessageUpdateAlignerTargetSizeInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerTargetSizeInfo();
						return FALSE;
					}

				}
			}
			break;

		case 8:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				double dDis[4];
				sc2Vector vMark[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);

				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[2].Distance(vMark[3]);
				dDis[2] = vMark[0].Distance(vMark[2]);
				dDis[3] = vMark[1].Distance(vMark[3]);

				if (pAlignerParam->GetAligner4Vs4Type()==eAlignerPos1234_56_4V2)
				{
					dDis[1] = dDis[0];
					dDis[2] = 0;
					dDis[3] = 0;
				}

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dTargetDis0 = dDis[0];
					checkInfo.m_dTargetDis1 = dDis[1];
					checkInfo.m_dTargetDis2 = dDis[2];
					checkInfo.m_dTargetDis3 = dDis[3];
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdSizeInfo();

					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSUnCheck;
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen56");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen78");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen57");
					m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];		// _T("TargetLen68");
					PostMessageUpdateAlignerTargetSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入45 D:%.3f 输入67 D:%.3f 输入46 D:%.3f 输入57 D:%.3f,"), 
							checkInfo.m_dTargetDis0, 
							checkInfo.m_dTargetDis1, 
							checkInfo.m_dTargetDis2, 
							checkInfo.m_dTargetDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查45 D:%.3f 检查67 D:%.3f 检查46 D:%.3f 检查57 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果45 R:%d 结果67 R:%d 结果46 R:%d 结果57 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];		// _T("TargetLen56");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];		// _T("TargetLen78");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];		// _T("TargetLen57");
					m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];		// _T("TargetLen68");

					BOOL bTargetDis0 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis1 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis2 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) <= checkInfo.m_dTargetDisThreshold );
					BOOL bTargetDis3 = ( fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) <= checkInfo.m_dTargetDisThreshold );

					// 目标位置4+位置5
					if (m_nCheckTargetDistanceIndex == 19)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 0.000, 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入45 D:%.3f 输入67 D:%.3f 输入46 D:%.3f 输入57 D:%.3f,"), checkInfo.m_dTargetDis0, 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查45 D:%.3f 检查67 D:%.3f 检查46 D:%.3f 检查57 D:%.3f,"), dDis[0], 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果45 D:%d 结果67 D:%d 结果46 D:%d 结果57 D:%d,"), bTargetDis0, TRUE, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}


//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis0)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

							PostMessageUpdateAlignerTargetSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

							PostMessageUpdateAlignerTargetSizeInfo();
							return TRUE;
						}
					}
					// 目标位置6+位置7
					else if (m_nCheckTargetDistanceIndex == 20)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), 0.000, 0.000, 0.000, 0.000, vMark[2].GetX(), vMark[2].GetY(), vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入45 D:%.3f 输入67 D:%.3f 输入46 D:%.3f 输入57 D:%.3f,"), 0.000, checkInfo.m_dTargetDis1, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查45 D:%.3f 检查67 D:%.3f 检查46 D:%.3f 检查57 D:%.3f,"), 0.000, dDis[1], 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果45 R:%d 结果67 R:%d 结果46 R:%d 结果57 R:%d,"), TRUE, bTargetDis1, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}


//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis1)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;

							PostMessageUpdateAlignerTargetSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;

							PostMessageUpdateAlignerTargetSizeInfo();
							return TRUE;
						}
					}
					else
					{
						// 日志记录
						{
							strTemp.Format(_T(" 目标产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), 
								vMark[0].GetX(), vMark[0].GetY(), 
								vMark[1].GetX(), vMark[1].GetY(), 
								vMark[2].GetX(), vMark[2].GetY(), 
								vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dTargetDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入45 D:%.3f 输入67 D:%.3f 输入46 D:%.3f 输入57 D:%.3f,"), 
								checkInfo.m_dTargetDis0, 
								checkInfo.m_dTargetDis1, 
								checkInfo.m_dTargetDis2, 
								checkInfo.m_dTargetDis3);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查45 D:%.3f 检查67 D:%.3f 检查46 D:%.3f 检查57 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果45 R:%d 结果67 R:%d 结果46 R:%d 结果57 R:%d,"), bTargetDis0, bTargetDis1, bTargetDis2, bTargetDis3);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}


//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis0)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						}


//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis1)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						}


//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis2)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dTargetDisThreshold)
						if(!bTargetDis3)
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
						}
						else
						{
							m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
						}


						if (bTargetDis0 && bTargetDis1 && bTargetDis2 && bTargetDis3)
						{
							PostMessageUpdateAlignerTargetSizeInfo();
							return TRUE;
						}
						else
						{
							PostMessageUpdateAlignerTargetSizeInfo();
							return FALSE;
						}
					}
				}
			}
			break;

		default:
			{

			}
			break;

		}
	}

	return TRUE;
}

BOOL vcXYDVisionAlign::CheckObjectDistance()
{
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		return FALSE;
	}

	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		return FALSE;
	}

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	if (pAlignerParam == NULL)
	{
		return FALSE;
	}

// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = GetPosNum();
	CString strInfo = _T("");
	CString strTemp = _T("");


	for(int i = 0; i < OBJECTSIZEMAXNUM; i++)
	{
		m_AlignerObjectSizeInfo.m_vdObjectSize[i] = 0;
		m_AlignerObjectSizeInfo.m_vnCheckStatus[i] = eSCSUnCheck;
	}

	int nExProductIdx = 0;
	if (optionInfo.m_bObjectSearchExProduct)
	{
		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
	}
	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);


	if (m_pSystempOptionConfig->m_bCheckModeOnly)
	{
		checkInfo.m_nCheckMode = 1;
		checkInfo.m_bCheckTargetAndObjectDis = TRUE;
		checkInfo.m_bCheckObjectDis = TRUE;
	}

	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
	{
		return TRUE;
	}



// 	if (sysPlatformInfo.m_bTarCamWithDiffObjCam && sysPlatformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 		if (nAlignnExProductIndex == 0)
// 		{
// 			double dDis;
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 			dDis = vMark[0].Distance(vMark[1]);
// 
// 			if (checkInfo.m_nCheckMode == 0)	
// 			{
// 				// 输入模式
// 				checkInfo.m_dObjectDis0 = dDis;
// 
// 				EnterCriticalSection(&m_csAlignCheckInfo);
// 				pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 				SaveProductDataInfo(nCurProductIndex);
// 
// 				PostMessageUpdateAlignerObjectStdSizeInfo();
// 				
// 				m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
// 				PostMessageUpdateAlignerObjectSizeInfo();
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dObjectDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果01 R:%d,"), TRUE);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				return TRUE;
// 			}
// 			else
// 			{
// 				// 检查模式
// 				m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
// 
// 				BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dObjectDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果01:%d,"), bObjectDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				// 对象位置0+位置1
// //				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
// 				if(!bObjectDis0)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 
// 					PostMessageUpdateAlignerObjectSizeInfo();
// 					return FALSE;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 
// 					PostMessageUpdateAlignerObjectSizeInfo();
// 					return TRUE;
// 				}
// 			}
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis;
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 			dDis = vMark[0].Distance(vMark[1]);
// 
// 			if (checkInfo.m_nCheckMode == 0)	
// 			{
// 				// 输入模式
// 				checkInfo.m_dObjectDis1 = dDis;
// 
// 				EnterCriticalSection(&m_csAlignCheckInfo);
// 				pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 				LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 				SaveProductDataInfo(nCurProductIndex);
// 
// 				PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 				m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
// 				PostMessageUpdateAlignerObjectSizeInfo();
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式23 D:%.3f,"), checkInfo.m_dObjectDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式23 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果23 R:%d,"), TRUE);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// 				return TRUE;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis;
// 
// 				BOOL bObjectDis1 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold );
// 
// 				// 日志记录
// 				{
// 					strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("输入模式23 D:%.3f,"), checkInfo.m_dObjectDis0);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("检查模式23 D:%.3f,"), dDis);
// 					strInfo = strInfo + strTemp;
// 
// 					strTemp.Format(_T("结果23 R:%d,"), bObjectDis1);
// 					strInfo = strInfo + strTemp;
// 
// 					VisionAlignLogRecord(m_strProductSizeData, strInfo);
// 				}
// 
// //				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
// 				if(!bObjectDis1)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 
// 					PostMessageUpdateAlignerObjectSizeInfo();
// 					return FALSE;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 
// 					PostMessageUpdateAlignerObjectSizeInfo();
// 					return TRUE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 	}


	if (!sysPlatformInfo.m_bTargetObjectCamSeparate)
	{
		// 目标、对象不分离

		switch(nPosNum)
		{
		case 2:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis = 0;
				sc2Vector vMark[2];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
				dDis = vMark[0].Distance(vMark[1]);

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis;
					checkInfo.m_dObjectDis1 = 0;
					checkInfo.m_dObjectDis2 = 0;
					checkInfo.m_dObjectDis3 = 0;
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();

					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
					PostMessageUpdateAlignerObjectSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dObjectDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;

					BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f,"), checkInfo.m_dObjectDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), bObjectDis0);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis0)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

						PostMessageUpdateAlignerObjectSizeInfo();
						return FALSE;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

						PostMessageUpdateAlignerObjectSizeInfo();
						return TRUE;
					}			
				}
			}
			break;

		case 3:
			{
				// 计算对象距离
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis[3];
				sc2Vector vMark[3];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);


				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[0].Distance(vMark[2]);
				dDis[2] = vMark[1].Distance(vMark[2]);

				if (checkInfo.m_nCheckMode == 0)
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis[0];
					checkInfo.m_dObjectDis1 = dDis[1];
					checkInfo.m_dObjectDis2 = dDis[2];
					checkInfo.m_dObjectDis3 = 0;
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");		
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");
					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					PostMessageUpdateAlignerObjectSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式02 D:%.3f 输入模式12 D:%.3f,"), checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式02 D:%.3f 检查模式12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果02 R:%d 结果12 R:%d,"), TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");		
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");


					BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis1 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis2 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) <= checkInfo.m_dObjectDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式02 D:%.3f 输入模式12 D:%.3f,"), checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式02 D:%.3f 检查模式12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果02 R:%d 结果12 R:%d,"), bObjectDis0, bObjectDis1, bObjectDis2);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis0)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}


//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis1)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis2)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}


					if (bObjectDis0 && bObjectDis1 && bObjectDis2)
					{
						PostMessageUpdateAlignerObjectSizeInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerObjectSizeInfo();
						return FALSE;
					}
				}
			}
			break;

		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis[4];
				sc2Vector vMark[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);

				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[2].Distance(vMark[3]);
				dDis[2] = vMark[0].Distance(vMark[2]);
				dDis[3] = vMark[1].Distance(vMark[3]);


				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis[0];
					checkInfo.m_dObjectDis1 = dDis[1];
					checkInfo.m_dObjectDis2 = dDis[2];
					checkInfo.m_dObjectDis3 = dDis[3];
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();

					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];		// _T("ObjectLen24");
					PostMessageUpdateAlignerObjectSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


					return TRUE;
				}
				else
				{
					// 检查模式

					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];		// _T("ObjectLen24");

					BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis1 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis2 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis3 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) <= checkInfo.m_dObjectDisThreshold );

					// 对象位置0+位置1
					if (m_nCheckObjectDistanceIndex == 19)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式:%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%f Y:%.3f,平台3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 0.000, 0.000, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), checkInfo.m_dObjectDis0, 0.000, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), bObjectDis0, TRUE, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
						if (!bObjectDis0)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
					}
					// 对象位置2+位置3
					else if (m_nCheckObjectDistanceIndex == 20)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 0.000, 0.000, 0.000, 0.000, vMark[2].GetX(), vMark[2].GetY(), vMark[3].GetX(), vMark[3].GetY());							
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01:%.3f 输入模式23:%.3f 输入模式02:%.3f 输入模式13:%.3f"), 0.000, checkInfo.m_dObjectDis1, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T(" 检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), 0.000, dDis[1], 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), TRUE, bObjectDis1, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}


//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis1)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;

							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;

							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
					}
					else if (m_nCheckObjectDistanceIndex == 23 || m_nCheckObjectDistanceIndex == 24)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;

						PostMessageUpdateAlignerObjectSizeInfo();
						return TRUE;
					}
					else
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("平台0 X:%.3f Y:%.3f,平台1 X:%.3f Y:%.3f,平台2 X:%.3f Y:%.3f,平台3 X:%.3f Y:%.3f,"), 
								vMark[0].GetX(), vMark[0].GetY(), 
								vMark[1].GetX(), vMark[1].GetY(), 
								vMark[2].GetX(), vMark[2].GetY(), 
								vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入模式01 D:%.3f 输入模式23 D:%.3f 输入模式02 D:%.3f 输入模式13 D:%.3f,"), 
								checkInfo.m_dObjectDis0, 
								checkInfo.m_dObjectDis1, 
								checkInfo.m_dObjectDis2, 
								checkInfo.m_dObjectDis3);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查模式01 D:%.3f 检查模式23 D:%.3f 检查模式02 D:%.3f 检查模式13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), bObjectDis0, bObjectDis1, bObjectDis2, bObjectDis3);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}


//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis0)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis1)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						}


//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis2)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						}


//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis3)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
						}

						if (bObjectDis0 && bObjectDis1 && bObjectDis2 && bObjectDis3)
						{
							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
						else
						{
							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
					}

				}
			}
			break;

		default:
			{

			}
			break;

		}
	}
	else
	{
		// 目标、对象分离

		switch(nPosNum)
		{
		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis = 0;
				sc2Vector vMark[2];
				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
				dDis = vMark[0].Distance(vMark[1]);


				if (checkInfo.m_nCheckMode == 0)	// 输入模式
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis;
					checkInfo.m_dObjectDis1 = 0;
					checkInfo.m_dObjectDis2 = 0;
					checkInfo.m_dObjectDis3 = 0;
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();

					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
					PostMessageUpdateAlignerObjectSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01 D:%.3f,"), checkInfo.m_dObjectDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;

					BOOL bObjectDis0 =  ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01 D:%.3f,"), checkInfo.m_dObjectDis0);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01 D:%.3f,"), dDis);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d,"), bObjectDis0);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis0)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

						PostMessageUpdateAlignerObjectSizeInfo();
						return FALSE;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

						PostMessageUpdateAlignerObjectSizeInfo();
						return TRUE;
					}
				}
			}
			break;

		case 6:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis[3];
				sc2Vector vMark[3];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[0].Distance(vMark[2]);
				dDis[2] = vMark[1].Distance(vMark[2]);


				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis[0];
					checkInfo.m_dObjectDis1 = dDis[1];
					checkInfo.m_dObjectDis2 = dDis[2];
					checkInfo.m_dObjectDis3 = 0;
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();
					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");
					PostMessageUpdateAlignerObjectSizeInfo();
					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01 D:%.3f 输入02 D:%.3f 输入12: D%.3f,"), checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01 D:%.3f 检查02 D:%.3f 检查12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果01 R:%d 结果02 R:%d 结果12 R:%d,"), TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");

					BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis1 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis2 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) <= checkInfo.m_dObjectDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01 D:%.3f 输入02 D:%.3f 输入12 D:%.3f,"), checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01 D:%.3f 检查02 D:%.3f 检查12 D:%.3f,"), dDis[0], dDis[1], dDis[2]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果02 R:%d 结果12 R:%d,"), bObjectDis0, bObjectDis1, bObjectDis2);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis0)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}

//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis1)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

//					if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dObjectDisThreshold)
					if(!bObjectDis2)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if (bObjectDis0 &&bObjectDis1 && bObjectDis2)
					{
						PostMessageUpdateAlignerObjectSizeInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerObjectSizeInfo();
						return FALSE;
					}


				}
			}
			break;

		case 8:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				double dDis[4];
				sc2Vector vMark[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);


				dDis[0] = vMark[0].Distance(vMark[1]);
				dDis[1] = vMark[2].Distance(vMark[3]);
				dDis[2] = vMark[0].Distance(vMark[2]);
				dDis[3] = vMark[1].Distance(vMark[3]);

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dDis[0];
					checkInfo.m_dObjectDis1 = dDis[1];
					checkInfo.m_dObjectDis2 = dDis[2];
					checkInfo.m_dObjectDis3 = dDis[3];
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdSizeInfo();

					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSUnCheck;
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];		// _T("ObjectLen24");
					PostMessageUpdateAlignerObjectSizeInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01 D:%.3f 输入23 D:%.3f 输入02 D:%.3f 输入13 D:%.3f,"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01 D:%.3f 检查23 D:%.3f 检查02 D:%.3f 检查13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式			
					m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];		// _T("ObjectLen12");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];		// _T("ObjectLen34");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];		// _T("ObjectLen13");			
					m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];		// _T("ObjectLen24");

					BOOL bObjectDis0 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis1 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis2 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectDis3 = ( fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) <= checkInfo.m_dObjectDisThreshold );


					// 对象位置0+位置1
					if (m_nCheckObjectDistanceIndex == 19)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 0.000, 0.000, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T(" 最大距离差值:%.3f"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入01 D:%.3f 输入23 D:%.3f 输入02 D:%.3f 输入13 D:%.3f,"), checkInfo.m_dObjectDis0, 0.000, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查01 D:%.3f 检查23 D:%.3f 检查02 D:%.3f 检查13 D:%.3f,"), dDis[0], 0.000, 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), bObjectDis0, TRUE, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis0)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;

							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;

							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
					}
					// 对象位置2+位置3
					else if (m_nCheckObjectDistanceIndex == 20)
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), 0.000, 0.000, 0.000, 0.000, vMark[2].GetX(), vMark[2].GetY(), vMark[3].GetX(), vMark[3].GetY());							
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入01 D:%.3f 输入23 D:%.3f 输入02 D:%.3f 输入13 D:%.3f,"), 0.000, checkInfo.m_dObjectDis1, 0.000, 0.000);								
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查01 D:%.3f 检查23 D:%.3f 检查02 D:%.3f 检查13 D:%.3f,"), 0.000, dDis[1], 0.000, 0.000);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), TRUE, bObjectDis1, TRUE, TRUE);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis1)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;

							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;

							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
					}
					else
					{
						// 日志记录
						{
							strTemp.Format(_T(" 对象产品尺寸信息 模式%d,"), checkInfo.m_nCheckMode);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), 
								vMark[0].GetX(), vMark[0].GetY(), 
								vMark[1].GetX(), vMark[1].GetY(), 
								vMark[2].GetX(), vMark[2].GetY(), 
								vMark[3].GetX(), vMark[3].GetY());
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("最大距离差值 D:%.3f,"), checkInfo.m_dObjectDisThreshold);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("输入01 D:%.3f 输入23 D:%.3f 输入02 D:%.3f 输入13 D:%.3f,"), 
								checkInfo.m_dObjectDis0, 
								checkInfo.m_dObjectDis1, 
								checkInfo.m_dObjectDis2, 
								checkInfo.m_dObjectDis3);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("检查01 D:%.3f 检查23: D%.3f 检查02: D%.3f 检查13 D:%.3f,"), dDis[0], dDis[1], dDis[2], dDis[3]);
							strInfo = strInfo + strTemp;

							strTemp.Format(_T("结果01 R:%d 结果23 R:%d 结果02 R:%d 结果13 R:%d,"), bObjectDis0, bObjectDis1, bObjectDis2, bObjectDis3);
							strInfo = strInfo + strTemp;

							VisionAlignLogRecord(m_strProductSizeData, strInfo);
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis0)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis1)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis2)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						}

//						if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dObjectDisThreshold)
						if(!bObjectDis3)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
						}

						if (bObjectDis0 && bObjectDis1 && bObjectDis2 && bObjectDis3)
						{
							PostMessageUpdateAlignerObjectSizeInfo();
							return TRUE;
						}
						else
						{
							PostMessageUpdateAlignerObjectSizeInfo();
							return FALSE;
						}
					}
				}
			}
			break;

		default:
			{

			}
			break;


		}
	}

	return TRUE;
}

BOOL vcXYDVisionAlign::CheckTargetAngle()
{
	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		return FALSE;
	}

	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		return FALSE;
	}

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	if (pAlignerParam == NULL)
	{
		return FALSE;
	}

// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	CString strInfo = _T("");
	CString strTemp = _T("");

	int nPosNum = sysPlatformInfo.m_nPositionNum;
	int nCamNum = sysPlatformInfo.m_nCamNum;


	for(int i = 0; i < TARGETSIZEMAXNUM; i++)
	{
		m_AlignerTargetAngleInfo.m_vdTargetAngle[i] = 0;
		m_AlignerTargetAngleInfo.m_vnCheckStatus[i] = eSCSUnCheck;
	}

	int nExProductIdx = 0;
	if (optionInfo.m_bTargetSearchExProduct)
	{
		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
	}
	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);

	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetAngle)
	{
		return TRUE;
	}

	if (!sysPlatformInfo.m_bTargetObjectCamSeparate)
	{
		// 目标、对象不分离
		switch(nPosNum)
		{

		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

 				sc2Vector vMark[4];
				scRadian raAngle[4];
				double dAngle[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);

				(vMark[1] - vMark[0]).Angle(raAngle[0]);
				(vMark[3] - vMark[1]).Angle(raAngle[1]);
				(vMark[3] - vMark[2]).Angle(raAngle[2]);
				(vMark[2] - vMark[0]).Angle(raAngle[3]);

				dAngle[0] = scDegree(raAngle[0]-raAngle[1]).SignedNorm().ToDouble();
				dAngle[1] = scDegree(raAngle[1]-raAngle[2]).SignedNorm().ToDouble();
				dAngle[2] = scDegree(raAngle[2]-raAngle[3]).SignedNorm().ToDouble();
				dAngle[3] = scDegree(raAngle[3]-raAngle[0]).SignedNorm().ToDouble();


				if (checkInfo.m_nCheckMode == 0)	
				{	
					// 输入模式
					checkInfo.m_dTargetAngle0 = dAngle[0];
					checkInfo.m_dTargetAngle1 = dAngle[1];
					checkInfo.m_dTargetAngle2 = dAngle[2];
					checkInfo.m_dTargetAngle3 = dAngle[3];
					checkInfo.m_dTargetAngle4 = 0;
					checkInfo.m_dTargetAngle5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdAngleInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品角度信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标0 X:%.3f Y:%.3f,目标1 X:%.3f Y:%.3f,目标2 X:%.3f Y:%.3f,目标3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dTargetAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 输入01-13 D:%.3f 输入13_32 D:%.3f 输入32_20 D:%.3f 输入20_01 D:%.3f,"), 
							checkInfo.m_dTargetAngle0, 
							checkInfo.m_dTargetAngle1, 
							checkInfo.m_dTargetAngle2, 
							checkInfo.m_dTargetAngle3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 检查01-13 D:%.3f 检查13_32 D:%.3f 检查32_20 D:%.3f 检查20_01 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01-13 R:%d 结果13_32 R:%d 结果32_20 R:%d 结果20_01 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetAngleInfo.m_vdTargetAngle[0] = dAngle[0];				// _T("TargetLen12");
					m_AlignerTargetAngleInfo.m_vdTargetAngle[1] = dAngle[1];				// _T("TargetLen34");
					m_AlignerTargetAngleInfo.m_vdTargetAngle[2] = dAngle[2];				// _T("TargetLen13");
					m_AlignerTargetAngleInfo.m_vdTargetAngle[3] = dAngle[3];				// _T("TargetLen24");

					BOOL bTargetAngle0 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[0] - checkInfo.m_dTargetAngle0) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle1 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[1] - checkInfo.m_dTargetAngle1) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle2 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[2] - checkInfo.m_dTargetAngle2) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle3 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[3] - checkInfo.m_dTargetAngle3) <= checkInfo.m_dTargetAngleThreshold );


					// 日志记录
					{
						strTemp.Format(_T(" 目标产品角度信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标0 X:%.3f Y:%.3f,目标1 X:%.3f Y:%.3f,目标2 X:%.3f Y:%.3f,目标3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dTargetAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01-13 D:%.3f 输入13_32 D:%.3f 输入32_20 D:%.3f 输入20_01 D:%.3f,"), 
							checkInfo.m_dTargetAngle0, 
							checkInfo.m_dTargetAngle1, 
							checkInfo.m_dTargetAngle2, 
							checkInfo.m_dTargetAngle3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01-13 D:%.3f 检查13_32 D:%.3f 检查32_20 D:%.3f 检查20_01 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01-13 R:%d 结果13_32 R:%d 结果32_20 R:%d 结果20_01 R:%d,"), bTargetAngle0, bTargetAngle1, bTargetAngle2, bTargetAngle3);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					if(!bTargetAngle0)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}

					if(!bTargetAngle1)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

					if(!bTargetAngle2)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if(!bTargetAngle3)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[3] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[3] = eSCSCheckOK;
					}

					if (bTargetAngle0 && bTargetAngle1 && bTargetAngle2 && bTargetAngle3)
					{
						PostMessageUpdateAlignerTargetAngleInfo();

						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerTargetAngleInfo();

						return FALSE;
					}

				}
			}
			break;

		default:
			{

			}
			break;

		}
	}
	else
	{
		// 目标、对象分离
		switch(nPosNum)
		{

		case 8:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();

				sc2Vector vMark[4];
				scRadian raAngle[4];
				double dAngle[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);

				(vMark[1] - vMark[0]).Angle(raAngle[0]);
				(vMark[3] - vMark[1]).Angle(raAngle[1]);
				(vMark[3] - vMark[2]).Angle(raAngle[2]);
				(vMark[2] - vMark[0]).Angle(raAngle[3]);

				dAngle[0] = scDegree(raAngle[0]-raAngle[1]).SignedNorm().ToDouble();
				dAngle[1] = scDegree(raAngle[1]-raAngle[2]).SignedNorm().ToDouble();
				dAngle[2] = scDegree(raAngle[2]-raAngle[3]).SignedNorm().ToDouble();
				dAngle[3] = scDegree(raAngle[3]-raAngle[0]).SignedNorm().ToDouble();

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dTargetDis0 = dAngle[0];
					checkInfo.m_dTargetDis1 = dAngle[1];
					checkInfo.m_dTargetDis2 = dAngle[2];
					checkInfo.m_dTargetDis3 = dAngle[3];
					checkInfo.m_dTargetDis4 = 0;
					checkInfo.m_dTargetDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerTargetStdAngleInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品角度信息 模式%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dTargetAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入45-57 D:%.3f 输入57-76 D:%.3f 输入76-64 D:%.3f 输入64-45 D:%.3f,"), 
							checkInfo.m_dTargetDis0, 
							checkInfo.m_dTargetDis1, 
							checkInfo.m_dTargetDis2, 
							checkInfo.m_dTargetDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查45-57 D:%.3f 检查57-76 D:%.3f 检查76-64 D:%.3f 检查64-45 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果45-57 R:%d 结果57-76 R:%d 结果76-64 R:%d 结果 R64-45:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式
					m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dAngle[0];		// _T("TargetLen56");
					m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dAngle[1];		// _T("TargetLen78");
					m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dAngle[2];		// _T("TargetLen57");
					m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dAngle[3];		// _T("TargetLen68");

					BOOL bTargetAngle0 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[0] - checkInfo.m_dTargetAngle0) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle1 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[1] - checkInfo.m_dTargetAngle1) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle2 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[2] - checkInfo.m_dTargetAngle2) <= checkInfo.m_dTargetAngleThreshold );
					BOOL bTargetAngle3 = ( fabs(m_AlignerTargetAngleInfo.m_vdTargetAngle[3] - checkInfo.m_dTargetAngle3) <= checkInfo.m_dTargetAngleThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 目标产品角度信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("目标4 X:%.3f Y:%.3f,目标5 X:%.3f Y:%.3f,目标6 X:%.3f Y:%.3f,目标7 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dTargetAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入45-57 D:%.3f 输入57-76 D:%.3f 输入76-64 D:%.3f 输入64-45 D:%.3f,"), 
							checkInfo.m_dTargetDis0, 
							checkInfo.m_dTargetDis1, 
							checkInfo.m_dTargetDis2, 
							checkInfo.m_dTargetDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查45-57 D:%.3f 检查57-76 D:%.3f 检查76-64 D:%.3f 检查64-45 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果45-57 R:%d 结果57-76 R:%d 结果76-64 R:%d 结果64-45 R:%d,"), bTargetAngle0, bTargetAngle1, bTargetAngle2, bTargetAngle3);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					if(!bTargetAngle0)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}

					if(!bTargetAngle1)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

					if(!bTargetAngle2)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if(!bTargetAngle3)
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[3] = eSCSCheckNG;
					}
					else
					{
						m_AlignerTargetAngleInfo.m_vnCheckStatus[3] = eSCSCheckOK;
					}


					if (bTargetAngle0 && bTargetAngle1 && bTargetAngle2 && bTargetAngle3)
					{
						PostMessageUpdateAlignerTargetAngleInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerTargetAngleInfo();
						return FALSE;
					}
				}
			}
			break;

		default:
			{

			}
			break;

		}
	}


	return TRUE;
}

BOOL vcXYDVisionAlign::CheckObjectAngle()
{
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		return FALSE;
	}

	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		return FALSE;
	}

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	if (pAlignerParam == NULL)
	{
		return FALSE;
	}

// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	CString strInfo = _T("");
	CString strTemp = _T("");

	int nPosNum = sysPlatformInfo.m_nPositionNum;
	int nCamNum = sysPlatformInfo.m_nCamNum;


// 	if (!sysPlatformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		// 目标、对象不分离
// 
// 		if ( (4 == nPosNum) && (4 == nCamNum) )
// 		{
// 
// 		}
// 		else
// 		{
// 			return TRUE;
// 		}
// 	}
// 	else
// 	{
// 		// 目标、对象分离
// 
// 		if ( (8 == nPosNum) || (8 == nCamNum) )
// 		{
// 			return TRUE;
// 		}
// 	}

	for(int i = 0; i < OBJECTSIZEMAXNUM; i++)
	{
		m_AlignerObjectAngleInfo.m_vdObjectAngle[i] = 0;
		m_AlignerObjectAngleInfo.m_vnCheckStatus[i] = eSCSUnCheck;
	}

	int nExProductIdx = 0;
	if (optionInfo.m_bObjectSearchExProduct)
	{
		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
	}
	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);


	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectAngle)
	{
		return TRUE;
	}



	if (!sysPlatformInfo.m_bTargetObjectCamSeparate)
	{
		// 目标、对象不分离

		switch(nPosNum)
		{
		case 4:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				sc2Vector vMark[4];
				scRadian raAngle[4];
				double dAngle[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);


// 				double dAngle01_13 = scDegree(raAngle[0]-raAngle[1]).SignedNorm().ToDouble();
// 				double dAngle13_32 = scDegree(raAngle[1]-raAngle[2]).SignedNorm().ToDouble();
// 				double dAngle32_20 = scDegree(raAngle[2]-raAngle[3]).SignedNorm().ToDouble();
// 				double dAngle20_01 = scDegree(raAngle[3]-raAngle[0]).SignedNorm().ToDouble();
				dAngle[0] = scDegree(raAngle[0]-raAngle[1]).SignedNorm().ToDouble();
				dAngle[1] = scDegree(raAngle[1]-raAngle[2]).SignedNorm().ToDouble();
				dAngle[2] = scDegree(raAngle[2]-raAngle[3]).SignedNorm().ToDouble();
				dAngle[3] = scDegree(raAngle[3]-raAngle[0]).SignedNorm().ToDouble();

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectAngle0 = dAngle[0];
					checkInfo.m_dObjectAngle1 = dAngle[1];
					checkInfo.m_dObjectAngle2 = dAngle[2];
					checkInfo.m_dObjectAngle3 = dAngle[3];
					checkInfo.m_dObjectAngle4 = 0;
					checkInfo.m_dObjectAngle5 = 0;


					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdAngleInfo();

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品角度信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dObjectAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01-13 D:%.3f 输入13-32 D:%.3f 输入32-20 D:%.3f 输入20-01 D:%.3f,"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01-13 D:%.3f 检查13-32 D:%.3f 检查32-20 D:%.3f 检查20-01 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01-13 R:%d 结果13-32 R:%d 结果32-20 R:%d 结果20-01 R:%d,"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


					return TRUE;
				}
				else
				{
					// 检查模式

					m_AlignerObjectAngleInfo.m_vdObjectAngle[0] = dAngle[0];		// _T("ObjectLen12");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[1] = dAngle[1];		// _T("ObjectLen34");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[2] = dAngle[2];		// _T("ObjectLen13");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[3] = dAngle[3];		// _T("ObjectLen24");

					BOOL bObjectAngle0 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[0] - checkInfo.m_dObjectAngle0) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectAngle1 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[1] - checkInfo.m_dObjectAngle1) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectAngle2 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[2] - checkInfo.m_dObjectAngle2) <= checkInfo.m_dObjectDisThreshold );
					BOOL bObjectAngle3 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[3] - checkInfo.m_dObjectAngle3) <= checkInfo.m_dObjectDisThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品角度信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("对象0 X:%.3f Y:%.3f,对象1 X:%.3f Y:%.3f,对象2 X:%.3f Y:%.3f,对象3 X:%.3f Y:%.3f,"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("最大角度差值 D:%.3f,"), checkInfo.m_dObjectAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("输入01-13 D:%.3f 输入13-32 D:%.3f 输入32-20 D:%.3f 输入20-01 D:%.3f,"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("检查01-13 D:%.3f 检查13-32 D:%.3f 检查32-20 D:%.3f 检查20-01 D:%.3f,"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T("结果01-13 R:%d 结果13-32 R:%d 结果32-20 R:%d 结果20-01 R:%d,"), bObjectAngle0, bObjectAngle1, bObjectAngle2, bObjectAngle3);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


					if(!bObjectAngle0)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
					}

					if(!bObjectAngle1)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
					}

					if(!bObjectAngle2)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
					}

					if(!bObjectAngle3)
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
					}
					else
					{
						m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
					}

					if (bObjectAngle0 && bObjectAngle1 && bObjectAngle2 && bObjectAngle3)
					{
						PostMessageUpdateAlignerObjectAngleInfo();
						return TRUE;
					}
					else
					{
						PostMessageUpdateAlignerObjectAngleInfo();
						return FALSE;
					}
				}
			}
			break;

		default:
			{

			}
			break;

		}
	}
	else
	{
		// 目标、对象分离

		switch(nPosNum)
		{
		case 8:
			{
				CMarkPlatformPos mpTargetMarkPos;
				mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();

				sc2Vector vMark[4];
				scRadian raAngle[4];
				double dAngle[4];

				vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
				vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);

				vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
				vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);

				vMark[2].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
				vMark[2].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);

				vMark[3].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
				vMark[3].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);

				dAngle[0] = scDegree(raAngle[0]-raAngle[1]).SignedNorm().ToDouble();
				dAngle[1] = scDegree(raAngle[1]-raAngle[2]).SignedNorm().ToDouble();
				dAngle[2] = scDegree(raAngle[2]-raAngle[3]).SignedNorm().ToDouble();
				dAngle[3] = scDegree(raAngle[3]-raAngle[0]).SignedNorm().ToDouble();

				if (checkInfo.m_nCheckMode == 0)	
				{
					// 输入模式
					checkInfo.m_dObjectDis0 = dAngle[0];
					checkInfo.m_dObjectDis1 = dAngle[1];
					checkInfo.m_dObjectDis2 = dAngle[2];
					checkInfo.m_dObjectDis3 = dAngle[3];
					checkInfo.m_dObjectDis4 = 0;
					checkInfo.m_dObjectDis5 = 0;

					EnterCriticalSection(&m_csAlignCheckInfo);
					pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
					LeaveCriticalSection(&m_csAlignCheckInfo);

					SaveProductDataInfo(nCurProductIndex);

					PostMessageUpdateAlignerObjectStdAngleInfo();


					// 日志记录
					{
						strTemp.Format(_T(" 对象产品角度信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 对象0:( %.3f, %.3f ) 对象1:( %f, %f) 对象2:( %f, %f) 对象3:( %f, %f)"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 最大角度差值:%.3f"), checkInfo.m_dObjectAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 输入01-13:%.3f 输入13-32:%.3f 输入32-20:%.3f 输入20-01:%.3f"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 检查01-13:%.3f 检查13-32:%.3f 检查32-20:%.3f 检查20-01:%.3f"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果01-13:%d 结果13-32:%d 结果32-20:%d 结果20-01:%d"), TRUE, TRUE, TRUE, TRUE);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}

					return TRUE;
				}
				else
				{
					// 检查模式			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[0] = dAngle[0];		// _T("ObjectLen12");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[1] = dAngle[1];		// _T("ObjectLen34");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[2] = dAngle[2];		// _T("ObjectLen13");			
					m_AlignerObjectAngleInfo.m_vdObjectAngle[3] = dAngle[3];		// _T("ObjectLen24");

					BOOL bObjectAngle0 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[0] - checkInfo.m_dObjectAngle0) <= checkInfo.m_dObjectAngleThreshold );
					BOOL bObjectAngle1 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[1] - checkInfo.m_dObjectAngle1) <= checkInfo.m_dObjectAngleThreshold );
					BOOL bObjectAngle2 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[2] - checkInfo.m_dObjectAngle2) <= checkInfo.m_dObjectAngleThreshold );
					BOOL bObjectAngle3 = ( fabs(m_AlignerObjectAngleInfo.m_vdObjectAngle[3] - checkInfo.m_dObjectAngle3) <= checkInfo.m_dObjectAngleThreshold );

					// 日志记录
					{
						strTemp.Format(_T(" 对象产品角度信息 模式:%d,"), checkInfo.m_nCheckMode);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 对象0:( %.3f, %.3f ) 对象1:( %f, %f) 对象2:( %f, %f) 对象3:( %f, %f)"), 
							vMark[0].GetX(), vMark[0].GetY(), 
							vMark[1].GetX(), vMark[1].GetY(), 
							vMark[2].GetX(), vMark[2].GetY(), 
							vMark[3].GetX(), vMark[3].GetY());
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 最大角度差值:%.3f"), checkInfo.m_dObjectAngleThreshold);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 输入01-13:%.3f 输入13-32:%.3f 输入32-20:%.3f 输入20-01:%.3f"), 
							checkInfo.m_dObjectDis0, 
							checkInfo.m_dObjectDis1, 
							checkInfo.m_dObjectDis2, 
							checkInfo.m_dObjectDis3);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 检查01-13:%.3f 检查13-32:%.3f 检查32-20:%.3f 检查20-01:%.3f"), dAngle[0], dAngle[1], dAngle[2], dAngle[3]);
						strInfo = strInfo + strTemp;

						strTemp.Format(_T(" 结果01-13:%d 结果13-32:%d 结果32-20:%d 结果20-01:%d"), bObjectAngle0, bObjectAngle1, bObjectAngle2, bObjectAngle3);
						strInfo = strInfo + strTemp;

						VisionAlignLogRecord(m_strProductSizeData, strInfo);
					}


					{

						if(!bObjectAngle0)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
						}

						if(!bObjectAngle1)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
						}

						if(!bObjectAngle2)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
						}

						if(!bObjectAngle3)
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
						}
						else
						{
							m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
						}

						if (bObjectAngle0 && bObjectAngle1 && bObjectAngle2 && bObjectAngle3)
						{
							PostMessageUpdateAlignerObjectAngleInfo();
							return TRUE;
						}
						else
						{
							PostMessageUpdateAlignerObjectAngleInfo();
							return FALSE;
						}
					}
				}
			}
			break;

		default:
			{

			}
			break;


		}
	}

	return TRUE;
}

// BOOL vcXYDVisionAlign::CheckTargetAngle()
// {
// 	// 	if (GetPosNum() != 2)
// 	// 	{
// 	// 		return TRUE;
// 	// 	}
// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}
// 	// 获取当前产品信息
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 	// 设置补偿量索引
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	int nExProductIdx = 0;
// 	if (optionInfo.m_bTargetSearchExProduct)
// 	{
// 		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
// 	}
// 	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);
// 	if (!checkInfo.m_bCheckTargetAngle)
// 	{
// 		return TRUE;
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 
// 	if (GetPosNum() == 2)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPos();
// 
// 		scRadian rAngle;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		(vMark[1] - vMark[0]).Angle(rAngle);
// 
// 		CString str;
// 		str.Format(_T("TargetAngle: Mark[0]( %f, %f ), Mark[1]( %f, %f), Angle: %f, MaxAngleDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			scDegree(rAngle).SignedNorm().ToDouble(), checkInfo.m_dMaxTargetAngle);
// 
// 		AlignLogRecord(str);
// 
// 		if (fabs(scDegree(rAngle).SignedNorm().ToDouble()) > checkInfo.m_dMaxTargetAngle)
// 		{
// 			return FALSE;
// 		}
// 		else
// 		{
// 			return TRUE;
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPos();
// 
// 		scRadian rAngle;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		(vMark[1] - vMark[0]).Angle(rAngle);
// 
// 		CString str;
// 		str.Format(_T("TargetAngle: Mark[2]( %f, %f ), Mark[3]( %f, %f), Angle: %f, MaxAngleDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			scDegree(rAngle).SignedNorm().ToDouble(), checkInfo.m_dMaxTargetAngle);
// 
// 		AlignLogRecord(str);
// 
// 		if (fabs(scDegree(rAngle).SignedNorm().ToDouble()) > checkInfo.m_dMaxTargetAngle)
// 		{
// 			return FALSE;
// 		}
// 		else
// 		{
// 			return TRUE;
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	return TRUE;
// }

// BOOL vcXYDVisionAlign::RecordTargetSizeIndfo(BOOL bSearchOK, BOOL bCheckOK)
// {
// 	if (!m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
// 	{
// 		return TRUE;
// 	}
// 
// 	CString strDir;
// 	CFileFind findFile;
// 	BOOL IsFild, IsDir;
// 	IsFild = FALSE;
// 	IsDir = FALSE;
// 
// 	strDir  = "D:\\VisionASMLog";
// 
// 	IsFild = findFile.FindFile(strDir);
// 	while (IsFild)
// 	{
// 		IsFild = findFile.FindNextFile();
// 		IsDir = findFile.IsDirectory();
// 		if(IsDir) break;
// 	}	
// 	if (!IsDir)
// 	{
// 		if (!CreateDirectory(strDir, NULL))// 创建文件夹
// 		{
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		CString strInfo;
// 
// 		if (!bSearchOK)
// 		{
// 			strInfo.Format(_T("Target : SearchOK(%d) ; CheckOK(%d) ; Mark[5] ( %.4f , %.4f ) OK(%d) ; Mark[6]( %.4f , %.4f ) OK(%d); Mark[7] ( %.4f , %.4f ) OK(%d) ; Mark[8]( %.4f , %.4f ) OK(%d); Dis56: %.4f ; Dis78: %.4f ; Dis57: %.4f ; Dis68: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(4)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(5)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(6)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(7)
// 				, 0.0, 0.0, 0.0, 0.0) ;
// 		}
// 		else
// 		{
// 			strInfo.Format(_T("Target : SearchOK(%d) ; CheckOK(%d) ; Mark[5] ( %.4f , %.4f ) OK(%d) ; Mark[6]( %.4f , %.4f ) OK(%d); Mark[7] ( %.4f , %.4f ) OK(%d) ; Mark[8]( %.4f , %.4f ) OK(%d); Dis56: %.4f ; Dis78: %.4f ; Dis57: %.4f ; Dis68: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(4)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(5)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(6)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(7)
// 				, m_AlignerTargetSizeInfo.m_vdTargetSize[0]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[1]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[2]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[3]) ;
// 		}
// 
// 
// 		SYSTEMTIME sys; 
// 		GetLocalTime( &sys ); 
// 		CString t;
// 		t.Format("%04d_%02d_%02d_%02d_%02d_%02d_%03d]",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 
// 		CString strTemp = "";
// 		strTemp += t;
// 		strTemp += strInfo;
// 		strTemp += "\n";
// 
// 		CString strPathFileName;
// 		strPathFileName.Format(_T("Platform%dProductSizeInfo%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 		FILE *stream;
// 		CString strPath;
// 		strPath = strDir + _T("\\") + strPathFileName;
// 		stream = fopen(strPath, _T("a+t"));
// 		if (stream != NULL)
// 		{
// 			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 			fclose(stream);	
// 		}
// 
// 		// 清空前2个月对应日期后3天的日志文件
// 		int nYear, nMonth, nDay;
// 		nYear = sys.wYear;
// 		nMonth = sys.wMonth;
// 		nDay = sys.wDay;
// 
// 		nMonth = nMonth - 2;
// 		if (nMonth <= 0)
// 		{
// 			nYear = nYear -1;
// 			nMonth = nMonth + 12;
// 		}
// 		for (int i = 0; i < 3; i++)
// 		{
// 			strPathFileName.Format(_T("Platform%dTargetSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 			CString strPath = strDir + _T("\\") + strPathFileName;
// 			::DeleteFile(strPath);
// 		}
// 
// 		if (!bSearchOK || !bCheckOK)
// 		{
// 			CString strTemp = "";
// 			strTemp += t;
// 			strTemp += strInfo;
// 			strTemp += "\n";
// 
// 			CString strPathFileName;
// 			strPathFileName.Format(_T("Platform%dTargetSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 			FILE *stream;
// 			CString strPath;
// 			strPath = strDir + _T("\\") + strPathFileName;
// 			stream = fopen(strPath, _T("a+t"));
// 			if (stream != NULL)
// 			{
// 				fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 				fclose(stream);	
// 			}
// 
// 			// 清空前2个月对应日期后3天的日志文件
// 			int nYear, nMonth, nDay;
// 			nYear = sys.wYear;
// 			nMonth = sys.wMonth;
// 			nDay = sys.wDay;
// 
// 			nMonth = nMonth - 2;
// 			if (nMonth <= 0)
// 			{
// 				nYear = nYear -1;
// 				nMonth = nMonth + 12;
// 			}
// 			for (int i = 0; i < 3; i++)
// 			{
// 				strPathFileName.Format(_T("Platform%dTargetSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 				CString strPath = strDir + _T("\\") + strPathFileName;
// 				::DeleteFile(strPath);
// 			}
// 		}
// 	}
// 
// 	return TRUE;
// }

// BOOL vcXYDVisionAlign::RecordObjectSizeIndfo(BOOL bSearchOK, BOOL bCheckOK)
// {
// 
// 	if (!m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
// 	{
// 		return TRUE;
// 	}
// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}
// 	CString strDir;
// 	CFileFind findFile;
// 	BOOL IsFild, IsDir;
// 	IsFild = FALSE;
// 	IsDir = FALSE;
// 
// 	strDir  = "D:\\VisionASMLog";
// 
// 	IsFild = findFile.FindFile(strDir);
// 	while (IsFild)
// 	{
// 		IsFild = findFile.FindNextFile();
// 		IsDir = findFile.IsDirectory();
// 		if(IsDir) break;
// 	}	
// 	if (!IsDir)
// 	{
// 		if (!CreateDirectory(strDir, NULL))// 创建文件夹
// 		{
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		CString strInfo;
// 		if (!bSearchOK)
// 		{
// 			strInfo.Format(_T("Object : SearchOK(%d) ; CheckOK(%d) ; Mark[1] ( %.4f , %.4f ) OK(%d) ; Mark[2]( %.4f , %.4f ) OK(%d); Mark[3] ( %.4f , %.4f ) OK(%d) ; Mark[4]( %.4f , %.4f ) OK(%d); Dis12: %.4f ; Dis34: %.4f ; Dis13: %.4f ; Dis24: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(0)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(1)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(2)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(3)
// 				,0.0
// 				,0.0
// 				,0.0
// 				,0.0) ;
// 		}
// 		else
// 		{
// 			strInfo.Format(_T("Object : SearchOK(%d) ; CheckOK(%d) ; Mark[1] ( %.4f , %.4f ) OK(%d) ; Mark[2]( %.4f , %.4f ) OK(%d); Mark[3] ( %.4f , %.4f ) OK(%d) ; Mark[4]( %.4f , %.4f ) OK(%d); Dis12: %.4f ; Dis34: %.4f ; Dis13: %.4f ; Dis24: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(0)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(1)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(2)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(3)
// 				,m_AlignerObjectSizeInfo.m_vdObjectSize[0]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[1]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[2]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[3]) ;
// 		}
// 
// 
// 		SYSTEMTIME sys; 
// 		GetLocalTime( &sys ); 
// 		CString t;
// 		t.Format("%04d_%02d_%02d_%02d_%02d_%02d_%03d]",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 
// 		CString strTemp = "";
// 		strTemp += t;
// 		strTemp += strInfo;
// 		strTemp += "\n";
// 
// 		CString strPathFileName;
// 		strPathFileName.Format(_T("Platform%dObjectSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 		FILE *stream;
// 		CString strPath;
// 		strPath = strDir + _T("\\") + strPathFileName;
// 		stream = fopen(strPath, _T("a+t"));
// 		if (stream != NULL)
// 		{
// 			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 			fclose(stream);	
// 		}
// 
// 		// 清空前2个月对应日期后3天的日志文件
// 		int nYear, nMonth, nDay;
// 		nYear = sys.wYear;
// 		nMonth = sys.wMonth;
// 		nDay = sys.wDay;
// 
// 		nMonth = nMonth - 2;
// 		if (nMonth <= 0)
// 		{
// 			nYear = nYear -1;
// 			nMonth = nMonth + 12;
// 		}
// 		for (int i = 0; i < 3; i++)
// 		{
// 			strPathFileName.Format(_T("Platform%dObjectSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 			CString strPath = strDir + _T("\\") + strPathFileName;
// 			::DeleteFile(strPath);
// 		}
// 
// 
// 
// 
// 		if (!bSearchOK || !bCheckOK)
// 		{
// 			CString strTemp = "";
// 			strTemp += t;
// 			strTemp += strInfo;
// 			strTemp += "\n";
// 
// 			CString strPathFileName;
// 			strPathFileName.Format(_T("Platform%dObjectSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 			FILE *stream;
// 			CString strPath;
// 			strPath = strDir + _T("\\") + strPathFileName;
// 			stream = fopen(strPath, _T("a+t"));
// 			if (stream != NULL)
// 			{
// 				fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 				fclose(stream);	
// 			}
// 
// 			// 清空前2个月对应日期后3天的日志文件
// 			int nYear, nMonth, nDay;
// 			nYear = sys.wYear;
// 			nMonth = sys.wMonth;
// 			nDay = sys.wDay;
// 
// 			nMonth = nMonth - 2;
// 			if (nMonth <= 0)
// 			{
// 				nYear = nYear -1;
// 				nMonth = nMonth + 12;
// 			}
// 			for (int i = 0; i < 3; i++)
// 			{
// 				strPathFileName.Format(_T("Platform%dObjectSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 				CString strPath = strDir + _T("\\") + strPathFileName;
// 				::DeleteFile(strPath);
// 			}
// 		}
// 	}
// 
// 	return TRUE;
// }

// BOOL vcXYDVisionAlign::RecordTargetSizeIndfo(BOOL bSearchOK, BOOL bCheckOK)
// {
// 	if (!m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
// 	{
// 		return TRUE;
// 	}
// 
// 	CString strDir;
// 	CFileFind findFile;
// 	BOOL IsFild, IsDir;
// 	IsFild = FALSE;
// 	IsDir = FALSE;
// 
// 	strDir  = "D:\\VisionASMLog";
// 
// 	IsFild = findFile.FindFile(strDir);
// 	while (IsFild)
// 	{
// 		IsFild = findFile.FindNextFile();
// 		IsDir = findFile.IsDirectory();
// 		if(IsDir) break;
// 	}	
// 	if (!IsDir)
// 	{
// 		if (!CreateDirectory(strDir, NULL))// 创建文件夹
// 		{
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	if (GetPosNum() == 8 
// 		&& sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 		// 		&& sysPlatformInfo.m_nObjectPositionNum == 4
// 		// 		&& sysPlatformInfo.m_nTargetPositionNum == 2)
// 	{
// 		CString strInfo;
// 
// 		if (!bSearchOK)
// 		{
// 			strInfo.Format(_T("Target : SearchOK(%d) ; CheckOK(%d) ; Mark[5] ( %.4f , %.4f ) OK(%d) ; Mark[6]( %.4f , %.4f ) OK(%d); Mark[7] ( %.4f , %.4f ) OK(%d) ; Mark[8]( %.4f , %.4f ) OK(%d); Dis56: %.4f ; Dis78: %.4f ; Dis57: %.4f ; Dis68: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(4)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(5)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(6)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(7)
// 				, 0.0, 0.0, 0.0, 0.0) ;
// 		}
// 		else
// 		{
// 			strInfo.Format(_T("Target : SearchOK(%d) ; CheckOK(%d) ; Mark[5] ( %.4f , %.4f ) OK(%d) ; Mark[6]( %.4f , %.4f ) OK(%d); Mark[7] ( %.4f , %.4f ) OK(%d) ; Mark[8]( %.4f , %.4f ) OK(%d); Dis56: %.4f ; Dis78: %.4f ; Dis57: %.4f ; Dis68: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(4).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(4)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(5).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(5)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(6).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(6)
// 				,m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosX(),m_mpTargetMarkImagePos.GetMarkImagePos(7).GetPosY(),m_mpTargetMarkImagePos.GetIsMarkImagePosOK(7)
// 				, m_AlignerTargetSizeInfo.m_vdTargetSize[0]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[1]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[2]
// 			, m_AlignerTargetSizeInfo.m_vdTargetSize[3]) ;
// 		}
// 
// 
// 		SYSTEMTIME sys; 
// 		GetLocalTime( &sys ); 
// 		CString t;
// 		t.Format("%04d_%02d_%02d_%02d_%02d_%02d_%03d]",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 
// 		CString strTemp = "";
// 		strTemp += t;
// 		strTemp += strInfo;
// 		strTemp += "\n";
// 
// 		CString strPathFileName;
// 		strPathFileName.Format(_T("Platform%dProductSizeInfo%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 		FILE *stream;
// 		CString strPath;
// 		strPath = strDir + _T("\\") + strPathFileName;
// 		stream = fopen(strPath, _T("a+t"));
// 		if (stream != NULL)
// 		{
// 			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 			fclose(stream);	
// 		}
// 
// 		// 清空前2个月对应日期后3天的日志文件
// 		int nYear, nMonth, nDay;
// 		nYear = sys.wYear;
// 		nMonth = sys.wMonth;
// 		nDay = sys.wDay;
// 
// 		nMonth = nMonth - 2;
// 		if (nMonth <= 0)
// 		{
// 			nYear = nYear -1;
// 			nMonth = nMonth + 12;
// 		}
// 		for (int i = 0; i < 3; i++)
// 		{
// 			strPathFileName.Format(_T("Platform%dTargetSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 			CString strPath = strDir + _T("\\") + strPathFileName;
// 			::DeleteFile(strPath);
// 		}
// 
// 		if (!bSearchOK || !bCheckOK)
// 		{
// 			CString strTemp = "";
// 			strTemp += t;
// 			strTemp += strInfo;
// 			strTemp += "\n";
// 
// 			CString strPathFileName;
// 			strPathFileName.Format(_T("Platform%dTargetSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 			FILE *stream;
// 			CString strPath;
// 			strPath = strDir + _T("\\") + strPathFileName;
// 			stream = fopen(strPath, _T("a+t"));
// 			if (stream != NULL)
// 			{
// 				fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 				fclose(stream);	
// 			}
// 
// 			// 清空前2个月对应日期后3天的日志文件
// 			int nYear, nMonth, nDay;
// 			nYear = sys.wYear;
// 			nMonth = sys.wMonth;
// 			nDay = sys.wDay;
// 
// 			nMonth = nMonth - 2;
// 			if (nMonth <= 0)
// 			{
// 				nYear = nYear -1;
// 				nMonth = nMonth + 12;
// 			}
// 			for (int i = 0; i < 3; i++)
// 			{
// 				strPathFileName.Format(_T("Platform%dTargetSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 				CString strPath = strDir + _T("\\") + strPathFileName;
// 				::DeleteFile(strPath);
// 			}
// 		}
// 	}
// 
// 	return TRUE;
// }
// 
// BOOL vcXYDVisionAlign::RecordObjectSizeIndfo(BOOL bSearchOK, BOOL bCheckOK)
// {
// 	if (!m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
// 	{
// 		return TRUE;
// 	}
// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}
// 	CString strDir;
// 	CFileFind findFile;
// 	BOOL IsFild, IsDir;
// 	IsFild = FALSE;
// 	IsDir = FALSE;
// 
// 	strDir  = "D:\\VisionASMLog";
// 
// 	IsFild = findFile.FindFile(strDir);
// 	while (IsFild)
// 	{
// 		IsFild = findFile.FindNextFile();
// 		IsDir = findFile.IsDirectory();
// 		if(IsDir) break;
// 	}	
// 	if (!IsDir)
// 	{
// 		if (!CreateDirectory(strDir, NULL))// 创建文件夹
// 		{
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	if (GetPosNum() == 8 
// 		&& sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 		// 		&& sysPlatformInfo.m_nObjectPositionNum == 4
// 		// 		&& sysPlatformInfo.m_nTargetPositionNum == 2)
// 	{
// 		CString strInfo;
// 		if (!bSearchOK)
// 		{
// 			strInfo.Format(_T("Object : SearchOK(%d) ; CheckOK(%d) ; Mark[1] ( %.4f , %.4f ) OK(%d) ; Mark[2]( %.4f , %.4f ) OK(%d); Mark[3] ( %.4f , %.4f ) OK(%d) ; Mark[4]( %.4f , %.4f ) OK(%d); Dis12: %.4f ; Dis34: %.4f ; Dis13: %.4f ; Dis24: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(0)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(1)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(2)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(3)
// 				,0.0
// 				,0.0
// 				,0.0
// 				,0.0) ;
// 		}
// 		else
// 		{
// 			strInfo.Format(_T("Object : SearchOK(%d) ; CheckOK(%d) ; Mark[1] ( %.4f , %.4f ) OK(%d) ; Mark[2]( %.4f , %.4f ) OK(%d); Mark[3] ( %.4f , %.4f ) OK(%d) ; Mark[4]( %.4f , %.4f ) OK(%d); Dis12: %.4f ; Dis34: %.4f ; Dis13: %.4f ; Dis24: %.4f ;")
// 				,bSearchOK, bCheckOK
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(0).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(0)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(1).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(1)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(2).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(2)
// 				,m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosX(),m_mpObjectMarkImagePos.GetMarkImagePos(3).GetPosY(),m_mpObjectMarkImagePos.GetIsMarkImagePosOK(3)
// 				,m_AlignerObjectSizeInfo.m_vdObjectSize[0]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[1]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[2]
// 			, m_AlignerObjectSizeInfo.m_vdObjectSize[3]) ;
// 		}
// 
// 
// 		SYSTEMTIME sys; 
// 		GetLocalTime( &sys ); 
// 		CString t;
// 		t.Format("%04d_%02d_%02d_%02d_%02d_%02d_%03d]",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 
// 		CString strTemp = "";
// 		strTemp += t;
// 		strTemp += strInfo;
// 		strTemp += "\n";
// 
// 		CString strPathFileName;
// 		strPathFileName.Format(_T("Platform%dObjectSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 		FILE *stream;
// 		CString strPath;
// 		strPath = strDir + _T("\\") + strPathFileName;
// 		stream = fopen(strPath, _T("a+t"));
// 		if (stream != NULL)
// 		{
// 			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 			fclose(stream);	
// 		}
// 
// 		// 清空前2个月对应日期后3天的日志文件
// 		int nYear, nMonth, nDay;
// 		nYear = sys.wYear;
// 		nMonth = sys.wMonth;
// 		nDay = sys.wDay;
// 
// 		nMonth = nMonth - 2;
// 		if (nMonth <= 0)
// 		{
// 			nYear = nYear -1;
// 			nMonth = nMonth + 12;
// 		}
// 		for (int i = 0; i < 3; i++)
// 		{
// 			strPathFileName.Format(_T("Platform%dObjectSize%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 			CString strPath = strDir + _T("\\") + strPathFileName;
// 			::DeleteFile(strPath);
// 		}
// 
// 		if (!bSearchOK || !bCheckOK)
// 		{
// 			CString strTemp = "";
// 			strTemp += t;
// 			strTemp += strInfo;
// 			strTemp += "\n";
// 
// 			CString strPathFileName;
// 			strPathFileName.Format(_T("Platform%dObjectSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 			FILE *stream;
// 			CString strPath;
// 			strPath = strDir + _T("\\") + strPathFileName;
// 			stream = fopen(strPath, _T("a+t"));
// 			if (stream != NULL)
// 			{
// 				fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 				fclose(stream);	
// 			}
// 
// 			// 清空前2个月对应日期后3天的日志文件
// 			int nYear, nMonth, nDay;
// 			nYear = sys.wYear;
// 			nMonth = sys.wMonth;
// 			nDay = sys.wDay;
// 
// 			nMonth = nMonth - 2;
// 			if (nMonth <= 0)
// 			{
// 				nYear = nYear -1;
// 				nMonth = nMonth + 12;
// 			}
// 			for (int i = 0; i < 3; i++)
// 			{
// 				strPathFileName.Format(_T("Platform%dObjectSizeNG%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 				CString strPath = strDir + _T("\\") + strPathFileName;
// 				::DeleteFile(strPath);
// 			}
// 		}
// 	}
// 
// 	return TRUE;
// }


// 检测目标距离
// BOOL vcXYDVisionAlign::CheckTargetDistance()
// {
// 
// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}
// 	// 获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 	// 获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	int nExProductIdx = 0;
// 	if (optionInfo.m_bTargetSearchExProduct)
// 	{
// 		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
// 	}
// 	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);
// 
// 	if (m_pSystempOptionConfig->m_bCheckModeOnly)
// 	{
// 		checkInfo.m_nCheckMode = 1;
// 		checkInfo.m_bCheckTargetAndObjectDis = TRUE;
// 		checkInfo.m_bCheckObjectDis = TRUE;
// 	}
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 	{
// 		return TRUE;
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam)
// 	{
// 		int nPosNum = sysPlatformInfo.m_nPositionNum;
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		int nMark = nPosNum/2;
// 		int nExMark = (nPosNum/4) * nAlignnExProductIndex;
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPos();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4 + nExMark).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4 + nExMark).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5 + nExMark).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5 + nExMark).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		// 		CString str;
// 		// 		str.Format(_T("TargetDistance: Mark[2]( %f, %f ), Mark[3]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 		// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 		// 			dDis, checkInfo.m_dTargetDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 		// 
// 		// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis0 = dDis;
// 			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis2 = dDis;
// 			checkInfo.m_dTargetDis3 = dDis;
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			if (fabs(dDis - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("TargetDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dTargetDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis0 = dDis;
// 			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis2 = dDis;
// 			checkInfo.m_dTargetDis3 = dDis;
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("TargetDistance: Mark[2]( %f, %f ), Mark[3]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dTargetDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis0 = dDis;
// 			checkInfo.m_dTargetDis1 = dDis;
// 			checkInfo.m_dTargetDis2 = dDis;
// 			checkInfo.m_dTargetDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 
// 			if (fabs(dDis - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		// 位置1,2
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		// 位置3, 4
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		// 位置1, 3
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		// 位置2, 4
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		// 		CString str;
// 		// 		str.Format(_T("TargetDistance: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 		// 					  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY,
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY, 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY, 
// 		// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 		// 					  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 		// 		AlignLogRecord(str);
// 
// 
// 		{
// 			CString str;
// 			str.Format(_T("TargetDistance_%d: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 						  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 						  m_nCheckTargetDistanceIndex,
// 						  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 						  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY, 
// 						  dDis[0], dDis[1], dDis[2], dDis[3], 
// 						  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 			AlignLogRecord(str);
// 		}
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{				
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 			else 
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			// 目标位置4+位置5
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 目标位置6+位置7
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				if ( fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis )
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				return TRUE;
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 		}
// 
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)  // jjh0730 3v3 目标检查模式
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 
// 
// 		CString str;
// 		str.Format(_T("TargetDistance_%d: Mark[3]( %f, %f ), Mark[4]( %f, %f), Mark[5]( %f, %f ), \
// 					  Dis34: %f, Dis35: %f, Dis45: %f, StdDis34: %f, StdDis35: %f, StdDis45: %f,  MaxDisDev: %f"), 
// 					  m_nCheckTargetDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{				
// 
// 			checkInfo.m_dTargetDis0 = dDis[0];
// 			checkInfo.m_dTargetDis1 = dDis[1];
// 			checkInfo.m_dTargetDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{		
// 			if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)  // jjh0730 3v3 目标检查模式
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 
// 
// 		CString str;
// 		str.Format(_T("TargetDistance_%d: Mark[3]( %f, %f ), Mark[4]( %f, %f), Mark[5]( %f, %f ), \
// 					  Dis34: %f, Dis35: %f, Dis45: %f, StdDis34: %f, StdDis35: %f, StdDis45: %f,  MaxDisDev: %f"), 
// 					  m_nCheckTargetDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{				
// 
// 			checkInfo.m_dTargetDis0 = dDis[0];
// 			checkInfo.m_dTargetDis1 = dDis[1];
// 			checkInfo.m_dTargetDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{		
// 			if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		if (pAlignerParam->GetAligner4Vs4Type()==eAlignerPos1234_56_4V2)
// 		{
// 			dDis[1] = dDis[0];
// 			dDis[2] = 0;
// 			dDis[3] = 0;
// 		}
// 		// 		CString str;
// 		// 		str.Format(_T("TargetDistance: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 		// 					  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY,
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY, 
// 		// 					  mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY, 
// 		// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 		// 					  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 		// 		AlignLogRecord(str);
// 
// 		// 目标位置4+位置5
// 		if (m_nCheckTargetDistanceIndex == 19)
// 		{
// 			CString str;
// 			str.Format(_T("TargetDistance_%d: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 						  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 						  m_nCheckTargetDistanceIndex,
// 						  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY,
// 						  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY, 
// 						  dDis[0], dDis[1], dDis[2], dDis[3], 
// 						  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 			AlignLogRecord(str);
// 		}
// 		// 目标位置6+位置7
// 		else if (m_nCheckTargetDistanceIndex == 20)
// 		{
// 			CString str;
// 			str.Format(_T("TargetDistance_%d: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 						  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 						  m_nCheckTargetDistanceIndex,
// 						  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY,
// 						  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY, 
// 						  dDis[0], dDis[1], dDis[2], dDis[3], 
// 						  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 			AlignLogRecord(str);
// 		}
// 		else 
// 		{
// 			CString str;
// 			str.Format(_T("TargetDistance_%d: Mark[4]( %f, %f ), Mark[5]( %f, %f), Mark[6]( %f, %f ), Mark[7]( %f, %f), \
// 						  Dis45: %f, Dis67: %f, Dis46: %f, Dis57: %f, StdDis45: %f, StdDis67: %f, StdDis46: %f, StdDis57: %f, MaxDisDev: %f"), 
// 						  m_nCheckTargetDistanceIndex,
// 						  mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY,
// 						  mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY, 
// 						  mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY, 
// 						  dDis[0], dDis[1], dDis[2], dDis[3], 
// 						  checkInfo.m_dTargetDis0, checkInfo.m_dTargetDis1, checkInfo.m_dTargetDis2, checkInfo.m_dTargetDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 			AlignLogRecord(str);
// 		}
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{				
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 			else 
// 			{
// 				checkInfo.m_dTargetDis0 = dDis[0];
// 				checkInfo.m_dTargetDis1 = dDis[1];
// 				checkInfo.m_dTargetDis2 = dDis[2];
// 				checkInfo.m_dTargetDis3 = dDis[3];
// 			}
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerTargetStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 			// 				|| fabs(dDis[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			// 目标位置4+位置5
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 目标位置6+位置7
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				if ( fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis )
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	// 
// 	// 	// 计算目标距离
// 	// 	CMarkPlatformPos mpTargetMarkPos;
// 	// 	mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPos();
// 	// 
// 	// 	double dDis = 0;
// 	// 	sc2Vector vMark[2];
// 	// 	vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 	// 	vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 	// 	vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 	// 	vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 	// 	dDis = vMark[0].Distance(vMark[1]);
// 	// 
// 	// 	if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 	// 	{
// 	// 		checkInfo.m_dTargetDis1 = dDis;
// 	// 		pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 		pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 
// 	// 		SaveProductDataInfo(nCurProductIndex);
// 	// 
// 	// 		return TRUE;
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (fabs(dDis - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 	// 		{
// 	// 			return FALSE;
// 	// 		}
// 	// 		else
// 	// 		{
// 	// 			return TRUE;
// 	// 		}
// 	// 	}
// 
// 	return TRUE;
// }

// 检测目标角度




// 检测对象距离
// BOOL vcXYDVisionAlign::CheckObjectDistance()
// {
// 	// 	if (GetPosNum() != 2)
// 	// 	{
// 	// 		return TRUE;
// 	// 	}
// 	if (m_searchAllResultStatus == eSARSResultNG1 ||m_searchAllResultStatus == eSARSResultNGMore)
// 	{
// 		return TRUE;
// 	}
// 	// 1.获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 2.获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	int nExProductIdx = 0;
// 	if (optionInfo.m_bObjectSearchExProduct)
// 	{
// 		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
// 	}
// 	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);
// 
// 	if (checkInfo.m_bEnableTarObjSeparateCheck)
// 	{
// 		return CheckObjectDistanceByObjMaxDis();
// 	}
// 
// 	if (m_pSystempOptionConfig->m_bCheckModeOnly)
// 	{
// 		checkInfo.m_nCheckMode = 1;
// 		checkInfo.m_bCheckTargetAndObjectDis = TRUE;
// 		checkInfo.m_bCheckObjectDis = TRUE;
// 	}
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 	{
// 		return TRUE;
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTarCamWithDiffObjCam && sysPlatformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		int nPosNum = sysPlatformInfo.m_nPositionNum;
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		int nMark = nPosNum/2;
// 		int nExMark = (nPosNum/4) * nAlignnExProductIndex;
// 
// 
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0 + nExMark).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0 + nExMark).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1 + nExMark).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1 + nExMark).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		// 		CString str;
// 		// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 		// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 		// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 		// 
// 		// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 
// 
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), Mark[3]( %f, %f), \
// 					  Dis01: %f, Dis23: %f, Dis02: %f, Dis13: %f, StdDis01: %f, StdDis23: %f, StdDis02: %f, StdDis13: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dObjectDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 			checkInfo.m_dObjectDis3 = dDis[3];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				return TRUE;
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)   //jjh0730 3v3检查模式
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), \
// 					  Dis01: %f, Dis02: %f, Dis12: %f, StdDis01: %f, StdDis02: %f, StdDis12: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY,  
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)   //jjh0730 3v3检查模式
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), \
// 					  Dis01: %f, Dis02: %f, Dis12: %f, StdDis01: %f, StdDis02: %f, StdDis12: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY,  
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), Mark[3]( %f, %f), \
// 					  Dis01: %f, Dis23: %f, Dis02: %f, Dis13: %f, StdDis01: %f, StdDis23: %f, StdDis02: %f, StdDis13: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dObjectDis3, checkInfo.m_dMaxTargetAndObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 			checkInfo.m_dObjectDis3 = dDis[3];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	// 	// 计算对象距离
// 	// 	CMarkPlatformPos mpTargetMarkPos;
// 	// 	mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPos();
// 	// 
// 	// 	double dDis = 0;
// 	// 	sc2Vector vMark[2];
// 	// 	vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 	// 	vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 	// 	vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 	// 	vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 	// 	dDis = vMark[0].Distance(vMark[1]);
// 	// 
// 	// 	if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 	// 	{
// 	// 		checkInfo.m_dObjectDis1 = dDis;
// 	// 		pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 		pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 
// 	// 		SaveProductDataInfo(nCurProductIndex);
// 	// 
// 	// 		return TRUE;
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 	// 		{
// 	// 			return FALSE;
// 	// 		}
// 	// 		else
// 	// 		{
// 	// 			return TRUE;
// 	// 		}
// 	// 	}
// 
// 	return TRUE;
// }

// 对象目标最大值分离时检测对象距离
// BOOL vcXYDVisionAlign::CheckObjectDistanceByObjMaxDis()
// {
// 	// 1.获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 2.获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	int nExProductIdx = 0;
// 	if (optionInfo.m_bObjectSearchExProduct)
// 	{
// 		nExProductIdx = m_pAlignerTool->GetAlignExProductIndex();
// 	}
// 	CAlignCheckInfo checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(nExProductIdx);
// 
// 	if (m_pSystempOptionConfig->m_bCheckModeOnly)
// 	{
// 		checkInfo.m_nCheckMode = 1;
// 		checkInfo.m_bCheckTargetAndObjectDis = TRUE;
// 		checkInfo.m_bCheckObjectDis = TRUE;
// 	}
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 	{
// 		return TRUE;
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTarCamWithDiffObjCam && sysPlatformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		int nPosNum = sysPlatformInfo.m_nPositionNum;
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		int nMark = nPosNum/2;
// 		int nExMark = (nPosNum/4) * nAlignnExProductIndex;
// 
// 
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0 + nExMark).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0 + nExMark).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1 + nExMark).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1 + nExMark).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		// 		CString str;
// 		// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 		// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 		// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxTargetAndObjectDis);
// 		// 
// 		// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 
// 
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), Mark[3]( %f, %f), \
// 					  Dis01: %f, Dis23: %f, Dis02: %f, Dis13: %f, StdDis01: %f, StdDis23: %f, StdDis02: %f, StdDis13: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dObjectDis3, checkInfo.m_dMaxObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 			checkInfo.m_dObjectDis3 = dDis[3];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				return TRUE;
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance: Mark[0]( %f, %f ), Mark[1]( %f, %f), Dis01: %f, StdDis: %f, MaxDisDev: %f"), 
// 			vMark[0].GetX(), vMark[0].GetY(), vMark[1].GetX(), vMark[1].GetY(), 
// 			dDis, checkInfo.m_dObjectDis1,checkInfo.m_dMaxObjectDis);
// 
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			//			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis0 = dDis;
// 			checkInfo.m_dObjectDis1 = dDis;
// 			checkInfo.m_dObjectDis2 = dDis;
// 			checkInfo.m_dObjectDis3 = dDis;
// 
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			// 			{
// 			// 				return FALSE;
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				return TRUE;
// 			// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 
// 			if (fabs(dDis - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)   //jjh0730 3v3检查模式
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), \
// 					  Dis01: %f, Dis02: %f, Dis12: %f, StdDis01: %f, StdDis02: %f, StdDis12: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY,  
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dMaxObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)   //jjh0730 3v3检查模式
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), \
// 					  Dis01: %f, Dis02: %f, Dis12: %f, StdDis01: %f, StdDis02: %f, StdDis12: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY,  
// 					  dDis[0], dDis[1], dDis[2], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dMaxObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis
// 				|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis
// 				|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		CString str;
// 		str.Format(_T("ObjectDistance_%d: : Mark[0]( %f, %f ), Mark[1]( %f, %f), Mark[2]( %f, %f ), Mark[3]( %f, %f), \
// 					  Dis01: %f, Dis23: %f, Dis02: %f, Dis13: %f, StdDis01: %f, StdDis23: %f, StdDis02: %f, StdDis13: %f, MaxDisDev: %f"), 
// 					  m_nCheckObjectDistanceIndex,
// 					  mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY,
// 					  mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY, 
// 					  mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX, mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY, 
// 					  dDis[0], dDis[1], dDis[2], dDis[3], 
// 					  checkInfo.m_dObjectDis0, checkInfo.m_dObjectDis1, checkInfo.m_dObjectDis2, checkInfo.m_dObjectDis3, checkInfo.m_dMaxObjectDis);
// 		AlignLogRecord(str);
// 
// 		if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 		{
// 			checkInfo.m_dObjectDis0 = dDis[0];
// 			checkInfo.m_dObjectDis1 = dDis[1];
// 			checkInfo.m_dObjectDis2 = dDis[2];
// 			checkInfo.m_dObjectDis3 = dDis[3];
// 
// 			EnterCriticalSection(&m_csAlignCheckInfo);
// 			// 设置当前产品对位参数
// 			pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			pAlignerParam->SetAlignCheckInfo(checkInfo,nExProductIdx);
// 			LeaveCriticalSection(&m_csAlignCheckInfo);
// 
// 			SaveProductDataInfo(nCurProductIndex);
// 
// 			PostMessageUpdateAlignerObjectStdSizeInfo();
// 
// 			return TRUE;
// 		}
// 		else
// 		{
// 			// 检查模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(dDis[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis
// 					|| fabs(dDis[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 				{
// 					return FALSE;
// 				}
// 				else
// 				{
// 					return TRUE;
// 				}
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	// 	// 计算对象距离
// 	// 	CMarkPlatformPos mpTargetMarkPos;
// 	// 	mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPos();
// 	// 
// 	// 	double dDis = 0;
// 	// 	sc2Vector vMark[2];
// 	// 	vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 	// 	vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 	// 	vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 	// 	vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 	// 	dDis = vMark[0].Distance(vMark[1]);
// 	// 
// 	// 	if (checkInfo.m_nCheckMode == 0)	// 输入模式
// 	// 	{
// 	// 		checkInfo.m_dObjectDis1 = dDis;
// 	// 		pCurProductData->m_pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 		pAlignerParam->SetAlignCheckInfo(checkInfo);
// 	// 
// 	// 		SaveProductDataInfo(nCurProductIndex);
// 	// 
// 	// 		return TRUE;
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (fabs(dDis - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 	// 		{
// 	// 			return FALSE;
// 	// 		}
// 	// 		else
// 	// 		{
// 	// 			return TRUE;
// 	// 		}
// 	// 	}
// 
// 	return TRUE;
// }

// 计算目标距离
// BOOL vcXYDVisionAlign::CalculateTargetDistance()
// {
// 	// 清除目标距离信息
// 	int i = 0;
// 	for(i = 0; i < TARGETSIZEMAXNUM; i++)
// 	{
// 		m_AlignerTargetSizeInfo.m_vdTargetSize[i] = 0;
// 		m_AlignerTargetSizeInfo.m_vnCheckStatus[i] = eSCSUnCheck;
// 	}
// 
// 	// 获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
// 
// 	CAlignCheckInfo checkInfo;
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	if (optionInfo.m_bTargetSearchExProduct)
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(m_pAlignerTool->GetAlignExProductIndex());
// 	}
// 	else
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(0);
// 	}
// 
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis)  // 是否开启检查模式
// 	{
// 		return TRUE;
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam && GetPosNum() == 8)
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialTargetMarkPlatformPos();
// 
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		if (nAlignnExProductIndex == 0)
// 		{
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = 0;
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = 0;
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = 0;
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = 0;
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[0];
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = 0;
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = 0;
// 
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式			
// 			// 目标位置4+位置5
// 			if (nAlignnExProductIndex == 0)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 目标位置6+位置7
// 			else if (nAlignnExProductIndex == 1)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 		}
// 
// 		PostMessageUpdateAlignerTargetSizeInfo();
// 
// 		return TRUE;
// 
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen12");
// 		m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;	
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_bCheckTargetDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen34");
// 		m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis;
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		{
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen78");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen57");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] = _T("TargetLen68");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式			
// 			// 目标位置4+位置5
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 目标位置6+位置7
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)  // jjh0730 3v3目标检查模式
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		{
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen45");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen46");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式			
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)  // jjh0730 3v3目标检查模式
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		{
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen45");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen46");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式			
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算目标距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetTargetMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(4).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(6).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(5).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(7).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		if (pAlignerParam->GetAligner4Vs4Type()==eAlignerPos1234_56_4V2)
// 		{
// 			dDis[1] = dDis[0];
// 			dDis[2] = 0;
// 			dDis[3] = 0;
// 		}
// 
// 		// 目标位置4+位置5
// 		if (m_nCheckTargetDistanceIndex == 19)
// 		{
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen78");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen57");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] = _T("TargetLen68");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];
// 		}
// 		// 目标位置6+位置7
// 		else if (m_nCheckTargetDistanceIndex == 20)
// 		{			
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen78");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen57");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] = _T("TargetLen68");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];
// 		}
// 		else
// 		{
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = _T("TargetLen56");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[0] = dDis[0];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] = _T("TargetLen78");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[1] = dDis[1];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] = _T("TargetLen57");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[2] = dDis[2];
// 
// 			m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] = _T("TargetLen68");
// 			m_AlignerTargetSizeInfo.m_vdTargetSize[3] = dDis[3];
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckTargetDis)
// 		{	
// 			// 不进行目标检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式			
// 			// 目标位置4+位置5
// 			if (m_nCheckTargetDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 目标位置6+位置7
// 			else if (m_nCheckTargetDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[0] - checkInfo.m_dTargetDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[1] - checkInfo.m_dTargetDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[2] - checkInfo.m_dTargetDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerTargetSizeInfo.m_vdTargetSize[3] - checkInfo.m_dTargetDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerTargetSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	PostMessageUpdateAlignerTargetSizeInfo();
// 
// 	return TRUE;
// }

// BOOL vcXYDVisionAlign::CalculateObjectDistance()
// {
// 	// 清除目标距离信息
// 	int i = 0;
// 	for(i = 0; i < OBJECTSIZEMAXNUM; i++)
// 	{
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[i] = 0;
// 		m_AlignerObjectSizeInfo.m_vnCheckStatus[i] = eSCSUnCheck;
// 	}
// 
// 	// 1.获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 2.获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 
// 	CAlignCheckInfo checkInfo;
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	if (optionInfo.m_bObjectSearchExProduct)
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(m_pAlignerTool->GetAlignExProductIndex());
// 	}
// 	else
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(0);
// 	}
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis)  // 是否开启检查模式
// 	{
// 		return TRUE;
// 	}
// 
// 	if (checkInfo.m_bEnableTarObjSeparateCheck)
// 	{
// 		return CalculateObjectDistanceByObjMaxDis();
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam && GetPosNum() == 8)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		if (nAlignnExProductIndex == 0)
// 		{
// 
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (nAlignnExProductIndex == 0)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (nAlignnExProductIndex == 1)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 
// 		PostMessageUpdateAlignerObjectSizeInfo();
// 
// 		return TRUE;
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;	
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		if (23 == m_nCheckObjectDistanceIndex)
// 		{
// 			;
// 		}
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	PostMessageUpdateAlignerObjectSizeInfo();
// 
// 	return TRUE;
// }

// 计算对象距离
// BOOL vcXYDVisionAlign::CalculateObjectDistance()
// {
// 	// 清除目标距离信息
// 	int i = 0;
// 	for(i = 0; i < OBJECTSIZEMAXNUM; i++)
// 	{
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[i] = 0;
// 		m_AlignerObjectSizeInfo.m_vnCheckStatus[i] = eSCSUnCheck;
// 	}
// 
// 	// 1.获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 2.获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 
// 	CAlignCheckInfo checkInfo;
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	if (optionInfo.m_bObjectSearchExProduct)
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(m_pAlignerTool->GetAlignExProductIndex());
// 	}
// 	else
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(0);
// 	}
// 
// 	if (!checkInfo.m_bCheckTargetAndObjectDis)  // 是否开启检查模式
// 	{
// 		return TRUE;
// 	}
// 
// 	if (checkInfo.m_bEnableTarObjSeparateCheck)
// 	{
// 		return CalculateObjectDistanceByObjMaxDis();
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam && GetPosNum() == 8)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		if (nAlignnExProductIndex == 0)
// 		{
// 
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (nAlignnExProductIndex == 0)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (nAlignnExProductIndex == 1)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 
// 		PostMessageUpdateAlignerObjectSizeInfo();
// 
// 		return TRUE;
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;	
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		if (23 == m_nCheckObjectDistanceIndex)
// 		{
// 			;
// 		}
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxTargetAndObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	PostMessageUpdateAlignerObjectSizeInfo();
// 
// 	return TRUE;
// }

//目标对位最大值分离时计算对象距离
// BOOL vcXYDVisionAlign::CalculateObjectDistanceByObjMaxDis()
// {
// 	// 1.获取当前产品序号
// 	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
// 	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
// 	{
// 		return FALSE;
// 	}
// 
// 	// 2.获取当前产品信息
// 	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
// 	if (pCurProductData == NULL)
// 	{
// 		return FALSE;
// 	}
// 
// 	CAlignCheckInfo checkInfo;
// 	CPlatformOptionInfo optionInfo;
// 	GetPlatformOptionInfo(optionInfo);
// 	if (optionInfo.m_bObjectSearchExProduct)
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(m_pAlignerTool->GetAlignExProductIndex());
// 	}
// 	else
// 	{
// 		checkInfo = pCurProductData->m_pAlignerParam->GetAlignCheckInfo(0);
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_bTarCamWithDiffObjCam && GetPosNum() == 8)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetSpecialObjectMarkPlatformPos();
// 
// 		int nAlignnExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
// 
// 		if (nAlignnExProductIndex == 0)
// 		{
// 
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else if (nAlignnExProductIndex == 1)
// 		{
// 			double dDis[1];
// 			sc2Vector vMark[2];
// 
// 			vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 			vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 			vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 			vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 			dDis[0] = vMark[0].Distance(vMark[1]);
// 
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[0];
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[2] = 0;
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[3] = 0;
// 
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (nAlignnExProductIndex == 0)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (nAlignnExProductIndex == 1)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 
// 		PostMessageUpdateAlignerObjectSizeInfo();
// 
// 		return TRUE;
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	if (GetPosNum() == 2)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;	
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis = 0;
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis;
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 4 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)
// 	{
// 		if (23 == m_nCheckObjectDistanceIndex)
// 		{
// 			;
// 		}
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else if (m_nCheckObjectDistanceIndex == 23||m_nCheckObjectDistanceIndex == 24)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 3 && sysPlatformInfo.m_bTargetObjectCamSeparate == FALSE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 
// 		}
// 	}
// 	else if (GetPosNum() == 6 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)  // jjh0730  3v3对象检查
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[3];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 			}
// 
// 			if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 			}
// 			else
// 			{
// 				m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 			}
// 		}
// 	}
// 	else if (GetPosNum() == 8 && sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		// 计算对象距离
// 		CMarkPlatformPos mpTargetMarkPos;
// 		mpTargetMarkPos = m_pAlignerTool->GetObjectMarkPlatformPosForCheck();
// 
// 		double dDis[4];
// 		sc2Vector vMark[2];
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		dDis[0] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[1] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(0).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(2).m_dPosY);
// 		dDis[2] = vMark[0].Distance(vMark[1]);
// 		vMark[0].SetX(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosX);
// 		vMark[0].SetY(mpTargetMarkPos.GetMarkPlatformPos(1).m_dPosY);
// 		vMark[1].SetX(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosX);
// 		vMark[1].SetY(mpTargetMarkPos.GetMarkPlatformPos(3).m_dPosY);
// 		dDis[3] = vMark[0].Distance(vMark[1]);
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = _T("ObjectLen12");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[0] = dDis[0];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1] = _T("ObjectLen34");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[1] = dDis[1];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2] = _T("ObjectLen13");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[2] = dDis[2];
// 
// 		//m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3] = _T("ObjectLen24");
// 		m_AlignerObjectSizeInfo.m_vdObjectSize[3] = dDis[3];
// 
// 		if (!checkInfo.m_bCheckTargetAndObjectDis || !checkInfo.m_bCheckObjectDis)
// 		{	
// 			// 不进行对象检测
// 		}
// 		else if (checkInfo.m_nCheckMode == 0)
// 		{
// 			// 输入模式
// 		}
// 		else
// 		{
// 			// 检测模式
// 			// 对象位置0+位置1
// 			if (m_nCheckObjectDistanceIndex == 19)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 			}
// 			// 对象位置2+位置3
// 			else if (m_nCheckObjectDistanceIndex == 20)
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 			}
// 			else
// 			{
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[0] - checkInfo.m_dObjectDis0) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[0] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[1] - checkInfo.m_dObjectDis1) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[1] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[2] - checkInfo.m_dObjectDis2) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[2] = eSCSCheckOK;
// 				}
// 
// 				if (fabs(m_AlignerObjectSizeInfo.m_vdObjectSize[3] - checkInfo.m_dObjectDis3) > checkInfo.m_dMaxObjectDis)
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckNG;
// 				}
// 				else
// 				{
// 					m_AlignerObjectSizeInfo.m_vnCheckStatus[3] = eSCSCheckOK;
// 				}
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// 	PostMessageUpdateAlignerObjectSizeInfo();
// 
// 	return TRUE;
// }

// 记录目标、对象距离信息
// BOOL vcXYDVisionAlign::RecordProductSizeIndfo()
// {
// 	if (!m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
// 	{
// 		return TRUE;
// 	}
// 
// 	CString strDir;
// 	CFileFind findFile;
// 	BOOL IsFild, IsDir;
// 	IsFild = FALSE;
// 	IsDir = FALSE;
// 
// 	strDir  = "D:\\VisionASMLog";
// 
// 	IsFild = findFile.FindFile(strDir);
// 	while (IsFild)
// 	{
// 		IsFild = findFile.FindNextFile();
// 		IsDir = findFile.IsDirectory();
// 		if(IsDir) break;
// 	}	
// 	if (!IsDir)
// 	{
// 		if (!CreateDirectory(strDir, NULL))// 创建文件夹
// 		{
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	SysPlatformInfo sysPlatformInfo;
// 	GetSysPlatformInfo(sysPlatformInfo);
// 	if (GetPosNum() == 8 
// 		&& sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE)
// 		// 		&& sysPlatformInfo.m_nObjectPositionNum == 4
// 		// 		&& sysPlatformInfo.m_nTargetPositionNum == 2)
// 	{
// 		CString strInfo;
// 		strInfo.Format(_T("产品尺寸(mm): 对象12长度: %.4f ; 对象34长度: %.4f ; 对象左13长度: %.4f ; 对象24长度: %.4f ;目标56长度: %.4f ;目标78长度: %.4f ; 目标57长度: %.4f ;目标68长度: %.4f ;"),
// 			m_AlignerObjectSizeInfo.m_vdObjectSize[0]
// 		, m_AlignerObjectSizeInfo.m_vdObjectSize[1]
// 		, m_AlignerObjectSizeInfo.m_vdObjectSize[2]
// 		, m_AlignerObjectSizeInfo.m_vdObjectSize[3]
// 		, m_AlignerTargetSizeInfo.m_vdTargetSize[0]
// 		, m_AlignerTargetSizeInfo.m_vdTargetSize[1]
// 		, m_AlignerTargetSizeInfo.m_vdTargetSize[2]
// 		, m_AlignerTargetSizeInfo.m_vdTargetSize[3]) ;
// 
// 		SYSTEMTIME sys; 
// 		GetLocalTime( &sys ); 
// 		CString t;
// 		t.Format("%04d_%02d_%02d_%02d_%02d_%02d_%03d]",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 
// 		CString strTemp = "";
// 		strTemp += t;
// 		strTemp += strInfo;
// 		strTemp += "\n";
// 
// 		CString strPathFileName;
// 		strPathFileName.Format(_T("Platform%dProductSizeInfo%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,sys.wYear,sys.wMonth,sys.wDay);
// 
// 		FILE *stream;
// 		CString strPath;
// 		strPath = strDir + _T("\\") + strPathFileName;
// 		stream = fopen(strPath, _T("a+t"));
// 		if (stream != NULL)
// 		{
// 			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
// 			fclose(stream);	
// 		}
// 
// 		// 清空前2个月对应日期后3天的日志文件
// 		int nYear, nMonth, nDay;
// 		nYear = sys.wYear;
// 		nMonth = sys.wMonth;
// 		nDay = sys.wDay;
// 
// 		nMonth = nMonth - 2;
// 		if (nMonth <= 0)
// 		{
// 			nYear = nYear -1;
// 			nMonth = nMonth + 12;
// 		}
// 		for (int i = 0; i < 3; i++)
// 		{
// 			strPathFileName.Format(_T("Platform%dProductSizeInfo%04d_%02d_%02d"),m_pPlatformInfo->m_nPlatformIndex,nYear,nMonth,nDay+i);
// 			CString strPath = strDir + _T("\\") + strPathFileName;
// 			::DeleteFile(strPath);
// 		}
// 
// 
// 	}
// 
// 	return TRUE;
// }

// 记录目标、对象尺寸信息

