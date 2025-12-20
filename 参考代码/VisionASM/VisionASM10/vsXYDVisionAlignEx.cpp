// vsXYDVisionAilgnerEx.cpp: implementation of the vcXYDVisionAlign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisionASM.h"
#include "vsXYDVisionAlign.h"
#include "svXMLConfigurator.h"
#include "VisionASMDlg.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL vcXYDVisionAlign::OnBendPressParaWrite()
{
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex >= 0 && nCurProductIndex < m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
		if (pCurProductData != NULL)
		{
			CBendInfo BendInfo = pCurProductData->m_pAlignerParam->GetBendInfo();
			std::vector<int> vnDtata;
			vnDtata.push_back(BendInfo.m_dBendPressOKYmin * 1000);
			vnDtata.push_back(BendInfo.m_dBendPressOKYmax * 1000);
			vnDtata.push_back(BendInfo.m_dBendPressNGYmin * 1000);
			vnDtata.push_back(BendInfo.m_dBendPressNGYmax * 1000);

			if (BendInfo.m_bBendPressParaWrite)
			{
				//打包写入数据
				CString strSend;
				if (m_XYDCommProtocol.PackWriteBendPressPara(strSend,BendInfo.m_lBendPressParaWriteAddre,vnDtata))
				{
					CString strInfo;
					CommStatus nCommStatus = eCommSucceed;
					CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
				}
			}
		}
	}

	return TRUE;
}

// 工作线程
UINT vcXYDVisionAlign::WorkThreadEx(LPVOID pParam)
{
	vcXYDVisionAlign* pOwner = (vcXYDVisionAlign*)pParam;

	CString strReceive;
	SysWorkCmdType sysWorkCmdType;

	std::vector<int> vnReceivedData;
	std::vector<int> nParam;
	std::vector<double> dParam;
	CString strInfo;
	BOOL bWrtWorkStatus = FALSE;
	

	BOOL bSpecialCommSuccess = FALSE;
	pOwner->m_bExitWorkThreaded = FALSE;
	Sleep(2000);
	CPlatformOptionInfo optionInfo;
	pOwner->GetPlatformOptionInfo(optionInfo);
	// 默认先获取
	BOOL bReLoadPlatformOptionInfo = TRUE;
	BOOL bHeart = FALSE;
	long tStrat = GetTickCount();

	scTimer cBendPressParaWriteTimer;
	BOOL bBendPressParaWriteStart = FALSE;

	while (FALSE == pOwner->m_bExitWorkThread)
	{
		if (pOwner->m_bExitWorkThread)
			break; // 跳出while循环体

		vnReceivedData.clear();
		//strReceive = _T("");
		
		pOwner->m_bCurThreadWorkFinished = FALSE;
		if (pOwner->m_bIsExecuteFixTargetCheck || pOwner->m_bIsExecuteSizeCheck)  //执行标定关联验证过程时 避免PLC获取轴数据出现冲突
		{
			pOwner->m_bCurThreadWorkFinished = TRUE;
			Sleep(2);
			continue;
		}
		if (!pOwner->m_bSuspendWorkThread)
		{
			// 重新Load配置
			if (bReLoadPlatformOptionInfo)
			{
				bReLoadPlatformOptionInfo = FALSE;
				pOwner->GetPlatformOptionInfo(optionInfo);
			}

			if (pOwner->m_pVisionASMConfig->m_DataRecord.m_bDataRecordTime)
			{
				CTime tCurrent = CTime::GetCurrentTime();
				if (pOwner->m_bNeedUpdateAddrTime
					||tCurrent.GetYear()!=pOwner->m_nYear || tCurrent.GetMonth()!=pOwner->m_nMonth ||
					tCurrent.GetDay()!=pOwner->m_nDay || tCurrent.GetHour()!=pOwner->m_nHour)
				{
					//pOwner->m_bNeedUpdateAddrTime=FALSE;
					pOwner->UpdateDataRecordTxtAddrTime();
				}

				pOwner->m_nYear = tCurrent.GetYear();
				pOwner->m_nMonth = tCurrent.GetMonth();
				pOwner->m_nDay = tCurrent.GetDay();
				pOwner->m_nHour = tCurrent.GetHour();
				pOwner->m_nMinute = tCurrent.GetMinute();

			}
			
			if (pOwner->m_bStopNow)
			{
				Sleep(2);
				continue;
			}
			// 通信重新连接
//			pOwner->ReOpenComm();
            bWrtWorkStatus = FALSE;

// 			if (pOwner->ReOpenComm())
// 			{
// 				if (TRUE == pOwner->m_pSystempOptionConfig->m_bVisionOnLineStatus)
// 				{
// 					pOwner->SendCommConnectStatus();
// 				}
// 			}

			if (pOwner->m_XYDCommProtocol.IsPlcProtocolType())	// 如果是PLC直接读取存储寄存器协议
			{
				pOwner->m_XYDCommProtocol.m_bCmdAxisPosReady = FALSE;//已获取指令中的轴位置标志复位
				pOwner->m_XYDCommProtocol.EnableReadWriteAxisPosWithCmd(pOwner->IsReadWriteAxisPosWithCmd());
				// 如果是共享的通信接口，共享源的通信comm
				if (eShareCommSrc == pOwner->GetShareCommType())
				{
					if (pOwner->IsShareCommPlatformParalle()		// 如果是并行的共享通信
						|| (!pOwner->IsShareCommPlatformParalle() && pOwner->IsShareCommCurPlatform())) // 如果是非并行的共享通信，且是得到通信令牌
					{
						if (pOwner->ReOpenComm())
						{
							if (TRUE == optionInfo.m_bVisionOnLineStatus)
							{
								long tend = GetTickCount();

								if((tend-tStrat)>=3000)
								{
									KeyenceCodeType eCode;
									eCode = bHeart? eCodeNone:eCodeOK;
									pOwner->m_XYDCommProtocol.m_dstrTranlateResultInfo.clear();  //清空指令
									// 发送连接状态异常时，进行重连
									if(pOwner->SendCommConnectStatusEx(eCode))  
									{
										tStrat = GetTickCount();
										bHeart = !bHeart;
									}
									else
									{
										tStrat = GetTickCount();
										bHeart = !bHeart;
										pOwner->m_CommStatus = eCommUnConnect;
										Sleep(10);
										continue;
									}

								}
							}
						}
					}
				}
				else if (eShareCommNo == pOwner->GetShareCommType())
				{
					if (pOwner->ReOpenComm())
					{
						if (TRUE == optionInfo.m_bVisionOnLineStatus)
						{
							long tend = GetTickCount();

							if((tend-tStrat)>3000)
							{
								KeyenceCodeType eCode;
								eCode = bHeart? eCodeNone:eCodeOK;
								if(pOwner->SendCommConnectStatusEx(eCode))  
								{
									tStrat = GetTickCount();
									bHeart = !bHeart;
								}
								else
								{
									tStrat = GetTickCount();
									bHeart = !bHeart;
									pOwner->m_CommStatus = eCommUnConnect;
									Sleep(10);
									continue;
								}
							}
						}

						//折弯压合参数写入
						if (TRUE == optionInfo.m_bShowBend)	// 如果是PLC直接读取存储寄存器协议
						{
							if (cBendPressParaWriteTimer.GetTime(FALSE) > 15
								|| !bBendPressParaWriteStart)
							{
								pOwner->OnBendPressParaWrite();
								cBendPressParaWriteTimer.Reset();
								bBendPressParaWriteStart = TRUE;
							}
						}
					}
				}

			}
			else
			{
				// 暂不支持无协议
				Sleep(10);
				continue;
			}

			//if (pOwner->m_pAlignerTool == NULL)
			//{
			//	continue;
			//}
			

			BOOL bReceived = FALSE;
			

			//// 应用7：检查相机是否掉线并报警提示或重新连接
			//CString strPath =  "D:\\CameLog.txt";
			//CString strOut;
			//SysPlatformInfo sysPlatformInfoCame;
			//pOwner->GetSysPlatformInfo(sysPlatformInfoCame);
			//for (int it=0;it<pOwner->GetCameraNum();it++)
			//{    //m_pPlatformInfo->m_nPlatformIndex
			//	//strOut.Format("工位%d主线程检测相机在线位置%d",sysPlatformInfoCame.m_nPlatformIndex,it);
			//	//FALSE==pOwner->IsCameraOnline(it)
			//	
			//	if(FALSE==pOwner->IsCameraOnlineStatus(it))
			//	{
			//		strOut.Format("工位%d主线程检测相机掉线位置%d",sysPlatformInfoCame.m_nPlatformIndex+1,it+1);
			//		pOwner->VisionAlignLogRecord(strPath, strOut);
			//	}
			//	//pOwner->VisionAlignLogRecord(strPath, _T("\n"), FALSE);
			//	
			//}

			// 查询通信接口收到的数据
			if (pOwner->m_XYDCommProtocol.IsPlcProtocolType())	// 如果是PLC直接读取存储寄存器协议
			{
// 				pOwner->m_XYDCommProtocol.m_dstrTranlateResultInfo.clear();  //清空指令
// 				bReceived = pOwner->ReceiveCommPlcReg(sysWorkCmdType);

				// 如果是共享的通信接口
				if (eShareCommNo != pOwner->GetShareCommType())
				{
					if (pOwner->IsShareCommPlatformParalle()
						 ||(!pOwner->IsShareCommPlatformParalle() && pOwner->IsShareCommCurPlatform()))	// 如果得到令牌
					{
						pOwner->m_XYDCommProtocol.m_dstrTranlateResultInfo.clear();  //清空指令

						if(pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
						{
							bReceived = pOwner->ReceiveCommPlcRegEx2(sysWorkCmdType);
						}
						else
						{
							bReceived = pOwner->ReceiveCommPlcRegEx(sysWorkCmdType);
							CString strCommlog = _T("");
							strCommlog.Format(_T(" bReceived:%d,sysWorkCmdType:%d"),bReceived,sysWorkCmdType);
							pOwner->VisionAlignLogRecord(_T("D:\\VisionASMLog\\commLog.txt"),strCommlog);

						}
						

						if (bReceived == FALSE)
						{
							//折弯压合参数写入
							if (TRUE == optionInfo.m_bShowBend)	// 如果是PLC直接读取存储寄存器协议
							{
								if (cBendPressParaWriteTimer.GetTime(FALSE) > 15
									|| !bBendPressParaWriteStart)
								{
									pOwner->OnBendPressParaWrite();
									cBendPressParaWriteTimer.Reset();
									bBendPressParaWriteStart = TRUE;
								}
							}

							//// 如果未收到触发指令，则交出令牌给下一个平台
							if (!pOwner->IsShareCommPlatformParalle())
							{
								pOwner->ShareCommNextPlatform();
							}
							
							Sleep(1);
							continue;
						}
						else
						{
							// 如果收到触发指令，则根据指令进行处理
						}
					}				
					else // 如果未得到令牌
					{
						Sleep(1);
						continue;
					}
				}
				else
				{					
					// 如果不是共享的通信接口
					pOwner->m_XYDCommProtocol.m_dstrTranlateResultInfo.clear();  //清空指令
					if(pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
					{
						bReceived = pOwner->ReceiveCommPlcRegEx2(sysWorkCmdType);
					}
					else
					{
						bReceived = pOwner->ReceiveCommPlcRegEx(sysWorkCmdType);
						//CString strCommlog = _T("");
						//strCommlog.Format(_T(" bReceived:%d,sysWorkCmdType:%d"),bReceived,sysWorkCmdType);
						//pOwner->VisionAlignLogRecord(_T("D:\\VisionASMLog\\commLog.txt"),strCommlog);
					}
					
				}
			}

			if (pOwner->m_nManualSearchMarkMode != eNoSearch || pOwner->m_bExecuteProductManage == TRUE || pOwner->m_bIsSetShutterInMain)
			{
				Sleep(2);
				continue;
			}





			if (bReceived)
			{
				pOwner->m_bCurThreadWorkFinished = FALSE;

				pOwner->m_bFaalBendProcess = FALSE; //默认不是折弯检测对位流程

				if (pOwner->m_XYDCommProtocol.IsPlcProtocolType())
				{
				}
				else
				{
					break;
					Sleep(2);
					//sysWorkCmdType = pOwner->m_XYDCommProtocol.AnalyzeSysWorkCmdReceived(strReceive);
				}

				SysPlatformInfo sysPlatformInfo;
				pOwner->GetSysPlatformInfo(sysPlatformInfo);

				if (pOwner->m_bIsAutoCalibrating && (sysWorkCmdType != eCmdCalibrateFCBP))
				{
					pOwner->ShowAllGuiCalibrateGraphics(FALSE);
					pOwner->m_bIsAutoCalibrating = FALSE;
				}
				
				//
				//pOwner->stGlobalSearchTimer.Reset();
				//stGlobalSearchTimer.Reset();
				nParam.clear();
				dParam.clear();
				strInfo.Empty();
				switch (sysWorkCmdType)
				{
				case eCmdObjectSearchFPMI:     //  FSPC：产品切换的设定，将产品切换至指定的产品No.
					{
						//
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FPMI");
						int nIndex = eKeyenceCmdFPMI-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						int nOffsetIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
						nParam.push_back(nOffsetIndex);
						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FPMIParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FPMIParam.m_strCmd, eCmdObjectSearchFPMI);
						//pOwner->OnlineAlignExFACO_WithSnap_Ex(nOffsetIndex);
						pOwner->OnlineAlnTargetSearchCandidateExFPMI_Ex(nOffsetIndex);
						

					}
					break;
				case eCmdACFInspectFBIS:     //  FSPC：产品切换的设定，将产品切换至指定的产品No.
					{
						//
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FBIS");
						int nIndex = eKeyenceCmdFBIS-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}


						// 多段
						if(pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
						{
							// 当多段地址触发时，不同地址对应相同
							pOwner->m_nCurFBIS_Index = pOwner->m_XYDCommProtocol.m_nCurRegTriggerIndex;
						}

						if(pOwner->m_nCurFBIS_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFBIS_Index = 0;
						}

						int nPosIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFBIS_Index];
						int nStartPatIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFBIS_Index];
						int nObjorTrateIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[pOwner->m_nCurFBIS_Index];

						nParam.push_back(nPosIndex);
						nParam.push_back(nStartPatIndex);
						nParam.push_back(nObjorTrateIndex);

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FBISParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FBISParam.m_strCmd, eCmdACFInspectFBIS);

						//BOOL bSearchSuss = pOwner->OnlineACFInspectCandidateExFCHB_Ex(nPosIndex-1, nStartPatIndex - 1);
						BOOL bSearchSuss = pOwner->OnlineACFInspectCandidateExFBIS_Ex(nPosIndex-1, nStartPatIndex - 1,nObjorTrateIndex);
						if(bSearchSuss)
						{
							if(!pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
							{
								pOwner->m_nCurFBIS_Index= pOwner->m_nCurFBIS_Index+1;
							}
						}

						pOwner->m_bSearchWithInspect = FALSE;

					}
					break;
				case eCmdSetProductIndexFSPC:     //  FSPC：产品切换的设定，将产品切换至指定的产品No.
					{
						int nIndex = eKeyenceCmdFSPC-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						// 大品种索引，从1递增
						int nCurProductIndexFa = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
						// 小品种索引，从1递增
						int nCurProductIndexChd = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[0];

						// nParam.push_back(pOwner->m_XYDCommProtocol.m_FSPCParam.P1);
						// nParam.push_back(pOwner->m_XYDCommProtocol.m_FSPCParam.P2);
						nParam.push_back(nCurProductIndexFa);
						if(sysPlatformInfo.m_bProductFaChd)//启用小品种才显示小品种
						nParam.push_back(nCurProductIndexChd);
						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FSPCParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FSPCParam.m_strCmd, eCmdSetProductIndexFSPC);
						int nCurProductIndex = 0; // 全部品种索引
						if(sysPlatformInfo.m_bProductFaChd)
						{
							nCurProductIndex = pOwner->m_pVisionASMConfig->m_TotalProductData.GetProductIndex(nCurProductIndexFa, nCurProductIndexChd);
						}
						else
						{
							// nCurProductIndex = pOwner->m_XYDCommProtocol.m_FSPCParam.P1 - 1;
							nCurProductIndex = nCurProductIndexFa - 1;
						}
						pOwner->OnlineSetProductIndexEx(nCurProductIndex);
					}
					break;	
				case eCmdCalibrateFCLB:     // 1、FCLB:Calibration 的执行 执行Calibration
					{				   
						int nIndex = eKeyenceCmdCalibrate-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}

						// 多段
						if(pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
						{
							// 当多段地址触发时，不同地址对应相同
							pOwner->m_nCurFCLB_Code_Index = pOwner->m_XYDCommProtocol.m_nCurRegTriggerIndex;
						}

						if(pOwner->m_nCurFCLB_Code_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFCLB_Code_Index = 0;
						}

						// 由于D51为驱动到位信号; 暂时不用P1;
						int nMarkType = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFCLB_Code_Index];
						//int nMarkType = 1;
						int nP2 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFCLB_Code_Index];

						int nExProductIndex = 0;
						if(pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE && ((nP2-1)<(pOwner->m_pPlatformInfo->m_nMultiCalibExtensionMaxNum+1)))
						{
							nExProductIndex = nP2-1;
							nParam.push_back(nP2);
						}
						
						nParam.push_back(nMarkType);
						
						pOwner->m_bCmdParamAxisPosEanble = FALSE;

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FCLBParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FCLBParam.m_strCmd, eCmdCalibrateFCLB);

						//pOwner->OnlineCalibrateFCLB_Ex(nMarkType); // 无需PLC多次发送标定指令
						pOwner->OnlineCalibrateFCLB_Ex2(nMarkType,nExProductIndex); // PLC多次发送标定指令
						pOwner->m_bCmdParamAxisPosEanble = FALSE;
					}
					break;	
				case eCmdTargetSearchFTGC:  // 执行实时目标搜索: 目标候补搜索 FTGC
					{
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FTGC");
						int nIndex = eKeyenceCmdFTGC-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						pOwner->m_bTargetSearchTest = TRUE;
						if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;

						if(pOwner->m_nCurFTGC_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFTGC_Index = 0;
						}

						int nPosIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFTGC_Index];
						int nStartPatIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFTGC_Index];
						int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[pOwner->m_nCurFTGC_Index];
						int nP4 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP4[pOwner->m_nCurFTGC_Index];
						int nP5 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP5[pOwner->m_nCurFTGC_Index];

						nParam.push_back(nPosIndex);
						nParam.push_back(nStartPatIndex);
						
						if(pOwner->m_XYDCommProtocol.IsPlcProtocolType())
						{
							//1. 寄存器通信时，每个参数都需要占一个寄存器参数位置，即使该参数不用
							//2. PLC设置参数时，在需要的参数位置设置值即可，不要的参数位置默认就行
							CPlatformOptionInfo optionInfo;
							pOwner->GetPlatformOptionInfo(optionInfo);
							nParam.push_back(nP3);
							//解析是否检测或者多目标对位序号参数
							nParam.push_back(pOwner->m_XYDCommProtocol.m_FTGCParam.nP3);
							if (optionInfo.m_bEnableMutiTarget)
							{		
								pOwner->m_nMutiTargetCurIndex=nP3-1;
							}
							else//是否检测//这两者不兼容 //占位
							{
								CPlatformOptionInfo optionInfo;
								pOwner->GetPlatformOptionInfo(optionInfo);
								pOwner->m_bSearchWithInspect = nP3 && optionInfo.m_bSearchWithDistanceInspect;
							}

							//解析扩展产品序号参数

							if ((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE) && (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1))
							{	
								nParam.push_back(nP4);
								if (optionInfo.m_bTargetSearchExProduct)
								{
									if (nP4-1>=0 && nP4-1 <= pOwner->m_pPlatformInfo->m_nMultiCalibExtensionMaxNum + 1)
									{					
										// 启用扩展且参数包含扩展序号	
										pOwner->SetAlignExProductIndex(nP4-1);
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;

									}
									else if (nP4 -1>=99 && nP4-1 <= 103)
									{
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = TRUE;
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_nCombinationCode = nP4 -1;
									}
									else //参数异常默认参数
									{
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
										pOwner->SetAlignExProductIndex(0);
									}
								}
								else
								{
									if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
									pOwner->SetAlignExProductIndex(0);
								}
							}

							//解析是否对位参数							
							if (optionInfo.m_bEnableAlignAfterSearch)
							{
								nParam.push_back(nP5);
								if (nP5 > 0 && nP5 < 100)
								{
									pOwner->m_nOffsetIndex=nP5-1;
									pOwner->m_nAlignType = 1;
								}
								else if (nP5 > 100)
								{
									pOwner->m_nOffsetIndex=nP5-101;
									pOwner->m_nAlignType = 2;
								}
								else
								{
									pOwner->m_nAlignType = -1;
								}
							}
						}
						if(pOwner->m_nMutiTargetCurIndex < 0 || pOwner->m_nMutiTargetCurIndex >= MUTITARGETMAXNUM)
						{//TT201029 防止多目标序号溢出
							pOwner->m_nMutiTargetCurIndex = 0;
						}
						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FTGCParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FTGCParam.m_strCmd, eCmdTargetSearchFTGC);
						
						BOOL bSearchSuss = pOwner->OnlineAlnTargetSearchCandidateExFTGC_Ex(nPosIndex-1, nStartPatIndex-1);
						if(bSearchSuss)
						{
							pOwner->m_nCurFTGC_Index= pOwner->m_nCurFTGC_Index+1;
						}
						pOwner->m_bCmdParamAxisPosEanble = FALSE;

						// 设置手动对象搜索序号
						pOwner->m_nManualTargetSearchIndex = nPosIndex;
						pOwner->m_bTargetSearchTest = FALSE;
						pOwner->m_nAlignType = -1;
					}
					break;
				case eCmdObjectSearchFOBC:  // 执行实时对象搜索: OBG （仅测试用） // FOBC
					{
						//
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FOBC");

						int nIndex = eKeyenceCmdFOBC-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						pOwner->m_bObjectSearchTest = TRUE;

						if(pOwner->m_nCurFOBC_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFOBC_Index = 0;
						}
						int nPosIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFOBC_Index];
						int nStartPatIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFOBC_Index];

						int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[pOwner->m_nCurFOBC_Index];
						int nP4 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP4[pOwner->m_nCurFOBC_Index];
						int nP5 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP5[pOwner->m_nCurFOBC_Index];

						nParam.push_back(nPosIndex);
						nParam.push_back(nStartPatIndex);

						if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
						if(pOwner->m_XYDCommProtocol.IsPlcProtocolType())
						{
							//1. 寄存器通信时，每个参数都需要占一个寄存器参数位置，即使该参数不用
							//2. PLC设置参数时，在需要的参数位置设置值即可，不要的参数位置默认就行

							CPlatformOptionInfo optionInfo;
							pOwner->GetPlatformOptionInfo(optionInfo);
							nParam.push_back(nP3);
							if (optionInfo.m_bEnableMutiTarget)
							{		
								pOwner->m_nMutiTargetCurIndex=nP3-1;											
							}
							else
							{
								pOwner->m_bSearchWithInspect = nP3 && optionInfo.m_bSearchWithDistanceInspect;
							}

							//解析扩展产品序号参数;

							if ((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE) && (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1))
							{	
								nParam.push_back(nP4);
								if (optionInfo.m_bObjectSearchExProduct)
								{
									if (nP4-1>=0 &&  nP4-1 <= pOwner->m_pPlatformInfo->m_nMultiCalibExtensionMaxNum + 1)
									{					
										// 启用扩展且参数包含扩展序号				
										pOwner->SetAlignExProductIndex(nP4-1);
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
									}
									else if (nP4 -1>=99 && nP4-1 <= 103)
									{
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = TRUE;
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_nCombinationCode = nP4 -1;
									}
									else //参数异常默认参数
									{
										if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
										pOwner->SetAlignExProductIndex(0);
									}
								}
								else
								{
									if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
									pOwner->SetAlignExProductIndex(0);
								}
							}

							//解析是否对位参数
							if (optionInfo.m_bEnableAlignAfterSearch)
							{
								nParam.push_back(nP5);
								if (nP5 > 0 && nP5 < 100)
								{
									pOwner->m_nOffsetIndex=nP5-1;
									pOwner->m_nAlignType = 1;
								}
								else if (nP5 > 100)
								{
									pOwner->m_nOffsetIndex=nP5-101;
									pOwner->m_nAlignType = 2;
								}
								else
								{
									pOwner->m_nAlignType = -1;
								}
							}
						}

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FOBCParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FOBCParam.m_strCmd, eCmdObjectSearchFOBC);

						BOOL bSearchSuss= pOwner->OnlineAlnObjectSearchCandidateExFOBC_Ex(nPosIndex-1, nStartPatIndex - 1);

						if(bSearchSuss)
						{
							pOwner->m_nCurFOBC_Index= pOwner->m_nCurFOBC_Index+1;
						}

						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_bObjectSearchTest = FALSE;
						pOwner->m_bSearchWithInspect = FALSE;

						// 设置手动对象搜索序号
						pOwner->m_nManualObjectSearchIndex = nPosIndex;
						pOwner->m_nAlignType = -1;
					
					}
					break;
				case eCmdGetTargetObjectOffsetFACO:   // 执行在线对位: AAE    // 计算偏差量 Fast：FACO
					{	
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FACO");

						int nIndex = eKeyenceCmdFACO-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						int nOffsetIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
						int nP2 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[0];
						int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[0];
						nParam.push_back(nOffsetIndex);

						int nBenchOffsetIndex = -1;
						int nExProductIndex = 0;
						int nTarFixOffsetIndex = 0;

						BOOL bExProduct = FALSE;

						//多目标对位序号与保留参数共用一个参数，所以扩展产品与多目标对位不兼容
						if (optionInfo.m_bEnableMutiTarget)
						{
							nParam.push_back(nP2);
							pOwner->m_nMutiTargetCurIndex=nP2-1;
						}
						else if (((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension  == TRUE) && (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1)))
						{
							CPlatformOptionInfo optionInfo;
							pOwner->GetPlatformOptionInfo(optionInfo);
							if (optionInfo.m_bAlignExProduct)
							{
								nBenchOffsetIndex = nP2-1;
								nExProductIndex = nP3-1;
								nParam.push_back(nP2);
								nParam.push_back(nP3);
								bExProduct = TRUE;
							}
						}

						if (pOwner->m_pPlatformInfo->m_bTargetCalibUseBoard  == TRUE)
						{
							nTarFixOffsetIndex = nP3-1;
							if (!bExProduct)
							{
								nParam.push_back(nP3);
							}
						}

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FACOParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FACOParam.m_strCmd, eCmdGetTargetObjectOffsetFACO);

						//pOwner->OnlineAlignExFACO_Ex(nOffsetIndex, nBenchOffsetIndex, nExProductIndex,nTarFixOffsetIndex);
						pOwner->OnlineAlignExFACO_WithSnap_Ex(nOffsetIndex);
					}
					break;
				case eCmdAutoAlignFAAC:   // 执行在线对位: AAE	   // 自动连续对位 Fast:FAAC，对象候补搜索
					{	
                        int nIndex = eKeyenceCmdFAAC-1;
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FAAL");

						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}

						pOwner->m_bAutoAlign = TRUE;
						pOwner->m_bCmdParamAxisPosEanble = FALSE;

						int P1 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
                        int P2 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[0];
                        int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[0];
                        int nP4 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP4[0];
                        int nP5 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP5[0];
                        
                        int nOffsetIndex = 0;
						int nStartPatIndex = 0;
						int nBenchOffsetIndex = -1;
						int nExProductIndex = 0;
						int nTarFixOffsetIndex = 0;
						
                        nOffsetIndex		= P1 - 1;
						nStartPatIndex = P2 - 1;
						nParam.push_back(P1);	
						nParam.push_back(P2);
						BOOL bExProduct = FALSE;
						
						{
							//1. 寄存器通信时，每个参数都需要占一个寄存器参数位置，即使该参数不用
							//2. PLC设置参数时，在需要的参数位置设置值即可，不要的参数位置默认就行
							//多目标对位序号与其它功能共用参数，不兼容
							//解析多目标对位序号
							if (optionInfo.m_bEnableMutiTarget)
							{	
								nParam.push_back(nP3);
								pOwner->m_nMutiTargetCurIndex=nP3-1;											
							}
							else
							{

								nParam.push_back(nP3);
								nParam.push_back(nP4);
								// 扩展协议
								if ( ((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE) && (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1)))
								{
									CPlatformOptionInfo optionInfo;
									pOwner->GetPlatformOptionInfo(optionInfo);
									if (optionInfo.m_bAlignExProduct)
									{
										nBenchOffsetIndex = nP3-1;
										nExProductIndex = nP4-1;
										bExProduct = TRUE;
									}
								}

								/*扩展产品序号与目标固定补偿序号共用参数，若启用扩展产品，则目标固定补偿序号与扩展产品序号为同一值，否则
								该参数单独用于目标固定补偿*/
								if (pOwner->m_pPlatformInfo->m_bTargetCalibUseBoard  == TRUE)
								{
									nTarFixOffsetIndex = nP4-1;
									if (!bExProduct)
									{
										nParam.push_back(nP4);
									}

								}
							}


						}
						if(pOwner->m_nMutiTargetCurIndex < 0 || pOwner->m_nMutiTargetCurIndex >= MUTITARGETMAXNUM)
						{//TT201029 防止多目标序号溢出
							pOwner->m_nMutiTargetCurIndex = 0;
						}
						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FAACParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FAACParam.m_strCmd, eCmdAutoAlignFAAC);
						pOwner->OnlineAlignExFAAC_Ex(nOffsetIndex, nStartPatIndex, nBenchOffsetIndex, nExProductIndex,nTarFixOffsetIndex);

						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_bAutoAlign = FALSE;

						// 设置手动对象搜索序号
						if (pOwner->m_pPlatformInfo != NULL)
						{
							if (pOwner->m_pPlatformInfo->m_bTargetObjectCamSeparate == TRUE)
							{
								pOwner->m_nManualObjectSearchIndex = 11;
							}
							else
							{
								pOwner->m_nManualObjectSearchIndex = 4;
							}
						}
					}
					break;	
				case eCmdACFInspectFCHB:  // ACF检测 FCHB
					{	
						int nIndex = eKeyenceCmdFCHB-1;
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FCHB");

						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}

						// 多段
						if(pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
						{
							// 当多段地址触发时，不同地址对应相同
							pOwner->m_nCurFCHB_Index = pOwner->m_XYDCommProtocol.m_nCurRegTriggerIndex;
						}

						if(pOwner->m_nCurFCHB_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFCHB_Index = 0;
						}

						int nPosIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFCHB_Index];
						int nStartPatIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFCHB_Index];

						nParam.push_back(nPosIndex);
						nParam.push_back(nStartPatIndex);

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FCHBParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FCHBParam.m_strCmd, eCmdACFInspectFCHB);
						
						BOOL bSearchSuss = pOwner->OnlineACFInspectCandidateExFCHB_Ex(nPosIndex-1, nStartPatIndex - 1);
						if(bSearchSuss)
						{
							if(!pOwner->m_pPlatformInfo->m_bEnableMulRegTrigger)
							{
								pOwner->m_nCurFCHB_Index= pOwner->m_nCurFCHB_Index+1;
							}
							
						}

						pOwner->m_bSearchWithInspect = FALSE;
					}
					break;
				case eCmdOnceAlignMovePickFALP:   // 执行在线对位: AAE	   // 自动连续对位 Fast:FALP
					{				
						//pOwner->OnlineAlign();
						int nIndex = eKeyenceCmdFALP-1;
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FALP");

						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						pOwner->m_bAutoAlign = TRUE;
						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_nPickMutiExMode = 0;

						int nOffsetIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
						int nP2= pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[0];
						nParam.push_back(nOffsetIndex);
						nParam.push_back(nP2);

						CPlatformOptionInfo optionInfo;
						pOwner->GetPlatformOptionInfo(optionInfo);

						if ((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE)&& (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1))
						{
							if (optionInfo.m_bAlignExProduct)
							{
								if (nP2-1>=0 && nP2-1 <= pOwner->m_pPlatformInfo->m_nMultiCalibExtensionMaxNum + 1)
								{					
									// 启用扩展且参数包含扩展序号
									pOwner->SetAlignExProductIndex(nP2-1);
									if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
								}
								else //参数异常默认参数
								{
									if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
									pOwner->SetAlignExProductIndex(0);
								}
								if(nP2 >= 100)
								{//P2大于100，多吸头应用多扩展模式
									pOwner->m_nPickMutiExMode = nP2;
								}
							}
							else
							{
								if (NULL != pOwner->m_pAlignerTool) pOwner->m_pAlignerTool->m_bEnableMultiCombinationCode = FALSE;
								pOwner->SetAlignExProductIndex(0);
							}
						}

						if(pOwner->m_nPickMutiExMode == 100 ||pOwner->m_nPickMutiExMode == 200)
						{
							pOwner->m_nPickMutiExMode += sysPlatformInfo.m_nMultiCalibExtensionMaxNum + 1;
						}

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FALPParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FALPParam.m_strCmd, eCmdOnceAlignMovePickFALP);

						int nCamNum = 0;
						if (pOwner->m_pPlatformInfo != NULL)
						{
							nCamNum = pOwner->m_pPlatformInfo->m_nCamNum;
						}

						// 一次发送多个对位结果(适用条件：单相机、多标记点模式（一个定位工具返回两个点图像坐标）、“选项”中“一次发送多个对位结果”)
						if ( (optionInfo.m_bSendMulAlignResult || pOwner->m_nPickMutiExMode) && (nCamNum == 1) )
						{//TT 20200831 支持一次计算所有扩展的结果
							pOwner->OnlineMulAlignPickExFALP_Ex(nOffsetIndex);
						}
						else
						{
							pOwner->OnlineAlignExFALP_Ex(nOffsetIndex);
						}

						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_bAutoAlign = FALSE;

						// 设置手动对象搜索序号
						if (pOwner->m_pPlatformInfo != NULL)
						{
							if (pOwner->m_pPlatformInfo->m_bTargetObjectCamSeparate == TRUE)
							{
								pOwner->m_nManualObjectSearchIndex = 11;
							}
							else
							{
								pOwner->m_nManualObjectSearchIndex = 4;
							}
						}
					}
					break;
				case eCmdGetTargetObjectOffsetFMCO:
					{
						int nIndex = eKeyenceCmdFMCO-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}

						int nOffsetCount = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0];
						nParam.push_back(nOffsetCount);
						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FMCOParam.m_strCmd,nParam,dParam);

						// 对指令参数进行限制 处理负数指令参数
						if (nOffsetCount<1)
						{
							break;
						}

						pOwner->OnlineAlignExFMCO_Ex(nOffsetCount);	

					}
					break;
				case eCmdCalibrateFCBP:     // 自动关联指令
					{

						int nIndex = eKeyenceCmdFCBP-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						
						if(pOwner->m_nCurFCBP_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFCBP_Index = 0;
						}

						int nType = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFCBP_Index];  // 类型
						int nCalibIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[pOwner->m_nCurFCBP_Index]; // 标定第n个靶标
						int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[pOwner->m_nCurFCBP_Index];
						int nP4 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP4[pOwner->m_nCurFCBP_Index];
						int nP5 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP5[pOwner->m_nCurFCBP_Index];

						pOwner->m_nCalibTargetMovePosIndex = nP3;//移动位置序号

						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_bIsAutoCalibrating = TRUE;
						int nExProductIndex = 0;

						nParam.push_back(nType);
						nParam.push_back(nCalibIndex);
						nParam.push_back(pOwner->m_nCalibTargetMovePosIndex);

						if(pOwner->m_XYDCommProtocol.IsPlcProtocolType())
						{
							if((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE) && (pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1)) 
							{
								CPlatformOptionInfo optionInfo;
								pOwner->GetPlatformOptionInfo(optionInfo);
								if (optionInfo.m_bAlignExProduct)
								{
									nExProductIndex = nP4-1;
								}
								nParam.push_back(nP4);
							}
						}
						if(nExProductIndex<0 || nExProductIndex>pOwner->m_pPlatformInfo->m_nMultiCalibExtensionMaxNum)
						{
							nExProductIndex = 0;
						}

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FCBPParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FCBPParam.m_strCmd, eCmdCalibrateFCBP);
						
						BOOL bSearchSuss= pOwner->OnlineFCBPCalibrate_Ex(nCalibIndex-1, nType,nExProductIndex);

						if(bSearchSuss)
						{
							pOwner->m_nCurFCBP_Index= pOwner->m_nCurFCBP_Index +1;
						}
						pOwner->m_bCmdParamAxisPosEanble = FALSE;

					}
					break;
				case eCmdCalibrateFCBS:     // 1、FCBS:Calibration 的执行 执行Calibration
					{

						int nIndex = eKeyenceCmdFCBS-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}

						if(pOwner->m_nCurFCBS_Index>=pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size())
						{
							pOwner->m_nCurFCBS_Index = 0;
						}

						int nMarkType = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[pOwner->m_nCurFCBS_Index];  // 类型

						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						nParam.push_back(nMarkType);

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FCBSParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FCBSParam.m_strCmd, eCmdCalibrateFCBS);


						BOOL bSearchSuss = pOwner->OnlineCalibrateFCBS_Ex(nMarkType);
						if(bSearchSuss)
						{
							if(nMarkType==0)
							{
								if(pOwner->m_nCurFCBS_XY_Index==pOwner->m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
								{
									pOwner->m_nCurFCBS_Index= pOwner->m_nCurFCBS_Index +1;
									pOwner->m_nCurFCBS_XY_Index =0;
								}
							}
							else if(nMarkType==1)
							{
								if(pOwner->m_nCurFCBS_Angle_Index==pOwner->m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
								{
									pOwner->m_nCurFCBS_Index= pOwner->m_nCurFCBS_Index +1;
									pOwner->m_nCurFCBS_Angle_Index =0;
								}
							}
							else if(nMarkType==2)
							{
								pOwner->m_nCurFCBS_Index= pOwner->m_nCurFCBS_Index +1;
							}
						}
						pOwner->m_bCmdParamAxisPosEanble = FALSE;
					}
					break;
				case eCmdAutoAlignFAAL:   // 执行在线对位: AAE	   // 自动连续对位 Fast:FAAL
					{	
						pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "触发指令:FAAL");

						pOwner->m_bAutoAlign = TRUE;
						pOwner->m_bCmdParamAxisPosEanble = FALSE;

						int nOffsetIndex = 0;
						int nReservedIndex = 10;
						int nBenchOffsetIndex = -1;
						int nExProductIndex = 0;
						int nTarFixOffsetIndex = 0;
						
						int nIndex = eKeyenceCmdFAAL-1;
						if(nIndex>pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.size() || nIndex<0)
						{
							break;
						}
						nOffsetIndex = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1[0]-1;
						int P2 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P2[0];
						int nP3 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP3[0];
						int nP4 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP4[0];
						int nP5 = pOwner->m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).nP5[0];

						nParam.push_back(nOffsetIndex);
						
						BOOL bExProduct = FALSE;
						
						//1. 寄存器通信时，每个参数都需要占一个寄存器参数位置，即使该参数不用
						//2. PLC设置参数时，在需要的参数位置设置值即可，不要的参数位置默认就行
						if (optionInfo.m_bEnableMutiTarget)
						{
							nParam.push_back(nP3);
							pOwner->m_nMutiTargetCurIndex=nP3-1;											
						}
						//解析扩展产品序号、下料补偿序号和目标固定补偿序号等
						else
						{
							nParam.push_back(nP3);
							nParam.push_back(nP4);
							nParam.push_back(nP5);
							// 扩展协议
							if (((pOwner->m_pPlatformInfo->m_bEnableMultiCalibExtension == TRUE) && 
								(pOwner->m_pVisionASMConfig->m_CommParamInfo.m_eExProtocolType == eExProReserved1)) 
									)
							{
								CPlatformOptionInfo optionInfo;
								pOwner->GetPlatformOptionInfo(optionInfo);
								if (optionInfo.m_bAlignExProduct)
								{
									nReservedIndex		= nP3;
									nBenchOffsetIndex	= nP4 - 1;
									nExProductIndex		= nP5 - 1;
									bExProduct = TRUE;
								}
							}

							/*扩展产品序号与目标固定补偿序号共用参数，若启用扩展产品，则目标固定补偿序号与扩展产品序号为同一值，否则
							该参数单独用于目标固定补偿*/
							if ( (pOwner->m_pPlatformInfo->m_bTargetCalibUseBoard  == TRUE) && (pOwner->m_pPlatformInfo->m_bTargetObjectCamSeparate  == TRUE))
							{
								nTarFixOffsetIndex = nP5-1;
								if (!bExProduct)
								{
									nParam.push_back(nP5);
								}
							}
						}
						

						pOwner->PackAndAddCommSimpleInfo(strInfo,pOwner->m_XYDCommProtocol.m_FAALParam.m_strCmd,nParam,dParam);
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_FAALParam.m_strCmd, eCmdAutoAlignFAAL);
						
						pOwner->OnlineAlignExFAAL_Base_Ex(nOffsetIndex, nReservedIndex, nBenchOffsetIndex, nExProductIndex,nTarFixOffsetIndex);

						
						pOwner->m_bCmdParamAxisPosEanble = FALSE;
						pOwner->m_bAutoAlign = FALSE;

						// 设置手动对象搜索序号
						if (pOwner->m_pPlatformInfo != NULL)
						{
							if (pOwner->m_pPlatformInfo->m_bTargetObjectCamSeparate == TRUE)
							{
								pOwner->m_nManualObjectSearchIndex = 11;
							}
							else
							{
								pOwner->m_nManualObjectSearchIndex = 4;
							}
						}
					}
					break;
				case eCmdUnknow:
					{
						pOwner->m_strStatusBarInfo1 = GetSysInfoString(pOwner->m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
						pOwner->m_strStatusBarInfo2 = GetSysInfoString(pOwner->m_psaSysInfoStrings, IDS_WARNING_COMMUNICATION_UNKNOWN);;
						pOwner->SetStatusBarInfo(FALSE, pOwner->m_strStatusBarInfo1, pOwner->m_strStatusBarInfo2);
						pOwner->PostMessageUpdateStatusBarInfo();
						pOwner->SetCommCommandName(pOwner->m_XYDCommProtocol.m_strCmdName, eCmdUnknow);
						CString strCmdName = _T("");
						pOwner->GetCommCommandName(strCmdName);
						pOwner->AddWarningInfo(pOwner->m_strStatusBarInfo1, (strCmdName + pOwner->m_strStatusBarInfo2));
					}
					break;

				default:
					break;
				}			
				pOwner->SaveImageIndexInfo();
				if (pOwner->m_XYDCommProtocol.IsPlcProtocolType())	// 如果是PLC直接读取存储寄存器协议
				{
					// 如果是共享的通信接口
					if (eShareCommNo != pOwner->GetShareCommType())
					{

						if (!pOwner->IsShareCommPlatformParalle() && pOwner->IsShareCommCurPlatform())	// 如果得到令牌
						{
							// 如果收到触发指令，根据指令进行处理完成后，交出令牌给下一个平台
							pOwner->ShareCommNextPlatform();
						}	
					}
				}				

				pOwner->VisionAlignLogRecord("D:\\TimeLog.txt", "指令处理完成\r\n\r\n");

			} // if
		}
        else
        {
			bReLoadPlatformOptionInfo = TRUE;
			pOwner->m_bNeedUpdateAddrTime = TRUE;
			pOwner->ResetAllIndex();
			for(int i = 0; i<pOwner->m_vvpMarkImagePos.size();i++)
			{
				for(int j = 0; j<pOwner->m_vvpMarkImagePos.at(i).size();j++)
				{
					if (pOwner->m_vvpMarkImagePos.at(i).at(j) != NULL)
					{
						delete pOwner->m_vvpMarkImagePos.at(i).at(j);
						pOwner->m_vvpMarkImagePos.at(i).at(j) = NULL;
					}
				}
				pOwner->m_vvpMarkImagePos.at(i).clear();
			}
             if (pOwner->m_XYDCommProtocol.IsPlcProtocolType() && FALSE == bWrtWorkStatus 
				 && TRUE == optionInfo.m_bVisionOnLineStatus)	// 如果是PLC直接读取存储寄存器协议
             {
                  ////设定软件挂起标识

				   //设定软件挂起标识
				  // 如果是共享的通信接口
				  if (eShareCommSrc == pOwner->GetShareCommType())
				  {
					  if (pOwner->IsShareCommPlatformParalle())
					  {
						  //设定软件挂起标识
							pOwner->SendCommConnectStatusEx(eCodeNG);
					  }
					  else
					  {
						  pOwner->m_ShareCommInfoArray.SetCurCommPlatformIndex(pOwner->GetSysPlatformIndex(), pOwner->GetSysPlatformIndex(), FALSE);
						  if (pOwner->IsShareCommCurPlatform())	// 如果得到令牌
						  {
							  //设定软件挂起标识
								pOwner->SendCommConnectStatusEx(eCodeNG);
						  }
						  pOwner->m_ShareCommInfoArray.SetCurCommPlatformIndex(pOwner->GetSysPlatformIndex(), pOwner->GetSysPlatformIndex(), TRUE);
					  }
				  }
				  else if (eShareCommNo == pOwner->GetShareCommType())
				  {
					  //设定软件挂起标识
						pOwner->SendCommConnectStatusEx(eCodeNG);
				  }
				  bWrtWorkStatus = TRUE;
             }
        }
		pOwner->m_bCurThreadWorkFinished = TRUE;
		Sleep(1);


	}// while
    if (pOwner->m_XYDCommProtocol.IsPlcProtocolType() && TRUE == optionInfo.m_bVisionOnLineStatus)	// 如果是PLC直接读取存储寄存器协议
    {
        ////设定软件退出标识
        //CString strSend;
        //pOwner->m_XYDCommProtocol.PackPlcWriteSetVisionStatus(strSend,eVsCloseSoft);
        //CommStatus nCommStatus = eCommSucceed;
        //CString strInfoStatus;
        //pOwner->CommunicateSendWriteRegStr(strSend, strInfoStatus, nCommStatus);

		//设定软件挂起标识
		// 如果是共享的通信接口
		if (eShareCommSrc == pOwner->GetShareCommType())
		{
			if (pOwner->IsShareCommPlatformParalle())
			{
				//设定软件退出标识
				pOwner->SendCommConnectStatusEx(eCodeNG);
			}
			else
			{
				pOwner->m_ShareCommInfoArray.SetCurCommPlatformIndex(pOwner->GetSysPlatformIndex(), pOwner->GetSysPlatformIndex(), FALSE);
				if (pOwner->IsShareCommCurPlatform())	// 如果得到令牌
				{
					//设定软件退出标识
					pOwner->SendCommConnectStatusEx(eCodeNG);
				}
				pOwner->m_ShareCommInfoArray.SetCurCommPlatformIndex(pOwner->GetSysPlatformIndex(), pOwner->GetSysPlatformIndex(), TRUE);
			}
		}
		else if (eShareCommNo == pOwner->GetShareCommType())
		{
			//设定软件退出标识
			pOwner->SendCommConnectStatusEx(eCodeNG);

		}
    }
	// 	CloseHandle(hEvent);
	// 	hEvent = NULL;
	pOwner->m_bExitWorkThreaded = TRUE;


	return 0;	
}

// 接受指令
BOOL vcXYDVisionAlign::ReceiveCommPlcRegEx(SysWorkCmdType& sysWorkCmdType)
{

	CString strTemp;

	LockShareComm(); //加锁共享通信
	ReceiveCommInfo(strTemp, TRUE); // TRUE清理通信数据
	UnlockShareComm(); //解锁共享通信

	//////////////////////////////////////////////////////////////////////////
	// 读取寄存器
	CString strSend;

	// 打包读取寄存器指令
	if (FALSE == m_XYDCommProtocol.PackPlcReadSysWorkCmdEx(strSend))
	{
		return FALSE;
	}

	CString ss;

	// 获取时间	
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	CString strTime;	
	CString strSendLog;
	CString strRecLog;
	strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
	strSendLog = strTime + strSend;

	// 发送读取命令
	LockShareComm(); //加锁共享通信
	if (FALSE == SendCommInfo(strSend, FALSE))	
	{
		UnlockShareComm(); //解锁共享通信
		return FALSE;
	}

	// 接收PLC回复
	BOOL bReceived;
	CString strInfo;
	CommStatus nCommStatus = eCommSucceed;
	bReceived = ReceiveCommRepeat(strInfo, nCommStatus, FALSE);
	UnlockShareComm(); //解锁共享通信

	if (bReceived == FALSE || nCommStatus != eCommSucceed)
	{
		CString strSendLog;
		CString tmpstr;
		int iplat= GetSysPlatformIndex();		
		tmpstr.Format(_T("     bReceived = %d  nCommStatus =%d     平台%d  m_bStopNow=%d"),bReceived,nCommStatus,iplat,m_bStopNow);
		strSendLog =  strSend+_T("-------ReceiveCommPlcRegEx    ")+tmpstr;
		VisionAlignLogRecord(_T("D:\\VisionASMLog\\commErrorLog.txt"),strSendLog);
		return FALSE;
	}

	GetLocalTime(&tm);
	strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
	strRecLog = strTime + strInfo;

	//////////////////////////////////////////////////////////////////////////
	// 基恩士串口PCLink通信：在PLC断电重启后，要先发送一个通信开始的指令，否则无法进行通信

	if (eProRegKeyenceSerialPCLink == m_XYDCommProtocol.GetProtocolType())
	{
		if (strInfo == "E1")
		{
			CString strSend = _T("CR");
			LockShareComm(); //加锁共享通信
			if (FALSE == SendCommInfo(strSend, FALSE))	
			{
				UnlockShareComm(); //解锁共享通信
				return FALSE;
			}

			AddCommCommandInfo(TRUE, strSend, TRUE,TRUE);

			CString strRecieve;
			bReceived = ReceiveCommRepeat(strRecieve,nCommStatus,FALSE);
			UnlockShareComm(); //解锁共享通信
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			AddCommCommandInfo(FALSE, strRecieve, TRUE,TRUE);

			if (strInfo == "CC")
			{
				return FALSE;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	// 解析收到的响应
	BOOL bTriggered = FALSE;
	CKeyenceCodeParam cCodeParam;
	BOOL bFixCode =  m_pPlatformInfo->m_bFixCode;
	if (bFixCode)
	{
		sysWorkCmdType = m_XYDCommProtocol.AnalyzePlcSysWorkCmdReceivedEx_FixCode(strInfo, bTriggered,cCodeParam);
		if(bTriggered && sysWorkCmdType!= eCmdUnknow)
		{
			// 启用实时读取寄存器指令码时，指令参数通过寄存器中值进行赋值
			if(m_XYDCommProtocol.m_nCurCodeIndex>=0 && 
				m_XYDCommProtocol.m_nCurCodeIndex<m_pVisionASMConfig->m_vKeyenceCodeParam.size() && 
				m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).m_bEnableNonFixedCode == TRUE)
			{
				for (int nSize =0;nSize<m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P1.size();nSize++ )
				{
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P1.at(nSize) = cCodeParam.P1.at(0);
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P2.at(nSize) = cCodeParam.P2.at(0);
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP3.at(nSize) = cCodeParam.nP3.at(0);
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP4.at(nSize) = cCodeParam.nP4.at(0);
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP5.at(nSize) = cCodeParam.nP5.at(0);
				}
			}
		}
	} 
	else
	{
		
		sysWorkCmdType = m_XYDCommProtocol.AnalyzePlcSysWorkCmdReceivedEx_CodeWithParam(strInfo, bTriggered,cCodeParam);

		if(bTriggered && sysWorkCmdType!= eCmdUnknow)
		{
			if(m_XYDCommProtocol.m_nCurCodeIndex>=0 && m_XYDCommProtocol.m_nCurCodeIndex<m_pVisionASMConfig->m_vKeyenceCodeParam.size())
			{
				m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P1 = cCodeParam.P1;
				m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P2 = cCodeParam.P2;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 判断指令是否触发
	if (bTriggered)
	{
		AddCommCommandInfo(TRUE, strSendLog, TRUE,TRUE);
		AddCommCommandInfo(FALSE, strRecLog, TRUE,TRUE);

		if (m_pVisionASMConfig)
		{
			if (m_pVisionASMConfig->m_DataRecord.m_bCommunication)
			{	
				CString strTemp;
				CString strOut =strTemp + _T("Receive:") + strInfo;

				strOut = _T("Send:") + strSendLog;
				VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strCommunicationData, strOut);

				strOut = _T("Rece:") + strRecLog;
				VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strCommunicationData, strOut);
			}
		}

		// 循环置0
			BOOL bClearSuss = FALSE;
			if (!ClearPlcPlatformTriggerEx(0))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2.Format(_T("置Plc指令触发寄存器为0失败！1段通讯 重试次数%d"),1);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				CString strCmd = _T("");
				CString strTempWarningType = _T("");
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

		
				
				CString tmpstr;
				int iplat= GetSysPlatformIndex();
				tmpstr.Format(_T("置Plc指令触发寄存器为0失败  1段通讯  平台=%d"),iplat);
				strSendLog = tmpstr;
				VisionAlignLogRecord(_T("D:\\VisionASMLog\\commErrorLog.txt"),strSendLog);
			}
			else
			{
				bClearSuss = TRUE;
			}
		
		return bClearSuss;
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}

BOOL vcXYDVisionAlign::ReceiveCommPlcRegEx2(SysWorkCmdType& sysWorkCmdType)
{

	CString strTemp;

	LockShareComm(); //加锁共享通信
	ReceiveCommInfo(strTemp, TRUE); // TRUE清理通信数据
	UnlockShareComm(); //解锁共享通信

	//////////////////////////////////////////////////////////////////////////
	// 读取寄存器
	CString strSend;
	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	//int nMulRegNum =sysPlatformInfo.m_mapMulRegTrigger.size();
	int nMulRegNum = optionInfo.m_vnMulRegTriggerSet.size();
	if(nMulRegNum<1)
	{
		nMulRegNum = 1;
	}

	BOOL bTriggered = FALSE;
	CString strInfo;
	CString strSendLog;
	CString strRecLog;

	int nCurTriggeredIndex = 0;

	for(int it=0;it<nMulRegNum;it++)
	{
		strSendLog = _T("");
		strSendLog = _T("");
		// 打包读取寄存器指令
		//if (FALSE == m_XYDCommProtocol.PackPlcReadSysWorkCmdEx2(strSend,RREG_KEYENCE_PLC_REG_NUMS*it)
		int nTmpRegOffset =RREG_KEYENCE_PLC_REG_NUMS*it;
		if(it>=0 && it< optionInfo.m_vnMulRegTriggerSet.size())
		{
			nTmpRegOffset = optionInfo.m_vnMulRegTriggerSet[it].m_nMulRegOffset;
		}
		
		if (FALSE == m_XYDCommProtocol.PackPlcReadSysWorkCmdEx2(strSend,nTmpRegOffset))
		{
			return FALSE;
		}

		CString ss;

		// 获取时间	
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CString strTime;	

		strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
		strSendLog = strTime + strSend;

		// 发送读取命令
		LockShareComm(); //加锁共享通信
		if (FALSE == SendCommInfo(strSend, FALSE))	
		{
			UnlockShareComm(); //解锁共享通信
			return FALSE;
		}

		// 接收PLC回复
		BOOL bReceived;
		
		CommStatus nCommStatus = eCommSucceed;
		bReceived = ReceiveCommRepeat(strInfo, nCommStatus, FALSE);
		UnlockShareComm(); //解锁共享通信
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{

			CString strSendLog;
			CString tmpstr;
			int iplat= GetSysPlatformIndex();
			tmpstr.Format(_T("     bReceived =%d  nCommStatus=%d     平台%d   ,m_bStopNow=%d"  ),bReceived ,nCommStatus ,iplat,m_bStopNow);
			strSendLog = strSend+_T("-------ReceiveCommPlcRegEx2    ")+tmpstr;
			VisionAlignLogRecord(_T("D:\\VisionASMLog\\commErrorLog.txt"),strSendLog);
			return FALSE;
		}

		GetLocalTime(&tm);
		strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
		strRecLog = strTime + strInfo;

		//////////////////////////////////////////////////////////////////////////
		// 基恩士串口PCLink通信：在PLC断电重启后，要先发送一个通信开始的指令，否则无法进行通信

		if (eProRegKeyenceSerialPCLink == m_XYDCommProtocol.GetProtocolType())
		{
			if (strInfo == "E1")
			{
				CString strSend = _T("CR");
				LockShareComm(); //加锁共享通信
				if (FALSE == SendCommInfo(strSend, FALSE))	
				{
					UnlockShareComm(); //解锁共享通信
					return FALSE;
				}

				AddCommCommandInfo(TRUE, strSend, TRUE,TRUE);

				CString strRecieve;
				bReceived = ReceiveCommRepeat(strRecieve,nCommStatus,FALSE);
				UnlockShareComm(); //解锁共享通信
				if (bReceived == FALSE || nCommStatus != eCommSucceed)
				{
					return FALSE;
				}

				AddCommCommandInfo(FALSE, strRecieve, TRUE,TRUE);

				if (strInfo == "CC")
				{
					return FALSE;
				}
			}
		}

		// 解析收到的响应
		
		CKeyenceCodeParam cCodeParam;
		BOOL bFixCode =  m_pPlatformInfo->m_bFixCode;
		if (bFixCode)
		{
			sysWorkCmdType = m_XYDCommProtocol.AnalyzePlcSysWorkCmdReceivedEx_FixCode2(strInfo, bTriggered,cCodeParam,it);
			if(bTriggered && sysWorkCmdType!= eCmdUnknow)
			{
				// 启用实时读取寄存器指令码时，指令参数通过寄存器中值进行赋值
				if(m_XYDCommProtocol.m_nCurCodeIndex>=0 && 
					m_XYDCommProtocol.m_nCurCodeIndex<m_pVisionASMConfig->m_vKeyenceCodeParam.size() && 
					m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).m_bEnableNonFixedCode == TRUE)
				{
					for (int nSize =0;nSize<m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P1.size();nSize++ )
					{
						m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P1.at(nSize) = cCodeParam.P1.at(0);
						m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).P2.at(nSize) = cCodeParam.P2.at(0);
						m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP3.at(nSize) = cCodeParam.nP3.at(0);
						m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP4.at(nSize) = cCodeParam.nP4.at(0);
						m_pVisionASMConfig->m_vKeyenceCodeParam.at(m_XYDCommProtocol.m_nCurCodeIndex).nP5.at(nSize) = cCodeParam.nP5.at(0);
					}
				}
				// 触发后 跳出
				nCurTriggeredIndex = it;
				break;
			}
		} 


	}





	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 判断指令是否触发
	if (bTriggered)
	{
		AddCommCommandInfo(TRUE, strSendLog, TRUE,TRUE);
		AddCommCommandInfo(FALSE, strRecLog, TRUE,TRUE);

		if (m_pVisionASMConfig)
		{
			if (m_pVisionASMConfig->m_DataRecord.m_bCommunication)
			{	
				CString strTemp;
				CString strOut =strTemp + _T("Receive:") + strInfo;

				strOut = _T("Send:") + strSendLog;
				VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strCommunicationData, strOut);

				strOut = _T("Rece:") + strRecLog;
				VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strCommunicationData, strOut);
			}
		}
		m_XYDCommProtocol.m_nCurRegTriggerIndex = nCurTriggeredIndex;

		int nTmpRegOffset =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;

		if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
		{
			nTmpRegOffset = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
		}


		BOOL bClearSuss = FALSE;
		

			if (!ClearPlcPlatformTriggerEx(0+nTmpRegOffset))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2.Format(_T("置Plc指令触发寄存器为0失败  多段通讯！重试次数%d"),1);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				CString strCmd = _T("");
				CString strTempWarningType = _T("");
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));


				SYSTEMTIME tm;
				GetLocalTime(&tm);
				CString strTime;
				CString tmpstr;
				int iplat= GetSysPlatformIndex();

				tmpstr.Format(_T("置Plc指令触发寄存器为0失败  多段通讯  平台%d"),iplat);
				strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
				strSendLog = strTime +tmpstr;
				VisionAlignLogRecord(_T("D:\\VisionASMLog\\commErrorLog.txt"),strSendLog);
			}
			else
			{
				bClearSuss = TRUE;

			}
		
		return bClearSuss;
		
		
		
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}

BOOL vcXYDVisionAlign::SendCommConnectStatusEx(KeyenceCodeType eCode)//限寄存器通信使用
{
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		// 打包写寄存器指令
		if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, eCode))
		{
			return FALSE;
		}
		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}

	}
	return TRUE;
}

BOOL vcXYDVisionAlign::ClearPlcPlatformTriggerEx(int nOffSet)
{
	//////////////////////////////////////////////////////////////////////////
	// 写PLC指令触发寄存器清零
	CString strSend;
	// 打包写寄存器指令
	if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,0,atoi(RREG_KEYENCE_PLC_TRIGGER) + nOffSet))
	{
		return FALSE;
	}

	// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
	BOOL bReceived;
	CString strInfo;
	CommStatus nCommStatus = eCommSucceed;
	bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
	if (bReceived == FALSE || nCommStatus != eCommSucceed)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineCalibrateFCLB_Ex(int nMarkType)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();

	CString strTempWarningType = _T("");
	CString strCmd = _T("VCLB:");
	GetCommCommandName(strCmd);


	BOOL bSucceed = FALSE;
	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if(sysPlatformInfo.m_bEnableKeyenceCode)
	{

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE)
		{
			bSucceed = FALSE;
		}

		// 暂时只能为XYD平台
		if(sysPlatformInfo.m_eMidPlatformType!= ePlatformXYD)
		{
			bSucceed = FALSE;
		}


		if(sysPlatformInfo.m_bTargetObjectCamSeparate==TRUE && sysPlatformInfo.m_bTarObjCamSepAutoCalibTar==FALSE)
		{
			for(int i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
		}
		else if(sysPlatformInfo.m_bTargetObjectCamSeparate==FALSE)
		{
			for(int i = 0; i < GetPosNum(); i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
		}
		else
		{
			bSucceed = FALSE;
		}

		// 拾取系统，XYD平台，相机固定安装
		if ( (m_pPlatformInfo->m_bPlatformPick) && (m_pPlatformInfo->m_ePickCamPlatformType == ePickCamPlatformSepFix) )
		{	
			// 情况1：标定验证功能
			if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
			{
				bSucceed = FALSE;
			}
			// 情况2：全局二维码标定
			else if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
			{
				bSucceed = FALSE;
			}
			// 情况3：传统标定+二维码标定
			else
			{
				bSucceed = MulPosCameraCalibrateFCLB(vnPosIndex, nMarkType);
			}
		}
		else
		{		
			{
				// 情况1：标定验证功能
				if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
				{
					bSucceed = FALSE;
				}
				// 情况2：全局二维码标定
				else if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
				{
					bSucceed = FALSE;
				}
				// 情况3：传统标定+二维码标定
				else
				{
					bSucceed = MulPosCameraCalibrateFCLB(vnPosIndex, nMarkType);
				}			
			}
		}
	}

	if (TRUE == bSucceed)
	{	
		CPlatformOptionInfo optionInfo;
		GetPlatformOptionInfo(optionInfo);
		if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
		{
			ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));// 向D4中写1

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1 = _T("标定轴移动拍照验证完成");
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
		}
		else
		{
			ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));// 向D4中写1

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
		}
	}
	else
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		if (!m_boIsSnapImgSucceed)
		{
			m_strStatusBarInfo2 = m_strSnapImgStatuString;
		}
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
	}

	// 平台为触发模式时，标定完成后恢复触发模式
	if (sysPlatformInfo.m_bCameraHardTriggerMode)
	{
		SetCameraTriggerMode(ePTHardwareTrigger);
	}

	return bSucceed;
}

BOOL vcXYDVisionAlign::MulPosCameraCalibrateFCLB(std::vector<int> vnPosIndex, int nMarkType)
{
	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = vnPosIndex.size();
	if(nPosNum<1 || nPosNum>MAXPOSNUM)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	for(int it=0;it<nPosNum;it++)
	{
		if(vnPosIndex.at(it)<0 || vnPosIndex.at(it)>sysPlatformInfo.m_nPositionNum)
		{
			ReportPLCFunc(eCodeNG);
			return FALSE;
		}
	}

	int nCalibPos = nPosNum;
	CString strTempWarningType = _T("");
	CString strCmd = _T("VCLB:");
	GetCommCommandName(strCmd);

	if (sysPlatformInfo.m_bEnableMultiCalibExtension)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	int nRemCalibPos = vnPosIndex[0];
	int nAllCalibPos = vnPosIndex.size();

	if(nMarkType !=1)
	{
		ReportPLCFunc(eCodeNG);
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_CALIBTYPE);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;
	}	

	int i=0;

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	{
		// 对位标定工具是否正常
		if (m_bValidCalibSearchTool!=TRUE)
		{		
			ReportPLCFunc(eCodeNG);	
			return FALSE;
		}

		// 标定开始
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_CALIBRATING);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		//标定开始
		int nCamPos = m_pPlatformInfo->m_nPositionNum;


		//////////////////////////////////////// 平台标定/////////////////////////////////////////////
		int nOffSetMul = 0;

		//先将PLC移动到位清零 腾盛PLC不写清零 会导致标定一开始就触发到位信号 出现P0和P1重合
		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		if (!ClearPlcPlatformTriggerEx(atoi(RREG_KEYENCE_PLC_MMVA)-atoi(RREG_KEYENCE_PLC_TRIGGER)+ nOffSetMul))
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("置Plc指令触发寄存器为0失败！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}



		std::vector<CPlatformAxisPos*> vpPlatformAxisPosAbsBase; 
		vpPlatformAxisPosAbsBase = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);
		// 1. 获取平台当前各轴的绝对位置（基准位置）,并设置入对位工具
		CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
		BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 

		//

		// 直接读取寄存器地址轴数据
		bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);
		
		if (FALSE == bCommuciateGetPlatformAxisAbsPos)
		{
			if (pPlatformAxisPos!=NULL)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;					
			}

			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		else
		{
			for (int k=0;k<nPosNum;k++)
			{
				SetCalibPlatformPos(pPlatformAxisPos);
			}

			if (m_CalibPlatformAxisAbsPos.size()<nRemCalibPos+1)
			{
				m_CalibPlatformAxisAbsPos.resize(nRemCalibPos+1);
			}
			m_CalibPlatformAxisAbsPos.at(nRemCalibPos)= *((CPlatformXYDAxisPos*)pPlatformAxisPos);

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_THE_PLATFORM_POS), pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;	

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

			if (pPlatformAxisPos)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;
			}
		}

		// 2. 获取平移标定和旋转标定时平台各轴的移动位置
		std::vector<CPlatformAxisPos*> vpPlatformAxisPos;   			
		m_pAlignerTool->SetCalibSendRelativePos(TRUE);
		vpPlatformAxisPos = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);
		m_pAlignerTool->SetCalibSendRelativePos(FALSE);

		if (vpPlatformAxisPos.size()!=m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
		{	

			ReportPLCFunc(eCodeNG);
			return OnFailedGetCalibPlatformAxisMovePos(strCmd);

		}
		else
		{
			// 记录平台标定时平台需移动的7个位置各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;

				CString strInfo;
				strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CALIB_SEVEN_PLATFORM_MOVE_POS);
				strInfo = strTemp + strInfo;

				CPlatformXYDAxisPos* pAxisPos;
				for (int i=0; i<vpPlatformAxisPos.size(); i++)
				{
					strTemp.Empty();
					pAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(i);           

					strTemp.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_CALIB_PLATFORM_POS), i, pAxisPos->m_dPosX, pAxisPos->m_dPosY, pAxisPos->m_dAngle);
					strInfo = strInfo + strTemp;
				}


				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			}
		}

		// 3. 驱动平台到位并定位
		CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
		std::vector<CMarkImagePos*> vpMarkImagePos;	   
		std::vector< std::vector<CMarkImagePos*> > vvpMarkImagePos;  // 存储所有位置图像坐标

		CMarkImagePos* pMarkImagePos = NULL;
		CMarkImagePos* pDisCorMarkImagePos = NULL;

		std::vector<CMarkImagePos*> vpTempMarkImagePos;
		std::vector<CMarkImagePos*> vpTempDisCorMarkImagePos;

		// 为每个位置各申请一个保存图像坐标的内存
		for (int i=0;i<nAllCalibPos;i++)
		{
			pMarkImagePos = new CMarkImagePos;
			pMarkImagePos->SetMarkImagePosNum(1);
			vpTempMarkImagePos.push_back(pMarkImagePos);

			pDisCorMarkImagePos = new CMarkImagePos;
			pDisCorMarkImagePos->SetMarkImagePosNum(1);
			vpTempDisCorMarkImagePos.push_back(pDisCorMarkImagePos);

			vvpMarkImagePos.push_back(vpMarkImagePos);
		}


		int nProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		CCalibData *pCalibData = m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nProductIndex);
		if (pCalibData == NULL || !pCalibData->IsValid())
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBDATA_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		CCalibrateParam *pCalibrateParam = pCalibData->m_vpCalibrateParam.at(nRemCalibPos);
		if (pCalibrateParam == NULL)
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBPARAM_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}


		CString strCalibPos;
		CString strTmp;
		for (int i =0 ; i< nCalibPos; i++)
		{
			strTmp.Format(_T("位置%d, "), nCalibPos+1);
			strCalibPos += strTmp;
		}

		int k=0;
		for (k=0; k<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType); k++)
		{ 

			CString strInfo;
			CString strTemp;

			strInfo.Format(_T("移动位置%d标定搜索开始"), k);
			strInfo = strCalibPos + strInfo;
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			//EnterCriticalSection(&m_csResource);
			m_bStatusBar = TRUE;
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_DRIVING_PLATFORM_MOVE), k);
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			//LeaveCriticalSection(&m_csResource);


			// 驱动到位
			pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(k);

			if(FALSE == CommuciateDrivePlatformAxisMoveEx(pPlatformXYDAxisPos,(CPlatformXYDAxisPos*)vpPlatformAxisPosAbsBase.at(k)))
			{	 
				ReportPLCFunc(eCodeNG);	
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < nAllCalibPos; i++)
				{
					for (int j = 0; j < vvpMarkImagePos[i].size(); j++)
					{
						if (vvpMarkImagePos[i][j])
						{
							delete vvpMarkImagePos[i][j];
							vvpMarkImagePos[i][j] = NULL;
						}
					}
				}

				return FALSE;//OnFailedCommuciateDrivePlatformAxisMove(strCmd, k);
			}


			Sleep(100);
			// 将相机移动到多个位置，逐个定位
			// 采集并定位
			// 等待搜索延时
			int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
			Sleep(m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nCurProdcutIndex)->m_vpCalibrateParam[nRemCalibPos]->m_nSearchDelayTime);

			// 准备开始定位			
			std::vector<SearchMode> vsmSearchMode;	

			for(int i = 0; i < nCalibPos; i++)
			{
				vsmSearchMode.push_back(eCalibSearch);
			}


			// 采集搜索图像
			if (!SnapSearchImage(vnPosIndex))
			{				
				ReportPLCFunc(eCodeNG);	
				// 释放保存内存
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < nAllCalibPos; i++)
				{
					for (int j = 0; j < vvpMarkImagePos[i].size(); j++)
					{
						if (vvpMarkImagePos[i][j])
						{
							delete vvpMarkImagePos[i][j];
							vvpMarkImagePos[i][j] = NULL;
						}
					}
				}
				return OnFailedSnapSearchImage(strCmd);
			}

			// 定位	

			for(int i = 0; i < nCalibPos; i++)
			{
				int nPosIndex = vnPosIndex[i];
				m_vnCalibSearchStartPatIndex[nPosIndex] = 0;
				m_vnCalibSearchEndPatIndex[nPosIndex] = 0;
			}

			{
				if (k==0)
				{
					m_bIsFirstCalibPos = TRUE;
				}
				else
				{
					m_bIsFirstCalibPos = FALSE;
				}
			}

			BOOL bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

			// 分析定位结果，发送指令响应
			if (FALSE == bSearchSucceed)
			{
				ReportPLCFunc(eCodeNG);
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < nAllCalibPos; i++)
				{
					for (int j = 0; j < vvpMarkImagePos[i].size(); j++)
					{
						if (vvpMarkImagePos[i][j])
						{
							delete vvpMarkImagePos[i][j];
							vvpMarkImagePos[i][j] = NULL;
						}
					}
				}
				return OnFailedCalibSearch(strCmd, k, nCalibPos);					
			}

			if(pCalibrateParam->m_nEnableDisCor)
			{
				for(int i = 0; i < nAllCalibPos; i++)
				{
					CCoordPos OutPutPoint;
					m_pAlignerTool->ExecuteDisCorPoint(vnPosIndex[i], m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]), OutPutPoint);
					vpTempDisCorMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), OutPutPoint);
				}
			}

			for(int i=0;i<nAllCalibPos;i++)
			{
				vpTempMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]));;
			}



			if(pCalibrateParam->m_nEnableDisCor)
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempDisCorMarkImagePos[i]);
					vvpMarkImagePos[i].push_back(markpos);
				}
			}
			else
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempMarkImagePos[i]);
					vvpMarkImagePos[i].push_back(markpos);
				}
			}

			// 记录图像搜索数据
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strOut;
				CCoordPos cp;
				strOut = strTemp + _T("CalibSearch: ");		

				for (int i=0; i< vpTempMarkImagePos.size(); i++)
				{
					strTemp = _T("");
					cp = vpTempMarkImagePos[i]->GetMarkImagePos(0);
					strTemp.Format(_T("CalibPos%d-MarkPos%d( %.2f , %.2f , %.3f )"), i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
					strOut += strTemp;
				}

				if(pCalibrateParam->m_nEnableDisCor)
				{
					for (int i=0; i< vpTempDisCorMarkImagePos.size(); i++)
					{
						strTemp = _T("");
						cp = vpTempDisCorMarkImagePos[i]->GetMarkImagePos(0);
						strTemp.Format(_T("CalibPos%d-DisCorMarkPos%d( %.2f , %.2f , %.3f )"),i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
						strOut += strTemp;
					}
				}

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			strInfo.Format(_T("位置%d移动位置%d标定搜索结束"),nCalibPos,k);
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);
		}  


		// 4. 将平台7个位置的所有图像搜索结果，设置入对位工具
		BOOL bOk=TRUE,bOk1=TRUE;

		{

			for (int i=0;i<nAllCalibPos;i++)
			{				
				bOk=ExecuteCalibrate(vvpMarkImagePos[i], vnPosIndex[i], nMarkType);
				if(!bOk)
				{		
					ReportPLCFunc(eCodeNG);		

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
					for (int i = 0; i < vpTempMarkImagePos.size(); i++)
					{
						if (vpTempMarkImagePos[i])
						{
							delete vpTempMarkImagePos[i];
							vpTempMarkImagePos[i] = NULL;
						}
					}

					for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
					{
						if (vpTempDisCorMarkImagePos[i])
						{
							delete vpTempDisCorMarkImagePos[i];
							vpTempDisCorMarkImagePos[i] = NULL;
						}
					}
					for (int i = 0; i < nAllCalibPos; i++)
					{
						for (int j = 0; j < vvpMarkImagePos[i].size(); j++)
						{
							if (vvpMarkImagePos[i][j])
							{
								delete vvpMarkImagePos[i][j];
								vvpMarkImagePos[i][j] = NULL;
							}
						}
					}

							
					return FALSE;
				}
				else
				{
					std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
					vpAllCalibratedInfo = GetAllCalibratedInfo();
					/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
					SetCurCalibratedInfo(vpAllCalibratedInfo);	

					// 保存标定后信息到当前产品中及配置文件中
					SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);
					
				}
			}

		}



		// 5. 释放内存
		for (int i = 0; i < vpTempMarkImagePos.size(); i++)
		{
			if (vpTempMarkImagePos[i])
			{
				delete vpTempMarkImagePos[i];
				vpTempMarkImagePos[i] = NULL;
			}
		}

		for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
		{
			if (vpTempDisCorMarkImagePos[i])
			{
				delete vpTempDisCorMarkImagePos[i];
				vpTempDisCorMarkImagePos[i] = NULL;
			}
		}
		for (int i = 0; i < nAllCalibPos; i++)
		{
			for (int j = 0; j < vvpMarkImagePos[i].size(); j++)
			{
				if (vvpMarkImagePos[i][j])
				{
					delete vvpMarkImagePos[i][j];
					vvpMarkImagePos[i][j] = NULL;
				}
			}
		}

		std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
		vpAllCalibratedInfo = GetAllCalibratedInfo();

		{
			/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
			SetCurCalibratedInfo(vpAllCalibratedInfo);	

			// 保存标定后信息到当前产品中及配置文件中
			SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);
		}

		return TRUE;
	}
}

BOOL vcXYDVisionAlign::ReportPLCFunc(int nResult,int nOffset)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		//BOOL bResult = (nResult==1)?TRUE:FALSE;
		CString strSimpleInfo;
		m_XYDCommProtocol.PackRePLCBaseFunc(strSimpleInfo, nResult);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);
		int nParam = nResult;
		
		int nOffSetMul = 0;
		
		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		// 打包写寄存器指令 向D2中写结果
		if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, nParam,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul))
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}
	else
	{
		// 不支持无协议
		return FALSE;
	}

	return TRUE;
}

//发送检测结果FCHB
BOOL vcXYDVisionAlign::ReportPLCWithDetectionResultFunc(int nResult,vector<double> DetectionResult,int nOffset)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送检测结果 开始");

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		CString strSimpleInfo;
		m_XYDCommProtocol.PackRePLCBaseFunc(strSimpleInfo, nResult);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);
		int nParam = nResult;

		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		// 打包写寄存器指令 向D2中写结果
		if (FALSE == m_XYDCommProtocol.PackWritePLCWithDetectionResultFunc(strSend, nParam,DetectionResult,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul))
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}
	else
	{
		// 不支持无协议
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送检测结果 结束");

	return TRUE;
}

//发送检测结果FCHB
BOOL vcXYDVisionAlign::ReportPLCWithDetectionResultFunc2(int nResult,vector<int> DetectionResult,int nOffset)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送多个回复参数 开始");

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		CString strSimpleInfo;
		m_XYDCommProtocol.PackRePLCBaseFunc2(strSimpleInfo, nResult,DetectionResult);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);
		int nParam = nResult;

		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		// 打包写寄存器指令 向D2中写结果
		if (FALSE == m_XYDCommProtocol.PackWritePLCWithDetectionResultFunc2(strSend, nParam,DetectionResult,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul))
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}
	else
	{
		// 不支持无协议
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送多个回复参数 结束");

	return TRUE;
}

BOOL vcXYDVisionAlign::SetPlcManualSearch_EX(BOOL bShow)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		//////////////////////////////////////////////////////////////////////////
		// 写PLC指令触发寄存器清零
		CString strSend;
		int nOffSetMul = 0;
		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		// 打包写寄存器指令
		if (optionInfo.m_bSetVisionManualSearchToPLC2)
		{
			if (FALSE == m_XYDCommProtocol.PackPlcWriteSetManualSearch3(strSend, bShow,optionInfo.m_nVisionManualSearchToPlcAddre))
			{
				return FALSE;
			}
		}
		else
		{
			if (FALSE == m_XYDCommProtocol.PackPlcWriteSetManualSearch(strSend, bShow,atoi(RREG_KEYENCE_VISION_PARAM_DIALOG)+nOffSetMul))
			{
				return FALSE;
			}
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}

	return TRUE;

}

// 获取平台各轴的绝对位置
BOOL vcXYDVisionAlign::CommuciateGetPlatformAxisAbsPosEx(CPlatformXYDAxisPos* pPlatformXYDAxisPos)
{
	CString strCmd = _T("");
	GetCommCommandName(strCmd);
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_bIsExecuteFixTargetCheck || m_bIsExecuteSizeCheck)  //执行标定关联验证过程时,不算做指令报警
	{
		strCmd = _T("");
	}
	CString strTempWarningType = _T("");
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		if (NULL == pPlatformXYDAxisPos)
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_AXISPOSERROR);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			return FALSE;
		}

		CString strSimpleInfo;
		m_XYDCommProtocol.PackGetPlatformAbsPos(strSimpleInfo);;
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);
		CmdAnsweredStatus answeredStatus = eAswUnkown;
		if(IsReadWriteAxisPosWithCmd() && m_XYDCommProtocol.m_bCmdAxisPosReady)
		{
			//TT 如果已经成功通过指令获取轴位置，则跳过通信
			*pPlatformXYDAxisPos = m_XYDCommProtocol.m_CurPlatformXYDAxisPos;
			m_XYDCommProtocol.m_bCmdAxisPosReady = FALSE;//已经获取了指令轴位置标志复位
			answeredStatus = eAswSucceed;
		}
		else
		{
			// 读取寄存器
			CString strSend;

			int nOffSetMul = 0;

			BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
			if(bEnableMulReg)
			{
				//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
				nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
				if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
				{
					nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
				}
			}

			// 打包读取寄存器指令  D60-64读取轴数据, D50为PLC数据操作起始地址，故偏移10个数据开始读取轴
			if (FALSE == m_XYDCommProtocol.PackReadPLCFunc(strSend,3,atoi(RREG_KEYENCE_PLC_AXIS_READ)-atoi(RREG_KEYENCE_PLC_TRIGGER)+nOffSetMul,TRUE))
			{
				return FALSE;
			}

			// 发送读取命令，接收PLC回复
			BOOL bReceived;
			CString strInfo;
			CommStatus nCommStatus = eCommSucceed;
			bReceived = CommunicateSendReadRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RECEIVE_GET_PLATFORM_POS_ASW_OUTTIME);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

				return FALSE;
			}

			// 解析收到的响应
			/*CmdAnsweredStatus */answeredStatus = m_XYDCommProtocol.AnalyzeAswPlcGetGetPlatformAbsPos(strInfo, pPlatformXYDAxisPos);
		}

		if (answeredStatus != eAswSucceed)
		{
			m_CommStatus = eCommUnConnect;

			int i = 0;
			CCommOptionInfo commOptionInfo;
			GetCommOptionInfo(commOptionInfo);
			/*for(i = 0; i < m_pSystempOptionConfig->m_nCommReSendTimes && i < 10; i++)*/
			for(i = 0; i < commOptionInfo.m_nCommReSendTimes && i < 0; i++)
			{
				// 读取寄存器
				CString strSend;
				int nOffSetMul = 0;

				BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
				if(bEnableMulReg)
				{
					//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
					nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
					if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
					{
						nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
					}
				}
				// 打包读取寄存器指令
				if (FALSE == m_XYDCommProtocol.PackReadPLCFunc(strSend,3,atoi(RREG_KEYENCE_PLC_AXIS_READ)-atoi(RREG_KEYENCE_PLC_TRIGGER)+nOffSetMul,TRUE))
				{
					return FALSE;
				}

				// 发送读取命令，接收PLC回复
				BOOL bReceived;
				CString strInfo;
				CommStatus nCommStatus = eCommSucceed;
				bReceived = CommunicateSendReadRegStr(strSend, strInfo, nCommStatus);
				if (bReceived == FALSE || nCommStatus != eCommSucceed)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RECEIVE_GET_PLATFORM_POS_ASW_OUTTIME);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
					return FALSE;
				}

				// 解析收到的响应
				CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswPlcGetGetPlatformAbsPos(strInfo, pPlatformXYDAxisPos);
				std::vector<int> nParam;
				std::vector<double> dParam;
				nParam.push_back(0);
				dParam.push_back(pPlatformXYDAxisPos->m_dPosX);
				dParam.push_back(pPlatformXYDAxisPos->m_dPosY);
				dParam.push_back(pPlatformXYDAxisPos->m_dAngle);
				PackAndAddCommSimpleInfo(strSimpleInfo,m_XYDCommProtocol.m_MNPSParam.m_strCmd,nParam,dParam);
				if (answeredStatus == eAswSucceed)
				{
					return TRUE;
				}
			}
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ANALYS_GET_PLATFORM_POS_ASW_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		else
		{
			std::vector<int> nParam;
			std::vector<double> dParam;
			nParam.push_back(0);
			dParam.push_back(pPlatformXYDAxisPos->m_dPosX);
			dParam.push_back(pPlatformXYDAxisPos->m_dPosY);
			dParam.push_back(pPlatformXYDAxisPos->m_dAngle);
			PackAndAddCommSimpleInfo(strSimpleInfo,m_XYDCommProtocol.m_MNPSParam.m_strCmd,nParam,dParam);
		}
	}
	else
	{
		// 暂不不支持无协议
		return FALSE;
	}

	return TRUE;

}

BOOL vcXYDVisionAlign::CommuciateDrivePlatformAxisMoveEx( CPlatformXYDAxisPos* pPlatformXYDAxisPos,CPlatformXYDAxisPos* pPlatformXYDAxisPosAbsBase)
{  
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	CString strCmd = _T("");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		if (NULL == pPlatformXYDAxisPos)
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_AXISPOSERROR);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			return FALSE;
		}

		CString strSimpleInfo;

		m_XYDCommProtocol.PackDrivePlatformMoveToAbsPosEx(strSimpleInfo, pPlatformXYDAxisPos,pPlatformXYDAxisPosAbsBase);
		//m_XYDCommProtocol.PackDrivePlatformMoveToAbsPos(strSimpleInfo, pPlatformXYDAxisPos);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;


		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 写要运动到轴位置 将运动量打包写入D10-D14
		if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, pPlatformXYDAxisPos,pPlatformXYDAxisPosAbsBase,nOffSetMul))
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_DATA_PROCESS_INVALID);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PACK_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEND_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}

		// D2中写拍照成功 
		if (FALSE == ReportPLCFunc(eCodeOK))	// 给拍照完成信号 客户才会走驱动过程
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEND_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		// 写要开始运动指令
		// 打包写寄存器指令：运动开始向D3中写1
		
		if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,1,atoi(RREG_KEYENCE_VISION_MMVA_TRIGGER) + nOffSetMul))
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_DATA_PROCESS_INVALID);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PACK_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEND_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

			return FALSE;
		}
		

		// 查询是否运动完成指令
		scTimer stTimer;
		stTimer.Reset();
		BOOL bMoveCompleted = FALSE;
		BOOL bMoveSuccess = FALSE;
		while(!bMoveCompleted)
		{
			if (TRUE == m_bStopNow)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("软件停止运行！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}


			//////////////////////////////////////////////////////////////////////////
			// 
			// 读取寄存器
			CString strSend;

			// 打包读取寄存器指令
			if (FALSE == m_XYDCommProtocol.PackReadPLCFunc(strSend,1,atoi(RREG_KEYENCE_PLC_MMVA)-atoi(RREG_KEYENCE_PLC_TRIGGER)+ nOffSetMul))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("打包读取驱动平台移动指令失败!");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}

			// 获取时间	
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CString strTime;	
			CString strSendLog;
			CString strRecLog;
			strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
			strSendLog = strTime + strSend;

			// 发送读取命令
			LockShareComm(); //加锁共享通信
			if (FALSE == SendCommInfo(strSend, FALSE))	
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = _T("发送平台移动指令失败！  CommuciateDrivePlatformAxisMoveEx");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

				UnlockShareComm(); //解锁共享通信
				return FALSE;
			}

			// 接收PLC回复
			BOOL bReceived;
			CString strInfo;
			CommStatus nCommStatus = eCommSucceed;
			bReceived = ReceiveCommRepeat(strInfo, nCommStatus, FALSE);
			UnlockShareComm(); //解锁共享通信

			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RECEIVE_GET_PLATFORM_POS_ASW_OUTTIME);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

				return FALSE;

// 				strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
// 				strSendLog = strTime + strSend;
// 
// 				// 再发一次
// 				// 发送读取命令
// 				LockShareComm(); //加锁共享通信
// 				if (FALSE == SendCommInfo(strSend, FALSE))	
// 				{
// 					m_bStatusBar = FALSE;
// 					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
// 					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEND_DRIVE_PLATFORM_ARRIVED_CMD_FAILED);
// 					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
// 					PostMessageUpdateStatusBarInfo();
// 					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
// 					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
// 
// 					UnlockShareComm(); //解锁共享通信
// 					return FALSE;
// 				}
// 
// 				// 接收PLC回复
// 				nCommStatus = eCommSucceed;
// 				bReceived = ReceiveCommRepeat(strInfo, nCommStatus, FALSE);
// 				UnlockShareComm(); //解锁共享通信

// 				if (bReceived == FALSE || nCommStatus != eCommSucceed)
// 				{
// 					m_bStatusBar = FALSE;
// 					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
// 					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RECEIVE_GET_PLATFORM_POS_ASW_OUTTIME);
// 					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
// 					PostMessageUpdateStatusBarInfo();
// 
// 					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
// 					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
// 
// 					return FALSE;
// 				}
			}

			GetLocalTime(&tm);
			strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);
			strRecLog = strTime + strInfo;

			// 解析收到的响应
			CmdAnsweredStatus answeredStatus = m_XYDCommProtocol.AnalyzeAswPlcDrivePlatformMoveCompletedEx(strInfo);
			if (answeredStatus == eAswSucceed)
			{	
				AddCommCommandInfo(TRUE, strSendLog, TRUE,TRUE);
				AddCommCommandInfo(FALSE, strRecLog, TRUE,TRUE);

				if (m_pVisionASMConfig)
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bCommunication)
					{	
						CString strTemp;
						CString strOut =strTemp + _T("Receive:") + strInfo;

						strOut = _T("Send:") + strSendLog;
						VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
						VisionAlignLogRecord(m_strCommunicationData, strOut);

						strOut = _T("Receive:") + strRecLog;
						VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
						VisionAlignLogRecord(m_strCommunicationData, strOut);
					}
				}

				bMoveCompleted = TRUE;	
				bMoveSuccess = TRUE;
			}
			else if (answeredStatus == eAswUnkown)
			{
				AddCommCommandInfo(TRUE, strSendLog, TRUE,TRUE);
				AddCommCommandInfo(FALSE, strRecLog, TRUE,TRUE);

				if (m_pVisionASMConfig)
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bCommunication)
					{	
						CString strTemp;
						CString strOut =strTemp + _T("Receive:") + strInfo;

						strOut = _T("Send:") + strSendLog;
						VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
						VisionAlignLogRecord(m_strCommunicationData, strOut);

						strOut = _T("Receive:") + strRecLog;
						VisionAlignLogRecord(m_strCommunicationData, _T("\n"), FALSE);
						VisionAlignLogRecord(m_strCommunicationData, strOut);
					}
				}

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_DRIVER_AXIS_FAILE);//_T("驱动平台轴移动失败");


				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_DRIVER_AXIS_RETURN_FAILE);//_T("驱动平台轴移动失败-返回非VMVA指令");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_DRIVER_AXIS_FAILE);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

				bMoveCompleted = TRUE;	
				bMoveSuccess = FALSE;

			} 

			// 超时
			if (stTimer.GetTimeMilli(FALSE) >= COMM_WAIT_TIME)
			{			

				AddCommCommandInfo(TRUE, strSendLog, TRUE,TRUE);
				AddCommCommandInfo(FALSE, strRecLog, TRUE,TRUE);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
				m_strStatusBarInfo2.Format(_T("驱动平台移动到位超时！！！最大等待时间 %dms"),COMM_WAIT_TIME);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				Sleep(500);
				return FALSE;
			}  

			Sleep(1);
		}

		// 置驱动到位信号触发寄存器为0
		if (!ClearPlcPlatformTriggerEx(atoi(RREG_KEYENCE_PLC_MMVA)-atoi(RREG_KEYENCE_PLC_TRIGGER)+ nOffSetMul))
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = _T("置Plc指令触发寄存器为0失败！");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_COMMUNICATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}

		std::vector<int> nParam;
		std::vector<double>dParam;

		if (bMoveSuccess == FALSE)
		{
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_ERROR);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_DRIVER_AXIS_FAILE);//_T("驱动平台轴移动失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_DRIVER_AXIS_FAILE);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			nParam.push_back(-1);
			PackAndAddCommSimpleInfo(strSimpleInfo,m_XYDCommProtocol.m_MMVAParam.m_strCmd,nParam,dParam);
			return FALSE;
		}		
		nParam.push_back(0);
		PackAndAddCommSimpleInfo(strSimpleInfo,m_XYDCommProtocol.m_MMVAParam.m_strCmd,nParam,dParam);
	}
	else
	{
		// 不支持无协议
		return FALSE;
	}
	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineCalibrateFCLB_Ex2(int nMarkType,int nExProductIndex)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();

	CString strTempWarningType = _T("");
	CString strCmd = _T("VCLB:");
	GetCommCommandName(strCmd);


	BOOL bSucceed = FALSE;
	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if(sysPlatformInfo.m_bEnableKeyenceCode)
	{

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		//if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE)
		//{
		//	//bSucceed = FALSE;
		//}

		// 暂时只能为XYD平台
		if(sysPlatformInfo.m_eMidPlatformType!= ePlatformXYD)
		{
			bSucceed = FALSE;
		}

		// 暂时默认同时标定全部对象位置
		if(sysPlatformInfo.m_bTargetObjectCamSeparate==TRUE && sysPlatformInfo.m_bTarObjCamSepAutoCalibTar==FALSE)
		{
			for(int i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
		}
		else if(sysPlatformInfo.m_bTargetObjectCamSeparate==FALSE)
		{
			for(int i = 0; i < GetPosNum(); i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
		}
		else
		{
			bSucceed = FALSE;
		}

		// 拾取系统，XYD平台，相机固定安装
		if ( (m_pPlatformInfo->m_bPlatformPick) && (m_pPlatformInfo->m_ePickCamPlatformType == ePickCamPlatformSepFix) )
		{	
			// 情况1：标定验证功能
			if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
			{
				bSucceed = FALSE;
			}
			// 情况2：全局二维码标定
			else if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
			{
				bSucceed = FALSE;
			}
			// 情况3：传统标定+二维码标定
			else
			{
				bSucceed = MulPosCameraCalibrateFCLB_Ex(vnPosIndex, nMarkType, nExProductIndex);
			}
		}
		else
		{		
			{
				// 情况1：标定验证功能
				if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
				{
					bSucceed = FALSE;
				}
				// 情况2：全局二维码标定
				else if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
				{
					bSucceed = FALSE;
				}
				// 情况3：传统标定+二维码标定
				else
				{
					bSucceed = MulPosCameraCalibrateFCLB_Ex(vnPosIndex, nMarkType, nExProductIndex);
				}			
			}
		}
	}

	if (TRUE == bSucceed)
	{	
		CPlatformOptionInfo optionInfo;
		GetPlatformOptionInfo(optionInfo);
		if (optionInfo.m_bEnableCalibrateCheckByAxisMoveOption)
		{
			//ReportPLCFunc(eCodeOK,2);
			ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
			m_bStatusBar = TRUE;
			m_strStatusBarInfo1 = _T("标定轴移动拍照验证完成");
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
		}
		else
		{
			CString tmp;
			tmp.Format(_T(" m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType)%d   当前m_nCurCalibrateIndex%d"),m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType),m_nCurCalibrateIndex) ;
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);
			

			if(m_nCurCalibrateIndex<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurCalibrateIndex>=0)
			{

				tmp.Format(_T(" 向D4中写3   当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
				VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

				//ReportPLCFunc(eCodeContinued,2);// 向D4中写3
				ReportPLCFunc(eCodeContinued,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				m_strStatusBarInfo2.Format(_T("标定走位第%d位置标定完成,继续标定"),m_nCurCalibrateIndex-1);

				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			}
			else if(m_nCurCalibrateIndex==m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
			{
				//ReportPLCFunc(eCodeOK,2);// 向D4中写1 OK 
				tmp.Format(_T(" 向D4中写1   当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
				VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

				CString strSend;
				CString strInfo;
				CommStatus nCommStatus = eCommSucceed;
				
				ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				
				m_strStatusBarInfo2 = _T("全部位置标定完成");
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			}
			else
			{

				//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				tmp.Format(_T(" 向D4中写2   当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
				VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

				//失败后，复位m_nCurCalibrateIndex结果
				m_nCurCalibrateIndex = 0;

				tmp.Format( _T(" m_nCurCalibrateIndex = 0;  当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
				VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);
				m_strStatusBarInfo2 = _T("");
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}
			
		}
	}
	else
	{
		m_nCurCalibrateIndex = 0;
		for(int i = 0; i<m_vvpMarkImagePos.size();i++)
		{
			for(int j = 0; j<m_vvpMarkImagePos.at(i).size();j++)
			{
				if (m_vvpMarkImagePos.at(i).at(j) != NULL)
				{
					delete m_vvpMarkImagePos.at(i).at(j);
					m_vvpMarkImagePos.at(i).at(j) = NULL;
				}
			}
			m_vvpMarkImagePos.at(i).clear();
		}
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		if (!m_boIsSnapImgSucceed)
		{
			m_strStatusBarInfo2 = m_strSnapImgStatuString;
		}
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
	}

	// 平台为触发模式时，标定完成后恢复触发模式
	if (sysPlatformInfo.m_bCameraHardTriggerMode)
	{
		SetCameraTriggerMode(ePTHardwareTrigger);
	}

	return bSucceed;
}
BOOL vcXYDVisionAlign::MulPosCameraCalibrateFCLB_Ex(std::vector<int> vnPosIndex, int nMarkType,int nExProductIndex)
{
	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = vnPosIndex.size();
	if(nPosNum<1 || nPosNum>MAXPOSNUM)
	{
		//失败后，复位m_nCurCalibrateIndex结果
		m_nCurCalibrateIndex = 0;
		//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
		ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

		return FALSE;
	}

	for(int it=0;it<nPosNum;it++)
	{
		if(vnPosIndex.at(it)<0 || vnPosIndex.at(it)>sysPlatformInfo.m_nPositionNum)
		{
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			return FALSE;
		}
	}

	int nCalibPos = nPosNum;
	CString strTempWarningType = _T("");
	CString strCmd = _T("VCLB:");
	GetCommCommandName(strCmd);

	//if (sysPlatformInfo.m_bEnableMultiCalibExtension)
	//{
	//	//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
	//ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

	//	return FALSE;
	//}

	if(m_nCurCalibrateIndex >= m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) || m_nCurCalibrateIndex<0)
	{
		//失败后，复位m_nCurCalibrateIndex结果
		m_nCurCalibrateIndex = 0;
		//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
		ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

		return FALSE;
	}

	int nRemCalibPos = vnPosIndex[0];
	int nAllCalibPos = vnPosIndex.size();

	if(nMarkType !=1)
	{
		//失败后，复位m_nCurCalibrateIndex结果
		m_nCurCalibrateIndex = 0;
		//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
		ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_CALIBTYPE);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;
	}	

	int i=0;

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	{
		// 对位标定工具是否正常
		if (m_bValidCalibSearchTool!=TRUE)
		{
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			return FALSE;
		}

		// 标定开始    
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_CALIBRATING);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		//标定开始
		int nCamPos = m_pPlatformInfo->m_nPositionNum;

		//////////////////////////////////////// 平台标定/////////////////////////////////////////////

		// 1. 获取平台当前各轴的绝对位置（基准位置）,并设置入对位工具
		// 第一标定走位时才获取轴位置
		if(m_nCurCalibrateIndex==0)
		{
			CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
			BOOL bCommuciateGetPlatformAxisAbsPos = FALSE;


			// 直接读取寄存器地址轴数据
			bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

			if (FALSE == bCommuciateGetPlatformAxisAbsPos)
			{
				if (pPlatformAxisPos!=NULL)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;					
				}

				//失败后，复位m_nCurCalibrateIndex结果
				m_nCurCalibrateIndex = 0;
				//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));


				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}
			else
			{
				for (int k=0;k<nPosNum;k++)
				{
					SetCalibPlatformPos(pPlatformAxisPos);
				}

				if (m_CalibPlatformAxisAbsPos.size()<nRemCalibPos+1)
				{
					m_CalibPlatformAxisAbsPos.resize(nRemCalibPos+1);
				}
				m_CalibPlatformAxisAbsPos.at(nRemCalibPos)= *((CPlatformXYDAxisPos*)pPlatformAxisPos);

				// 记录当前平台各个轴的绝对位置
				if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
				{
					CString strTemp;
					CString strInfo;
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_THE_PLATFORM_POS), pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
					strInfo = strTemp + strInfo;	

					VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
					VisionAlignLogRecord(m_strAlignProcessData, strInfo);
				}

				if (pPlatformAxisPos)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;
				}
			}
		}

		// 2. 获取平移标定和旋转标定时平台各轴的移动位置
		std::vector<CPlatformAxisPos*> vpPlatformAxisPos; 

		std::vector<CPlatformAxisPos*> vpPlatformAxisPosAbsBase; 
		std::vector<CPlatformXYDAxisPos> vPlatformAxisPosAbsBaseSend;
		m_pAlignerTool->SetCalibSendRelativePos(FALSE);
		vpPlatformAxisPosAbsBase = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);
		vPlatformAxisPosAbsBaseSend.resize(vpPlatformAxisPosAbsBase.size());
		for (int ii=0;ii<vPlatformAxisPosAbsBaseSend.size();ii++)
		{
			vPlatformAxisPosAbsBaseSend.at(ii) = (*(CPlatformXYDAxisPos*)vpPlatformAxisPosAbsBase.at(ii));
		}



		// 向D10-14中写入相对偏差量 临时设置启用相对偏差量

		m_pAlignerTool->SetCalibSendRelativePos(TRUE);
		vpPlatformAxisPos = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);
		m_pAlignerTool->SetCalibSendRelativePos(FALSE);
		//std::vector<CString> strSend;
		//strSend.resize(vpPlatformAxisPosAbsBase.size());

		//
		//CString strlog2 = _T("");
		//for (int ii=0;ii<vPlatformAxisPosAbsBaseSend.size();ii++)
		//{
		//	strlog2.Format(_T("BX,%f BY:%f  BD:%f PX,%f PY:%f pD:%f\n"), vPlatformAxisPosAbsBaseSend.at(ii).m_dPosX,
		//		vPlatformAxisPosAbsBaseSend.at(ii).m_dPosY, vPlatformAxisPosAbsBaseSend.at(ii).m_dAngle, 
		//		((CPlatformXYDAxisPos*)vpPlatformAxisPos.at(ii))->m_dPosX,
		//		((CPlatformXYDAxisPos*)vpPlatformAxisPos.at(ii))->m_dPosY,
		//		((CPlatformXYDAxisPos*)vpPlatformAxisPos.at(ii))->m_dAngle);
		//	//m_dstrTranlateResultInfo.push_back(strlog2);
		//	OutputDebugString(strlog2);
		//}
		//OutputDebugString(_T("\n\n"));

		//for (int ii=0;ii<vPlatformAxisPosAbsBaseSend.size();ii++)
		//{
		//	
		//	m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend.at(ii), vpPlatformAxisPos.at(ii),(&vPlatformAxisPosAbsBaseSend.at(ii)),0);
		//}

		if (vpPlatformAxisPos.size()!=m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
		{	
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			return OnFailedGetCalibPlatformAxisMovePos(strCmd);
		}
		else
		{
			// 记录平台标定时平台需移动的7个位置各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;

				CString strInfo;
				strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CALIB_SEVEN_PLATFORM_MOVE_POS);
				strInfo = strTemp + strInfo;

				CPlatformXYDAxisPos* pAxisPos;
				for (int i=0; i<vpPlatformAxisPos.size(); i++)
				{
					strTemp.Empty();
					pAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(i);           

					strTemp.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_CALIB_PLATFORM_POS), i, pAxisPos->m_dPosX, pAxisPos->m_dPosY, pAxisPos->m_dAngle);
					strInfo = strInfo + strTemp;
				}


				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			}
		}

		// 3. 驱动平台到位并定位
		CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;

		CMarkImagePos* pMarkImagePos = NULL;
		CMarkImagePos* pDisCorMarkImagePos = NULL;

		std::vector<CMarkImagePos*> vpTempMarkImagePos;
		std::vector<CMarkImagePos*> vpTempDisCorMarkImagePos;

		// 为每个位置各申请一个保存图像坐标的内存
		for (int i=0;i<nAllCalibPos;i++)
		{
			pMarkImagePos = new CMarkImagePos;
			pMarkImagePos->SetMarkImagePosNum(1);
			vpTempMarkImagePos.push_back(pMarkImagePos);

			pDisCorMarkImagePos = new CMarkImagePos;
			pDisCorMarkImagePos->SetMarkImagePosNum(1);
			vpTempDisCorMarkImagePos.push_back(pDisCorMarkImagePos);

		}

		int nProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		CCalibData *pCalibData = m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nProductIndex);
		if (pCalibData == NULL || !pCalibData->IsValid())
		{
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBDATA_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		CCalibrateParam *pCalibrateParam = pCalibData->m_vpCalibrateParam.at(nRemCalibPos);
		if (pCalibrateParam == NULL)
		{
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBPARAM_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}


		CString strCalibPos;
		CString strTmp;
		for (int i =0 ; i< nCalibPos; i++)
		{
			strTmp.Format(_T("位置%d, "), nCalibPos+1);
			strCalibPos += strTmp;
		}




		int k=m_nCurCalibrateIndex;
		//for (k=0; k<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType); k++)
		{ 

			CString strInfo;
			CString strTemp;

			strInfo.Format(_T("移动位置%d标定搜索开始"), k);
			strInfo = strCalibPos + strInfo;
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			//EnterCriticalSection(&m_csResource);
			m_bStatusBar = TRUE;
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_DRIVING_PLATFORM_MOVE), k);
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			//LeaveCriticalSection(&m_csResource);


			// 驱动到位
			pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(k);

			if(FALSE == CommuciateDrivePlatformAxisMoveEx(pPlatformXYDAxisPos, (CPlatformXYDAxisPos*)(&vPlatformAxisPosAbsBaseSend.at(k))))
			{	
				//失败后，复位m_nCurCalibrateIndex结果
				m_nCurCalibrateIndex = 0;
				//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				return OnFailedCommuciateDrivePlatformAxisMove(strCmd, k);
			}


			Sleep(100);
			// 将相机移动到多个位置，逐个定位
			// 采集并定位
			// 等待搜索延时
			int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
			Sleep(m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nCurProdcutIndex)->m_vpCalibrateParam[nRemCalibPos]->m_nSearchDelayTime);

			// 准备开始定位			
			std::vector<SearchMode> vsmSearchMode;	

			for(int i = 0; i < nCalibPos; i++)
			{
				vsmSearchMode.push_back(eCalibSearch);
			}


			// 采集搜索图像
			if (!SnapSearchImage(vnPosIndex))
			{	
				//失败后，复位m_nCurCalibrateIndex结果
				m_nCurCalibrateIndex = 0;
				//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

				// 释放保存内存
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				return OnFailedSnapSearchImage(strCmd);
			}

			// 定位	

			for(int i = 0; i < nCalibPos; i++)
			{
				int nPosIndex = vnPosIndex[i];
				m_vnCalibSearchStartPatIndex[nPosIndex] = 0;
				m_vnCalibSearchEndPatIndex[nPosIndex] = 0;
			}

			{
				if (k==0)
				{
					m_bIsFirstCalibPos = TRUE;
				}
				else
				{
					m_bIsFirstCalibPos = FALSE;
				}
			}

			BOOL bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

			// 分析定位结果，发送指令响应
			if (FALSE == bSearchSucceed)
			{
				//失败后，复位m_nCurCalibrateIndex结果
				m_nCurCalibrateIndex = 0;
				//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				return OnFailedCalibSearch(strCmd, k, vnPosIndex[0]);					
			}

			if(pCalibrateParam->m_nEnableDisCor)
			{
				for(int i = 0; i < nAllCalibPos; i++)
				{
					CCoordPos OutPutPoint;
					m_pAlignerTool->ExecuteDisCorPoint(vnPosIndex[i], m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]), OutPutPoint);
					vpTempDisCorMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), OutPutPoint);
				}
			}

			for(int i=0;i<nAllCalibPos;i++)
			{
				vpTempMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]));;
			}

			if(pCalibrateParam->m_nEnableDisCor)
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempDisCorMarkImagePos[i]);
					m_vvpMarkImagePos[i].push_back(markpos);
				}
			}
			else
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempMarkImagePos[i]);
					m_vvpMarkImagePos[i].push_back(markpos);
				}
			}

			// 记录图像搜索数据
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strOut;
				CCoordPos cp;
				strOut = strTemp + _T("CalibSearch: ");		

				for (int i=0; i< vpTempMarkImagePos.size(); i++)
				{
					strTemp = _T("");
					cp = vpTempMarkImagePos[i]->GetMarkImagePos(0);
					strTemp.Format(_T("CalibPos%d-MarkPos%d( %.2f , %.2f , %.3f )"), i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
					strOut += strTemp;
				}

				if(pCalibrateParam->m_nEnableDisCor)
				{
					for (int i=0; i< vpTempDisCorMarkImagePos.size(); i++)
					{
						strTemp = _T("");
						cp = vpTempDisCorMarkImagePos[i]->GetMarkImagePos(0);
						strTemp.Format(_T("CalibPos%d-DisCorMarkPos%d( %.2f , %.2f , %.3f )"),i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
						strOut += strTemp;
					}
				}

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			strInfo.Format(_T("位置%d移动位置%d标定搜索结束"),vnPosIndex[0],k);
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			// 执行完成一个标定走位过程
			CString tmp;
			tmp.Format(_T("执行完成一个标定走位过程当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

			m_nCurCalibrateIndex++;

			tmp.Format(_T("执行完成一个标定走位过程 m_nCurCalibrateIndex++   m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

			tmp.Format(_T("m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) %d"),m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType));
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

			// 回复PLC 继续标定过程；
			if(m_nCurCalibrateIndex<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurCalibrateIndex>=0)
			{
				tmp.Format(_T("if(m_nCurCalibrateIndex<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurCalibrateIndex>=0)  当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
				VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				// 返回局部成功，还需要继续标定
				// D2中写拍照成功 按照客户要求 调整为在驱动轴前给出D2拍照成功信号
				//ReportPLCFunc(eCodeOK);
				return TRUE;
			}

		}  


		// 4. 将平台7个位置的所有图像搜索结果，设置入对位工具
		BOOL bOk=TRUE,bOk1=TRUE;

		{
			CString tmp;
			tmp.Format(_T("ExecuteCalibrate   当前m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);
			for (int i=0;i<nAllCalibPos;i++)
			{				
				bOk=ExecuteCalibrate(m_vvpMarkImagePos[i], vnPosIndex[i]+nExProductIndex*GetPosNum(), nMarkType);
				if(!bOk)
				{	
					// 标定失败 D4写2
					//ReportPLCFunc(eCodeNG,2);

					//失败后，复位m_nCurCalibrateIndex结果
					m_nCurCalibrateIndex = 0;
					ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
					for (int i = 0; i < vpTempMarkImagePos.size(); i++)
					{
						if (vpTempMarkImagePos[i])
						{
							delete vpTempMarkImagePos[i];
							vpTempMarkImagePos[i] = NULL;
						}
					}

					for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
					{
						if (vpTempDisCorMarkImagePos[i])
						{
							delete vpTempDisCorMarkImagePos[i];
							vpTempDisCorMarkImagePos[i] = NULL;
						}
					}


					return FALSE;
				}
				else
				{
					std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
					vpAllCalibratedInfo = GetAllCalibratedInfo();
					/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
					SetCurCalibratedInfo(vpAllCalibratedInfo);	

					// 保存标定后信息到当前产品中及配置文件中
					SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

				}
				bOk1 = bOk1 && bOk;
			}

		}



		// 5. 释放内存
		for (int i = 0; i < vpTempMarkImagePos.size(); i++)
		{
			if (vpTempMarkImagePos[i])
			{
				delete vpTempMarkImagePos[i];
				vpTempMarkImagePos[i] = NULL;
			}
		}

		for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
		{
			if (vpTempDisCorMarkImagePos[i])
			{
				delete vpTempDisCorMarkImagePos[i];
				vpTempDisCorMarkImagePos[i] = NULL;
			}
		}

		if (bOk1)
		{
			std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
			vpAllCalibratedInfo = GetAllCalibratedInfo();

			CString tmp;
			tmp.Format(_T("if (bOk1) m_nCurCalibrateIndex%d"),m_nCurCalibrateIndex);
			VisionAlignLogRecord(_T("D://m_nCurCalibrateIndex.txt "),tmp);

			/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
			SetCurCalibratedInfo(vpAllCalibratedInfo);	

			// 保存标定后信息到当前产品中及配置文件中
			SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);


			if(optionInfo.m_bAutoCopyCalibData && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
			{
				if (!ExecuteCopyCalibrateData(1,nExProductIndex))
				{
					//失败后，复位m_nCurCalibrateIndex结果
					m_nCurCalibrateIndex = 0;
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
					m_strStatusBarInfo2 = _T("执行标定拷贝失败！");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));	
					//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
					ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

					return FALSE;
				}
			}

			return TRUE;
		}
		else
		{
			//失败后，复位m_nCurCalibrateIndex结果
			m_nCurCalibrateIndex = 0;
			//ReportPLCFunc(eCodeNG,2);// 向D4中写2 NG
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

			return FALSE;
		}
	}
}
BOOL vcXYDVisionAlign::ReportPLC_Offset(CCoordPos platOffset,int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos,int nOffset)
{
	if (m_bIsExecuteFixTargetCheck)  // 执行检查验证时不进行指令回复
	{
		return TRUE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    0");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	BOOL bResult = (nResult==1)? TRUE:FALSE;
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    1");

	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSimpleInfo;
		m_XYDCommProtocol.PackReAlignResultInfo_NoProtocol(strSimpleInfo,platOffset,nResult,pPlatformXYDAxisPos);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    2");

		if(optionInfo.m_bEnableAngleCorrect)
		{	
			scDegree Sd = scDegree(optionInfo.m_dAngleCorrect);
			scRadian Ra = scRadian(Sd);
			double dx = platOffset.GetPosX();
			double dy = platOffset.GetPosY();

			double dx1 = cos(Ra)*dx+sin(Ra)*dy;
			double dy1 = -sin(Ra)*dx+cos(Ra)*dy;

			platOffset.m_dPosX = dx1;
			platOffset.m_dPosY = dy1;
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    3");

		CPlatformXYDAxisPos* pPlatformXYDAxisPosMove = new CPlatformXYDAxisPos();
		pPlatformXYDAxisPosMove->m_dPosX = platOffset.GetPosX();
		pPlatformXYDAxisPosMove->m_dPosY = platOffset.GetPosY();
		pPlatformXYDAxisPosMove->m_dAngle = platOffset.GetAngle();

		int nOffSetMul = 0;
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    4");

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    5 发送轴位置");

		if(IsReadWriteAxisPosWithCmd())  // 轴偏差随指令一并读写
		{
			// 打包写寄存器指令	
			if(FALSE == m_XYDCommProtocol.PackWritePLCFuncWithAxisEx(strSend, pPlatformXYDAxisPosMove,pPlatformXYDAxisPos,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul,nResult))
			{
				return FALSE;
			}
			if (pPlatformXYDAxisPosMove!=NULL)
			{
				delete pPlatformXYDAxisPosMove;
				pPlatformXYDAxisPosMove = NULL;					
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Result and Offset    6");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Result and Offset    7");
		}
		else
		{
			// 打包写寄存器指令			  PackWritePLCWithDetectionResultFunc(strSend,nResult+1,m_vInspect, atoi(RREG_KEYENCE_VISION_INSPECT_READ)+nOffSetMul))
			if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, pPlatformXYDAxisPosMove,pPlatformXYDAxisPos,nOffset+nOffSetMul))
			{
				return FALSE;
			}
			if (pPlatformXYDAxisPosMove!=NULL)
			{
				delete pPlatformXYDAxisPosMove;
				pPlatformXYDAxisPosMove = NULL;					
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    6");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    7");

			// 向D2中写入状态对位OK或NG
			if(FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul))
			{
				return FALSE;
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    8 发送拍照成功");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
			nCommStatus = eCommSucceed;
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    9");
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
BOOL vcXYDVisionAlign::ReportPLC_OffsetAndBenchCenter(CCoordPos platOffset,int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos,CPlatformAxisPos* vpPlatAxisPos,int nOffset)
{
	if (m_bIsExecuteFixTargetCheck)  // 执行检查验证时不进行指令回复
	{
		return TRUE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    0");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	BOOL bResult = (nResult==1)? TRUE:FALSE;
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    1");

	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSimpleInfo;
		m_XYDCommProtocol.PackReAlignResultInfo_NoProtocol(strSimpleInfo,platOffset,nResult,pPlatformXYDAxisPos);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    2");

		if(optionInfo.m_bEnableAngleCorrect)
		{	
			scDegree Sd = scDegree(optionInfo.m_dAngleCorrect);
			scRadian Ra = scRadian(Sd);
			double dx = platOffset.GetPosX();
			double dy = platOffset.GetPosY();

			double dx1 = cos(Ra)*dx+sin(Ra)*dy;
			double dy1 = -sin(Ra)*dx+cos(Ra)*dy;

			platOffset.m_dPosX = dx1;
			platOffset.m_dPosY = dy1;
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    3");

		CPlatformXYDAxisPos* pPlatformXYDAxisPosMove = new CPlatformXYDAxisPos();
		pPlatformXYDAxisPosMove->m_dPosX = platOffset.GetPosX();
		pPlatformXYDAxisPosMove->m_dPosY = platOffset.GetPosY();
		pPlatformXYDAxisPosMove->m_dAngle = platOffset.GetAngle();

		int nOffSetMul = 0;
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    4");

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    5 发送轴位置");

		if(IsReadWriteAxisPosWithCmd())  // 轴偏差随指令一并读写
		{
			if (optionInfo.m_bEnableAlignBenchCenter)
			{
				//// 打包写寄存器指令
				//if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, vpPlatAxisPos,nOffset+nOffSetMul))
				//{
				//	return FALSE;
				//}

				// 打包写寄存器指令	同时发送居中对位结果
				if(FALSE == m_XYDCommProtocol.PackWritePLCFuncWithAxisAndBenchCenterEx(strSend, pPlatformXYDAxisPosMove,pPlatformXYDAxisPos,vpPlatAxisPos,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul,nResult))
				{
					return FALSE;
				}

			}
			else
			{
				// 打包写寄存器指令	
				if(FALSE == m_XYDCommProtocol.PackWritePLCFuncWithAxisEx(strSend, pPlatformXYDAxisPosMove,pPlatformXYDAxisPos,atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul,nResult))
				{
					return FALSE;
				}

			}
			
			if (pPlatformXYDAxisPosMove!=NULL)
			{
				delete pPlatformXYDAxisPosMove;
				pPlatformXYDAxisPosMove = NULL;					
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Result and Offset    6");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Result and Offset    7");
		}
		else
		{
			// 打包写寄存器指令			  PackWritePLCWithDetectionResultFunc(strSend,nResult+1,m_vInspect, atoi(RREG_KEYENCE_VISION_INSPECT_READ)+nOffSetMul))
			if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, pPlatformXYDAxisPosMove,pPlatformXYDAxisPos,nOffset+nOffSetMul))
			{
				return FALSE;
			}
			if (pPlatformXYDAxisPosMove!=NULL)
			{
				delete pPlatformXYDAxisPosMove;
				pPlatformXYDAxisPosMove = NULL;					
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    6");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    7");

			// 向D2中写入状态对位OK或NG
			if(FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffset+nOffSetMul))
			{
				return FALSE;
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    8 发送拍照成功");

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
			nCommStatus = eCommSucceed;
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Offset    9");
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}



BOOL vcXYDVisionAlign::ReportPLC_Inspect(int nResult,BendInspectResultInfo* BendInspectResult /*= NULL*/)
{
	if (m_bIsExecuteFixTargetCheck)  // 执行检查验证时不进行指令回复
	{
		return TRUE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    0");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    1");

	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		int nOffSetMul = 0;
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    2");
		
		BendInspectResultInfo m_BendInspectResult;
		GetBendInspectResult(m_BendInspectResult);
		
		/*CString strBendInfo = _T("");
		strBendInfo.Format(_T(" %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f,"), 
		m_BendInspectResult.m_dLx,				m_BendInspectResult.m_dLy,
		m_BendInspectResult.m_dRx,				m_BendInspectResult.m_dRy,
		m_BendInspectResult.m_dDistanceL,			m_BendInspectResult.m_dDistanceR,		
		m_BendInspectResult.m_dTargetDistance,	m_BendInspectResult.m_dObjectDistance,
		m_BendInspectResult.m_dDistanceSubX,	m_BendInspectResult.m_dDistanceSubY,
		m_BendInspectResult.m_dCircleRadius[0],	m_BendInspectResult.m_dCircleRadius[1]);*/

		std::vector<double> m_vInspect;							// 保存要发送的数据
		/*m_vInspect.resize(6,0.0);*/
		m_vInspect.push_back(m_BendInspectResult.m_dLx);
		m_vInspect.push_back(m_BendInspectResult.m_dLy);
		m_vInspect.push_back(m_BendInspectResult.m_dDistanceL);
		m_vInspect.push_back(m_BendInspectResult.m_dRx);
		m_vInspect.push_back(m_BendInspectResult.m_dRy);
		m_vInspect.push_back(m_BendInspectResult.m_dDistanceR);



		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    4");

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    5 发送检测数据");

		

		// 向D30中写入检测数据
		if(FALSE == m_XYDCommProtocol.PackWritePLCWithDetectionResultFunc(strSend,nResult+1,m_vInspect, atoi(RREG_KEYENCE_VISION_INSPECT_READ)+nOffSetMul))
		{
			return FALSE;
		}

		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    8 发送拍照成功");

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
		nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "ReportPLC_Inspect    9");

	}
	else
	{
		return FALSE;
	}

	return TRUE;

}

BOOL vcXYDVisionAlign::ReportPLC_RowCols(int nResult,int nSize,std::vector<int> vnRows,std::vector<int> vnCols,int nOffset)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		CString strInfo;
		BOOL bReceived;
		CommStatus nCommStatus = eCommSucceed;

		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		if(nSize<1)
		{

			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffSetMul))
			{
				return FALSE;
			}
			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
		else
		{
			CString strSimpleInfo;
			m_XYDCommProtocol.PackReMulRowColsSimpleInfo_Ex(strSimpleInfo,nResult,nSize,vnRows,vnCols);
			AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackReMulRowCols_Ex(strSend,nSize,vnRows,vnCols,nOffset+nOffSetMul))
			{
				return FALSE;
			}
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			// 向D2中写入状态对位OK或NG
			if(FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffSetMul))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位
			nCommStatus = eCommSucceed;
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL vcXYDVisionAlign::ReportPLC_WriteAbsPosEx(CPlatformAxisPos* vpPlatAxisPos,int nOffSet)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSend;
		BOOL bReceived = FALSE;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;

		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;

			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}
		// 打包写寄存器指令
		if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, vpPlatAxisPos,nOffSet+nOffSetMul))
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}

BOOL vcXYDVisionAlign::OnObjectSearchCandidateExFOBC_Ex(std::vector<int> vnPosIndex, int nStartPatIndex,BOOL* pbIsSizeNG /*= NULL*/)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	int nPosIndex = 0;
	CString strCmd = _T("VOBC:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	int nPosNum = vnPosIndex.size();
	VisionAlignLogRecord("D:\\TimeLog.txt", "开始 对象模板曝光切换");
	// 对象模板曝光切换
	{
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;
		//BOOL bObjectCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateEnable();
		
		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableObjectSearchShutter(nStartPatIndex);
		BOOL bObjectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		//	if (m_pSystempOptionConfig->m_bEnableSearchShutter == TRUE)
		if (SystempOptionSearchShutter /*&& bObjectCandidate*/ && bObjectShutter /*&& (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	

			for(int i = 0; i < vnPosIndex.size(); i++)
			{
				vsmCameraExposureType.push_back(eObjectSearchCameraExposure);
			}

			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "结束 对象模板曝光切换");

	scTimer stCommuciateTimer; double dCommuciateTime(0.0);
	stCommuciateTimer.Reset();

	dCommuciateTime = stCommuciateTimer.GetTimeMilli(FALSE);
	CString str;

	// 3.开启定位
	scTimer stObjectSearchTimer; double dObjectSearchTime(0.0);
	stObjectSearchTimer.Reset();

	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	int nDelayTime2 = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime2();
	BOOL bEnableDelayTime2 = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableObjectSearchDelayTime2();

	Sleep(nDelayTime);

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;	
	for(int i = 0; i < vnPosIndex.size(); i++)
	{
		nPosIndex = vnPosIndex[i];
		vsmSearchMode.push_back(eObjectSearch);	
	}

	m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eObjectSearch);

	if (vnPosIndex.size()==1 && (optionInfo.m_bEnableMultiMarkMode) && (vnPosIndex[0] == 0) && (GetPosNum()==2))
	{
		std::vector<int> vnPosIndex1;
		vnPosIndex1.push_back(vnPosIndex[0]+1);	
		m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex1, eObjectSearch);
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "开始 对象 拍照");

	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "结束 对象 拍照");

	// 帧有效显示时，刷新位置1图像
	if (vnPosIndex.size()==1 && (m_pPlatformInfo->m_nPositionNum>1) && (optionInfo.m_bEnableMultiMarkMode) && (nPosIndex == 0) && (m_pSystempOptionConfig->m_nDisplayMode == 1))
	{
		std::vector<int> vnPosIndex1;
		vnPosIndex1.push_back(vnPosIndex[0]+1);
		SnapSearchImage(vnPosIndex1);
	}

	scTimer stSearchTimer;	// 搜索时间
	stSearchTimer.Reset();

	VisionAlignLogRecord("D:\\TimeLog.txt", "开始 对象 搜索");

	// 定位
	BOOL bObjectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateEnable();
	BOOL bObjectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateSyncEnable();
	BOOL bSearchSucceed = FALSE;
	if (bObjectSearchCandidate == FALSE)
	{
		for(int i = 0; i < nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			m_vnObjectSearchStartPatIndex[nPosIndex] = 0;
			m_vnObjectSearchEndPatIndex[nPosIndex] = 0;
		}
		bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
	}
	else
	{
		if (bObjectSearchSync == TRUE)
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
			{
				nTempStartPatIndex = nStartPatIndex;
			}
			else if (nStartPatIndex == 9)
			{
				nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[vnPosIndex[0]];
			}
			else
			{
				nTempStartPatIndex = 0;
			}
			nTempEndPatIndex = OBJECTCANDIDATENUM-1;

			int j = 0;
			for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
			{
				if (j > 0 && (FALSE == GetAlnObjectSearchCandidatePatEnable(j)))
				{
					continue;
				}

				for(int i = 0; i < nPosNum; i++)
				{
					nPosIndex = vnPosIndex[i];
					m_vnObjectSearchStartPatIndex[nPosIndex] = j;
					m_vnObjectSearchEndPatIndex[nPosIndex] = j;
				}			
				bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

				if (bSearchSucceed)
				{
					break;
				}	
			}
		}
		else
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			for(int i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
				{
					nTempStartPatIndex = nStartPatIndex;
				}
				else if (nStartPatIndex == 9)
				{
					nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[nPosIndex];
				}
				else
				{
					nTempStartPatIndex = 0;
				}
				nTempEndPatIndex = OBJECTCANDIDATENUM-1;

				m_vnObjectSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
				m_vnObjectSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
			}
			bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		}
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "结束 对象 搜索");

	if (FALSE == bSearchSucceed)
	{
		if (optionInfo.m_bSearchTimes)
		{
			for (int k = 0; k < optionInfo.m_nSearchTimes; k++)
			{
				VisionAlignLogRecord("D:\\TimeLog.txt", "开始 对象 重拍&搜索");
				//定位延时	 m_nSearchDelayTime
				Sleep(optionInfo.m_nSearchDelayTime);
				// 采集搜索图像
				if (!SnapSearchImage(vnPosIndex))
				{
					OnFailedSnapSearchImage(strCmd);
					return FALSE;
				}

				// 定位
				BOOL bObjectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateEnable();
				BOOL bObjectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateSyncEnable();
				if (bObjectSearchCandidate == FALSE)
				{
					for(int i = 0; i < nPosNum; i++)
					{
						nPosIndex = vnPosIndex[i];
						m_vnObjectSearchStartPatIndex[nPosIndex] = 0;
						m_vnObjectSearchEndPatIndex[nPosIndex] = 0;
					}
					bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				}
				else
				{
					if (bObjectSearchSync == TRUE)
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
						{
							nTempStartPatIndex = nStartPatIndex;
						}
						else if (nStartPatIndex == 9)
						{
							nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[vnPosIndex[0]];
						}
						else
						{
							nTempStartPatIndex = 0;
						}
						nTempEndPatIndex = OBJECTCANDIDATENUM-1;

						int j = 0;
						for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
						{
							if (j > 0 && (FALSE == GetAlnObjectSearchCandidatePatEnable(j)))
							{
								continue;
							}

							for(int i = 0; i < nPosNum; i++)
							{
								nPosIndex = vnPosIndex[i];
								m_vnObjectSearchStartPatIndex[nPosIndex] = j;
								m_vnObjectSearchEndPatIndex[nPosIndex] = j;
							}			
							bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

							if (bSearchSucceed)
							{
								break;
							}	
						}
					}
					else
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						for(int i = 0; i < nPosNum; i++)
						{
							nPosIndex = vnPosIndex[i];
							if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
							{
								nTempStartPatIndex = nStartPatIndex;
							}
							else if (nStartPatIndex == 9)
							{
								nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[nPosIndex];
							}
							else
							{
								nTempStartPatIndex = 0;
							}
							nTempEndPatIndex = OBJECTCANDIDATENUM-1;

							m_vnObjectSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
							m_vnObjectSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
						}
						bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
					}
				}

				if (bSearchSucceed)
				{
					VisionAlignLogRecord("D:\\TimeLog.txt", "结束 对象 重拍&搜索");
					break;
				}
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "结束 对象 重拍&搜索");
		}
	}

	if (TRUE == bSearchSucceed && m_bObjectSearchTest)
	{
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		if (optionInfo.m_bManualAfObjSearchFail && !bSearchSucceed)
		{
			// 不支持手动选点时发送PLC
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉弹出手动搜索框
				SetPlcManualSearch_EX(TRUE);
			}
   //         else


			if (FALSE ==ExcuteManualSearch(1,vnPosIndex))
			{
				//if (optionInfo.m_bSetVisionManualSearchToPLC)
				//{
				//	// 通知PLC，视觉关闭手动搜索框
				//	SetPlcManualSearch_EX(FALSE);
				//}
				bSearchSucceed = FALSE;
			}
            else
            {
                bSearchSucceed = TRUE;
            }
            
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉关闭手动搜索框
				SetPlcManualSearch_EX(FALSE);
			}
		}
	
	}

	 m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
	 m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
	 if (vnPosIndex.size()==1 && (optionInfo.m_bEnableMultiMarkMode) && (vnPosIndex[0] == 0) && (GetPosNum()==2))
	 {
		 std::vector<int> vnPosIndex1;
		 vnPosIndex1.push_back(vnPosIndex[0]+1);

		 m_cVisionAlignGui.SetPosSearchResult(vnPosIndex1, eObjectSearch);
	 }

	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile
		&& m_pVisionASMConfig->m_DataRecord.m_bSaveObjectSnapVDBFile //失败直接存储、非FAAL流程存储、FAAL未开启AAL流程存图时存储
		&& (!bSearchSucceed || !m_bFaalBendProcess || (m_bFaalBendProcess && !m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile))) 
	{
		ExeSaveVDBFile(vnPosIndex,eObjectSearch);
	}
	
	if (bSearchSucceed)
	{
		
	}
	else
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
		m_strStatusBarInfo2 = m_StrSearchErrorInfo;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_SEARCH);
		m_strStatusBarInfo1 += m_StrSearchErrorInfo;
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
//		RecordObjectSizeIndfo(FALSE, FALSE);

		return FALSE;
	}

	// 5. 设置对位工具
	SetMultiEXObjectMarkSearchResultsWithCombinationCode(m_mpObjectMarkImagePos);
	SetObjectMarkSearchResults(m_mpObjectMarkImagePos);

	// 计算对象距离
	if (!optionInfo.m_bExeObjectCheckModeWhenAlign && !CheckObjectDistance())
	{
		if (pbIsSizeNG != NULL)
		{
			*pbIsSizeNG = TRUE;
		}
		
		if(m_bObjectSearchTest) ReportObjectSearchResultFOBC(FALSE);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_OBJECT_LEN_OVERFLOW); //__T("对象距离超过设定标准，对象定位结果可能出错");
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}
	//if ((/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == TRUE && m_nAlignTime == 1) 
	//	|| (/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == FALSE))
	//{
	//	SaveObjectPosInfo(GetTotalProuductData()->GetCurProductIndex());
	//}
	SaveObjectPosInfo(GetTotalProuductData()->GetCurProductIndex());
	// 记录图像搜索数据
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		CString strTemp;

		CString strOut;
		CCoordPos cp;
		strOut = strTemp + _T("ObjectSearch: ");		
		for (int i=0; i<m_mpObjectMarkImagePos.GetMarkImagePosNum(); i++)
		{
			strTemp = _T("");
			cp = m_mpObjectMarkImagePos.GetMarkImagePos(i);

			strTemp.Format(_T("MarkPos%d(%.2f,%.2f) "), i, cp.GetPosX(), cp.GetPosY());

			strOut += strTemp;
		}
		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}

	
	dObjectSearchTime = stObjectSearchTimer.GetTimeMilli(FALSE);
	VisionAlignLogRecord("D:\\TimeLog.txt", "end---OnObjectSearchCandidateExFOBC_Ex");
	return TRUE;
}

BOOL vcXYDVisionAlign::OnTargetSearchCandidateExFTGC_Ex(std::vector<int> vnPosIndex, int nStartPatIndex)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	int nPosIndex = 0;
	CString strCmd = _T("VTGC:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");

	scTimer stSearchTimer;	// 搜索时间
	stSearchTimer.Reset();
	int i=0;
	int nPosNum = vnPosIndex.size();
	// 目标模板曝光切换
	{
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;
		//BOOL bTargetCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateEnable();

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableTargetSearchShutter(nStartPatIndex);
		BOOL bTargetShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		//	if (m_pSystempOptionConfig->m_bEnableSearchShutter == TRUE)
		if (SystempOptionSearchShutter /*&& bTargetCandidate*/ && bTargetShutter /*&& (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;

			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eTargetSearchCameraExposure);
			}

			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	// 3.开启定位
	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetSearchDelayTime();
	Sleep(nDelayTime);

	// 准备开始定位			
	//	std::vector<int> vnPosIndex;
	std::vector<SearchMode> vsmSearchMode;	
	// 定义全部位置向量，包含0/1对象和2/3目标
	std::vector<int> vnAllPosIndex;
	int nAllPosIndex = 0;

	//	int nPosIndex = 0;
	for(i = 0; i < nPosNum; i++)
	{
		nPosIndex = vnPosIndex[i]; 	
		vsmSearchMode.push_back(eTargetSearch);	// 
		if (nPosIndex>=GetPosNum()/2)
		{
			nAllPosIndex = nPosIndex-GetPosNum()/2;
			vnAllPosIndex.push_back(nAllPosIndex);

			nAllPosIndex = nPosIndex;
			vnAllPosIndex.push_back(nAllPosIndex);
		}

	}

	// 如果目标和对象分离，并且关联标定不使用靶标，不读取轴位置，则将目标图像坐标映射到对象图像中，刷新所有图像的目标十字和文字GUI
	if (TRUE == m_pPlatformInfo->m_bTargetObjectCamSeparate/* &&  m_pSystempOptionConfig->m_bShowSepTargetPos*//* && FALSE == m_pPlatformInfo->m_bTargetCalibUseBoard && TRUE == m_pPlatformInfo->m_bTarObjCamSepNoAxisCalib*/)
	{
		m_cVisionAlignGui.SetPosGuiInvisible(vnAllPosIndex, eTargetSearch);
	}
	else
	{
		m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eTargetSearch);
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "开始 目标 拍照");
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "结束 目标 拍照");

	VisionAlignLogRecord("D:\\TimeLog.txt", "开始 目标 搜索");
	// 定位
	BOOL bTargetSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateEnable();
	BOOL bTargetSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateSyncEnable();
	BOOL bSearchSucceed = FALSE;
	if (bTargetSearchCandidate == FALSE)
	{
		for(i = 0; i < nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			m_vnTargetSearchStartPatIndex[nPosIndex] = 0;
			m_vnTargetSearchEndPatIndex[nPosIndex] = 0;
		}
		bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
	}
	else
	{
		if (bTargetSearchSync == TRUE)
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
			{
				nTempStartPatIndex = nStartPatIndex;
			}
			else if (nStartPatIndex == 9)
			{
				nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[vnPosIndex[0]];
			}
			else
			{
				nTempStartPatIndex = 0;
			}
			nTempEndPatIndex = TARGETCANDIDATENUM-1;

			int j = 0;
			for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
			{
				if (j > 0 && (FALSE == GetAlnTargetSearchCandidatePatEnable(j)))
				{
					continue;
				}

				for(i = 0; i < nPosNum; i++)
				{
					nPosIndex = vnPosIndex[i];
					m_vnTargetSearchStartPatIndex[nPosIndex] = j;
					m_vnTargetSearchEndPatIndex[nPosIndex] = j;
				}
				bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				if (bSearchSucceed)
				{
					break;
				}
			}
		}
		else
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
				{
					nTempStartPatIndex = nStartPatIndex;
				}
				else if (nStartPatIndex == 9)
				{
					nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[nPosIndex];
				}
				else
				{
					nTempStartPatIndex = 0;
				}
				nTempEndPatIndex = TARGETCANDIDATENUM-1;

				m_vnTargetSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
				m_vnTargetSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
			}
			bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		}
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "结束 目标 搜索");

	if (FALSE == bSearchSucceed)
	{
		if (optionInfo.m_bSearchTimes)
		{
			for (int k = 0; k < optionInfo.m_nSearchTimes; k++)
			{
				VisionAlignLogRecord("D:\\TimeLog.txt", "开始 目标 重拍&搜索");
				//定位延时	 m_nSearchDelayTime
				Sleep(optionInfo.m_nSearchDelayTime);
				// 采集搜索图像
				if (!SnapSearchImage(vnPosIndex))
				{
					OnFailedSnapSearchImage(strCmd);
					return FALSE;
				}

				// 定位
				BOOL bTargetSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateEnable();
				BOOL bTargetSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateSyncEnable();
				if (bTargetSearchCandidate == FALSE)
				{
					for(i = 0; i < nPosNum; i++)
					{
						nPosIndex = vnPosIndex[i];
						m_vnTargetSearchStartPatIndex[nPosIndex] = 0;
						m_vnTargetSearchEndPatIndex[nPosIndex] = 0;
					}
					bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				}
				else
				{
					if (bTargetSearchSync == TRUE)
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
						{
							nTempStartPatIndex = nStartPatIndex;
						}
						else if (nStartPatIndex == 9)
						{
							nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[vnPosIndex[0]];
						}
						else
						{
							nTempStartPatIndex = 0;
						}
						nTempEndPatIndex = TARGETCANDIDATENUM-1;

						int j = 0;
						for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
						{
							if (j > 0 && (FALSE == GetAlnTargetSearchCandidatePatEnable(j)))
							{
								continue;
							}

							for(i = 0; i < nPosNum; i++)
							{
								nPosIndex = vnPosIndex[i];
								m_vnTargetSearchStartPatIndex[nPosIndex] = j;
								m_vnTargetSearchEndPatIndex[nPosIndex] = j;
							}
							bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
							if (bSearchSucceed)
							{
								break;
							}
						}
					}
					else
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						for(int i = 0; i < vnPosIndex.size(); i++)
						{
							nPosIndex = vnPosIndex[i];
							if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
							{
								nTempStartPatIndex = nStartPatIndex;
							}
							else if (nStartPatIndex == 9)
							{
								nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[nPosIndex];
							}
							else
							{
								nTempStartPatIndex = 0;
							}
							nTempEndPatIndex = TARGETCANDIDATENUM-1;

							m_vnTargetSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
							m_vnTargetSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
						}
						bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
					}
				}

				if (bSearchSucceed)
				{
					VisionAlignLogRecord("D:\\TimeLog.txt", "结束 目标 重拍&搜索");
					break;
				}
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "结束 目标 重拍&搜索");
		}
	}

	if (TRUE == bSearchSucceed)
	{
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		if (optionInfo.m_bManualAfTarSearchFail)
		{
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉弹出手动搜索框
				SetPlcManualSearch_EX(TRUE);
			}

			if (FALSE == ExcuteManualSearch(0,vnPosIndex))
			{
				//if (optionInfo.m_bSetVisionManualSearchToPLC)
				//{
				//	// 通知PLC，视觉关闭手动搜索框
				//	SetPlcManualSearch_EX(FALSE);
				//}
				bSearchSucceed = FALSE;
			}
			else
			{
				bSearchSucceed = TRUE;
			}
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉关闭手动搜索框
				SetPlcManualSearch_EX(FALSE);
			}
		}

	}

	// 如果目标和对象分离，并且关联标定不使用靶标，不读取轴位置，则将目标图像坐标映射到对象图像中，刷新所有图像的目标十字和文字GUI
	if (TRUE == m_pPlatformInfo->m_bTargetObjectCamSeparate &&  m_pSystempOptionConfig->m_bShowSepTargetPos/* && FALSE == m_pPlatformInfo->m_bTargetCalibUseBoard && TRUE == m_pPlatformInfo->m_bTarObjCamSepNoAxisCalib*/)
	{
		int nHomoPosIndex = 0;
		std::vector<int> vnHomoPosIndex;			// 源目标位置
		vnHomoPosIndex = vnPosIndex;

		// 对目标2和3循环进行映射
		for (int i = 0; i<vnHomoPosIndex.size(); i++)
		{
			nHomoPosIndex = vnHomoPosIndex.at(i);
			if (TRUE == m_mpTargetMarkImagePos.GetIsMarkImagePosOK(nHomoPosIndex))
			{
				CCoordPos ObjImgPos;
				CCoordPos TarImgPos = m_mpTargetMarkImagePos.GetMarkImagePos(nHomoPosIndex);

				// 根据目标图像到对象图像映射结果，将目标图像坐标映射到对象图像中
				if (!m_pAlignerTool->ConvertTarImgPosToObjImgPosUsingImageHomographyProject(ObjImgPos, nHomoPosIndex, TarImgPos))
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1.Format(_T("执行目标图像坐标映射对象图像失败"));
					m_strStatusBarInfo2.Format(_T(""));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
				}

				m_mpTargetMarkImagePos.SetMarkImagePos(nHomoPosIndex - GetPosNum()/2, TRUE, ObjImgPos);
			}
		}

		m_cVisionAlignGui.SetPosSearchResult(vnAllPosIndex, eTargetSearch);
	}
	else
	{
		m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);

	}

	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveTargetSnapVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eTargetSearch);
	}

	if (bSearchSucceed)
	{

	}
	else
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
		m_strStatusBarInfo2 = m_StrSearchErrorInfo;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_SEARCH);
		m_strStatusBarInfo1 += m_StrSearchErrorInfo;
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	// 4. 关闭定位

	// 5. 设置对位工具
	m_vmpTargetMarkImagePosMultiEx.at(m_pAlignerTool->GetAlignExProductIndex()) = m_mpTargetMarkImagePos;
	SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
	if (optionInfo.m_bEnableMutiTarget)
	{
		m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex) = m_mpTargetMarkImagePos;
		m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex)=TRUE;
	}

	// 不计算目标距离
	
	SaveTargetPosInfo(GetTotalProuductData()->GetCurProductIndex());

	// 记录图像搜索数据
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		CString strTemp;

		CString strOut;
		CCoordPos cp;
		strOut = strTemp + _T("TargetSearch: ");		
		for (int i=0; i<m_mpTargetMarkImagePos.GetMarkImagePosNum(); i++)
		{
			strTemp = _T("");
			cp = m_mpTargetMarkImagePos.GetMarkImagePos(i);

			strTemp.Format(_T("MarkPos%d(%.2f,%.2f) "), i, cp.GetPosX(), cp.GetPosY());

			strOut += strTemp;
		}

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "end---OnTargetSearchCandidateExFTGC_Ex");
	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineAlignExFACO_Ex(int nOffsetIndex, int nBenchOffsetIndex/* = -1*/, int nExProductIndex/* = 0*/,int nTarFixOffsetIndex/* = 0 */)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	SetAlignBenchOffsetIndex(nBenchOffsetIndex);
	SetAlignExProductIndex(nExProductIndex);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	// start 直接根据searchdata中图像坐标和轴位置 设置进对位工具中；
	{
		int nSize1 = m_vvpTargetPlatformAxisPosMultiEx.size();
		int nSize2 = m_vvpObjectPlatformAxisPosMultiEx.size();
		int nSize3 = m_vmpObjectMarkImagePos_MultiEX.size();
		int nSize4 = m_vmpTargetMarkImagePosMultiEx.size();
		bool bTmp = (nExProductIndex >=0) &&  (nExProductIndex <nSize1) &&  (nExProductIndex <nSize2) &&  (nExProductIndex <nSize3)  &&  (nExProductIndex <nSize4);

		if (bTmp)
		{

			vector<CPlatformAxisPos*> tmpObjectPlatformAxisPos;
			vector<CPlatformAxisPos*> tmpTargetPlatformAxisPos;
			tmpObjectPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);
			tmpTargetPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);

			if (m_pPlatformInfo->m_bEnableMultiCalibExtension)
			{
				if (optionInfo.m_bObjectSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{
						tmpObjectPlatformAxisPos.at(jt) = m_vvpObjectPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpObjectMarkImagePos = m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex);
				}
				else
				{
					tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				}

				if (optionInfo.m_bTargetSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{
						tmpTargetPlatformAxisPos.at(jt) = m_vvpTargetPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpTargetMarkImagePos = m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex);
				}
				else
				{
					tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
				}

			}
			else
			{
				tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
			}


			m_pAlignerTool->SetTargetPlatformPos(tmpTargetPlatformAxisPos);
			//m_pAlignerTool->SetTargetMarkSearchResultsInit(tmpTargetImagePos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

			m_pAlignerTool->SetObjectPlatformPos(tmpObjectPlatformAxisPos);
			//m_pAlignerTool->SetObjectMarkSearchResultsAll(tmpObjectImagePos);
			m_pAlignerTool->SetObjectMarkSearchResults(m_mpObjectMarkImagePos);


			//lzk 0417
			std::vector<int> nObjectPosIndex;
			std::vector<int> nTargetPosIndex;
			CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
			BOOL bObjectPositionVirtualMod = pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;
			BOOL bTargetPositionVirtualMod = pAlignerParam->GetProductPositionVirtualInfo().m_bEnableTargetPositionVirtualMode;
			BOOL bTargetAndObjectPositionVirtualMod = m_pAlignerTool->GetEnablePositionVirtualMode();
			int nObjectPositionVirtual = pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
			int nTargetPositionVirtual = pAlignerParam->GetProductPositionVirtualInfo().m_nTargetPositionVirtualMode;
			//对象位置虚拟
			if (bTargetAndObjectPositionVirtualMod && bObjectPositionVirtualMod)
			{
				//对象位置虚拟模式：12拍照34虚拟(BOOL)
				if (nObjectPositionVirtual == 0)
				{
					nObjectPosIndex.push_back(0);
					nObjectPosIndex.push_back(1);
				}
				//对象位置虚拟模式：34拍照12虚拟
				else if (nObjectPositionVirtual == 1)
				{
					nObjectPosIndex.push_back(2);
					nObjectPosIndex.push_back(3);
				}
				//对象位置虚拟模式：13拍照24虚拟
				else if (nObjectPositionVirtual == 2)
				{
					nObjectPosIndex.push_back(0);
					nObjectPosIndex.push_back(2);
				}
				//对象位置虚拟模式：24拍照13虚拟
				else if (nObjectPositionVirtual == 3)
				{
					nObjectPosIndex.push_back(1);
					nObjectPosIndex.push_back(3);
				}

				m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK(nObjectPosIndex);
			}
			else
			{
				m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();
			}

			//目标位置虚拟
			if (bTargetAndObjectPositionVirtualMod && bTargetPositionVirtualMod)
			{
				//目标位置虚拟模式：56拍照78虚拟
				if (nTargetPositionVirtual == 0)
				{
					nTargetPosIndex.push_back(4);
					nTargetPosIndex.push_back(5);
				}
				//目标位置虚拟模式：78拍照56虚拟
				else if (nTargetPositionVirtual == 1)
				{
					nTargetPosIndex.push_back(6);
					nTargetPosIndex.push_back(7);
				}
				//目标位置虚拟模式：57拍照68虚拟
				else if (nTargetPositionVirtual == 2)
				{
					nTargetPosIndex.push_back(4);
					nTargetPosIndex.push_back(6);
				}
				//目标位置虚拟模式：68拍照57虚拟
				else if (nTargetPositionVirtual == 3)
				{
					nTargetPosIndex.push_back(5);
					nTargetPosIndex.push_back(7);
				}
				m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK(nTargetPosIndex);
			}
			else
			{
				m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
			}

		}		
	}
	// end  直接根据searchdata中图像坐标和轴位置 设置进对位工具中；

	m_AlignerTargetSizeInfo.m_nExProductIndex = nExProductIndex;

	if (optionInfo.m_bEnableMutiTarget)
	{
		m_bTargetMarkSearchSucceed=m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex);
		if (m_bTargetMarkSearchSucceed)
		{
			SetTargetMarkSearchResults(m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex));
			m_mpTargetMarkImagePos=m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex);
			int nPosNum = m_pPlatformInfo->m_nPositionNum;
			for (int i=0;i<nPosNum;i++)
			{
				SetTargetPlatformPos(m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i),i);
				SetTempTargetPlatformAxisPos(i,m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i));
			}
			PostMessageShowMutiTargetImagePosToDisplay(m_bTargetMarkSearchSucceed);	
		}	
	}

	CCoordPos platOffset;
	CString strCmd = _T("VACO:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	for(int i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerBenchOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerBenchOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);

	PostMessageUpdateAlignOffsetInfo();

	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);


	m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
	// 检查目标Mark是否已成功定位
	if (eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
	{
		if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
		{
			if (FALSE == m_bTargetMarkSearchSucceed)
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();		
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}
		}
	}

	// 检查对象Mark是否已成功定位
	if (eObjectBench != curProductData.m_pAlignerParam->m_eObjectMarkType)
	{
		if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
		{
			if (FALSE == m_bObjectMarkSearchSucceed)
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CONFIRM_OBJECT_SEARCH_SUCCESS);//_T("请先保证实时对象Mark定位成功" );
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}	
		}

		if (!m_bIsExecuteFixTargetCheck) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
		{
			if (!CheckObjectDistance())
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_DIS_OUT_RANGE);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			if (!CheckObjectAngle())
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("对象角度超过设定标准，对象定位结果可能出错");
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}
		}
	}

	if (!m_bIsExecuteFixTargetCheck) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
	{
		//		CalculateTargetDistance(); 
		if (!CheckTargetDistance())
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_DIS_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			return FALSE;
		}

		if (!CheckTargetAngle())
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");

			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			return FALSE;
		}
	}

	scTimer stAlignTimer;   double dAlignTime(0.0);	
	stAlignTimer.Reset();	

	// 执行对位计算
	CCoordPos cpBenchOffset;
	CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL;	
	if (optionInfo.m_bEnableAlignBenchCenter == TRUE)
	{
		// 执行对位计算
		// 设置图像中心十字基准目标
		SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
		m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
		m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

		// 执行对位
		m_pAlignerTool->SetInnerAlignBench(TRUE);	// 设置为内部图像十字中心基准对位有效
		if(FALSE == ExecuteAlign())
		{
			m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
			// 设置实时的目标
			m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
			m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
			return FALSE;     
		}

		pBenchPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
		cpBenchOffset = GetAlignPlatformPosOffset();

		m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
		// 设置实时的目标
		m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
		m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);		
	}

	// 执行对位计算
	if(FALSE == ExecuteAlign())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
		m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
		return FALSE;     
	}

	// 更新主界面对位精度、次数、对位总时间
	CCoordPos cpProduct = GetAlignProductOffset();

	// 一次对位时，勾选目标补偿时出现异常情况
	double dPosX = pAlignerParam->GetAlnTargetOffsetX();
	double dPosY = pAlignerParam->GetAlnTargetOffsetY();
	CPlatformXYDInfo* platformXYDInfo = (CPlatformXYDInfo*)(m_pVisionASMConfig->m_pPlatformInfo);
	int iPlatformYCoordType = platformXYDInfo->m_nPlatformYCoordType;
	int iPlatformXCoordType = platformXYDInfo->m_nPlatformXCoordType;
	if (eDirectNegative == iPlatformXCoordType) dPosX = -1*dPosX;
	if (eDirectNegative == iPlatformYCoordType) dPosY = -1*dPosY;

	if (pAlignerParam->GetEnableAlnTargetOffset())
	{
		cpProduct.m_dPosX+=dPosX;
		cpProduct.m_dPosY+=dPosY;
	}

	m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
	m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
	m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

	CCoordPos cp = GetAlignPlatformOffset();
	if (pAlignerParam->GetEnableAlnTargetOffset())
	{
		cp.m_dPosX+=dPosX;
		cp.m_dPosY+=dPosY;
	}

	m_apAlignerProcesInfo.nTimes  = 1;
	m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
	m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
	m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;
	m_apAlignerProcesInfo.dTime = stAlignTimer.GetTimeMilli(TRUE);

	dAlignTime=m_apAlignerProcesInfo.dTime;

	PostMessageUpdateAlignerProcessInfo();	

	if(IsAxisMoveOverRun(cp))
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
		m_strStatusBarInfo2.Format(_T(""));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
	m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dAlignTime);
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();				

	// 记录对位精度
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
	{
		CString strTemp;
		CString strOut;					
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
		strOut = strTemp + strOut;
		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}

	// 记录对位时间
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
	{
		CString strTemp;
		CString strOut;						
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), 0, dAlignTime);
		strOut = strTemp + strOut;
		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}	


	CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();

	CCoordPos cpOffset;
	cpOffset = GetAlignPlatformPosOffset();

	if (pAlignerParam->GetEnableAlnTargetOffset())
	{
		cpOffset.m_dPosX += dPosX;
		cpOffset.m_dPosY += dPosY;
		AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
	}

	// 启用中心对位，将中心对位结果写入D20地址
	if(optionInfo.m_bEnableAlignBenchCenter)
	{
		CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
		pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
		pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosX();
		pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
		ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
		if (pPlatformXYDAxisPosMove1!=NULL)
		{
			delete pPlatformXYDAxisPosMove1;
			pPlatformXYDAxisPosMove1 = NULL;					
		}
	}

	if (!pAlignerParam->GetAlignAccordingToPickPlatformEnable())
	{
		ReportPLC_Offset(cpOffset, eCodeOK, pPlatformXYDAxisPos);
	}

	// 对位完成后 清理拍照状态；避免没有更新拍照，以旧的Mark点去执行对位
	if(optionInfo.m_bAlignFinishClearSnapStatus)
	{
		for (int it=0;it<m_mpObjectMarkImagePos.m_vbOK.size();it++)
		{
			m_mpObjectMarkImagePos.m_vbOK.at(it) = FALSE;
		}

		for (int it=0;it<m_mpTargetMarkImagePos.m_vbOK.size();it++)
		{
			m_mpTargetMarkImagePos.m_vbOK.at(it) = FALSE;
		}

		if (nExProductIndex< m_vmpObjectMarkImagePos_MultiEX.size() && nExProductIndex<m_vmpTargetMarkImagePosMultiEx.size())
		{
			m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex) = m_mpObjectMarkImagePos;
			m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex) = m_mpTargetMarkImagePos;
		}
	}

	return TRUE;  

}
BOOL vcXYDVisionAlign::OnlineAlignExFACO_WithSnap_Ex(int nOffsetIndex)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	int nBenchOffsetIndex = -1;
	int nExProductIndex = 0;
	int nTarFixOffsetIndex = 0;

	SetAlignBenchOffsetIndex(nBenchOffsetIndex);
	SetAlignExProductIndex(nExProductIndex);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	m_AlignerTargetSizeInfo.m_nExProductIndex = nExProductIndex;

	CCoordPos platOffset;
	CString strCmd = _T("VACO:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	for(int i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}
	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerBenchOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerBenchOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);
	PostMessageUpdateAlignOffsetInfo();

	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);

	// 执行目标对象拍照;

	std::vector<int> vnTargetPosIndex; // 目标拍照位置序列
	std::vector<int> vnObjectPosIndex; // 对象拍照位置序列
	int nTempPosIndex = 0;

	// 对象目标相机分离 目标后一半位置拍照 、对象前一半位置
	if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
	{
		for(nTempPosIndex = GetPosNum()/2; nTempPosIndex < GetPosNum(); nTempPosIndex++)
		{
			vnTargetPosIndex.push_back(nTempPosIndex);
		}

		for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
		{
			vnObjectPosIndex.push_back(nTempPosIndex);
		}
	}
	else //对象目标相机不分离 目标全部位置拍照
	{
		for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
		{
			vnTargetPosIndex.push_back(nTempPosIndex);
			vnObjectPosIndex.push_back(nTempPosIndex);
		}		
	}

	// 获取轴位置
	{
		CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
		BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
		// 直接读取寄存器地址轴数据
		bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

		if (FALSE == bCommuciateGetPlatformAxisAbsPos)
		{
			if (pPlatformAxisPos!=NULL)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;					
			}

			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}
		else
		{
			for (int k=0;k<vnObjectPosIndex.size();k++)
			{
				int nPosIndex = vnObjectPosIndex[k];
				SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
				SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
				SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
			}

			for (int k=0;k<vnObjectPosIndex.size();k++)
			{
				int nPosIndex = vnTargetPosIndex[k];
				SetTargetPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
				SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
				SetTempTargetPlatformAxisPos(nPosIndex,pPlatformAxisPos);
			}

			m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
			m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
			m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
					pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;	
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}


			if (pPlatformAxisPos)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;
			}
		}
	}


	int nStartPatIndex = 0;
	// 目标拍照
	if (FALSE == OnTargetSearchCandidateExFTGC_Ex(vnTargetPosIndex,nStartPatIndex))
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		

		return FALSE;  
	}
	m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
	// 对象拍照
	SetDefaultObjectSearchMarkPos(TRUE);
	if (FALSE == OnObjectSearchCandidateExFOBC_Ex(vnObjectPosIndex, nStartPatIndex))
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
		if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
		{
			m_strStatusBarInfo2 = m_strStatusBarInfo2 + _T(",") + m_strSnapImgStatuString;
		}
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

		return FALSE;     			
	}
	m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();
	{
		m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
		// 检查目标Mark是否已成功定位
		if (eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
		{
			if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
			{
				if (FALSE == m_bTargetMarkSearchSucceed)
				{
					ReportPLCFunc(eCodeNG);

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
					m_strStatusBarInfo2 = _T("");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();		
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;
				}
			}
		}
	}
	// 检查对象Mark是否已成功定位
	if (eObjectBench != curProductData.m_pAlignerParam->m_eObjectMarkType)
	{
		if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
		{
			if (FALSE == m_bObjectMarkSearchSucceed)
			{
				ReportPLCFunc(eCodeNG);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CONFIRM_OBJECT_SEARCH_SUCCESS);//_T("请先保证实时对象Mark定位成功" );
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}	
		}

		if (!m_bIsExecuteFixTargetCheck) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
		{
			//			CalculateObjectDistance(); 
			if (!CheckObjectDistance())
			{
				ReportPLCFunc(eCodeNG);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_DIS_OUT_RANGE);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}


			if (!CheckObjectAngle())
			{
				ReportPLCFunc(eCodeNG);

				m_bStatusBar = FALSE;
				//				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_ANGLE_OUT_RANGE);
				m_strStatusBarInfo1 = _T("对象角度超过设定标准，对象定位结果可能出错");
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

				return FALSE;
			}


		}
	}

	if (!m_bIsExecuteFixTargetCheck) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
	{
		//		CalculateTargetDistance(); 
		if (!CheckTargetDistance())
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_DIS_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}

		if (!CheckTargetAngle())
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}
	}

	scTimer stAlignTimer;   double dAlignTime(0.0);	
	stAlignTimer.Reset();	

	// 执行对位计算
	CCoordPos cpBenchOffset;
	CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL;	

	// 执行对位计算
	if(FALSE == ExecuteAlign())
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
		m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));

		
		return FALSE;     
	}


	// 更新主界面对位精度、次数、对位总时间
	CCoordPos cpProduct = GetAlignProductOffset();

	// 一次对位时，勾选目标补偿时出现异常情况
	double dPosX = pAlignerParam->GetAlnTargetOffsetX();
	double dPosY = pAlignerParam->GetAlnTargetOffsetY();
	CPlatformXYDInfo* platformXYDInfo = (CPlatformXYDInfo*)(m_pVisionASMConfig->m_pPlatformInfo);
	int iPlatformYCoordType = platformXYDInfo->m_nPlatformYCoordType;
	int iPlatformXCoordType = platformXYDInfo->m_nPlatformXCoordType;
	if (eDirectNegative == iPlatformXCoordType) dPosX = -1*dPosX;
	if (eDirectNegative == iPlatformYCoordType) dPosY = -1*dPosY;


	if (pAlignerParam->GetEnableAlnTargetOffset())
	{
		cpProduct.m_dPosX+=dPosX;
		cpProduct.m_dPosY+=dPosY;
	}

	m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
	m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
	m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

	CCoordPos cp = GetAlignPlatformOffset();
	if (pAlignerParam->GetEnableAlnTargetOffset())
	{
		cp.m_dPosX+=dPosX;
		cp.m_dPosY+=dPosY;
	}

	m_apAlignerProcesInfo.nTimes  = 1;

	m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
	m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
	m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;
	m_apAlignerProcesInfo.dTime = stAlignTimer.GetTimeMilli(TRUE);

	dAlignTime=m_apAlignerProcesInfo.dTime;

	PostMessageUpdateAlignerProcessInfo();	

	if(IsAxisMoveOverRun(cp))
	{

		CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
		pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();


		CCoordPos cpOffset;
		cpOffset = GetAlignPlatformPosOffset();
		if (pAlignerParam->GetEnableAlnTargetOffset())
		{
			cpOffset.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
			cpOffset.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();

			AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
		}

		ReportPLC_Offset(cpOffset,eCodeNG,pPlatformXYDAxisPos);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
		m_strStatusBarInfo2.Format(_T(""));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
	m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dAlignTime);
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();				

	// 记录对位精度
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
	{
		CString strTemp;

		CString strOut;					
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
		strOut = strTemp + strOut;

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}

	// 记录对位时间
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
	{
		CString strTemp;

		CString strOut;						
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), 0, dAlignTime);
		strOut = strTemp + strOut;

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}	

	CCoordPos cpOffset;
	cpOffset = GetAlignPlatformPosOffset();
	CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();


	if (TRUE == optionInfo.m_bAlignOneTime)
	{
		
		if (pAlignerParam->GetEnableAlnTargetOffset())
		{
			cpOffset.m_dPosX += dPosX;
			cpOffset.m_dPosY += dPosY;

			AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
		}
		m_nCurAlignerTimesIndex = 0;
		ReportPLC_Offset(cpOffset, eCodeOK, pPlatformXYDAxisPos);
		return TRUE;
	}

	// 继续对位
	{
		if(m_nCurAlignerTimesIndex!=0 && IsAlignFinished(m_nCurAlignerTimesIndex+1))
		{
			//对位完成 D2写入成功
			m_nCurAlignerTimesIndex = 0;
			if(IsAlignSucceed())
			{
				ReportPLC_Offset(cpOffset, eCodeOK, pPlatformXYDAxisPos);
			}
			else
			{
				ReportPLC_Offset(cpOffset, eCodeNG, pPlatformXYDAxisPos);
			}
			

		}
		else
		{

			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cpOffset.m_dPosX += dPosX;
				cpOffset.m_dPosY += dPosY;

				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
			}
			m_nCurAlignerTimesIndex ++;
			ReportPLC_Offset(cpOffset, eCodeContinued, pPlatformXYDAxisPos);
		}
	}



	// 对位完成后 清理拍照状态；避免没有更新拍照，以旧的Mark点去执行对位
	if(optionInfo.m_bAlignFinishClearSnapStatus)
	{
		for (int it=0;it<m_mpObjectMarkImagePos.m_vbOK.size();it++)
		{
			m_mpObjectMarkImagePos.m_vbOK.at(it) = FALSE;
		}

		if (nExProductIndex< m_vmpObjectMarkImagePos_MultiEX.size() && nExProductIndex<m_vmpTargetMarkImagePosMultiEx.size())
		{
			m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex) = m_mpObjectMarkImagePos;
			//m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex) = m_mpTargetMarkImagePos;
		}
	}

	return TRUE;  

}

BOOL vcXYDVisionAlign::OnlineAlnObjectSearchCandidateExFOBC_Ex(int nPosIndex, int nStartPatIndex)
{
	CString strTempWarningType = _T("");
	CString strCmd = _T("VOBC:");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd))
	{
		return FALSE;
	}
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	m_nCheckObjectDistanceIndex = 0;

	if(nStartPatIndex<0 || nStartPatIndex>=OBJECTCANDIDATENUM || nPosIndex<0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_OBJECT_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_STR_PARAMETER_ERROR); //参数错误
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;
	}

	SetDefaultObjectSearchMarkPos(TRUE);


	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CAlignerParam* pAlignerParam = GetProductAlignerParam();

	//对象光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(0))
		{
			if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	std::vector<int> vnPosIndex;
	int nIndex = 0;

	if (optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜第一个位置。
	{
		nPosIndex = 0;
	}

	if (nPosIndex>=0 && nPosIndex<=3)
	{
		vnPosIndex.push_back(nPosIndex);
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if(nPosIndex==4)
	{
		for(int i = 0; i < GetPosNum(); i++)
		{
			vnPosIndex.push_back(i);
		}
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if (nPosIndex==11) // 所有相机中的前一半编号
	{
		// 兼容凯达扬2VS4的拍照模式
		if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
		{
			int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
			CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);

			if (pCurProductData == NULL)
			{
				return FALSE;
			}

			int nObjectVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
			BOOL bEnableObjectPositionVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;

			if (bEnableObjectPositionVirtualMode)
			{
				switch(nObjectVirtualMode)
				{
				case 0:
					{
						// 位置1、位置2
						vnPosIndex.push_back(0);
						vnPosIndex.push_back(1);

					}
					break;

				case 1:
					{
						// 位置3、位置4
						vnPosIndex.push_back(2);
						vnPosIndex.push_back(3);
					}
					break;

				case 2:
					{
						// 位置1、位置3
						vnPosIndex.push_back(0);
						vnPosIndex.push_back(2);
					}
					break;

				case 3:
					{
						// 位置2、位置4
						vnPosIndex.push_back(1);
						vnPosIndex.push_back(3);
					}
					break;

				default:
					{
						return FALSE;
					}
					break;
				}
			}
			else
			{
				vnPosIndex.push_back(0);
				vnPosIndex.push_back(1);
				vnPosIndex.push_back(2);
				vnPosIndex.push_back(3);
			}

			m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
		}
		else
		{
			if (m_pPlatformInfo->m_bUseAlignFAHLex == TRUE)  // 山本集银-叠片机
			{
				m_nCheckObjectDistanceIndex = 19;
			}
			
			int nPosIndex = 0;

			for(int i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
			m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
		}

	}
	// 针对联得2对4项目增加:对象2相机，目标4相机
	// 对象两个相机同时定位，位置1+位置2
	else if (nPosIndex==19)
	{
		//m_nCheckObjectDistanceIndex = 19;

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		int i = 0;
		for(i = 0; i < GetPosNum()/4; i++)
		{
			nPosIndex = i;
			vnPosIndex.push_back(nPosIndex);
		}
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	// 对象两个相机同时定位，位置3+位置4
	else if (nPosIndex==20)
	{
		//m_nCheckObjectDistanceIndex = 20;

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		int i = 0;
		for(i = 0; i < GetPosNum()/4; i++)
		{
			nPosIndex = i+GetPosNum()/4;
			vnPosIndex.push_back(nPosIndex);
		}
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	//对象13拍照
	else if (nPosIndex==21)
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(0);
		vnPosIndex.push_back(2);
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	//对象24拍照
	else if (nPosIndex==22)
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(1);
		vnPosIndex.push_back(3);
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if (nPosIndex==23)  // 集银叠片机，目标、对象同时拍照
	{
		m_nCheckObjectDistanceIndex = 23;   // iyo
		std::vector<int> vnObjPosIndex;
		int nPosIndex = 0;
		int i = 0;
		for(i = 0; i < GetPosNum()/2; i++)  //0，1
		{
			nPosIndex = i;
			vnObjPosIndex.push_back(nPosIndex);
		}
		std::vector<int> vnTarPosIndex;


		for(i = 0; i < GetPosNum()/2; i++)  //2，3
		{
			nPosIndex = i+GetPosNum()/2;
			vnTarPosIndex.push_back(nPosIndex);
		}
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerTargetAndObjectSearchFOBC(vnTarPosIndex, vnObjPosIndex,0);

	}
	else if (nPosIndex==24)  // 集银叠片机，目标、对象同时拍照
	{
		m_nCheckObjectDistanceIndex = 24;   // iyo
		std::vector<int> vnObjPosIndex;
		int nPosIndex = 0;
		int i = 0;
		for(i = 0; i < GetPosNum()/2; i++)  //0，1
		{
			nPosIndex = i;
			vnObjPosIndex.push_back(nPosIndex);
		}
		std::vector<int> vnTarPosIndex;


		for(i = 0; i < GetPosNum()/2; i++)  //0，1
		{
			nPosIndex = i;
			vnTarPosIndex.push_back(nPosIndex);
		}
		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerTargetAndObjectSearchFOBC(vnTarPosIndex, vnObjPosIndex,1);

	}
	else if (nPosIndex>=256)
	{
		std::vector<int> vnPosIndex;
		nPosIndex = nPosIndex - 255;

		int nMask = 1;
		for (int i=0; i<GetPosNum(); i++)
		{
			int nMaskResult = nMask<<i;

			if ( (nPosIndex & nMaskResult) == nMaskResult )
			{
				vnPosIndex.push_back(i);
			}
		}

		m_bObjectMarkSearchSucceed = OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex);
	}

	m_nCheckObjectDistanceIndex = 0;

	return m_bObjectMarkSearchSucceed;

}

BOOL vcXYDVisionAlign::OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(std::vector<int> vnPosIndex, int nStartPatIndex)
{
	scTimer stSearchTimer;	// 搜索时间
	stSearchTimer.Reset();

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	CString strCmd = _T("VOBC:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckPlatformInfo(strCmd) || !CheckObjectSearchTool(strCmd)/*m_bValidObjectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);
		return FALSE;
	}
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	int i = 0;
	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_OBJECT_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;
		return FALSE;
	}

	for(i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_OBJECT_SEARCH_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;
			return FALSE;
		}    	
	}

	


	// 对象模板曝光切换
	{
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableObjectSearchShutter(nStartPatIndex);
		BOOL bObjectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);
		
		if (SystempOptionSearchShutter /*&& bObjectCandidate*/ && bObjectShutter /*&& (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	

			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eObjectSearchCameraExposure);
			}

			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
	scTimer stCommuciateTimer; double dCommuciateTime(0.0);

	stCommuciateTimer.Reset();
	BOOL bCommuciateGetPlatformAxisAbsPos = FALSE;
	if (!optionInfo.m_bAskStagePosEachMove && m_bAutoAlign == TRUE && m_nAlignTime > 1)
	{
		*pPlatformAxisPos = m_CurPlatformAxisPos;
		bCommuciateGetPlatformAxisAbsPos = TRUE;
	}
	else
	{
		if (optionInfo.m_bObjectPosSame == TRUE)
		{

			if (optionInfo.m_bAskStagePosEachMove && m_bAutoAlign == TRUE && m_nAlignTime > 1)
			{
				bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);
			}
			else
			{
				if (m_vpObjectPlatformAxisPos.size() <= 0 || m_vpObjectPlatformAxisPos.at(vnPosIndex[0]) == NULL)
				{
					bCommuciateGetPlatformAxisAbsPos = FALSE;
				}
				else
				{
					*pPlatformAxisPos = *((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(vnPosIndex[0]));
					bCommuciateGetPlatformAxisAbsPos = TRUE;
				}	
			}
		}
		else
		{
			if (m_bCmdParamAxisPosEanble == TRUE)		// 指令参数轴位置有效
			{
				if (m_bAutoAlign == TRUE && m_nAlignTime > 1)	// 全自动对位且不是第1次对位时通信获取轴位置
				{					
					bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);						
				}
				else	// 不是全自动对位，或者全自动第1次对位时，指令参数获取轴位置
				{
					bCommuciateGetPlatformAxisAbsPos = CommParamGetPlatformAxisAbsPos(pPlatformAxisPos);
				}
			}
			else
			{
				bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);				
			}

		}

	}
																							 
	if (m_bAutoAlign == TRUE && m_nAlignTime > 1)	// 全自动对位且不是第1次对位时通信获取轴位置
	{
		SysPlatformInfo sysPlatformInfo;
		GetSysPlatformInfo(sysPlatformInfo);
		if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType) //如果是XY+D平台，D角度不累加
		{						
			if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
			{
				if (optionInfo.m_bTargetAngleMulAlignerXYPD)
				{

				}
				else
				{
					pPlatformAxisPos->m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(vnPosIndex[0]))->m_dAngle;
				}

			}
			else
			{
				pPlatformAxisPos->m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(vnPosIndex[0]))->m_dAngle;
			}

		}
	}

	if (!bCommuciateGetPlatformAxisAbsPos)
	{
		if (pPlatformAxisPos!=NULL)
		{
			delete pPlatformAxisPos;
			pPlatformAxisPos = NULL;					
		}

		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;

		OnFailedCommuciateGetPlatformAxisAbsPos(strCmd);
		return FALSE;
	}
	else
	{ 
		for(i=0; i<nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
			SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
			if ((/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == TRUE && m_nAlignTime == 1) 
				|| (/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == FALSE))
			{
				SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
			}

		}

		m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
		m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
		m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
		// 记录当前平台各个轴的绝对位置
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
		{
			CString strTemp;
			CString strInfo;
			strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
				pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
			strInfo = strTemp + strInfo;
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);
		}

		if (pPlatformAxisPos)
		{
			delete pPlatformAxisPos;
			pPlatformAxisPos = NULL;
		}
	}

	dCommuciateTime = stCommuciateTimer.GetTimeMilli(FALSE);
	CString str;
	std::vector<CPlatformXYAxisPos*> vpCamPlatformXYAxisPos;
	CPlatformXYAxisPos* pCamPlatformXYAxisPos = NULL;		


	// 3.开启定位
	scTimer stObjectSearchTimer; double dObjectSearchTime(0.0);
	stObjectSearchTimer.Reset();

	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	int nDelayTime2 = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime2();
	BOOL bEnableDelayTime2 = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableObjectSearchDelayTime2();

	// 自动对位过程中
	if (m_bAutoAlign == TRUE)
	{
		if (TRUE == bEnableDelayTime2)
		{
			if (1 == m_nAlignTime)
			{
				Sleep(nDelayTime);
			}
			else
			{
				Sleep(nDelayTime2);
			}
		}
		else
		{
			Sleep(nDelayTime);
		}

	}
	else
	{
		Sleep(nDelayTime);
	}

	std::vector<SearchMode> vsmSearchMode;	

	for(i = 0; i < nPosNum; i++)
	{
		nPosIndex = vnPosIndex[i];
		vsmSearchMode.push_back(eObjectSearch);	// 
	}

	m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eObjectSearch);


	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}

	// 定位
	BOOL bObjectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateEnable();
	BOOL bObjectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateSyncEnable();
	BOOL bSearchSucceed = FALSE;
	if (bObjectSearchCandidate == FALSE)
	{
		for(i = 0; i < nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			m_vnObjectSearchStartPatIndex[nPosIndex] = 0;
			m_vnObjectSearchEndPatIndex[nPosIndex] = 0;
		}
		bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
	}
	else
	{
		if (bObjectSearchSync == TRUE)
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
			{
				nTempStartPatIndex = nStartPatIndex;
			}
			else if (nStartPatIndex == 9)
			{
				nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[vnPosIndex[0]];
			}
			else
			{
				nTempStartPatIndex = 0;
			}
			nTempEndPatIndex = OBJECTCANDIDATENUM-1;

			int j = 0;
			for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
			{
				if (j > 0 && (FALSE == GetAlnObjectSearchCandidatePatEnable(j)))
				{
					continue;
				}

				for(i = 0; i < nPosNum; i++)
				{
					nPosIndex = vnPosIndex[i];
					m_vnObjectSearchStartPatIndex[nPosIndex] = j;
					m_vnObjectSearchEndPatIndex[nPosIndex] = j;
				}			
				bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

				if (bSearchSucceed)
				{
					break;
				}	
			}
		}
		else
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
				{
					nTempStartPatIndex = nStartPatIndex;
				}
				else if (nStartPatIndex == 9)
				{
					nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[nPosIndex];
				}
				else
				{
					nTempStartPatIndex = 0;
				}
				nTempEndPatIndex = OBJECTCANDIDATENUM-1;

				m_vnObjectSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
				m_vnObjectSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
			}
			bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		}
	}



	if (FALSE == bSearchSucceed)
	{
		if (optionInfo.m_bSearchTimes)
		{
			for (int k = 0; k < optionInfo.m_nSearchTimes; k++)
			{
				//定位延时	 m_nSearchDelayTime
				Sleep(optionInfo.m_nSearchDelayTime);
				// 采集搜索图像
				if (!SnapSearchImage(vnPosIndex))
				{
					if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;
					OnFailedSnapSearchImage(strCmd);
					return FALSE;
				}

				// 定位
				BOOL bObjectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateEnable();
				BOOL bObjectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectCandidateSyncEnable();
				if (bObjectSearchCandidate == FALSE)
				{
					for(i = 0; i < nPosNum; i++)
					{
						nPosIndex = vnPosIndex[i];
						m_vnObjectSearchStartPatIndex[nPosIndex] = 0;
						m_vnObjectSearchEndPatIndex[nPosIndex] = 0;
					}
					bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				}
				else
				{
					if (bObjectSearchSync == TRUE)
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
						{
							nTempStartPatIndex = nStartPatIndex;
						}
						else if (nStartPatIndex == 9)
						{
							nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[vnPosIndex[0]];
						}
						else
						{
							nTempStartPatIndex = 0;
						}
						nTempEndPatIndex = OBJECTCANDIDATENUM-1;

						int j = 0;
						for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
						{
							if (j > 0 && (FALSE == GetAlnObjectSearchCandidatePatEnable(j)))
							{
								continue;
							}

							for(i = 0; i < nPosNum; i++)
							{
								nPosIndex = vnPosIndex[i];
								m_vnObjectSearchStartPatIndex[nPosIndex] = j;
								m_vnObjectSearchEndPatIndex[nPosIndex] = j;
							}			
							bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

							if (bSearchSucceed)
							{
								break;
							}	
						}
					}
					else
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						for(i = 0; i < nPosNum; i++)
						{
							nPosIndex = vnPosIndex[i];
							if (nStartPatIndex>=0 && nStartPatIndex < OBJECTCANDIDATENUM)
							{
								nTempStartPatIndex = nStartPatIndex;
							}
							else if (nStartPatIndex == 9)
							{
								nTempStartPatIndex = m_vnObjectSearchSuccessPatIndex[nPosIndex];
							}
							else
							{
								nTempStartPatIndex = 0;
							}
							nTempEndPatIndex = OBJECTCANDIDATENUM-1;

							m_vnObjectSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
							m_vnObjectSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
						}
						bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
					}
				}

				if (bSearchSucceed)
				{
					break;
				}
			}
		}
	}


	if (TRUE == bSearchSucceed)
	{
		m_bStatusBar = TRUE;
		/*m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();*/
	}
	else
	{
		if (optionInfo.m_bManualAfObjSearchFail)
		{
			//if (optionInfo.m_bSetVisionManualSearchToPLC)
			//{
			//	SetPlcManualSearch(TRUE);
			//}

			if (FALSE ==ExcuteManualSearch(1,vnPosIndex))
			{
				//if (optionInfo.m_bSetVisionManualSearchToPLC)
				//{
				//	SetPlcManualSearch(FALSE);
				//}
				bSearchSucceed = FALSE;
			}
            else
            {
                bSearchSucceed = TRUE;
            }
            
			//if (optionInfo.m_bSetVisionManualSearchToPLC)
			//{
			//	SetPlcManualSearch(FALSE);
			//}

		}
	
	}

	 m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
	 //m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);


	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveObjectSnapVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eObjectSearch);
	}
	if (bSearchSucceed)
	{
		
	}
	else
	{
		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
		m_strStatusBarInfo2 = m_StrSearchErrorInfo;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_SEARCH);
		m_strStatusBarInfo1 += m_StrSearchErrorInfo;
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	// 5. 设置对位工具
	SetMultiEXObjectMarkSearchResultsWithCombinationCode(m_mpObjectMarkImagePos);
	SetObjectMarkSearchResults(m_mpObjectMarkImagePos);

	// 计算对象距离
	if (!optionInfo.m_bExeObjectCheckModeWhenAlign && !CheckObjectDistance())
	{
		if(m_bObjectSearchTest) ReportPLCFunc(eCodeNG);;

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_OBJECT_LEN_OVERFLOW); //__T("对象距离超过设定标准，对象定位结果可能出错");
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		
		if (m_pVisionASMConfig->m_DataRecord.m_bSaveNGImage)
		{		
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				CTime t = CTime::GetCurrentTime();	
				m_vstrTemp.at(nPosIndex).Format(_T("%04d-%02d-%02d-%02d-%02d-%02d-CH%02d-ImageObjectSearchFailed-Check.bmp"),
					t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), nPosIndex);

				SYSTEMTIME sys; 
				GetLocalTime(&sys); 
				m_vstrTemp.at(nPosIndex).Format(_T("%04d-%02d-%02d-%02d-%02d-%02d-%03d-CH%02d-ImageObjectSearchFailed-Check.bmp"),
					sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, nPosIndex);

				m_vSaveAlignerObjectSearchImageFailIndex.at(nPosIndex) = m_vSaveAlignerObjectSearchImageFailIndex.at(nPosIndex) + 1;																			
				if (m_pVisionASMConfig->m_DataRecord.m_bDataRecordTime == FALSE)
				{
					m_vstrTemp.at(nPosIndex).Format(_T("ImageObjectSearchFailed-CH%02d-%03d-Check.bmp"), 
						nPosIndex,m_vSaveAlignerObjectSearchImageFailIndex.at(nPosIndex));
				}
				SaveImage(nPosIndex,m_vImageSearch[nPosIndex],m_strDataRecord + _T("\\") + m_vstrTemp.at(nPosIndex));
				if(m_vSaveAlignerObjectSearchImageFailIndex.at(nPosIndex) > m_pVisionASMConfig->m_DataRecord.m_nSaveFailedImageNum)
				{
					m_vSaveAlignerObjectSearchImageFailIndex.at(nPosIndex) = 0;
				}
			}
		}

		return FALSE;
	}

	//**************************************************************

	if ((/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == TRUE && m_nAlignTime == 1) 
		|| (/*m_pSystempOptionConfig->m_bSaveAlignPos && */ m_bAutoAlign == FALSE))
	{
		SaveObjectPosInfo(GetTotalProuductData()->GetCurProductIndex());
	}

	// 记录图像搜索数据
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		CString strTemp;
		CString strOut;
		CCoordPos cp;
		strOut = strTemp + _T("ObjectSearch: ");		
		for (int i=0; i<m_mpObjectMarkImagePos.GetMarkImagePosNum(); i++)
		{
			strTemp = _T("");
			cp = m_mpObjectMarkImagePos.GetMarkImagePos(i);

			strTemp.Format(_T("MarkPos%d(%.2f,%.2f) "), i, cp.GetPosX(), cp.GetPosY());

			strOut += strTemp;
		}

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}


	dObjectSearchTime = stObjectSearchTimer.GetTimeMilli(FALSE);

	if (optionInfo.m_bEnableAlignAfterSearch && m_nAlignType!=-1)
	{
		 BOOL m_bFOBCAndFTGC = ExecuteAlignAfterFOBCAndFTGC_Ex(vnPosIndex,1);


		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		return m_bFOBCAndFTGC;
	}

	if(m_bObjectSearchTest)
	{
		ReportPLCFunc(eCodeOK);

		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineAlnTargetSearchCandidateExFTGC_Ex(int nPosIndex, int nStartPatIndex)
{
	CString strCmd = _T("VTGC:");
	CString strTempWarningType = _T("");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd)/*NULL == m_pPlatformInfo*/)
	{
		return FALSE;
	}
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	m_nCheckTargetDistanceIndex = 0;

	if(nStartPatIndex<0 || nStartPatIndex>=TARGETCANDIDATENUM || nPosIndex<0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_TARGET_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_STR_PARAMETER_ERROR); //参数错误
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;
	}

	// 隐藏对象结果图形			
	int nPosNum = m_pPlatformInfo->m_nPositionNum;
	std::vector<int> vnObjectPosIndex;
	int nObjectPosIndex = 0;
	int i = 0;
	for(i = 0; i < nPosNum; i++)
	{
		nObjectPosIndex = i;
		vnObjectPosIndex.push_back(nObjectPosIndex);
	}	

	m_cVisionAlignGui.SetPosGuiInvisible(vnObjectPosIndex, eObjectSearch);

	if (nPosIndex == 19)
	{
		//m_nCheckTargetDistanceIndex = 19;
	}
	else if (nPosIndex == 20)
	{
		//m_nCheckTargetDistanceIndex = 20;
	}
	SetDefaultTargetSearchMarkPos(TRUE);

	//目标光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(1))
		{
			if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	m_bTagetLightFinish = TRUE; //多次对位过程中拍目标时再次进行对位需进行切换

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CAlignerParam* pAlignerParam = GetProductAlignerParam();
	if (NULL != pAlignerParam && optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜目标搜后一半的第一个位置。
	{
		nPosIndex = 0;
		if (sysPlatformInfo.m_bTargetObjectCamSeparate)
		{
			nPosIndex = sysPlatformInfo.m_nPositionNum/2 ;
		}
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(nPosIndex);
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
		m_nCheckTargetDistanceIndex = 0;
		return m_bTargetMarkSearchSucceed;
	}

	if (nPosIndex>=0 && nPosIndex <= 3)	//编号0～3的相机中的一个
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(nPosIndex);

		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if(nPosIndex==4)	// 所有相机
	{
		std::vector<int> vnPosIndex;

		for(i = 0; i < GetPosNum(); i++)
		{
			vnPosIndex.push_back(i);
		}
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if (nPosIndex>=9 && nPosIndex<=10 ) //编号2～3的相机中的一个
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(nPosIndex - 7);
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if (nPosIndex==11) // 所有相机中的后一半编号
	{
		// 兼容凯达扬2VS4的拍照模式
		if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
		{
			int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
			CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);

			if (pCurProductData == NULL)
			{
				return FALSE;
			}

			std::vector<int> vnPosIndex;
			int nTargetVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_nTargetPositionVirtualMode;
			BOOL bEnableTargetPositionVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableTargetPositionVirtualMode;

			if (bEnableTargetPositionVirtualMode)
			{
				switch(nTargetVirtualMode)
				{
				case 0:
					{
						// 位置5、位置6
						vnPosIndex.push_back(4);
						vnPosIndex.push_back(5);

					}
					break;

				case 1:
					{
						// 位置7、位置8
						vnPosIndex.push_back(6);
						vnPosIndex.push_back(7);
					}
					break;

				case 2:
					{
						// 位置5、位置7
						vnPosIndex.push_back(4);
						vnPosIndex.push_back(6);
					}
					break;

				case 3:
					{
						// 位置6、位置8
						vnPosIndex.push_back(5);
						vnPosIndex.push_back(7);
					}
					break;

				default:
					{
						return FALSE;
					}
					break;
				}
			}
			else
			{
				vnPosIndex.push_back(4);
				vnPosIndex.push_back(5);
				vnPosIndex.push_back(6);
				vnPosIndex.push_back(7);
			}

			m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);

		}
		else
		{
			std::vector<int> vnPosIndex;
			int nPosIndex = 0;
			for(i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i + GetPosNum()/2;
				vnPosIndex.push_back(nPosIndex);
			}

			m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
		}

	}
	//目标对象分离，目标单个位置拍照的情况
	else if (nPosIndex >= 14 && nPosIndex <= 17)
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(nPosIndex - 10);
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	// 针对联得2对4项目增加:对象4相机，目标2相机
	// 目标位置5+位置6
	else if (nPosIndex == 19)
	{
		//m_nCheckTargetDistanceIndex = 19;

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		for(i = 0; i < GetPosNum()/4; i++)
		{
			nPosIndex = i + GetPosNum()/2;
			vnPosIndex.push_back(nPosIndex);
		}
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	// 目标位置7+位置8
	else if (nPosIndex == 20)
	{
		//m_nCheckTargetDistanceIndex = 20;  

		std::vector<int> vnPosIndex;
		int nPosIndex = 0;
		for(i = 0; i < GetPosNum()/4; i++)
		{
			nPosIndex = (i+GetPosNum()/4) + GetPosNum()/2;
			vnPosIndex.push_back(nPosIndex);
		}
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	//目标57拍照
	else if (nPosIndex == 21)
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(4);
		vnPosIndex.push_back(6);
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	////目标68拍照
	else if (nPosIndex == 22)
	{
		std::vector<int> vnPosIndex;
		vnPosIndex.push_back(5);
		vnPosIndex.push_back(7);
		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}
	else if (nPosIndex>=256)
	{
		std::vector<int> vnPosIndex;
		nPosIndex = nPosIndex - 255;

		int nMask = 1;
		for (int i=0; i<GetPosNum(); i++)
		{
			int nMaskResult = nMask<<i;

			if ( (nPosIndex & nMaskResult) == nMaskResult )
			{
				vnPosIndex.push_back(i);
			}
		}

		m_bTargetMarkSearchSucceed = OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(vnPosIndex, nStartPatIndex);
	}

	m_nCheckTargetDistanceIndex = 0;

	return m_bTargetMarkSearchSucceed;

}

BOOL vcXYDVisionAlign::OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(std::vector<int> vnPosIndex, int nStartPatIndex)
{
	
	scTimer stSearchTimer;	// 搜索时间
	stSearchTimer.Reset();
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	CString strCmd = _T("VTGC:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckTargetSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidTargetSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;
		return FALSE;
	}
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	int i = 0;
	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_TARGET_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	for(i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_TARGET_SEARCH_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;
			return FALSE;
		}    	
	}

	

	// 目标模板曝光切换
	{
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableTargetSearchShutter(nStartPatIndex);
		BOOL bTargetShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bTargetCandidate*/ && bTargetShutter /*&& (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;

			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eTargetSearchCameraExposure);
			}

			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	
	
	
	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	//读码
	CString strProductID = _T("");
	//多次对位只有第0次才获取SN码
	/*if((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendInspectGetSnOnce&&m_nCurAlignerTimesIndex==0)
	||(!pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendInspectGetSnOnce)
	)*/
		if ( (pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
		{
			if (!CommunicateGetAlignProcessProductID(strProductID))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品ID读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}

			m_BendInspectResultInfo.m_strProductID = strProductID;
			m_strProductID = strProductID;
		}


	// 1. 通信获取当前平台各个轴的绝对位置
	CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
	BOOL bCommuciateGetPlatformAxisAbsPos = FALSE;
	if (optionInfo.m_bTargetPosSame == TRUE)
	{
		if (m_vpTargetPlatformAxisPos.size() <= 0 
			|| m_vpTargetPlatformAxisPos.at(vnPosIndex[0]) == NULL)
		{
			bCommuciateGetPlatformAxisAbsPos = FALSE;
		}
		else
		{
			*pPlatformAxisPos = *((CPlatformXYDAxisPos*)m_vpTargetPlatformAxisPos.at(vnPosIndex[0]));
			bCommuciateGetPlatformAxisAbsPos = TRUE;
		}		
	}
	else
	{
		if (m_bCmdParamAxisPosEanble == TRUE)		// 指令参数轴位置有效
		{		
			bCommuciateGetPlatformAxisAbsPos = CommParamGetPlatformAxisAbsPos(pPlatformAxisPos);
		}
		else
		{
			bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);
		}
	}

	if (!bCommuciateGetPlatformAxisAbsPos)	
	{
		if (pPlatformAxisPos!=NULL)
		{
			delete pPlatformAxisPos;
			pPlatformAxisPos = NULL;					
		}

		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

		OnFailedCommuciateGetPlatformAxisAbsPos(strCmd);
		return FALSE;
	}
	else
	{ 
		for (i=0;i<nPosNum;i++)
		{
			nPosIndex = vnPosIndex[i];
			SetTargetPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
			SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
			SetTempTargetPlatformAxisPos(nPosIndex,pPlatformAxisPos);
			if (optionInfo.m_bEnableMutiTarget)
			{
				SetMutiTargetPlatformPos(pPlatformAxisPos,nPosIndex);
			}
		}
		m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
		m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
		m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
		// 记录当前平台各个轴的绝对位置
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
		{
			CString strTemp;
			CString strInfo;
			strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_TARGET_SEARCH_PLATFORM_POS), 
				pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
			strInfo = strTemp + strInfo;

			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);
		}

		if (pPlatformAxisPos)
		{
			delete pPlatformAxisPos;
			pPlatformAxisPos = NULL;
		}
	}


	// 2. 通信获取当前所有相机平台各个轴的绝对位置
	std::vector<CPlatformXYAxisPos*> vpCamPlatformXYAxisPos;
	CPlatformXYAxisPos* pCamPlatformXYAxisPos = NULL;		

	if (0/*(m_pPlatformInfo->m_eCamPlatformType!=eCamPlatformSepFix) && (m_pPlatformInfo->m_eCamPlatformType!=eCamPlatformShareX)*/)
	{
		for(i=0; i<m_pPlatformInfo->m_nCamNum; i++)
		{
			pCamPlatformXYAxisPos = new CPlatformXYAxisPos;
			vpCamPlatformXYAxisPos.push_back(pCamPlatformXYAxisPos);      
		}

		if (FALSE == CommuciateGetCamPlatformAxisAbsPos(vpCamPlatformXYAxisPos))
		{
			if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_GET_CAMERA_PLATFORM_POS_FAILED_II);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			for (i=0; i<vpCamPlatformXYAxisPos.size(); i++)
			{
				if (vpCamPlatformXYAxisPos.at(i)!=NULL)
				{
					delete vpCamPlatformXYAxisPos.at(i);
					vpCamPlatformXYAxisPos.at(i) = NULL;
				}
			}

			return FALSE;
		}
		else
		{
			// 记录当前相机平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;

				CString strInfo;
				strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_CAM_PLATFORM_POS);
				strInfo = strTemp + strInfo;

				for (int i=0; i<vpCamPlatformXYAxisPos.size(); i++)
				{
					strTemp = _T("");
					strTemp.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CAM_PLATFORM_POS), i, vpCamPlatformXYAxisPos.at(i)->m_dPosX, vpCamPlatformXYAxisPos.at(i)->m_dPosY);
					strInfo = strInfo + strTemp;
				}

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

			for (i=0; i<vpCamPlatformXYAxisPos.size(); i++)
			{
				if (vpCamPlatformXYAxisPos.at(i)!=NULL)
				{
					delete vpCamPlatformXYAxisPos.at(i);
					vpCamPlatformXYAxisPos.at(i) = NULL;
				}
			}
		}	
	}


	// 3.开启定位
	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetSearchDelayTime();
	Sleep(nDelayTime);

	// 准备开始定位			
	//	std::vector<int> vnPosIndex;
	std::vector<SearchMode> vsmSearchMode;	
	// 定义全部位置向量，包含0/1对象和2/3目标
	std::vector<int> vnAllPosIndex;
	int nAllPosIndex = 0;

	for(i = 0; i < nPosNum; i++)
	{
		nPosIndex = vnPosIndex[i];
		vsmSearchMode.push_back(eTargetSearch);	// 

		if (nPosIndex>=GetPosNum()/2)
		{
			nAllPosIndex = nPosIndex-GetPosNum()/2;
			vnAllPosIndex.push_back(nAllPosIndex);

			nAllPosIndex = nPosIndex;
			vnAllPosIndex.push_back(nAllPosIndex);
		}

	}

	// 如果目标和对象分离，并且关联标定不使用靶标，不读取轴位置，则将目标图像坐标映射到对象图像中，刷新所有图像的目标十字和文字GUI
	if (TRUE == m_pPlatformInfo->m_bTargetObjectCamSeparate/* &&  m_pSystempOptionConfig->m_bShowSepTargetPos*//* && FALSE == m_pPlatformInfo->m_bTargetCalibUseBoard && TRUE == m_pPlatformInfo->m_bTarObjCamSepNoAxisCalib*/)
	{
		m_cVisionAlignGui.SetPosGuiInvisible(vnAllPosIndex, eTargetSearch);
	}
	else
	{
		m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eTargetSearch);
	}


	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}

	// 定位
	BOOL bTargetSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateEnable();
	BOOL bTargetSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateSyncEnable();
	BOOL bSearchSucceed = FALSE;
	if (bTargetSearchCandidate == FALSE)
	{
		for(i = 0; i < nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			m_vnTargetSearchStartPatIndex[nPosIndex] = 0;
			m_vnTargetSearchEndPatIndex[nPosIndex] = 0;
		}
		bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
	}
	else
	{
		if (bTargetSearchSync == TRUE)
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
			{
				nTempStartPatIndex = nStartPatIndex;
			}
			else if (nStartPatIndex == 9)
			{
				nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[vnPosIndex[0]];
			}
			else
			{
				nTempStartPatIndex = 0;
			}
			nTempEndPatIndex = TARGETCANDIDATENUM-1;

			int j = 0;
			for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
			{
				if (j > 0 && (FALSE == GetAlnTargetSearchCandidatePatEnable(j)))
				{
					continue;
				}

				for(i = 0; i < nPosNum; i++)
				{
					nPosIndex = vnPosIndex[i];
					m_vnTargetSearchStartPatIndex[nPosIndex] = j;
					m_vnTargetSearchEndPatIndex[nPosIndex] = j;
				}
				bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				if (bSearchSucceed)
				{
					break;
				}
			}
		}
		else
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
				{
					nTempStartPatIndex = nStartPatIndex;
				}
				else if (nStartPatIndex == 9)
				{
					nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[nPosIndex];
				}
				else
				{
					nTempStartPatIndex = 0;
				}
				nTempEndPatIndex = TARGETCANDIDATENUM-1;

				m_vnTargetSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
				m_vnTargetSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
			}
			bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		}
	}


	if (FALSE == bSearchSucceed)
	{
		if (optionInfo.m_bSearchTimes)
		{
			for (int k = 0; k < optionInfo.m_nSearchTimes; k++)
			{
				//定位延时	 m_nSearchDelayTime
				Sleep(optionInfo.m_nSearchDelayTime);
				// 采集搜索图像
				if (!SnapSearchImage(vnPosIndex))
				{
					if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

					OnFailedSnapSearchImage(strCmd);
					return FALSE;
				}

				// 定位
				BOOL bTargetSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateEnable();
				BOOL bTargetSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetTargetCandidateSyncEnable();
				if (bTargetSearchCandidate == FALSE)
				{
					for(i = 0; i < nPosNum; i++)
					{
						nPosIndex = vnPosIndex[i];
						m_vnTargetSearchStartPatIndex[nPosIndex] = 0;
						m_vnTargetSearchEndPatIndex[nPosIndex] = 0;
					}
					bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
				}
				else
				{
					if (bTargetSearchSync == TRUE)
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
						{
							nTempStartPatIndex = nStartPatIndex;
						}
						else if (nStartPatIndex == 9)
						{
							nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[vnPosIndex[0]];
						}
						else
						{
							nTempStartPatIndex = 0;
						}
						nTempEndPatIndex = TARGETCANDIDATENUM-1;

						int j = 0;
						for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
						{
							if (j > 0 && (FALSE == GetAlnTargetSearchCandidatePatEnable(j)))
							{
								continue;
							}

							for(i = 0; i < nPosNum; i++)
							{
								nPosIndex = vnPosIndex[i];
								m_vnTargetSearchStartPatIndex[nPosIndex] = j;
								m_vnTargetSearchEndPatIndex[nPosIndex] = j;
							}
							bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
							if (bSearchSucceed)
							{
								break;
							}
						}
					}
					else
					{
						int nTempStartPatIndex, nTempEndPatIndex;
						for(i = 0; i < nPosNum; i++)
						{
							nPosIndex = vnPosIndex[i];
							if (nStartPatIndex>=0 && nStartPatIndex < TARGETCANDIDATENUM)
							{
								nTempStartPatIndex = nStartPatIndex;
							}
							else if (nStartPatIndex == 9)
							{
								nTempStartPatIndex = m_vnTargetSearchSuccessPatIndex[nPosIndex];
							}
							else
							{
								nTempStartPatIndex = 0;
							}
							nTempEndPatIndex = TARGETCANDIDATENUM-1;

							m_vnTargetSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
							m_vnTargetSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
						}
						bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
					}
				}

				if (bSearchSucceed)
				{
					break;
				}
			}
		}
	}

	if (TRUE == bSearchSucceed)
	{
		m_bStatusBar = TRUE;
		/*m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();*/
	}
	else
	{
		if (optionInfo.m_bManualAfTarSearchFail)
		{
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉弹出手动搜索框
				SetPlcManualSearch_EX(TRUE);
			}

			if (FALSE == ExcuteManualSearch(0,vnPosIndex))
			{
				//if (optionInfo.m_bSetVisionManualSearchToPLC)
				//{
				//	// 通知PLC，视觉关闭手动搜索框
				//	SetPlcManualSearch(FALSE);
				//}
				bSearchSucceed = FALSE;
			}
			else
			{
				bSearchSucceed = TRUE;
			}
			if (optionInfo.m_bSetVisionManualSearchToPLC)
			{
				// 通知PLC，视觉关闭手动搜索框
				SetPlcManualSearch_EX(FALSE);
			}


		}

	}

	// 如果目标和对象分离，并且关联标定不使用靶标，不读取轴位置，则将目标图像坐标映射到对象图像中，刷新所有图像的目标十字和文字GUI
	if (TRUE == m_pPlatformInfo->m_bTargetObjectCamSeparate &&  m_pSystempOptionConfig->m_bShowSepTargetPos/* && FALSE == m_pPlatformInfo->m_bTargetCalibUseBoard && TRUE == m_pPlatformInfo->m_bTarObjCamSepNoAxisCalib*/)
	{
		int nHomoPosIndex = 0;
		std::vector<int> vnHomoPosIndex;			// 源目标位置
		vnHomoPosIndex = vnPosIndex;

		// 对目标2和3循环进行映射
		for (int i = 0; i<vnHomoPosIndex.size(); i++)
		{
			nHomoPosIndex = vnHomoPosIndex.at(i);
			if (TRUE == m_mpTargetMarkImagePos.GetIsMarkImagePosOK(nHomoPosIndex))
			{
				CCoordPos ObjImgPos;
				CCoordPos TarImgPos = m_mpTargetMarkImagePos.GetMarkImagePos(nHomoPosIndex);

				// 根据目标图像到对象图像映射结果，将目标图像坐标映射到对象图像中
				if (!m_pAlignerTool->ConvertTarImgPosToObjImgPosUsingImageHomographyProject(ObjImgPos, nHomoPosIndex, TarImgPos))
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1.Format(_T("执行目标图像坐标映射对象图像失败"));
					m_strStatusBarInfo2.Format(_T(""));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				}
				// 将映射后图像坐标赋值给m_mpTargetMarkImagePos
				m_mpTargetMarkImagePos.SetMarkImagePos(nHomoPosIndex - GetPosNum()/2, TRUE, ObjImgPos);
			}
		}

		m_cVisionAlignGui.SetPosSearchResult(vnAllPosIndex, eTargetSearch);
	}
	else
	{
		m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
	}

	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveTargetSnapVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eTargetSearch);
	}
	if (bSearchSucceed)
	{

	}
	else
	{
		// SendCommInfo(CMD_TGG_SEARCH_SEARCH_ERR);
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
		m_strStatusBarInfo2 = m_StrSearchErrorInfo;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_SEARCH);
		m_strStatusBarInfo1 += m_StrSearchErrorInfo;
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	// 5. 设置对位工具
	m_vmpTargetMarkImagePosMultiEx.at(m_pAlignerTool->GetAlignExProductIndex()) = m_mpTargetMarkImagePos;
	SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
	if (optionInfo.m_bEnableMutiTarget)
	{
		m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex) = m_mpTargetMarkImagePos;
		m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex)=TRUE;
	}

	if (!optionInfo.m_bExeTargetCheckModeWhenAlign && !CheckTargetDistance())
	{
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_LEN_OVERFLOW);//_T("目标距离超过设定标准，目标定位结果可能出错");
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		if (m_pVisionASMConfig->m_DataRecord.m_bSaveNGImage)
		{	
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				CTime t = CTime::GetCurrentTime();	
				m_vstrTemp.at(nPosIndex).Format(_T("%04d-%02d-%02d-%02d-%02d-%02d-CH%02d-ImageTargetSearchFailed-Check.bmp"),
					t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), nPosIndex);

				SYSTEMTIME sys; 
				GetLocalTime(&sys); 
				m_vstrTemp.at(nPosIndex).Format(_T("%04d-%02d-%02d-%02d-%02d-%02d-%03d-CH%02d-ImageTargetSearchFailed-Check.bmp"),
					sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, nPosIndex);

				m_vSaveAlignerTargetSearchImageFailIndex.at(nPosIndex) = m_vSaveAlignerTargetSearchImageFailIndex.at(nPosIndex) + 1;																			
				if (m_pVisionASMConfig->m_DataRecord.m_bDataRecordTime == FALSE)
				{
					m_vstrTemp.at(nPosIndex).Format(_T("ImageTargetSearchFailed-CH%02d-%03d-Check.bmp"), 
						nPosIndex,m_vSaveAlignerTargetSearchImageFailIndex.at(nPosIndex));
				}

				SaveImage(nPosIndex,m_vImageSearch[nPosIndex],m_strDataRecord + _T("\\") + m_vstrTemp.at(nPosIndex));
				if(m_vSaveAlignerTargetSearchImageFailIndex.at(nPosIndex) > m_pVisionASMConfig->m_DataRecord.m_nSaveFailedImageNum)
				{
					m_vSaveAlignerTargetSearchImageFailIndex.at(nPosIndex) = 0;
				}
			}
		}

		return FALSE;
	}

	if (!optionInfo.m_bExeTargetCheckModeWhenAlign && !CheckTargetAngle())
	{
		if(m_bTargetSearchTest) ReportPLCFunc(eCodeNG);;

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	SaveTargetPosInfo(GetTotalProuductData()->GetCurProductIndex());

	// 记录图像搜索数据
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		CString strTemp;

		CString strOut;
		CCoordPos cp;
		strOut = strTemp + _T("TargetSearch: ");		
		for (int i=0; i<m_mpTargetMarkImagePos.GetMarkImagePosNum(); i++)
		{
			strTemp = _T("");
			cp = m_mpTargetMarkImagePos.GetMarkImagePos(i);

			strTemp.Format(_T("MarkPos%d(%.2f,%.2f) "), i, cp.GetPosX(), cp.GetPosY());

			strOut += strTemp;
		}

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);
	}

	if (optionInfo.m_bEnableAlignAfterSearch && m_nAlignType!=-1)
	{
		BOOL m_bFOBCAndFTGC = ExecuteAlignAfterFOBCAndFTGC_Ex(vnPosIndex,2);


		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		return m_bFOBCAndFTGC;
	}

	ReportPLCFunc(eCodeOK);

	if (bSearchSucceed)
	{
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_SUCCEED);
		m_strStatusBarInfo2 = _T("");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}

	return TRUE;
}

// 根据索引指令设置当前产品  （产品切换）
BOOL vcXYDVisionAlign::OnlineSetProductIndexEx(int nCurProductIndex)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	CString strCmd = _T("VSPC:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd)/*NULL == m_pPlatformInfo || NULL == m_pVisionASMConfig*/)
	{
		ReportPLCFunc(eCodeNG);//SendCommInfo(CMD_TN_UNKNOWN_ERR);
		return FALSE;
	}

	if (IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		GetSysPlatformInfo(sysPlatformInfo);

		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			if (!ProductSearchToolLoadState(nCurProductIndex))
			{
				ReportPLCFunc(eCodeNG);//SendCommInfo(CMD_TN_UNKNOWN_ERR);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CHANGE_PRODUCT_NAME_FAILE);//_T("切换产品失败");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}

		}

	}

	int nCalibDataIndex = m_pVisionASMConfig->m_TotalCalibData.GetCurCalibIndex();
	int nProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if(nCalibDataIndex != nCurProductIndex && nProductIndex != nCurProductIndex)
	{
		if (!SetCurCalibDataAndProductIndex(nCurProductIndex) || !SetProductCameraShutter(nCurProductIndex))
		{
			ReportPLCFunc(eCodeNG);//SendCommInfo(CMD_TN_UNKNOWN_ERR);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CHANGE_PRODUCT_NAME_FAILE);//_T("切换产品失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SET_PRODUCT_NAME_FAILE);//_T("设置产品失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
			return FALSE;
		}

		PostMessageUpdateCurProductAlignInfo();
	}

	ReportPLCFunc(eCodeOK);//SendCommInfo(CMD_TN_SUCCEED);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CHANGE_PRODUCT_NAME_SUCCESS);//_T("切换产品成功");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	return TRUE;
}
//总的FBIS
BOOL vcXYDVisionAlign::OnlineACFInspectCandidateExFBIS_Ex(int nPosIndex, int nStartPatIndex,int nTarFixOffsetIndex /*= 0*/)
{
	CString strCmd = _T("VBIS:");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd)/*NULL == m_pPlatformInfo*/)
	{
		return FALSE;
	}	

	//if(nStartPatIndex<0 || nStartPatIndex>=INSPECTCANDIDATENUM)
	//{
	//	return FALSE;
	//}


	BOOL bACFInspect = FALSE;

	m_bisFirstAlign = TRUE;

	//使用候补的方式 例如：FCHB 5 101 就是拍位置12，基准位置
	if (nStartPatIndex>99 && nStartPatIndex<131)
	{
		// 位置参数
		std::vector<int> vnPosIndex;
		if (nPosIndex>=0 && nPosIndex<=3)
		{
			vnPosIndex.push_back(nPosIndex);
		}
		else if (nPosIndex==4)
		{
			for (int i=0; i<GetPosNum(); i++)
			{
				vnPosIndex.push_back(i);
			}
		}
		else if (nPosIndex==11) // 所有相机中的前一半编号
		{
			for (int i=0; i<GetPosNum()/2; i++)
			{
				vnPosIndex.push_back(i);
			}
		}
		else if (nPosIndex==12) // 所有相机中的后一半编号
		{
			int nPosIndex = 0;
			int i = 0;
			for(i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i + GetPosNum()/2;
				vnPosIndex.push_back(nPosIndex);
			}

		}

		// 模板参数
		std::vector<int> vnPatIndex;
		vnPatIndex.resize(5, 0);
		int nPatIndex = nStartPatIndex - 99;
		int nMaskResult = 1;
		int nMask = 1;
		for (int i=0; i<5; i++)
		{
			nMaskResult = nMask<<i;
			if ( (nPatIndex & nMaskResult) == nMaskResult )
			{
				vnPatIndex.at(i) = 1;
			}
		}
		// nPatIndex只对1到31有效，十进制转换二进制，从低位1到高位5，对应标准0模板到候补1/2/3/4模板，哪位置1则哪个模板有效
		// 十进制数和模板组合对应关系，详见EXCEL查找表

		//bACFInspect = OnSerialCameraACFInspectCandidateExFBIS_Ex2(vnPosIndex, vnPatIndex,nTarFixOffsetIndex);

		bACFInspect = OnSerialCameraACFInspectCandidateExFBIS_Ex3(vnPosIndex, vnPatIndex,nTarFixOffsetIndex);
	}

	return bACFInspect;

}


//总的FCHB
BOOL vcXYDVisionAlign::OnlineACFInspectCandidateExFCHB_Ex(int nPosIndex, int nStartPatIndex)
{
	CString strCmd = _T("VCHB:");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd)/*NULL == m_pPlatformInfo*/)
	{
		return FALSE;
	}	

	//if(nStartPatIndex<0 || nStartPatIndex>=INSPECTCANDIDATENUM)
	//{
	//	return FALSE;
	//}

	BOOL bACFInspect = FALSE;

	m_bisFirstAlign = TRUE;

	//使用候补的方式 例如：FCHB 5 101 就是拍位置12，基准位置
	if (nStartPatIndex>99 && nStartPatIndex<131)
	{
		// 位置参数
		std::vector<int> vnPosIndex;
		if (nPosIndex>=0 && nPosIndex<=3)
		{
			vnPosIndex.push_back(nPosIndex);
		}
		else if (nPosIndex==4)
		{
			for (int i=0; i<GetPosNum(); i++)
			{
				vnPosIndex.push_back(i);
			}
		}
		else if (nPosIndex==11) // 所有相机中的前一半编号
		{
			for (int i=0; i<GetPosNum()/2; i++)
			{
				vnPosIndex.push_back(i);
			}
		}
		else if (nPosIndex==12) // 所有相机中的后一半编号
		{
			int nPosIndex = 0;
			int i = 0;
			for(i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i + GetPosNum()/2;
				vnPosIndex.push_back(nPosIndex);
			}

		}

		// 模板参数
		std::vector<int> vnPatIndex;
		vnPatIndex.resize(5, 0);
		int nPatIndex = nStartPatIndex - 99;
		int nMaskResult = 1;
		int nMask = 1;
		for (int i=0; i<5; i++)
		{
			nMaskResult = nMask<<i;
			if ( (nPatIndex & nMaskResult) == nMaskResult )
			{
				vnPatIndex.at(i) = 1;
			}
		}
		// nPatIndex只对1到31有效，十进制转换二进制，从低位1到高位5，对应标准0模板到候补1/2/3/4模板，哪位置1则哪个模板有效
		// 十进制数和模板组合对应关系，详见EXCEL查找表

		//bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex2(vnPosIndex, vnPatIndex);
		bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex3(vnPosIndex, vnPatIndex);

	}
	else//不使用候补的方式 例如：FCHB 5  就是拍位置12，可以使用候补
	{
		if (nPosIndex>=0 && nPosIndex<=3)
		{
			std::vector<int> vnPosIndex;

			vnPosIndex.push_back(nPosIndex);

			bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex(vnPosIndex, nStartPatIndex);
		}
		else if(nPosIndex==4)
		{
			std::vector<int> vnPosIndex;
			int i = 0;
			for(i = 0; i < GetPosNum(); i++)
			{
				vnPosIndex.push_back(i);
			}
			bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex(vnPosIndex, nStartPatIndex);
		}
		else if (nPosIndex==11) // 所有相机中的前一半编号 实际发12
		{
			std::vector<int> vnPosIndex;
			int nPosIndex = 0;
			int i = 0;
			for(i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i;
				vnPosIndex.push_back(nPosIndex);
			}
			bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex(vnPosIndex, nStartPatIndex);

		}
		else if (nPosIndex==12) // 所有相机中的后一半编号 实际发13
		{
			std::vector<int> vnPosIndex;
			int nPosIndex = 0;
			int i = 0;
			for(i = 0; i < GetPosNum()/2; i++)
			{
				nPosIndex = i + GetPosNum()/2;
				vnPosIndex.push_back(nPosIndex);
			}
			bACFInspect = OnSerialCameraACFInspectCandidateExFCHB_Ex(vnPosIndex, nStartPatIndex);
		}
	}

	return bACFInspect;

}

BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFCHB_Ex(std::vector<int> vnPosIndex, int nStartPatIndex)
{

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	vector<int> vnDetectionResult; //增加模板编号搜索成功失败数据发送
	for (int i =0; i <vnPosIndex.size();i++)
	{
		vnDetectionResult.push_back(eCodeNG);
	}

	CString strCmd = _T("VCHB:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
		return FALSE;
	}
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	int i = 0;
	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_INSPECT_FAIL);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_CAMNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
		return FALSE;
	}

	//	int i=0;
	for(i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_INSPECT_FAIL);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
			return FALSE;
		}    	
	}

	scTimer stSearchTimer;	// 搜索时间
	stSearchTimer.Reset();

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;	

	for(i = 0; i < nPosNum; i++)
	{
		nPosIndex = vnPosIndex[i];
		vsmSearchMode.push_back(eInspectSearch);	// 
	}

	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;
		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate */&& bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	

			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}

			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	CString str;
	// 3.开启定位
	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);


	HideObjTarGuiResultInSpect(vnPosIndex, FALSE);

	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}

	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();
	BOOL bSearchSucceed = FALSE;
	if (bInspectSearchCandidate == FALSE)
	{
		for(i = 0; i < nPosNum; i++)
		{
			nPosIndex = vnPosIndex[i];
			m_vnInspectSearchStartPatIndex[nPosIndex] = nStartPatIndex;
			m_vnInspectSearchEndPatIndex[nPosIndex] = nStartPatIndex;
		}
		bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
	}
	else
	{
		if (bInspectSearchSync == TRUE)
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			if (nStartPatIndex>=0 && nStartPatIndex < INSPECTCANDIDATENUM)
			{
				nTempStartPatIndex = nStartPatIndex;
			}
			else if (nStartPatIndex == 9)
			{
				nTempStartPatIndex = m_vnInspectSearchSuccessPatIndex[vnPosIndex[0]];
			}
			else
			{
				nTempStartPatIndex = 0;
			}
			nTempEndPatIndex = INSPECTCANDIDATENUM-1;

			int j = 0;
			for(j = nTempStartPatIndex ; j <= nTempEndPatIndex; j++ )
			{
				if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
				{
					continue;
				}

				for(i = 0; i < nPosNum; i++)
				{
					nPosIndex = vnPosIndex[i];
					m_vnInspectSearchStartPatIndex[nPosIndex] = j;
					m_vnInspectSearchEndPatIndex[nPosIndex] = j;
				}			
				bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);

				if (bSearchSucceed)
				{
					break;
				}	
			}
		}
		else
		{
			int nTempStartPatIndex, nTempEndPatIndex;
			for(i = 0; i < nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				if (nStartPatIndex>=0 && nStartPatIndex < INSPECTCANDIDATENUM)
				{
					nTempStartPatIndex = nStartPatIndex;
				}
				else if (nStartPatIndex == 9)
				{
					nTempStartPatIndex = m_vnInspectSearchSuccessPatIndex[nPosIndex];
				}
				else
				{
					nTempStartPatIndex = 0;
				}
				nTempEndPatIndex = INSPECTCANDIDATENUM-1;

				m_vnInspectSearchStartPatIndex[nPosIndex] = nTempStartPatIndex;
				m_vnInspectSearchEndPatIndex[nPosIndex] = nTempEndPatIndex;
			}
			bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		}
	}

	//增加模板编号搜索成功失败数据发送
	for (int i=0; i<nPosNum; i++)
	{
		vnDetectionResult[i] = m_vbSearchSucceed[vnPosIndex[i]] ? eCodeOK:eCodeNG;
	}

	// 设定、显示结果图形
	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eInspectSearch);

	//ID、平台号信息读取刷新
	{
		CString strProductID = _T("");
		CString strProductSTATION = _T("");
		CString strInspectStation = _T("");
		CString strStartIime = _T("");
		CString strEndIime = _T("");
		CString strProsessingIime = _T("");
		CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex);
		if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
		{
			if (!CommunicateGetAlignProcessProductID(strProductID))
			{
				ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品ID读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
		}
		if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
		{
			if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
			{
				ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品工位读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
		}
		m_strProductID = strProductID;
		m_strProductSTATION = strProductSTATION;
		m_strInspectStation = strInspectStation;

		vector<CString> vSearchResult;
		vector<BOOL> vbResult;
		CString m_StrTextID;
		vSearchResult.clear();
		if (strProductID != "")
		{
			for (int i = 0; i < vnPosIndex.size(); i++)
			{
				vbResult.push_back(1);
				m_StrTextID.Format(_T("SN:%s"),m_strProductID);
				vSearchResult.push_back(m_StrTextID);
			}
		}
		if (strProductSTATION != "")
		{
			for (int i = 0; i < vnPosIndex.size(); i++)
			{
				vbResult.push_back(1);
				m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,strProductSTATION);
				vSearchResult.push_back(m_StrTextID);
			}
		}
		if (strInspectStation != "")
		{
			for (int i = 0; i < vnPosIndex.size(); i++)
			{
				vbResult.push_back(1);
				m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
				vSearchResult.push_back(m_StrTextID);
			}
		}

		if (vSearchResult.size())
		{
			m_cVisionAlignGui.SetCurPosText(vnPosIndex,vbResult,vSearchResult);
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eInspectSearch);
		}
	}

	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch);
	}
	if (TRUE == bSearchSucceed)
	{
		m_bStatusBar = TRUE;
		if (!m_bSearchWithInspect)
		{
			m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_ACF_INSPECT_SUCCESS);
		}
		else
		{
			m_strStatusBarInfo1 = _T("距离检测成功");
		}
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();


		std::vector<double> vdCircleRadius;

		std::vector<BOOL> vbBendCircleSearchTool;
		// 满盘检测
		{
			int nPosIndexInspect =  vnPosIndex.at(0);
			// 获取指定位置、指定候补模板的定位工具
			CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndexInspect, 0);
			if(NULL != pSearchTool)
			{
				CSearchToolType eType = pSearchTool->GetSearchToolType();
				switch(eType)
				{
				case eACFInspectTool:
					{
						CSearchResult result;
						if(pSearchTool->GetResult(0, result))
						{
							int nRows = result.m_vACFRowColStatus.size();
							int nVacancy=0;// 空穴数目
							vector<int> vRow;
							vector<int> vCol;
							if(nRows>0)
							{
								int Cols = result.m_vACFRowColStatus.at(0).size();
								for(int iRow=0;iRow<nRows;iRow++)
								{
									for(int iCol=0;iCol<nRows;iCol++)
									{
										if(result.m_vACFRowColStatus.at(iRow).at(iCol)==2)
										{
											nVacancy++;
											vRow.push_back(iRow+1);
											vCol.push_back(iCol+1);
										}
									}
								}

								if(nVacancy>0)
								{
									int nSize =nVacancy;
									if(nVacancy>=2)
									{
										nSize = 2;
									}
									// 暂时为D10开始写空穴行列
									ReportPLC_RowCols(eCodeOK,nSize,vRow,vCol);	
								}
								else // 没有空穴
								{
									int nSize =nVacancy;
									ReportPLC_RowCols(eCodeOK,nSize,vRow,vCol);
								}
							}
							else
							{
								ReportPLCFunc(eCodeNG);
								m_bStatusBar = FALSE;
								if (!m_bSearchWithInspect)
								{
									m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_ACF_INSPECT_FAILE);
								}
								else
								{
									m_strStatusBarInfo1 = _T("距离检测失败");
								}

								m_strStatusBarInfo2 = _T("");
								SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
								PostMessageUpdateStatusBarInfo();
								strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
								AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
								return FALSE;
							}
						}

					}
					break;
				default:
					break;
				}
			}
		}

	}
	else
	{

		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc2(eCodeNG,vnDetectionResult);

		m_bStatusBar = FALSE;
		if (!m_bSearchWithInspect)
		{
			m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_ACF_INSPECT_FAILE);
		}
		else
		{
			m_strStatusBarInfo1 = _T("距离检测失败");
		}

		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	if(1/*m_bObjectSearchTest*/)
	{
		//ReportPLCFunc(eCodeOK);
		ReportPLCWithDetectionResultFunc2(eCodeOK,vnDetectionResult);
	}
	return TRUE;
}

BOOL vcXYDVisionAlign::ReportPLC_FALP(int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos,int nOffset)
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	BOOL bResult = (nResult==1)? TRUE:FALSE;

	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSimpleInfo;
		m_XYDCommProtocol.PackReAlignResultInfoFALP(strSimpleInfo,pPlatformXYDAxisPos,bResult);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;

		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}

		// 打包写寄存器指令
		if (FALSE == m_XYDCommProtocol.PackPlcWriteDrivePlatformMoveToAbsPosEx(strSend, pPlatformXYDAxisPos),nOffset+nOffSetMul)
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
		// 向D2中写入状态对位OK或NG
		if(FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend, nResult, atoi(RREG_KEYENCE_VISION_TRIGGER))+nOffset+nOffSetMul)
		{
			return FALSE;
		}

		// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确 此处命令码和触发位一致 视觉指令触发置位


		nCommStatus = eCommSucceed;
		bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
		if (bReceived == FALSE || nCommStatus != eCommSucceed)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineAlignExFALP_Ex(int nOffsetIndex)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();
	CString str;
	CString strCmd = _T("VALP:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	CPlatformXYDAxisPos *pPlatformXYDAxisPos=NULL;
	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());

	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd)/*NULL == m_pPlatformInfo || NULL == m_pVisionASMConfig*/)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd)/*FALSE == m_bValidObjectSearchTool*/)
	{

		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd)/*FALSE == m_bValidAlignerTool*/)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd)/*FALSE == m_bValidComm*/)
	{

		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	for(int i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);	
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();		
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	PostMessageUpdateAlignOffsetInfo();
	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);

	SysPlatformInfo platformInfo;
	GetSysPlatformInfo(platformInfo);

	if( m_vpTargetPlatformAxisPos.size()==2 && platformInfo.m_bTargetPick)
	{
		std::vector<int> vnPosIndex;
		if(optionInfo.m_bEnableMultiMarkMode)
		{
			vnPosIndex.push_back(0);
		}
		else
		{
			for(int i = 0; i < GetPosNum(); i++)
			{
				vnPosIndex.push_back(i);
			}	
		}
		m_bTargetMarkSearchSucceed = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex,0);
	}

	if (((m_vpObjectPlatformAxisPos.size()== 2) && (*(CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(0) == *(CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(1))/*&& m_bObjectMarkSearchSucceed == FALSE*/)
		|| (m_vpObjectPlatformAxisPos.size()== 2 && platformInfo.m_bTargetPick))
	{
		std::vector<int> vnPosIndex;
		if (optionInfo.m_bEnableMultiMarkMode)
		{
			vnPosIndex.push_back(0);
		}
		else
		{
			for(int i = 0; i < GetPosNum(); i++)
			{
				vnPosIndex.push_back(i);
			}
		}

		// 获取轴位置
		{
			CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
			BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
			// 直接读取寄存器地址轴数据
			bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

			if (FALSE == bCommuciateGetPlatformAxisAbsPos)
			{
				if (pPlatformAxisPos!=NULL)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;					
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}
			else
			{
				for (int k=0;k<2;k++)
				{
					int nPosIndex = k;
					SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
					SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
					SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
				}



				m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
				m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
				m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
				// 记录当前平台各个轴的绝对位置
				if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
				{
					CString strTemp;
					CString strInfo;
					strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
						pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
					strInfo = strTemp + strInfo;	
					VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
					VisionAlignLogRecord(m_strAlignProcessData, strInfo);
				}


				if (pPlatformAxisPos)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;
				}
			}
		}

		BOOL bIsSizeNG = FALSE;
		m_bObjectMarkSearchSucceed = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0,&bIsSizeNG);
		if (m_bObjectMarkSearchSucceed || bIsSizeNG)
		{
			if (bIsSizeNG || !CheckObjectDistance())
			{
				//ReportPLCFunc(eCodeNG);
				ReportPLCFunc(eCodeSizeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_OBJECT_LEN_OVERFLOW); //__T("对象距离超过设定标准，对象定位结果可能出错");
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}
		}
	}

	if (FALSE ==m_bObjectMarkSearchSucceed )
	{
		CPlatformOptionInfo optionInfo;
		GetPlatformOptionInfo(optionInfo);

		if (TRUE == optionInfo.m_bOmitRePickObjectEnable)
		{
			m_vObjectSearchedSeachResults.clear();
		}

		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;     			
	}
	if( m_vpTargetPlatformAxisPos.size()==2 && platformInfo.m_bTargetPick)
	{
		if(FALSE == m_bTargetMarkSearchSucceed)
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();		
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}
	}

	scTimer stAlignTimer;   double dAlignTime(0.0);	
	stAlignTimer.Reset();	

	// 执行对位计算
	if(platformInfo.m_bTargetPick)
	{
		m_pAlignerTool->SetPlatformPick(FALSE);
	}

	// 获取当前品种搜索成功的定位工具，如果是区域定位工具（威创达DM定制）则获取搜索区域 //默认1*1为不启用分区域补偿则不显示GUI
	if(pCurProductData->m_pAlignerParam->GetAlignOffsetByArea().m_bEnable)
	{
		// 判断是否在区域内
		sc2Vector ptMid((m_mpObjectMarkImagePos.m_vPos.at(0).m_dPosX + m_mpObjectMarkImagePos.m_vPos.at(1).m_dPosX) / 2, (m_mpObjectMarkImagePos.m_vPos.at(0).m_dPosY + m_mpObjectMarkImagePos.m_vPos.at(1).m_dPosY) / 2);
		nOffsetIndex = m_cVisionAlignGui.GetAlignOffsetByArea(ptMid);

		// 设置补偿量索引
		EnterCriticalSection(&m_csAlignerOffset);
		pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
		CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
		pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
		LeaveCriticalSection(&m_csAlignerOffset);

		PostMessageUpdateAlignOffsetInfo();

		m_cVisionAlignGui.SetAllPosAlignOffsetByArea(ptMid);
	}

	if(FALSE == ExecuteAlign())
	{

		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
		m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();	

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
		

		m_bObjectMarkSearchSucceed = FALSE;
		return FALSE;     
	}


	m_bObjectMarkSearchSucceed = FALSE;
	// 更新主界面对位精度、次数、对位总时间
	CCoordPos cp = GetAlignPlatformOffset();

	m_apAlignerProcesInfo.nTimes  = 1;
	m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
	m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
	m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;
	m_apAlignerProcesInfo.dTime = stAlignTimer.GetTimeMilli(TRUE);
	dAlignTime=m_apAlignerProcesInfo.dTime;
	PostMessageUpdateAlignerProcessInfo();	

	MotionControlType TmpMotionControlType = eMCNo;
	CVisionASMConfig *vsConfig;
	GetVSconfig(vsConfig);
	if (NULL != vsConfig)
	{
		TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
	}
	// 针对于叠膜机出双模拾取应用，带运动控制的Yamaha机器人定制，将拾取绝对位置控制在一个小范围内（P101或P103）
	if (TmpMotionControlType == eMCYamahaRobot || TmpMotionControlType == eMCYamahaRobot340)
	{

	}
	else
	{
		CPlatformOptionInfo optionInfo;
		GetPlatformOptionInfo(optionInfo);
		if(IsAxisMoveOverRun(cp))
		{
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());   //chaiyongqiang20181022 修改在本次拾取超出最大移动量时返回的是上一次绝对位置的bug,获取最新对位位置
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
			m_strStatusBarInfo2.Format(_T(""));
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}

		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dAlignTime);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
	}

	// 记录对位精度
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
	{
		CString strTemp;
		CTime t = CTime::GetCurrentTime();
		strTemp.Format(_T("\n\n%d-%d-%d-%d-%d-%d "),t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

		CString strOut;					
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
		strOut = strTemp + strOut;

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);

	}

	// 记录对位时间
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
	{
		CString strTemp;
		CTime t = CTime::GetCurrentTime();
		strTemp.Format(_T("\n\n%d-%d-%d-%d-%d-%d "),t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

		CString strOut;						
		strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), 0, dAlignTime);
		strOut = strTemp + strOut;

		VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
		VisionAlignLogRecord(m_strAlignProcessData, strOut);					
	}	

	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());
	//调整为相对偏差量
	BOOL bSendRel  = optionInfo.m_bEnablePickSendRelativateAxis;
	if(bSendRel)
	{
		pPlatformXYDAxisPos->m_dPosX = cp.m_dPosX;
		pPlatformXYDAxisPos->m_dPosY = cp.m_dPosY;
		pPlatformXYDAxisPos->m_dAngle = cp.m_dAngle;
	}

	// 针对于叠膜机出双模拾取应用，带运动控制的Yamaha机器人定制，将拾取绝对位置控制在一个小范围内（P101或P103）
	if (TmpMotionControlType == eMCYamahaRobot || TmpMotionControlType == eMCYamahaRobot340)
	{
		BOOL bInTeachPosRange0 = FALSE;
		BOOL bInTeachPosRange1 = FALSE;
		if (pCurProductData->m_RobotTeachInfo.m_vbRobotTearchPosValid.size()>0 && pCurProductData->m_RobotTeachInfo.m_vbRobotTearchPosValid.at(0))
		{
			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);
			if ((pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosX-pPlatformXYDAxisPos->m_dPosX) > optionInfo.m_dAxisMoveMaxX
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosY-pPlatformXYDAxisPos->m_dPosY) > optionInfo.m_dAxisMoveMaxY
				|| (scDegree(scDegree(pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosR).Norm()-scDegree(pPlatformXYDAxisPos->m_dAngle).Norm()).SignedNorm().ToDouble()) > optionInfo.m_dAxisMoveMaxD
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosX-pPlatformXYDAxisPos->m_dPosX) < optionInfo.m_dAxisMoveMaxX_LowerLimit
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosY-pPlatformXYDAxisPos->m_dPosY) < optionInfo.m_dAxisMoveMaxY_LowerLimit
				|| (scDegree(scDegree(pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(0).m_dPosR).Norm()-scDegree(pPlatformXYDAxisPos->m_dAngle).Norm()).SignedNorm().ToDouble()) < optionInfo.m_dAxisMoveMaxD_LowerLimit
				)
			{
				bInTeachPosRange0 = FALSE;
			}
			else
			{
				bInTeachPosRange0 = TRUE;
			}
		}

		if (pCurProductData->m_RobotTeachInfo.m_vbRobotTearchPosValid.size()>1 && pCurProductData->m_RobotTeachInfo.m_vbRobotTearchPosValid.at(1))
		{
			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);

			if ((pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosX-pPlatformXYDAxisPos->m_dPosX) > optionInfo.m_dAxisMoveMaxX
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosY-pPlatformXYDAxisPos->m_dPosY) > optionInfo.m_dAxisMoveMaxY
				|| (scDegree(scDegree(pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosR).Norm()-scDegree(pPlatformXYDAxisPos->m_dAngle).Norm()).SignedNorm().ToDouble()) > optionInfo.m_dAxisMoveMaxD
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosX-pPlatformXYDAxisPos->m_dPosX) < optionInfo.m_dAxisMoveMaxX_LowerLimit
				|| (pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosY-pPlatformXYDAxisPos->m_dPosY) < optionInfo.m_dAxisMoveMaxY_LowerLimit
				|| (scDegree(scDegree(pCurProductData->m_RobotTeachInfo.m_vrpRobotTeachPos.at(2).m_dPosR).Norm()-scDegree(pPlatformXYDAxisPos->m_dAngle).Norm()).SignedNorm().ToDouble()) < optionInfo.m_dAxisMoveMaxD_LowerLimit
				)
			{
				bInTeachPosRange1 = FALSE;
			}
			else
			{
				bInTeachPosRange1 = TRUE;
			}
		}

		if (bInTeachPosRange0 || bInTeachPosRange1)
		{
			ReportAlignResultFALP(pPlatformXYDAxisPos,TRUE);

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dAlignTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();		

			return TRUE;
		}
		else
		{
			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
			m_strStatusBarInfo2.Format(_T(""));
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;

		}
	}
	if(platformInfo.m_bTargetPick)
	{
		//如果拾取目标，提供产品相对偏移量
		CCoordPos coordPos = GetAlignProductOffset();

		m_apAlignerProcesInfo.dXOffset = coordPos.m_dPosX;
		m_apAlignerProcesInfo.dYOffset = coordPos.m_dPosY;
		m_apAlignerProcesInfo.dDOffset = coordPos.m_dAngle;

		m_apAlignerProcesInfo.dXMove = cp.m_dPosX;
		m_apAlignerProcesInfo.dYMove = cp.m_dPosY;
		m_apAlignerProcesInfo.dDMove = cp.m_dAngle;
		pPlatformXYDAxisPos->m_dPosX = cp.m_dPosX;
		pPlatformXYDAxisPos->m_dPosY = cp.m_dPosY;
		pPlatformXYDAxisPos->m_dAngle = cp.m_dAngle;

		m_apAlignerProcesInfo.nTimes = 1;
		PostMessageUpdateAlignerProcessInfo();


	}

	if (TRUE == optionInfo.m_bLastPickSetting)
	{
		BOOL bLastPick = FALSE;
		int i = 0;
		for(i = 0; i < m_vnObjectSearchResultNumber.size(); i++)
		{
			if (m_vnObjectSearchResultNumber[i] == 1)
			{
				bLastPick = TRUE;
			}
			else
			{
				bLastPick = FALSE;
				break;
			}
		}
		//if (m_vnObjectSearchResultNumber[0] == 1 && m_vnObjectSearchResultNumber[1] == 1)
		if(bLastPick == TRUE)
		{
			//ReportAlignResultNumberFALP(pPlatformXYDAxisPos,1);
			// 暂时回复4表示最后一片拾取判定
			ReportPLC_FALP(4,pPlatformXYDAxisPos);
			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);
			/*if (TRUE == m_pSystempOptionConfig->m_bOmitRePickObjectEnable)*/
			if (TRUE == optionInfo.m_bOmitRePickObjectEnable)
			{
				// 如果搜索失败，且开启了忽略拾取失败功能，那么清空已有的搜索结果
				m_vObjectSearchedSeachResults.clear();
			}

		}
		else
		{
			ReportPLC_FALP(eCodeOK,pPlatformXYDAxisPos);
		}
	}
	else
	{
		ReportPLC_FALP(eCodeOK,pPlatformXYDAxisPos);
	}
	return TRUE; 
}

BOOL vcXYDVisionAlign::OnlineAlignExFAAC_Ex(int nOffsetIndex, int nStartPatIndex,int nBenchOffsetIndex/* = -1*/, int nExProductIndex/* = 0*/,int nTarFixOffsetIndex /* = 0 */)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	SetAlignBenchOffsetIndex(nBenchOffsetIndex);
	SetAlignExProductIndex(nExProductIndex);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	// start 直接根据searchdata中图像坐标和轴位置 设置进对位工具中；
	{
		int nSize1 = m_vvpTargetPlatformAxisPosMultiEx.size();
		int nSize2 = m_vvpObjectPlatformAxisPosMultiEx.size();
		int nSize3 = m_vmpObjectMarkImagePos_MultiEX.size();
		int nSize4 = m_vmpTargetMarkImagePosMultiEx.size();
		bool bTmp = (nExProductIndex >=0) &&  (nExProductIndex <nSize1) &&  (nExProductIndex <nSize2) &&  (nExProductIndex <nSize3)  &&  (nExProductIndex <nSize4);

		if (bTmp)
		{

			vector<CPlatformAxisPos*> tmpObjectPlatformAxisPos;
			vector<CPlatformAxisPos*> tmpTargetPlatformAxisPos;
			tmpObjectPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);
			tmpTargetPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);

			if (m_pPlatformInfo->m_bEnableMultiCalibExtension)
			{
				if (optionInfo.m_bObjectSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{
						tmpObjectPlatformAxisPos.at(jt) = m_vvpObjectPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpObjectMarkImagePos = m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex);
				}
				else
				{
					tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				}

				if (optionInfo.m_bTargetSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{	
						tmpTargetPlatformAxisPos.at(jt) = m_vvpTargetPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpTargetMarkImagePos = m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex);
				}
				else
				{
					tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
				}

			}
			else
			{
				tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
			}


			m_pAlignerTool->SetTargetPlatformPos(tmpTargetPlatformAxisPos);
			//m_pAlignerTool->SetTargetMarkSearchResultsInit(tmpTargetImagePos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

			m_pAlignerTool->SetObjectPlatformPos(tmpObjectPlatformAxisPos);
			//m_pAlignerTool->SetObjectMarkSearchResultsAll(tmpObjectImagePos);
			m_pAlignerTool->SetObjectMarkSearchResults(m_mpObjectMarkImagePos);


			m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
			m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();

		}
	}
	// end  直接根据searchdata中图像坐标和轴位置 设置进对位工具中；

	if (optionInfo.m_bEnableMutiTarget)
	{
		m_bTargetMarkSearchSucceed=m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex);
		if (m_bTargetMarkSearchSucceed)
		{
			SetTargetMarkSearchResults(m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex));
			m_mpTargetMarkImagePos=m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex);
			int nPosNum = m_pPlatformInfo->m_nPositionNum;
			for (int i=0;i<nPosNum;i++)
			{
				SetTargetPlatformPos(m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i),i);
				SetTempTargetPlatformAxisPos(i,m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i));
			}
			PostMessageShowMutiTargetImagePosToDisplay(m_bTargetMarkSearchSucceed);	
		}	
	}

	CString strCmd = _T("VAAC:");
	CString strTempWarningType = _T("");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd))
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd))
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	int i=0;
	for(i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();	
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);
	PostMessageUpdateAlignOffsetInfo();

	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);

	scTimer stTotalAlignTimer;
	double dTotalAlignTime(0.0);

	stTotalAlignTimer.Reset();

	m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
	if (eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
	{
		if (FALSE == m_bTargetMarkSearchSucceed)
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();	
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo(); 

	// 获取当前产品最大对位次数
	int nMaxAlignTime = curProductData.m_pAlignerParam->m_nAlignerMaxTime;

	scTimer stObjectSearchTimer; double dObjectSearchTime(0.0);
	scTimer stCaluteAlignTimer;  double dCaluteAlignTime(0.0);
	scTimer stAlignTimer;   double dAlignTime(0.0);

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	// 对位居中变量
	CCoordPos cpBenchOffset;
	CPlatformXYDAxisPos benchPlatformXYDAxisPos;

	//对象光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl
		&& (m_nCurAlignerTimesIndex == 0 || m_bTagetLightFinish))  //仅第一次做切换
	{
		if (!OnLightOnOff(0))
		{
			ReportPLCFunc(eCodeNG);
			m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
		m_bTagetLightFinish = FALSE;
	}

	// for (i=0; i<nMaxAlignTime+1; i++)
	i=m_nCurAlignerTimesIndex;
	if((i<-1) || (i>=nMaxAlignTime+1))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("");
		m_strStatusBarInfo2.Format(_T("对位次数%d（最大次数:%d）超限"),i,nMaxAlignTime);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	CString strProductID = _T("");
	CString strProductSTATION = _T("");
	CString strInspectStation = _T("");
	CString strStartIime = _T("");
	CString strEndIime = _T("");
	CString strProsessingIime = _T("");
	{
		//ID读取
		if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID)|| m_XYDCommProtocol.GetIsEnableIDRead())
		{
			if (!CommunicateGetAlignProcessProductID(strProductID))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品ID读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
			m_strProductID = strProductID;
		}

		//平台号读取
		if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
		{
			if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品工位读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
			m_strProductSTATION = strProductSTATION;
			m_strInspectStation = strInspectStation;
		}
	}

	m_nAlignTime =i+1;
	{
		stAlignTimer.Reset();
		// 执行对象拍照
		{
			stObjectSearchTimer.Reset();

			if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
			{
				SetDefaultObjectSearchMarkPos(TRUE);
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;

				if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
				{
					int nObjectVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
					BOOL bEnableObjectPositionVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;

					if (bEnableObjectPositionVirtualMode)
					{
						switch(nObjectVirtualMode)
						{
						case 0:
							{
								// 位置1、位置2
								vnPosIndex.push_back(0);
								vnPosIndex.push_back(1);

							}
							break;

						case 1:
							{
								// 位置3、位置4
								vnPosIndex.push_back(2);
								vnPosIndex.push_back(3);
							}
							break;

						case 2:
							{
								// 位置1、位置3
								vnPosIndex.push_back(0);
								vnPosIndex.push_back(2);
							}
							break;

						case 3:
							{
								// 位置2、位置4
								vnPosIndex.push_back(1);
								vnPosIndex.push_back(3);
							}
							break;

						default:
							{
								return FALSE;
							}
							break;
						}
					}
					else
					{
						vnPosIndex.push_back(0);
						vnPosIndex.push_back(1);
						vnPosIndex.push_back(2);
						vnPosIndex.push_back(3);
					}
				}
				else
				{
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
				}

				if (FALSE == OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex, nStartPatIndex))
				{
					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
					if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
					{
						m_strStatusBarInfo2 = m_strStatusBarInfo2 + _T(",") + m_strSnapImgStatuString;
					}
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();		

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

					return FALSE;     			
				}

			}
			else
			{
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}
				if (FALSE == OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(vnPosIndex,nStartPatIndex))
				{
					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
					if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
					{
						m_strStatusBarInfo2 = m_strStatusBarInfo2 + _T(",") + m_strSnapImgStatuString;
					}
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();	

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

					return FALSE;     			
				}
			}

			dObjectSearchTime = stObjectSearchTimer.GetTimeMilli(FALSE);

		}

		//刷新SN、平台号信息
		{
			vector<CString> vSearchResult;
			vector<BOOL> vbResult;
			CString m_StrTextID;
			vSearchResult.clear();
			if (strProductID != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("SN:%s"),m_strProductID);
					vSearchResult.push_back(m_StrTextID);
				}
			}
			if (strProductSTATION != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,strProductSTATION);
					vSearchResult.push_back(m_StrTextID);
				}
			}
			if (strInspectStation != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
					vSearchResult.push_back(m_StrTextID);
				}
			}

			if (vSearchResult.size())
			{
				std::vector<int> vnPosIndex; 
				vnPosIndex.push_back(0);
				vnPosIndex.push_back(1);
				m_cVisionAlignGui.SetAllPosText(vbResult,vSearchResult);
				m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
			}
		}

		// 执行基准对位
		if(optionInfo.m_bEnableAlignBenchCenter == TRUE)
		{
			if (i == 0)
			{
				// 设置图像中心十字基准目标
				SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
				m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

				m_pAlignerTool->SetInnerAlignBench(TRUE);	// 设置为内部图像十字中心基准对位有效
				if(FALSE == ExecuteAlign(i))
				{
					m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
					// 设置实时的目标
					m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
					m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
					m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();	
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
					return FALSE;     
				}

				// 获取对位结果
				cpBenchOffset = GetAlignPlatformOffset();
				CPlatformXYDAxisPos* pAlignPlatformMovementResults = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
				benchPlatformXYDAxisPos = *pAlignPlatformMovementResults;
				m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效

				// 设置实时的目标
				m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}
		}

		// 执行对位计算
		stCaluteAlignTimer.Reset();
		if(FALSE == ExecuteAlign(i))
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
			m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();	
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
			return FALSE;     
		}
		dCaluteAlignTime = stCaluteAlignTimer.GetTimeMilli(FALSE); 

		// 执行一次对位
		if (TRUE == optionInfo.m_bAlignOneTime)
		{
			CCoordPos cpProduct = GetAlignProductOffset();
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cpProduct.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cpProduct.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
			}
			m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
			m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
			}
			m_apAlignerProcesInfo.nTimes  = i+1;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			PostMessageUpdateAlignerProcessInfo();


			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);
			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();

			// 当启用目标补偿，且不发送目标补偿到PLC时，在MMVA指令中增加目标补偿运动量，否则在报告对位成功指令中将目标补偿绝对位置放在备用轴位置寄存器中
			if (pAlignerParam->GetEnableAlnTargetOffset() && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{
				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
			}

			scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
			stCommunicateTimer.Reset();
			// 通信驱动平台运动到指定位置

			// 启用中心对位，将中心对位结果写入D20地址
			if(optionInfo.m_bEnableAlignBenchCenter)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
				pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
				ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				if (pPlatformXYDAxisPosMove1!=NULL)
				{
					delete pPlatformXYDAxisPosMove1;
					pPlatformXYDAxisPosMove1 = NULL;					
				}
			}

			// 打包偏差量结果并给出对位OK信号
			ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);

			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
					//D角度不累加

				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

				}
				//D角度不累加
				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}
			dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), m_apAlignerProcesInfo.dTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

			return TRUE;	
		}

		// 判断是否执行对位动作
		if (i!=0 && TRUE == IsAlignFinished(i+1))
		{
			dTotalAlignTime = stTotalAlignTimer.GetTimeMilli(FALSE);

			// 更新主界面对位精度、次数、对位总时间
			CCoordPos cpProduct = GetAlignProductOffset();
			m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
			m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

			CCoordPos cp = GetAlignPlatformOffset();
			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				cpProduct = GetAlignProductOffsetXYPD();
			}
			m_apAlignerProcesInfo.nTimes  = i;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			PostMessageUpdateAlignerProcessInfo();

			m_bStatusBar = IsAlignSucceed();
			// 启用中心对位，将中心对位结果写入D20地址
			if(m_bStatusBar && optionInfo.m_bEnableAlignBenchCenter)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
				pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
				ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				if (pPlatformXYDAxisPosMove1!=NULL)
				{
					delete pPlatformXYDAxisPosMove1;
					pPlatformXYDAxisPosMove1 = NULL;					
				}
			}

			if (pAlignerParam->GetEnableAlnTargetOffset() && IsAlignSucceed() && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{				
				CPlatformOptionInfo optionInfo;
				GetPlatformOptionInfo(optionInfo);
				CCoordPos cp = GetAlignPlatformOffset();
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();

				if(IsAxisMoveOverRun(cp))
				{
					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
					m_strStatusBarInfo2.Format(_T(""));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

				// 执行本次对位操作
				// 获取平台对位应移动到的绝对位置
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
				pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();			

				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);

				scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
				stCommunicateTimer.Reset();

				if (!m_bStatusBar)
				{
					ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
				}
				else
				{
					ReportPLC_Offset(cp, eCodeNG, pPlatformXYDAxisPos);
				}

				if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
				{
					m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
					m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
					if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
						//D角度不累加

					}
					else
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

					}
					//D角度不累加
					if (m_pSystempOptionConfig->m_bShowSepTargetPos)
					{
						UpdateTargetPosAfterRoate();
					}
				}
				else
				{
					m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
				}	
				dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);

				return m_bStatusBar;
			}

			if (m_bStatusBar)
			{
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);
				//ReportPLCFunc(eCodeOK);m_nCurAlignerTimesIndex = 0;
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
				pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);
				ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
				m_nCurAlignerTimesIndex = 0;
			}
			else
			{
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			}

			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), i, dTotalAlignTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CString strOut;					
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CString strOut;						
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), i, dTotalAlignTime);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			return m_bStatusBar;  
		}
		else
		{
			CCoordPos cpProduct = GetAlignProductOffset();
			m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
			m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i+1;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			PostMessageUpdateAlignerProcessInfo();

			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);
			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();

			scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
			stCommunicateTimer.Reset();

			// 打包回复偏差量到D10-14
			ReportPLC_Offset(cp, eCodeContinued, pPlatformXYDAxisPos);
			m_nCurAlignerTimesIndex = i+1;

			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

				}
				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}
			dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), m_apAlignerProcesInfo.dTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}
		} // else

	}// for	

	return TRUE;
}

// 对象拍照或者目标拍照之后，执行对位计算
BOOL vcXYDVisionAlign::ExecuteAlignAfterFOBCAndFTGC_Ex(std::vector<int>& vnPosIndex,int nObject)
{
	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();

	BOOL bTargetSearchSync = pAlignerParam->GetTargetCandidateSyncEnable();

	m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();
	m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();

	CPlatformXYDAxisPos *pPlatformXYDAxisPos=NULL;
	int nExProductIndex = m_pAlignerTool->GetAlignExProductIndex();
	BOOL bSucc = OnlineAlignExFACOAfterSearch(m_nOffsetIndex,-1,nExProductIndex,m_nOffsetIndex);

	//绝对值
	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());
	CCoordPos cpOffset;
	cpOffset = GetAlignPlatformPosOffset();	


	if(m_nAlignType == 1)//绝对轴
	{
		cpOffset.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
		cpOffset.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
		cpOffset.m_dAngle = pPlatformXYDAxisPos->m_dAngle;
	}

	if(nObject == 1)//对象拍完照之后执行对位
	{
		if(m_bObjectSearchTest)
		{
			if (bSucc)
			{
				ReportPLC_Offset(cpOffset, eCodeOK, pPlatformXYDAxisPos);
			}
			else
			{
				ReportPLCFunc(eCodeNG);
				return FALSE;
			}
		}

	}

	if(nObject == 2)//目标拍完照之后发送对位结果
	{
		if(m_bTargetSearchTest)
		{
			if (bSucc)
			{
				ReportPLC_Offset(cpOffset, eCodeOK, pPlatformXYDAxisPos);
			}
			else
			{
				ReportPLCFunc(eCodeNG);
				return FALSE;
			}
		}

	}
	return TRUE;

}

BOOL vcXYDVisionAlign::ReportMulAlignPosResult_Ex(std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult)	// 对位拾取响应
{
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	CString strSimpleInfo;
	m_XYDCommProtocol.PackReMulAlignPosInfo_Ex(strSimpleInfo,vpPlatformXYDOffset,nResult);
	BOOL bResult = (nResult==1)?TRUE:FALSE;
	if (m_XYDCommProtocol.IsPlcProtocolType())
	{
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;

		if (bResult)
		{
			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackRePlcMulAlignPosResultInfoAxisPos_Ex(strSend,vpPlatformXYDOffset,nResult))
			{
				return FALSE;
			}

			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

		}
		else
		{
			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL vcXYDVisionAlign::OnlineAlignExFMCO_Ex(int nOffsetCount)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	// 所有轴位置信息
	std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset;

	if (NULL == m_pPlatformInfo || NULL == m_pVisionASMConfig)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}	

	if(nOffsetCount<1)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查对象定位工具是否有效
	if ( FALSE == m_bValidObjectSearchTool)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_TOOL_INVALID);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_CHECK_INIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		return FALSE;
	}

	// 检查对位工具是否有效
	if (FALSE == m_bValidAlignerTool)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TOOL_INVALID);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_CHECK_INIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		return FALSE;

	}

	// 检查通信工具是否有效
	if (FALSE == m_bValidComm)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_COMM_TOOL_INVALID);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_CHECK_INIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	for(int i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_CAMERA_GRABBING);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		

		return FALSE;
	}

	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);

	m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
	// 检查目标Mark是否已成功定位
	if(!optionInfo.m_bEnableMutiTarget)
	{
		if (eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
		{
			if (FALSE == m_bTargetMarkSearchSucceed)
			{
				ReportPLCFunc(eCodeNG);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		

				return FALSE;
			}
		}

		//	CalculateTargetDistance(); 
		if (!CheckTargetDistance())
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_DIS_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			return FALSE;
		}

		if (!CheckTargetAngle())
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			return FALSE;
		}

	}

	// 检查对象Mark是否已成功定位
	if (eObjectBench != curProductData.m_pAlignerParam->m_eObjectMarkType)
	{
		if (FALSE == m_bObjectMarkSearchSucceed)
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CONFIRM_OBJECT_SEARCH_SUCCESS);//_T("请先保证实时对象Mark定位成功" );
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}	

		//		CalculateObjectDistance(); 
		if (!CheckObjectDistance())
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_DIS_OUT_RANGE);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	scTimer stAlignTimer;   double dAlignTime(0.0),dTotalTime(0.0);	
	BOOL bPlatformMoveOverflow = TRUE;

	for (int i = 0; i < nOffsetCount && i<ALIGNOFFSETNUM && i<ALIGNBENCHOFFSETNUM; i++)
	{
		stAlignTimer.Reset();
		//如果启用多目标，则设置多目标序号
		if (optionInfo.m_bEnableMutiTarget && i < MUTITARGETMAXNUM)
		{
			m_nMutiTargetCurIndex = i;
			m_bTargetMarkSearchSucceed = m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex);
			if (m_bTargetMarkSearchSucceed)
			{
				SetTargetMarkSearchResults(m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex));
				m_mpTargetMarkImagePos=m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex);
				int nPosNum = m_pPlatformInfo->m_nPositionNum;
				for (int i = 0;i<nPosNum;i++)
				{
					SetTargetPlatformPos(m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i),i);
					SetTempTargetPlatformAxisPos(i,m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i));
				}
				PostMessageShowMutiTargetImagePosToDisplay(m_bTargetMarkSearchSucceed);	
				//				CalculateTargetDistance(); 
				if (!CheckTargetDistance())
				{
					ReportPLCFunc(eCodeNG);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_DIS_OUT_RANGE);
					m_strStatusBarInfo2 = _T("");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
					return FALSE;
				}
				if (!CheckTargetAngle())
				{
					ReportPLCFunc(eCodeNG);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
					m_strStatusBarInfo2 = _T("");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
					return FALSE;
				}
			}	
			else
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
				m_strStatusBarInfo2.Format(_T("多目标_第%d组尚未定位成功"),i + 1);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
				return FALSE;
			}
		}

		EnterCriticalSection(&m_csAlignerOffset);
		pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(i);
		pCurProductData->m_pAlignerParam->SetCurAlignerBenchOffsetIndex(i);
		CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
		pAlignerParam->SetCurAlignerOffsetIndex(i);
		pAlignerParam->SetCurAlignerBenchOffsetIndex(i);
		LeaveCriticalSection(&m_csAlignerOffset);
		PostMessageUpdateAlignOffsetInfo();

		// 执行对位计算
		CCoordPos cpBenchOffset;
		CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL;	
		if (optionInfo.m_bEnableAlignBenchCenter == TRUE)
		{
			// 执行对位计算
			// 设置图像中心十字基准目标
			SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
			m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

			// 执行对位
			m_pAlignerTool->SetInnerAlignBench(TRUE);	// 设置为内部图像十字中心基准对位有效
			if(FALSE == ExecuteAlign())
			{
				m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
				// 设置实时的目标
				m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

				ReportPLCFunc(eCodeNG);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);

				m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();		

				
				return FALSE;     
			}
			pBenchPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
			cpBenchOffset = GetAlignPlatformPosOffset();

			m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
			// 设置实时的目标
			m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);		
		}

		// 执行对位计算
		if(FALSE == ExecuteAlign())
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);

			m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();		
			
			return FALSE;     
		}


		// 更新主界面对位精度、次数、对位总时间
		CCoordPos cpProduct = GetAlignProductOffset();
		if (pAlignerParam->GetEnableAlnTargetOffset())
		{
			cpProduct.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
			cpProduct.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
		}

		m_apAlignerProcesInfo.dXOffset  = cpProduct.m_dPosX;  
		m_apAlignerProcesInfo.dYOffset  = cpProduct.m_dPosY;
		m_apAlignerProcesInfo.dDOffset  = cpProduct.m_dAngle;

		CCoordPos cp = GetAlignPlatformOffset();
		if (pAlignerParam->GetEnableAlnTargetOffset())
		{
			cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
			cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
		}

		m_apAlignerProcesInfo.nTimes  = 1;

		m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
		m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
		m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;
		m_apAlignerProcesInfo.dTime = stAlignTimer.GetTimeMilli(TRUE);

		dAlignTime=m_apAlignerProcesInfo.dTime;
		dTotalTime += dAlignTime;

		vpPlatformXYDOffset.push_back(make_pair(cp, i));
		PostMessageUpdateAlignerProcessInfo();

		if(IsAxisMoveOverRun(cp))
		{//超运动量则失败 TT201106
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
			m_strStatusBarInfo2.Format(_T("第%d组(X:%.3f,Y:%.3f,D:%.3f)"),i + 1,cp.m_dPosX,cp.m_dPosY,cp.m_dAngle);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			bPlatformMoveOverflow = TRUE;
			break;
		}
		bPlatformMoveOverflow = FALSE;


		// 记录对位精度
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
		{
			CString strTemp;
			CString strOut;					
			strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
			strOut = strTemp + strOut;
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strOut);
		}

		// 记录对位时间
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
		{
			CString strTemp;
			CString strOut;						
			strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), 0, dAlignTime);
			strOut = strTemp + strOut;
			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strOut);
		}	
	}

	if (bPlatformMoveOverflow == TRUE)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dTotalTime);
	}
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();				

	ReportMulAlignPosResult_Ex(vpPlatformXYDOffset,m_bStatusBar);

	return TRUE;  
}
//回复发多个结果 相对偏差量
BOOL vcXYDVisionAlign:: ReportMulAlignPosResultFALP_Ex(std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送拾取 结果");

	BOOL bResult = TRUE;
	if(vvPlatformXYDAxisPos.size() <= 0)
	{
		bResult = FALSE;
	}
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSimpleInfo;
		m_XYDCommProtocol.PackReMulAlignPosInfoFALP_Ex(strSimpleInfo,vvPlatformXYDAxisPos,nResult,m_nPickMutiExMode);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;

		if (bResult)
		{
			// 打包写寄存器指令
			std::vector<int> vnSend;
			int nRegAddr;
			if (FALSE == m_XYDCommProtocol.PackRePlcMulAlignPosResultInfoFALPAxisPos_Ex(strSend,vnSend,nRegAddr,vvPlatformXYDAxisPos, nResult,m_nPickMutiExMode))
			{
				return FALSE;
			}

			bReceived = CommunicateWriteReg(vnSend,strInfo,nCommStatus,nRegAddr,true,false);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}


			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
		else
		{
			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "发送拾取 完成");

	return TRUE;
}
//拾取用这个
BOOL vcXYDVisionAlign::OnlineMulAlignPickExFALP_Ex(int nOffsetIndex)
{
	scTimer stAlignTimer;   double dAlignTime(0.0);	
	stAlignTimer.Reset();	
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();

	CString strCmd = _T("VALP:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");
	
	// 所有轴位置信息
	std::vector<CPlatformXYDAxisPos> vPlatformXYDAxisPos;				//单个扩展的全部结果
	std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos;	//全部扩展的全部结果

	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd)/*NULL == m_pPlatformInfo || NULL == m_pVisionASMConfig*/)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	for(int i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);	
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();		
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	// 设置补偿量
	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);

	PostMessageUpdateAlignOffsetInfo();

	SysPlatformInfo platformInfo;
	GetSysPlatformInfo(platformInfo);
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	//对象光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(0))
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	if ((m_vpObjectPlatformAxisPos.size()== 2) && (*(CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(0) == *(CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(1))/*&& m_bObjectMarkSearchSucceed == FALSE*/)
	{
		std::vector<int> vnPosIndex;
		if (optionInfo.m_bEnableMultiMarkMode)
		{
			vnPosIndex.push_back(0);
			VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴位置 开始");

			// 获取轴位置
			{
				CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
				BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
				// 直接读取寄存器地址轴数据
				bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

				if (FALSE == bCommuciateGetPlatformAxisAbsPos)
				{
					if (pPlatformAxisPos!=NULL)
					{
						delete pPlatformAxisPos;
						pPlatformAxisPos = NULL;					
					}

					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
					return FALSE;
				}
				else
				{
					for (int k=0;k<2;k++)
					{
						int nPosIndex = k;
						SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
						SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
						SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
					}

					

					m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
					m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
					m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
					// 记录当前平台各个轴的绝对位置
					if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
					{
						CString strTemp;
						CString strInfo;
						strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
							pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
						strInfo = strTemp + strInfo;	
						VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
						VisionAlignLogRecord(m_strAlignProcessData, strInfo);
					}


					if (pPlatformAxisPos)
					{
						delete pPlatformAxisPos;
						pPlatformAxisPos = NULL;
					}
				}
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴位置 结束");

			BOOL bIsSizeNG = FALSE;
			m_bObjectMarkSearchSucceed = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0,&bIsSizeNG);
			if (bIsSizeNG) //拍照过程中尺寸检测NG直接回复退出
			{
				ReportPLCFunc(eCodeSizeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_OBJECT_LEN_OVERFLOW); //__T("对象距离超过设定标准，对象定位结果可能出错");
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}
		}
	}
	else
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("位置1、2 轴位置不同错误");
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	if (FALSE == m_bObjectMarkSearchSucceed )
	{
		//搜索失败首先进行空盘检测
		std::vector<int> vnPosIndex;
		std::vector<SearchMode> vsmSearchMode;	
		vnPosIndex.push_back(0);		// 	
		vsmSearchMode.push_back(eInspectSearch);	// 
		m_vnInspectSearchStartPatIndex[0] = 4;
		m_vnInspectSearchEndPatIndex[0] = 4;
		if(SearchMark(vnPosIndex, vsmSearchMode))
		{
			//如果空盘检测OK则报告没有产品
			CPlatformXYDAxisPos tempPlatformXYDAxisPos;
			//ReportAlignResultFALP(&tempPlatformXYDAxisPos,2);
			// 空盘中没有产品 暂时回复4，通知PLC换盘
			//ReportPLCFunc(4,atoi(RREG_KEYENCE_VISION_PARAM_RESERVED)-atoi(RREG_KEYENCE_VISION_TRIGGER));
			//ReportPLCFunc(4);

			int  nDataNum = 6;
			std::vector<double> vdDate;
			vdDate.resize(nDataNum,0.0);
			int nSizeStatus = 4;
			ReportMulAlignPosResultFALP_Ex2(vdDate,eCodeNG,nSizeStatus);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拾取");
			m_strStatusBarInfo2 = _T("没有产品");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;     		
		}

		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
		if (!m_boIsSnapImgSucceed)
		{
			m_strStatusBarInfo2 = m_strStatusBarInfo2 + _T(",") + m_strSnapImgStatuString;
		}
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;     			
	}

	
	std::vector<sc2Vector> vvAreaPoint;
	// 每次对位轴位置偏差量
	CPlatformXYDAxisPos *pPlatformXYDAxisPos = NULL;
	pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());

	int nSuccessPatIndex = m_vnObjectSearchSuccessPatIndex[0];
	std::vector<CSearchResult> vTempSeachResults = GetAlnObjectSearchResults(0,nSuccessPatIndex);
	int nSearchNum = GetAlnObjectSearchResultNumber(0,nSuccessPatIndex);

	int nExNum = 1;//需要计算的扩展数量,不需要计算多扩展时仅计算一个，即当前扩展
	int	nMaxResultNum = 1000;
	if(m_nPickMutiExMode != 0)
	{//需要时计算全部扩展
		nExNum = m_pPlatformInfo->m_nMultiCalibExtensionMaxNum + 1;
		nMaxResultNum = m_nPickMutiExMode % 100;
	}
	
	int  nDataNum = 6;
	std::vector<double> vdDate;
	vdDate.resize(nDataNum,0.0);
	int nSizeStatus = 0;
	
	if(nSearchNum==2)
	{
		nSizeStatus=3;
	}
	else if(nSearchNum==0)
	{
		nSizeStatus=0;
	}
	

	for(int nExProductIndex = 0;nExProductIndex < nExNum;nExProductIndex++)
	{
		if(m_nPickMutiExMode != 0)
		{//设置扩展序号
			SetAlignExProductIndex(nExProductIndex);
		}
		vPlatformXYDAxisPos.clear();
		for (int i = 0; (i < nSearchNum) && (i < EXTRA_GRAPHICS_NUM_POS); i++)
		{
			if(m_nPickMutiExMode == 300)
			{//nMutiExMode == 300 时，仅计算与扩展序号相同序号的搜索结果
				BOOL bEnable = TRUE;
				if(bEnable && nSearchNum==1)
				{
					CBaseSearchTool* pSearchTool = GetRepoAlnObjectSearchTool(nCurProductIndex, 0, 0);
					if((pSearchTool->GetSearchToolType()==eMQuickPickSearchTool)
						||(pSearchTool->GetSearchToolType()==eMPatternPickSearchTool)
						||(pSearchTool->GetSearchToolType()==eMQuickPickCornerSearchTool)
						||(pSearchTool->GetSearchToolType()==eMPatternPickCornerSearchTool)
						||(pSearchTool->GetSearchToolType()==eAreaInspectPickTool))
					{     
						if(vTempSeachResults.at(i).m_bResultPickIndex==1)
						{
							nSizeStatus=1;
							if(nExProductIndex == 1)
							{
								continue;
							}
						}
						else if(vTempSeachResults.at(i).m_bResultPickIndex==2)
						{
							nSizeStatus=2;
							if(nExProductIndex == 0)
							{
								continue;
							}
						}
					}
				}
				else
				{
					if(i != nExProductIndex)
					{
						continue;
					}
				}
			}
			if((m_nPickMutiExMode >= 100 && m_nPickMutiExMode < 300) && i >= nMaxResultNum)
			{//nMutiExMode == 1xx/2xx 时，输出结果数受nMaxResultNum限制
				continue;
			}
			CCoordPos cpPosCH0;
			cpPosCH0.m_dPosX = vTempSeachResults.at(i).m_dPosX;
			cpPosCH0.m_dPosY = vTempSeachResults.at(i).m_dPosY;
			cpPosCH0.m_dAngle = vTempSeachResults.at(i).m_dAngle;

			CCoordPos cpPosCH1;
			cpPosCH1.m_dPosX = vTempSeachResults.at(i).m_vdAuxiliaryPosX.at(0);
			cpPosCH1.m_dPosY = vTempSeachResults.at(i).m_vdAuxiliaryPosY.at(0);
			cpPosCH1.m_dAngle = vTempSeachResults.at(i).m_vdAuxiliaryAngle.at(0);

			m_mpObjectMarkImagePos.SetMarkImagePos(0, m_vbSearchSucceed[0], cpPosCH0);
			m_mpObjectMarkImagePos.SetMarkImagePos(1, m_vbSearchSucceed[0], cpPosCH1);

			SetObjectMarkSearchResults(m_mpObjectMarkImagePos); 

			if (!CheckObjectDistance())
			{
				//ReportAlignResultFALP(pPlatformXYDAxisPos,FALSE);
				ReportPLCFunc(eCodeSizeNG);

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_OBJECT_LEN_OVERFLOW); //__T("对象距离超过设定标准，对象定位结果可能出错");
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			// 获取当前品种搜索成功的定位工具，如果是区域定位工具（威创达DM定制）则获取搜索区域 //默认1*1为不启用分区域补偿则不显示GUI
			if(pCurProductData->m_pAlignerParam->GetAlignOffsetByArea().m_bEnable)
			{
				// 判断是否在区域内
				sc2Vector ptMid((m_mpObjectMarkImagePos.m_vPos.at(0).m_dPosX + m_mpObjectMarkImagePos.m_vPos.at(1).m_dPosX) / 2, (m_mpObjectMarkImagePos.m_vPos.at(0).m_dPosY + m_mpObjectMarkImagePos.m_vPos.at(1).m_dPosY) / 2);
				nOffsetIndex = m_cVisionAlignGui.GetAlignOffsetByArea(ptMid);

				// 设置补偿量索引
				EnterCriticalSection(&m_csAlignerOffset);
				pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
				CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
				pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
				//pAlignerParam->SetAlignOffsetByArea(pCurProductData->m_pAlignerParam->GetAlignOffsetByArea());
				LeaveCriticalSection(&m_csAlignerOffset);

				PostMessageUpdateAlignOffsetInfo();

				vvAreaPoint.push_back(ptMid);

			}


			if(FALSE == ExecuteAlign(i))
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
				m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
				
				m_bObjectMarkSearchSucceed = FALSE;
				return FALSE;     
			}

			m_bObjectMarkSearchSucceed = FALSE;
			// 更新主界面对位精度、次数、对位总时间
			CCoordPos cp = GetAlignPlatformOffset();
			m_apAlignerProcesInfo.nTimes  = 1;
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;
			m_apAlignerProcesInfo.dTime = stAlignTimer.GetTimeMilli(FALSE);
			dAlignTime=m_apAlignerProcesInfo.dTime;

			PostMessageUpdateAlignerProcessInfo();	

			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);

			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			m_bStatusBar = TRUE;
			/*m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, dAlignTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();*/	

			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)(GetAlignPlatformMovementResults());

			CPlatformXYDAxisPos TempAxisMovePos;
			// 将绝对轴调整为相对偏移量
			if(optionInfo.m_bEnablePickSendRelativateAxis)
			{
				CCoordPos cp = GetAlignPlatformOffset();

				TempAxisMovePos.m_dPosX = cp.m_dPosX;
				TempAxisMovePos.m_dPosY = cp.m_dPosY;
				TempAxisMovePos.m_dAngle = cp.m_dAngle;

				if(m_nPickMutiExMode == 300 && nExNum==2)
				{
					vdDate[0+nExProductIndex*3] = cp.m_dPosX;
					vdDate[1+nExProductIndex*3] = cp.m_dPosY;
					vdDate[2+nExProductIndex*3] = cp.m_dAngle;
				}
			}
			else
			{
				TempAxisMovePos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				TempAxisMovePos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				TempAxisMovePos.m_dAngle = pPlatformXYDAxisPos->m_dAngle;
			}


			vPlatformXYDAxisPos.push_back(TempAxisMovePos);

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CTime t = CTime::GetCurrentTime();
				strTemp.Format(_T("\n\n%d-%d-%d-%d-%d-%d "),t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
				CString strOut;					
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
	
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CTime t = CTime::GetCurrentTime();
				strTemp.Format(_T("\n\n%d-%d-%d-%d-%d-%d "),t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
				CString strOut;						
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), 0, dAlignTime);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);			
			}	
		}
		vvPlatformXYDAxisPos.push_back(vPlatformXYDAxisPos);
	}
	if(m_nPickMutiExMode == 300)
	{//每个扩展仅有一个结果，所有结果集合到第一个扩展的结果中一并输出
		vvPlatformXYDAxisPos[0].resize(1);//扩展1的结果仅保留第1个
		for(int i = 0;i<vvPlatformXYDAxisPos.size();i++)
		{//第i个扩展的结果添加到第1个扩展之后
			if(vvPlatformXYDAxisPos[i].size() == 1)
			{
				vvPlatformXYDAxisPos[0].resize(i+1);				
				vvPlatformXYDAxisPos[0][i] = vvPlatformXYDAxisPos[i][0];
			}
			else
			{
				break;
			}
		}
		//仅保留修改后的扩展1的结果数据，当中保存的数据为扩展1的结果1，扩展2的结果2，扩展3的结果3。。。
		vvPlatformXYDAxisPos.resize(1);
	}

	if(pCurProductData->m_pAlignerParam->GetAlignOffsetByArea().m_bEnable)
	{
		m_cVisionAlignGui.SetAllPosAlignOffsetByArea(vvAreaPoint);
	}

	if(FALSE==optionInfo.m_bEnablePickSendRelativateAxis)
	{
		ReportMulAlignPosResultFALP_Ex(vvPlatformXYDAxisPos, TRUE);
	}
	else
	{
		ReportMulAlignPosResultFALP_Ex2(vdDate,eCodeOK,nSizeStatus);
	}

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);	
	m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), m_apAlignerProcesInfo.nTimes, stAlignTimer.GetTimeMilli(FALSE));
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();	

	return TRUE;  
}
// 自动关联
BOOL vcXYDVisionAlign::OnlineFCBPCalibrate_Ex(int nCalibPos, int nMarkType,int nExProductIndex/*=0*/)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);
	CString strCmd = _T("VCBP:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");

	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd) || !CheckCommuciation(strCmd) || !CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	// 平台为触发模式，标定时需要设置为非触发模式
	if (sysPlatformInfo.m_bCameraHardTriggerMode)
	{
		SetCameraTriggerMode(ePTNotTrigger);
	}

	BOOL bSucceed = FALSE;

	// 执行标定
	switch(nMarkType)
	{
	case 0:
		{
			std::vector<int> vnPosIndex;
			if (nCalibPos>=0 && nCalibPos<GetPosNum())
			{
				vnPosIndex.push_back(nCalibPos);
			}
			else if(nCalibPos == 9)
			{
				for (int i=0;i<GetPosNum();i++)
				{
					vnPosIndex.push_back(i);
				}
			}
			else if(nCalibPos == 11)
			{
				for (int i=0;i<GetPosNum()/2;i++)
				{
					vnPosIndex.push_back(i);
				}
			}
			else if(nCalibPos == 12)
			{
				for (int i=0;i<GetPosNum()/2;i++)
				{
					vnPosIndex.push_back(i+GetPosNum()/2);
				}
			}
			else if(nCalibPos >= 19 && nCalibPos < 28)
			{
				if (GetPosNum() == 8)
				{
					switch(nCalibPos)
					{
					case 19:
						{
							for (int i=0;i<GetPosNum()/4;i++)
							{
								vnPosIndex.push_back(i);
							}
						}
						break;
					case 20:
						{
							for (int i=0;i<GetPosNum()/4;i++)
							{
								vnPosIndex.push_back(i+GetPosNum()/4);
							}
						}
						break;
					case 21:
						{
							vnPosIndex.push_back(0);
							vnPosIndex.push_back(2);
						}
						break;
					case 22:
						{
							vnPosIndex.push_back(1);
							vnPosIndex.push_back(3);
						}
						break;
					case 23:
						{
							for (int i=0;i<GetPosNum()/4;i++)
							{
								vnPosIndex.push_back(i+GetPosNum()/2);
							}
						}
						break;
					case 24:
						{
							for (int i=0;i<GetPosNum()/4;i++)
							{
								vnPosIndex.push_back(i+GetPosNum()/2+GetPosNum()/4);
							}
						}
						break;
					case 25:
						{
							vnPosIndex.push_back(4);
							vnPosIndex.push_back(6);
						}
						break;
					case 26:
						{
							vnPosIndex.push_back(5);
							vnPosIndex.push_back(7);
						}
						break;
					}
				}
				else
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_PARAMETER_ERROR);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					break;
				}

			}
			else if(nCalibPos >= 256)
			{
				nCalibPos = nCalibPos - 255;

				int nMask = 1;
				for (int i=0; i<GetPosNum(); i++)
				{
					int nMaskResult = nMask<<i;

					if ( (nCalibPos & nMaskResult) == nMaskResult )
					{
						vnPosIndex.push_back(i);
					}
				}
			}
			else
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_PARAMETER_ERROR);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				break;

			}
			bSucceed = SerialCameraCalibAlignerTargetSearch(vnPosIndex); // 单相机关联搜索
		}
		break;
	case 1:
		{
			bSucceed = GetCalibAlignerTargetPlatformAxisPos();; // 获取关联基准轴位置
			m_bGetAutoCalibrateBaseAxisSucceed = bSucceed;
		}
		break;
	case 2:
		{
			bSucceed = ExecuteCalibrateTarget(nExProductIndex); 
		}
		break;
	default:
		break;
	}

	// 返回结果
	if (TRUE == bSucceed)
	{
		if(1)
		{
			// 流程序列没有执行完成时,回复PLC 3，通知PLC 需要继续发送关联指令
			int nIndex = eKeyenceCmdFCBP-1;
			if(m_nCurFCBP_Index >= m_pVisionASMConfig->m_vKeyenceCodeParam.at(nIndex).P1.size()-1)
			{
				ReportPLCFunc(eCodeOK);
			}
			else
			{
				ReportPLCFunc(eCodeContinued);
			}
		}
		else
		{
			ReportPLCFunc(eCodeOK);
		}
	}
	else
	{
		ReportPLCFunc(eCodeNG);
	}

	// 平台为触发模式时，标定完成后恢复触发模式
	if (sysPlatformInfo.m_bCameraHardTriggerMode)
	{
		SetCameraTriggerMode(ePTHardwareTrigger);
	}

	return bSucceed;
}
// FCBS
BOOL vcXYDVisionAlign::OnlineCalibrateFCBS_Ex(int nMarkType)
{
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	// 隐藏对象结果图形
	int nPosNum = m_pPlatformInfo->m_nPositionNum;
	std::vector<int> vnObjectPosIndex;
	int nObjectPosIndex = 0;
	int i = 0;
	for(i = 0; i < nPosNum; i++)
	{
		nObjectPosIndex = i;
		vnObjectPosIndex.push_back(nObjectPosIndex);
	}
	m_cVisionAlignGui.SetPosGuiInvisible(vnObjectPosIndex, eObjectSearch);

	// 隐藏目标结果图形	
	std::vector<int> vnTargetPosIndex;
	int nTargetPosIndex = 0;
	for(i = 0; i < nPosNum; i++)
	{
		nTargetPosIndex = i;
		vnTargetPosIndex.push_back(nTargetPosIndex);
	}
	m_cVisionAlignGui.SetPosGuiInvisible(vnTargetPosIndex, eTargetSearch);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	BOOL bSucceed = FALSE;
	std::vector<int> vnPosIndex;

	// 拾取系统，XY+D平台，相机安装在XY轴上
	if ( (m_pPlatformInfo->m_bPlatformPick) && (m_pPlatformInfo->m_ePickCamPlatformType == ePickCamPlatformShareXY))
	{
		// 情况1：全局二维码标定
		if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
		{
			// 1为角度标定，0为平移标定，2为计算
			bSucceed = FALSE;
		}
		// 情况2：传统标定+二维码标定
		else
		{
			// 1为角度标定，0为平移标定，2为计算
			i=0;
			vnPosIndex.push_back(i);
			bSucceed = MulCameraCalibrateFCBS_Ex(vnPosIndex, nMarkType);
		}
	}
	else
	{
		CString strCmd = _T("VCBS");
		CString strTempWarningType = _T("");
		GetCommCommandName(strCmd);

		if (ePlatformXYPD != m_pPlatformInfo->m_eMidPlatformType)
		{
			ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_EXCUTE_SYS_CALIB_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_INITIALIZATION_PLATFORMTYPE);
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_INITIALIZATION);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

			return FALSE;
		}

		// 情况1：全局二维码标定
		if (optionInfo.m_bEnableDmCodeCalib && GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
		{
			bSucceed = FALSE;
		}
		else // 情况2：传统标定+二维码标定
		{
			for(int it = 0; it < GetPosNum()/2; it++)
			{
				vnPosIndex.push_back(it+GetPosNum()/2);
			}
			bSucceed = MulCameraCalibrateFCBS_Ex(vnPosIndex, nMarkType);
		}
	}

	if (TRUE == bSucceed)
	{
		if(nMarkType==0)
		{
			if(m_nCurFCBS_XY_Index<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurFCBS_XY_Index>0)
			{
				//ReportPLCFunc(eCodeContinued,2);// 向D4中写3
				ReportPLCFunc(eCodeContinued,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			}
			else if(m_nCurFCBS_XY_Index==m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
			{
				//ReportPLCFunc(eCodeOK,2);// 向D4中写1 OK 
				ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				//m_nCurFCBS_XY_Index = 0;
				for(int i = 0; i<m_vvpMarkImagePos.size();i++)
				{
					for(int j = 0; j<m_vvpMarkImagePos.at(i).size();j++)
					{
						if (m_vvpMarkImagePos.at(i).at(j) != NULL)
						{
							delete m_vvpMarkImagePos.at(i).at(j);
							m_vvpMarkImagePos.at(i).at(j) = NULL;
						}
					}
					m_vvpMarkImagePos.at(i).clear();
				}

				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				m_strStatusBarInfo2 = _T("继续标定");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}

		}
		else if(nMarkType==1)
		{
			if(m_nCurFCBS_Angle_Index<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurFCBS_Angle_Index>0)
			{
				//ReportPLCFunc(eCodeContinued,2);// 向D4中写3
				ReportPLCFunc(eCodeContinued,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			}
			else if(m_nCurFCBS_Angle_Index==m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
			{
				//ReportPLCFunc(eCodeOK,2);// 向D4中写1 OK 
				ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
				//m_nCurFCBS_Angle_Index = 0;
				for(int i = 0; i<m_vvpMarkImagePos.size();i++)
				{
					for(int j = 0; j<m_vvpMarkImagePos.at(i).size();j++)
					{
						if (m_vvpMarkImagePos.at(i).at(j) != NULL)
						{
							delete m_vvpMarkImagePos.at(i).at(j);
							m_vvpMarkImagePos.at(i).at(j) = NULL;
						}
					}
					m_vvpMarkImagePos.at(i).clear();
				}

				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);
				m_strStatusBarInfo2 = _T("继续标定");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}

		}
		else if(nMarkType == 2)
		{
			ReportPLCFunc(eCodeOK,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));
		}

		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_SUCCEED);

		if	(nMarkType == 0)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_XY_SUCCEED);//_T("平台XY轴平移标定成功");
		}
		else if(nMarkType == 1)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_D_SUCCEED);//_T("平台D轴旋转标定成功");
		}
		else if(nMarkType == 2)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_CALCU_SUCCEED);//_T("平台标定计算成功");
		}

		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
	}
	else
	{

		ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);

		if	(nMarkType == 0)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_XY_FAILED);//_T("平台XY轴平移标定失败");
		}
		else if(nMarkType == 1)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_D_FAILED);//_T("平台D轴旋转标定失败");
		}
		else if(nMarkType == 2)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_CALCU_FAILED);//_T("平台标定计算失败");
		}
		if (!m_boIsSnapImgSucceed)
		{
			m_strStatusBarInfo2 = m_strSnapImgStatuString;
		}

		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

	}

	return bSucceed;
}
BOOL vcXYDVisionAlign::MulCameraCalibrateFCBS_Ex(std::vector<int> vnPosIndex, int nMarkType)
{

	CString strCmd = _T("VCBS");
	CString strTempWarningType = _T("");
	GetCommCommandName(strCmd);

	int nPosNum = vnPosIndex.size();
	if(nPosNum<1 || nPosNum>MAXPOSNUM)
	{
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	for(int it=0;it<nPosNum;it++)
	{
		if(vnPosIndex.at(it)<0 || vnPosIndex.at(it)>m_pPlatformInfo->m_nPositionNum)
		{
			ReportPLCFunc(eCodeNG);
			return FALSE;
		}
	}

	int nAllCalibPos = nPosNum;


	if (nMarkType==0)
	{
		if(m_nCurFCBS_XY_Index >= m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) || m_nCurFCBS_XY_Index<0)
		{
			ReportPLCFunc(eCodeNG);
			return FALSE;
		}
	} 
	else if(nMarkType==1)
	{
		if(m_nCurFCBS_Angle_Index >= m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) || m_nCurFCBS_Angle_Index<0)
		{
			ReportPLCFunc(eCodeNG);
			return FALSE;
		}
	}


	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	if (optionInfo.m_bEnableDmCodeCalib && (GetCalibMarkPatternMode() == eCalibUseDmCode))
	{
		// 暂不支持二维码标定
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	int i = 0;

	if((nMarkType < 0 || nMarkType > 2) && nMarkType!=5 )
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_CALIBTYPE);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		return FALSE;
	}


	// 标定开始    
	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_CALIBRATING);
	m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();


	//只计算
	if(nMarkType == 2)
	{
		std::vector<CMarkImagePos*> Nullimage;
		Nullimage.clear();

		for (int it=0;it<nAllCalibPos;it++)
		{
			int nPosIndex = vnPosIndex[it];
			if(FALSE == ExecuteCalibrateFCBS(Nullimage,nPosIndex,nMarkType))
			{
				ReportPLCFunc(eCodeNG);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
				m_strStatusBarInfo2 = _T("");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			//////////////////////////////////////// 获取标定结果///////////////////////////////////////////

			// 获取系统标定后的所有信息
			std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
			vpAllCalibratedInfo = GetAllCalibratedInfo();
			
			SetCurCalibratedInfo(vpAllCalibratedInfo);	
			SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

			if (!ExecuteCalibrate_ObjectBench(nPosIndex))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
				m_strStatusBarInfo2 = _T("执行对象标准基准计算失败！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				ReportPLCFunc(eCodeNG);
				return FALSE;
			}

			if (!ExecuteCalibrate_CopyInfo_SingleCamera(nPosIndex, nMarkType))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
				m_strStatusBarInfo2 = _T("执行对象目标虚拟时的标定拷贝失败！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				ReportPLCFunc(eCodeNG);
				return FALSE;
			}
		}

		int nProductIdx=(GetTotalProuductData()->GetCurProductIndex());
		CProductData *pProductData=(GetTotalProuductData()->GetItemPtr(nProductIdx));
		if (sysPlatformInfo.m_bTargetObjectCamSeparate == TRUE && sysPlatformInfo.m_nPositionNum == 8 
			&& pProductData->m_pAlignerParam->GetAligner4Vs4Type()==eAlignerPos1234_56_4V2)
		{
			for (int it=0;it<nAllCalibPos;it++)
			{
				int nPosIndex = vnPosIndex[it];
				BOOL bResult=FALSE;
				std::vector<int> vnPos;
				int nExProductIdx = 0;
				if (nPosIndex==4 + nExProductIdx*GetPosNum())
				{
					vnPos.push_back(6 + nExProductIdx*GetPosNum());
					bResult = SetSameCalibrateInfo(nPosIndex,vnPos);
				}
				if (nPosIndex==5 + nExProductIdx*GetPosNum())
				{
					vnPos.push_back(7 + nExProductIdx*GetPosNum());
					bResult = SetSameCalibrateInfo(nPosIndex,vnPos);
				}

				if (FALSE == bResult)
				{
					ReportPLCFunc(eCodeNG);
					return OnFailedSetSameCalibrateInfo(strCmd);
				}
			}
			std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
			vpAllCalibratedInfo = GetAllCalibratedInfo();

			SetCurCalibratedInfo(vpAllCalibratedInfo);	
			SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);
		}


		// 保存目标定位结果信息
		for (int it=0;it<nAllCalibPos;it++)
		{
			int nPosIndex = vnPosIndex[it];
			m_bTargetMarkSearchSucceed = FALSE;	
			m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, FALSE, m_mpTargetMarkImagePos.GetMarkImagePos(nPosIndex));
			SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			SaveTargetPosInfo(GetTotalProuductData()->GetCurProductIndex());

			if (!ExecuteCalibrate_TargetBench(nPosIndex))
			{
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
				m_strStatusBarInfo2 = _T("设置标定位置为目标基准位置失败！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				ReportPLCFunc(eCodeNG);
				return FALSE;
			}

			// ...标定结果分析过程日志
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CalibrateParam_LogRecord(nPosIndex);
				CalibrateResult_LogRecord(nPosIndex, nMarkType);
			}

		}
		return TRUE;
	}


	// ...标定结果分析过程日志-标定相关参数
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		int nCurProductIndexFa = 0;
		int nCurProductIndexCh = 0;
		int nExProductIndex = 0;
		m_pVisionASMConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa, nCurProductIndexCh);

		nExProductIndex = 0 ;			// 扩展产品序号

		for (int it=0;it<nAllCalibPos;it++)
		{
			int nPosIndex = vnPosIndex[it];
			CString strInfo;
			if (0 == nMarkType)
			{
				strInfo.Format(_T("	XY标定大品种%d, 小品种%d, 扩展产品%d, 位置%d	\n"),nCurProductIndexFa + 1, nCurProductIndexCh + 1, nExProductIndex + 1, nPosIndex + 1);
				VisionAlignLogRecord(m_strCalibrateProcessPath1, strInfo, TRUE);
			}
			else if (1 == nMarkType)
			{
				strInfo.Format(_T("	D标定大品种%d, 小品种%d, 扩展产品%d, 位置%d	\n"),nCurProductIndexFa + 1, nCurProductIndexCh + 1, nExProductIndex + 1, nPosIndex + 1);
				VisionAlignLogRecord(m_strCalibrateProcessPath1, strInfo, TRUE);
			}
		}	
	}

	int nRemCalibPos = 0;
	// 1. 获取平台当前各轴的绝对位置（基准位置）,并设置入对位工具
	if((m_nCurFCBS_XY_Index==0 && nMarkType==0) || (m_nCurFCBS_Angle_Index==0 && nMarkType==1))
	{
		CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
		BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
		bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);
		
		if (FALSE == bCommuciateGetPlatformAxisAbsPos)
		{
			ReportPLCFunc(eCodeNG);
			if (pPlatformAxisPos!=NULL)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;					
			}
			OnFailedCommuciateGetPlatformAxisAbsPos(strCmd);
			return FALSE;
		}
		else
		{
			SetCalibPlatformPos(pPlatformAxisPos);		
			if (m_CalibPlatformAxisAbsPos.size()<nRemCalibPos+1)
			{
				m_CalibPlatformAxisAbsPos.resize(nRemCalibPos+1);
			}
			m_CalibPlatformAxisAbsPos.at(nRemCalibPos)= *((CPlatformXYDAxisPos*)pPlatformAxisPos);

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_THE_PLATFORM_POS), pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);


				// ...标定结果分析过程日志
				strTemp.Empty();
				strInfo.Empty();

				if (nMarkType==0)
				{
					strTemp.Format(_T("平台标定XY时平台的绝对位置:\n"));
				}
				else if (nMarkType==1)
				{
					strTemp.Format(_T("平台标定D时平台的绝对位置:\n"));				
				}
				strInfo += strTemp;

				strTemp.Format(_T("......X轴: %.3f; Y轴: %.3f; D轴: %.3f;\n"),pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
				strInfo += strTemp;

				VisionAlignLogRecord(m_strCalibrateProcessPath1, strInfo, FALSE);

			}

			if (pPlatformAxisPos)
			{
				delete pPlatformAxisPos;
				pPlatformAxisPos = NULL;
			}
		}
	}


	// 2. 获取平移标定和旋转标定时平台各轴的移动位置
	std::vector<CPlatformAxisPos*> vpPlatformAxisPos; 
	std::vector<CPlatformAxisPos*> vpPlatformAxisPosAbsBase; 
	vpPlatformAxisPosAbsBase = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);

	// 向D10-14中写入相对偏差量 临时设置启用相对偏差量
	m_pAlignerTool->SetCalibSendRelativePos(TRUE);
	vpPlatformAxisPos = GetCalibPlatformAxisMovePos(nRemCalibPos,nMarkType);
	m_pAlignerTool->SetCalibSendRelativePos(FALSE);

	if (vpPlatformAxisPos.size()!=m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType))
	{
		ReportPLCFunc(eCodeNG);		
		return OnFailedGetCalibPlatformAxisMovePos(strCmd);
	}
	else
	{
		// 记录平台标定时平台需移动的7个位置各个轴的绝对位置
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
		{
			CString strTemp;
			CString strInfo;
			strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CALIB_SEVEN_PLATFORM_MOVE_POS);
			strInfo = strTemp + strInfo;

			CPlatformXYDAxisPos* pAxisPos;
			for (int i=0; i<vpPlatformAxisPos.size(); i++)
			{
				strTemp.Empty();
				pAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(i);           

				strTemp.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_CALIB_PLATFORM_POS), i, pAxisPos->m_dPosX, pAxisPos->m_dPosY, pAxisPos->m_dAngle);
				strInfo = strInfo + strTemp;
			}

			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strInfo);

			// ...标定结果分析过程日志
			strTemp.Empty();
			strInfo.Empty();

			if (nMarkType==0)
			{
				strTemp.Format(_T("平台标定XY时所需要移动的绝对轴位置:\n"));
			}
			else if (nMarkType==1)
			{
				strTemp.Format(_T("平台标定D时所需要移动的绝对轴位置:\n"));
			}
			strInfo += strTemp;


			CPlatformXYDAxisPos* pTempAxisPos;
			for (int i=0; i<vpPlatformAxisPos.size(); i++)
			{
				strTemp.Empty();
				pTempAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(i);           
				strTemp.Format(_T("......平台位置%d: X轴: %.3f; Y轴: %.3f; D轴: %.3f;\n"), i, pTempAxisPos->m_dPosX, pTempAxisPos->m_dPosY, pTempAxisPos->m_dAngle);
				strInfo = strInfo + strTemp;

			}

			VisionAlignLogRecord(m_strCalibrateProcessPath1, strInfo, FALSE);
		}
	}

	int nProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	CCalibData *pCalibData = m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nProductIndex);
	if (pCalibData == NULL || !pCalibData->IsValid())
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBDATA_FAILED);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

		return FALSE;
	}

	CCalibrateParam *pCalibrateParam = pCalibData->m_vpCalibrateParam.at(nRemCalibPos);
	if (pCalibrateParam == NULL)
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_WARNING_PROCESSING_GETCALIBPARAM_FAILED);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

		return FALSE;
	}

	// 3. 驱动平台到位并定位
	CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
	std::vector<CMarkImagePos*> vpMarkImagePos;
	CMarkImagePos* pMarkImagePos = NULL;
	CMarkImagePos* pDisCorMarkImagePos = NULL;

	int k=0;
	//for (k=0; k<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType); k++)

	if(nMarkType==0)
	{
		k = m_nCurFCBS_XY_Index;
	}
	else if(nMarkType==1)
	{
		k = m_nCurFCBS_Angle_Index;
	}		


	std::vector<CMarkImagePos*> vpTempMarkImagePos;
	std::vector<CMarkImagePos*> vpTempDisCorMarkImagePos;
	{
		m_bStatusBar = TRUE;
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_IS_DRIVING_PLATFORM_MOVE), k);
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_WAIT);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

		// 驱动到位
		pPlatformXYDAxisPos = NULL;
		pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vpPlatformAxisPos.at(k);

		if(FALSE == CommuciateDrivePlatformAxisMoveEx(pPlatformXYDAxisPos,(CPlatformXYDAxisPos*)vpPlatformAxisPosAbsBase.at(k)))
		{
			ReportPLCFunc(eCodeNG);
			for (int i = 0; i < vpTempMarkImagePos.size(); i++)
			{
				if (vpTempMarkImagePos[i])
				{
					delete vpTempMarkImagePos[i];
					vpTempMarkImagePos[i] = NULL;
				}
			}

			for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
			{
				if (vpTempDisCorMarkImagePos[i])
				{
					delete vpTempDisCorMarkImagePos[i];
					vpTempDisCorMarkImagePos[i] = NULL;
				}
			}
			return OnFailedCommuciateDrivePlatformAxisMove(strCmd, k);
		}

		Sleep(100);
		// 等待搜索延时
		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		Sleep(m_pVisionASMConfig->m_TotalCalibData.GetItemPtr(nCurProdcutIndex)->m_vpCalibrateParam[nRemCalibPos]->m_nSearchDelayTime);


		// 将相机移动到多个位置，逐个定位
		pMarkImagePos = NULL;
		pDisCorMarkImagePos = NULL;




		// 为每个位置各申请一个保存图像坐标的内存
		for (int i=0;i<nAllCalibPos;i++)
		{
			pMarkImagePos = new CMarkImagePos;
			pMarkImagePos->SetMarkImagePosNum(1);
			vpTempMarkImagePos.push_back(pMarkImagePos);

			pDisCorMarkImagePos = new CMarkImagePos;
			pDisCorMarkImagePos->SetMarkImagePosNum(1);
			vpTempDisCorMarkImagePos.push_back(pDisCorMarkImagePos);
		}


		// 准备开始定位			
		std::vector<SearchMode> vsmSearchMode;	

		if (optionInfo.m_bEnableDmCodeCalib && (GetCalibMarkPatternMode() == eCalibUseDmCode))
		{
			vsmSearchMode.push_back(eCalibSearch);	// 标定搜索
		}
		else
		{
			if (nMarkType==0)
			{
				for(int it = 0; it < nAllCalibPos; it++)
				{
					vsmSearchMode.push_back(eCalibSearch); // 标定搜索
				}
			}
			else if(nMarkType==1)
			{
				for(int it = 0; it < nAllCalibPos; it++)
				{
					vsmSearchMode.push_back(eTargetSearch);	// 目标搜索
				}
			}
		}

		// 采集搜索图像
		if (!SnapSearchImage(vnPosIndex))
		{			

			ReportPLCFunc(eCodeNG);				

			if (pMarkImagePos!=NULL)
			{
				delete pMarkImagePos;
				pMarkImagePos = NULL;
			}

			if(pDisCorMarkImagePos != NULL)
			{
				delete pDisCorMarkImagePos;
				pDisCorMarkImagePos = NULL;
			}

			for (int p=0; p<vpMarkImagePos.size(); p++)
			{
				if (vpMarkImagePos.at(p))
				{
					delete vpMarkImagePos.at(p);
					vpMarkImagePos.at(p) = NULL;
				}
			}  

			return OnFailedSnapSearchImage(strCmd);
		}


		{
			// 定位
			if (nMarkType==0)
			{
				for(int i = 0; i < nAllCalibPos; i++)
				{
					int nPosIndex = vnPosIndex[i];
					m_vnCalibSearchStartPatIndex[nPosIndex] = 0;
					m_vnCalibSearchEndPatIndex[nPosIndex] = 0;
				}
			}
			else if(nMarkType==1)
			{
				for(int i = 0; i < nAllCalibPos; i++)
				{
					int nPosIndex = vnPosIndex[i];
					m_vnTargetSearchStartPatIndex[nPosIndex] = 0;
					m_vnTargetSearchEndPatIndex[nPosIndex] = 0;
				}
			}
		}


		BOOL bSearchSucceed = SearchMark(vnPosIndex, vsmSearchMode);
		if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
		{
			ExeSaveVDBFile(vnPosIndex,eCalibSearch);
		}

		if(nMarkType==1)
		{
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
		}

		// 分析定位结果，发送指令响应
		if (FALSE == bSearchSucceed)
		{
			ReportPLCFunc(eCodeNG);		

			for (int i = 0; i < vpTempMarkImagePos.size(); i++)
			{
				if (vpTempMarkImagePos[i])
				{
					delete vpTempMarkImagePos[i];
					vpTempMarkImagePos[i] = NULL;
				}
			}

			for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
			{
				if (vpTempDisCorMarkImagePos[i])
				{
					delete vpTempDisCorMarkImagePos[i];
					vpTempDisCorMarkImagePos[i] = NULL;
				}
			}

			for (int p=0; p<vpMarkImagePos.size(); p++)
			{
				if (vpMarkImagePos.at(p))
				{
					delete vpMarkImagePos.at(p);
					vpMarkImagePos.at(p) = NULL;
				}
			}  

			return OnFailedCalibSearch(strCmd, k, vnPosIndex[0]);
		}

		// 保存定位结果	
		{
			if (nMarkType==0)
			{	
				if(pCalibrateParam->m_nEnableDisCor)
				{
					for(int i = 0; i < nAllCalibPos; i++)
					{
						CCoordPos OutPutPoint;
						m_pAlignerTool->ExecuteDisCorPoint(vnPosIndex[i], m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]), OutPutPoint);
						vpTempDisCorMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), OutPutPoint);
					}

				}

				for(int i=0;i<nAllCalibPos;i++)
				{
					vpTempMarkImagePos[i]->SetMarkImagePos(0, m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]));;
				}
			}
			else if (nMarkType==1)
			{
				if(pCalibrateParam->m_nEnableDisCor)
				{
					for(int i = 0; i < nAllCalibPos; i++)
					{
						CCoordPos OutPutPoint;
						m_pAlignerTool->ExecuteDisCorPoint(vnPosIndex[i], m_mpTargetMarkImagePos.GetMarkImagePos(vnPosIndex[i]), OutPutPoint);
						vpTempDisCorMarkImagePos[i]->SetMarkImagePos(0, m_mpTargetMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), OutPutPoint);
					}
				}

				for(int i=0;i<nAllCalibPos;i++)
				{
					vpTempMarkImagePos[i]->SetMarkImagePos(0, m_mpTargetMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]), m_mpTargetMarkImagePos.GetMarkImagePos(vnPosIndex[i]));;
				}

			}

			if(pCalibrateParam->m_nEnableDisCor)
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempDisCorMarkImagePos[i]);
					m_vvpMarkImagePos[i].push_back(markpos);
				}
			}
			else
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{					 
					CMarkImagePos * markpos = new CMarkImagePos(*vpTempMarkImagePos[i]);
					m_vvpMarkImagePos[i].push_back(markpos);
				}
			}
		}


		// 记录图像搜索数据
		if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
		{
			CString strTemp;
			CString strOut;
			CCoordPos cp;
			strOut = strTemp + _T("CalibSearch: ");	

			for (int i=0; i< vpTempMarkImagePos.size(); i++)
			{
				strTemp = _T("");
				cp = vpTempMarkImagePos[i]->GetMarkImagePos(0);
				strTemp.Format(_T("CalibPos%d-MarkPos%d( %.2f , %.2f , %.3f )"), i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
				strOut += strTemp;
			}

			if(pCalibrateParam->m_nEnableDisCor)
			{
				for (int i=0; i< vpTempDisCorMarkImagePos.size(); i++)
				{
					strTemp = _T("");
					cp = vpTempDisCorMarkImagePos[i]->GetMarkImagePos(0);
					strTemp.Format(_T("CalibPos%d-DisCorMarkPos%d( %.2f , %.2f , %.3f )"),i, k, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
					strOut += strTemp;
				}
			}

			VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
			VisionAlignLogRecord(m_strAlignProcessData, strOut);
		}

		// 执行完成一个标定走位过程
		if (nMarkType==0)
		{
			m_nCurFCBS_XY_Index++;

			// 回复PLC 继续标定过程；
			if(m_nCurFCBS_XY_Index<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurFCBS_XY_Index>0)
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				// 返回局部成功，还需要继续标定
				// D2中写拍照成功
				ReportPLCFunc(eCodeOK);
				return TRUE;
			}


		}
		else if(nMarkType==1)
		{
			m_nCurFCBS_Angle_Index++;

			// 回复PLC 继续标定过程；
			if(m_nCurFCBS_Angle_Index<m_pAlignerTool->GetCalibPlatformMovePosNum(nMarkType) && m_nCurFCBS_Angle_Index>0)
			{
				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}

				// 返回局部成功，还需要继续标定
				// D2中写拍照成功
				ReportPLCFunc(eCodeOK);
				return TRUE;
			}

		}

	}

	// 4. 将平台7个位置的所有图像搜索结果，设置入对位工具
	BOOL bOk=TRUE,bOk1=TRUE;
	{
		for (int i=0;i<nAllCalibPos;i++)
		{				
			bOk=ExecuteCalibrateFCBS(m_vvpMarkImagePos[i], vnPosIndex[i], nMarkType);
			if(!bOk)
			{	
				// 标定失败 D4写2
				//ReportPLCFunc(eCodeNG,2);
				ReportPLCFunc(eCodeNG,atoi(RREG_KEYENCE_VISION_CALIB_TRIGGER)-atoi(RREG_KEYENCE_VISION_TRIGGER));

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_CALIB_CALCULATE_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));

				for (int i = 0; i < vpTempMarkImagePos.size(); i++)
				{
					if (vpTempMarkImagePos[i])
					{
						delete vpTempMarkImagePos[i];
						vpTempMarkImagePos[i] = NULL;
					}
				}

				for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
				{
					if (vpTempDisCorMarkImagePos[i])
					{
						delete vpTempDisCorMarkImagePos[i];
						vpTempDisCorMarkImagePos[i] = NULL;
					}
				}		
				return FALSE;
			}
			else
			{
				std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
				vpAllCalibratedInfo = GetAllCalibratedInfo();
				/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
				SetCurCalibratedInfo(vpAllCalibratedInfo);	

				// 保存标定后信息到当前产品中及配置文件中
				SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

			}
		}
	}


	// ...标定结果分析过程日志
	if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
	{
		CString strInfo = _T("");		
		CString strTemp = _T("");	

		for(int it=0;it<m_vvpMarkImagePos.size();it++)
		{
			if (nMarkType==0)
			{
				strTemp.Format(_T("平台标定XY时图像坐标:\n"));
			}
			else if (nMarkType==1)
			{
				strTemp.Format(_T("平台标定D时图像坐标:\n"));
			}
			strInfo += strTemp;

			for (int i=0; i<m_vvpMarkImagePos[it].size(); i++)
			{
				strTemp.Empty();
				CCoordPos cp;
				cp = m_vvpMarkImagePos[it].at(i)->GetMarkImagePos(0);
				strTemp.Format(_T("......MarkPos%d: X: %.2f; Y: %.2f; D: %.3f;\n"), i, cp.GetPosX(), cp.GetPosY(), cp.GetAngle());
				strInfo += strTemp;

			}

			VisionAlignLogRecord(m_strCalibrateProcessPath1, strInfo, FALSE);
		}
	}


	for (int i = 0; i < vpTempMarkImagePos.size(); i++)
	{
		if (vpTempMarkImagePos[i])
		{
			delete vpTempMarkImagePos[i];
			vpTempMarkImagePos[i] = NULL;
		}
	}

	for (int i = 0; i < vpTempDisCorMarkImagePos.size(); i++)
	{
		if (vpTempDisCorMarkImagePos[i])
		{
			delete vpTempDisCorMarkImagePos[i];
			vpTempDisCorMarkImagePos[i] = NULL;
		}
	}	

	return TRUE;
}
BOOL vcXYDVisionAlign::OnlineAlignExFAAL_Base_Ex(int nOffsetIndex, int nReservedIndex/* = 10*/, int nBenchOffsetIndex/* = -1*/, int nExProductIndex/* = 0*/,int nTarFixOffsetIndex /*= 0*/)
{
	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}

	BOOL bBendInspect = pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendAlignInspect;

	if (bBendInspect)
	{
		m_bFaalBendProcess = TRUE;
		return OnlineAlignExFAAL_BendAlign_Ex(nOffsetIndex, nReservedIndex, nBenchOffsetIndex, nExProductIndex,nTarFixOffsetIndex);
	}
	else
	{
		return OnlineAlignExFAAL_Ex(nOffsetIndex, nReservedIndex, nBenchOffsetIndex, nExProductIndex,nTarFixOffsetIndex);
	}

	return TRUE;
}
//非折弯FAAL
BOOL vcXYDVisionAlign::OnlineAlignExFAAL_Ex(int nOffsetIndex, int nReservedIndex/* = 10*/, int nBenchOffsetIndex/* = -1*/, int nExProductIndex/* = 0*/,int nTarFixOffsetIndex /*= 0*/)
{
	scTimer stTotalAlignTimer;
	double dTotalAlignTime(0.0);

	stTotalAlignTimer.Reset();

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	CString str;
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");
	CString strCmd = _T("VAAL:");
	CString strTempWarningType = _T("");
	scTimer stCheckInvalidTimer;   double dCheckInvalidTime(0.0);

	stCheckInvalidTimer.Reset();
	// 避免没有进入产品设置，导致对位工具类为空时的崩溃
	if (m_pAlignerTool == NULL) 
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	SetAlignReservedIndex(nReservedIndex);
	SetAlignBenchOffsetIndex(nBenchOffsetIndex);
	SetAlignExProductIndex(nExProductIndex);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	// start 直接根据searchdata中图像坐标和轴位置 设置进对位工具中；
	{
		int nSize1 = m_vvpTargetPlatformAxisPosMultiEx.size();
		int nSize2 = m_vvpObjectPlatformAxisPosMultiEx.size();
		int nSize3 = m_vmpObjectMarkImagePos_MultiEX.size();
		int nSize4 = m_vmpTargetMarkImagePosMultiEx.size();
		bool bTmp = (nExProductIndex >=0) &&  (nExProductIndex <nSize1) &&  (nExProductIndex <nSize2) &&  (nExProductIndex <nSize3)  &&  (nExProductIndex <nSize4);

		if (bTmp)
		{

			vector<CPlatformAxisPos*> tmpObjectPlatformAxisPos;
			vector<CPlatformAxisPos*> tmpTargetPlatformAxisPos;
			tmpObjectPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);
			tmpTargetPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);

			if (m_pPlatformInfo->m_bEnableMultiCalibExtension)
			{
				if (optionInfo.m_bObjectSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{
						tmpObjectPlatformAxisPos.at(jt) = m_vvpObjectPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpObjectMarkImagePos = m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex);
				}
				else
				{
					tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				}

				if (optionInfo.m_bTargetSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{					
						tmpTargetPlatformAxisPos.at(jt) = m_vvpTargetPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}		
					m_mpTargetMarkImagePos = m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex);
				}
				else
				{
					tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
				}

			}
			else
			{
				tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
			}

			m_pAlignerTool->SetTargetPlatformPos(tmpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			m_pAlignerTool->SetObjectPlatformPos(tmpObjectPlatformAxisPos);
			m_pAlignerTool->SetObjectMarkSearchResults(m_mpObjectMarkImagePos);

			//lzk 0417
			std::vector<int> nObjectPosIndex;
			std::vector<int> nTargetPosIndex;
			CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
			BOOL bObjectPositionVirtualMod = pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;
			BOOL bTargetPositionVirtualMod = pAlignerParam->GetProductPositionVirtualInfo().m_bEnableTargetPositionVirtualMode;
			BOOL bTargetAndObjectPositionVirtualMod = m_pAlignerTool->GetEnablePositionVirtualMode();
			int nObjectPositionVirtual = pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
			int nTargetPositionVirtual = pAlignerParam->GetProductPositionVirtualInfo().m_nTargetPositionVirtualMode;
			//对象位置虚拟
			if (bTargetAndObjectPositionVirtualMod && bObjectPositionVirtualMod)
			{
				//对象位置虚拟模式：12拍照34虚拟(BOOL)
				if (nObjectPositionVirtual == 0)
				{
					nObjectPosIndex.push_back(0);
					nObjectPosIndex.push_back(1);
				}
				//对象位置虚拟模式：34拍照12虚拟
				else if (nObjectPositionVirtual == 1)
				{
					nObjectPosIndex.push_back(2);
					nObjectPosIndex.push_back(3);
				}
				//对象位置虚拟模式：13拍照24虚拟
				else if (nObjectPositionVirtual == 2)
				{
					nObjectPosIndex.push_back(0);
					nObjectPosIndex.push_back(2);
				}
				//对象位置虚拟模式：24拍照13虚拟
				else if (nObjectPositionVirtual == 3)
				{
					nObjectPosIndex.push_back(1);
					nObjectPosIndex.push_back(3);
				}

				m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK(nObjectPosIndex);
			}
			else
			{
				m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();
			}

			//目标位置虚拟
			if (bTargetAndObjectPositionVirtualMod && bTargetPositionVirtualMod)
			{
				//目标位置虚拟模式：56拍照78虚拟
				if (nTargetPositionVirtual == 0)
				{
					nTargetPosIndex.push_back(4);
					nTargetPosIndex.push_back(5);
				}
				//目标位置虚拟模式：78拍照56虚拟
				else if (nTargetPositionVirtual == 1)
				{
					nTargetPosIndex.push_back(6);
					nTargetPosIndex.push_back(7);
				}
				//目标位置虚拟模式：57拍照68虚拟
				else if (nTargetPositionVirtual == 2)
				{
					nTargetPosIndex.push_back(4);
					nTargetPosIndex.push_back(6);
				}
				//目标位置虚拟模式：68拍照57虚拟
				else if (nTargetPositionVirtual == 3)
				{
					nTargetPosIndex.push_back(5);
					nTargetPosIndex.push_back(7);
				}
				m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK(nTargetPosIndex);
			}
			else
			{
				m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
			}

		}
	}

	// end  直接根据searchdata中图像坐标和轴位置 设置进对位工具中；
	if (optionInfo.m_bEnableMutiTarget)
	{
		m_bTargetMarkSearchSucceed=m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex);
		if (m_bTargetMarkSearchSucceed)
		{
			SetTargetMarkSearchResults(m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex));
			m_mpTargetMarkImagePos=m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex);
			int nPosNum = m_pPlatformInfo->m_nPositionNum;
			for (int i=0;i<nPosNum;i++)
			{
				SetTargetPlatformPos(m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i),i);
				SetTempTargetPlatformAxisPos(i,m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i));
			}
			PostMessageShowMutiTargetImagePosToDisplay(m_bTargetMarkSearchSucceed);	
		}	
	}

	//CString strCmd = _T("VAAL:");
	//CString strTempWarningType = _T("");
	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd)/*NULL == m_pPlatformInfo || NULL == m_pVisionASMConfig*/)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd)/*FALSE == m_bValidObjectSearchTool*/)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd)/*FALSE == m_bValidAlignerTool*/)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;

	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd)/*FALSE == m_bValidComm*/)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	int i=0;
	for(i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	if (pCurProductData->m_pAlignerParam== NULL)
	{
		ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}
	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerBenchOffsetIndex(nBenchOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerBenchOffsetIndex(nBenchOffsetIndex);
	pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);
	PostMessageUpdateAlignOffsetInfo();

	CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);



	m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
	// 检查目标Mark是否已成功定位 目标也进行拍照，暂时不进行目标拍照检查
	if (0 &&  eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
	{
		if (FALSE == m_bTargetMarkSearchSucceed)
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
			m_strStatusBarInfo2 = _T("");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();		
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			return FALSE;
		}
	}

	dCheckInvalidTime=stCheckInvalidTimer.GetTimeMilli(FALSE);

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo(); 

	// 获取当前产品最大对位次数
	int nMaxAlignTime = curProductData.m_pAlignerParam->m_nAlignerMaxTime;

	scTimer stObjectSearchTimer; double dObjectSearchTime(0.0);
	scTimer stCaluteAlignTimer;  double dCaluteAlignTime(0.0);
	scTimer stAlignTimer;   double dAlignTime(0.0);

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	CCoordPos cpBenchOffset;
	CPlatformXYDAxisPos benchPlatformXYDAxisPos;

	// for (i=0; i<nMaxAlignTime+1; i++)
	i=m_nCurAlignerTimesIndex;
	if((i<-1) || (i>=nMaxAlignTime+1))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("");
		m_strStatusBarInfo2.Format(_T("对位次数%d（最大次数:%d）超限"),i,nMaxAlignTime);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}
	m_nAlignTime = i+1;
	{
		stAlignTimer.Reset();
		// 定义位置序列
		std::vector<int> vnTargetPosIndex; // 目标拍照位置序列
		std::vector<int> vnObjectPosIndex; // 对象拍照位置序列
		int nTempPosIndex = 0;
		// 对象目标相机分离 目标后一半位置拍照 、对象前一半位置
		if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
		{
			for(nTempPosIndex = GetPosNum()/2; nTempPosIndex < GetPosNum(); nTempPosIndex++)
			{
				vnTargetPosIndex.push_back(nTempPosIndex);
			}

			for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
			{
				vnObjectPosIndex.push_back(nTempPosIndex);
			}
		}
		else //对象目标相机不分离 目标全部位置拍照
		{
			for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
			{
				vnTargetPosIndex.push_back(nTempPosIndex);
				vnObjectPosIndex.push_back(nTempPosIndex);
			}		
		}

		//ID读取
		CString strProductID = _T("");
		if ( (pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID)|| m_XYDCommProtocol.GetIsEnableIDRead())
		{
			if (!CommunicateGetAlignProcessProductID(strProductID))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品ID读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
			m_BendInspectResultInfo.m_strProductID = strProductID;
			m_strProductID = strProductID;
		}

		//平台号读取
		CString strProductSTATION = _T("");
		CString strInspectStation = _T("");
		CString strStartIime = _T("");
		CString strEndIime = _T("");
		CString strProsessingIime = _T("");
		if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
		{
			if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品工位读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
			m_strProductSTATION = strProductSTATION;
			m_strInspectStation = strInspectStation;
		}

		VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴位置");
		// 获取轴位置
		{
			CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
			BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
			// 直接读取寄存器地址轴数据
			bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

			if (FALSE == bCommuciateGetPlatformAxisAbsPos)
			{
				if (pPlatformAxisPos!=NULL)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;					
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}
			else
			{
				for (int k=0;k<vnObjectPosIndex.size();k++)
				{
					int nPosIndex = vnObjectPosIndex[k];
					SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
					SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
					SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
				}

				for (int k=0;k<vnTargetPosIndex.size();k++)
				{
					int nPosIndex = vnTargetPosIndex[k];
					SetTargetPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
					SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
					SetTempTargetPlatformAxisPos(nPosIndex,pPlatformAxisPos);
				}

				m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
				m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
				m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
				// 记录当前平台各个轴的绝对位置
				if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
				{
					CString strTemp;
					CString strInfo;
					strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
						pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
					strInfo = strTemp + strInfo;	
					VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
					VisionAlignLogRecord(m_strAlignProcessData, strInfo);
				}


				if (pPlatformAxisPos)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;
				}
			}
		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴位置 结束");
		// 目标对象拍照
		{
			stObjectSearchTimer.Reset();

			// 定义位置序列
			int nTempPosIndex = 0;

			if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
			{
				// 目标拍照
				if(m_nCurAlignerTimesIndex==0)
				{
					//目标光源切换
					if (optionInfo.m_bEnablePhotoLightCtrl)
					{
						if (!OnLightOnOff(1))
						{
							ReportPLCFunc(eCodeNG);
							m_nCurAlignerTimesIndex = 0;
							m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = _T("拍照失败：");
							m_strStatusBarInfo2 = _T("光源切换失败！");
							SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
							PostMessageUpdateStatusBarInfo();
							return FALSE;
						}
					}

					BOOL bSearchTarget = FALSE;
					if (optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜目标搜后一半的第一个位置。
					{
						std::vector<int> vnPosIndex2;
						vnPosIndex2.push_back(GetPosNum()/2);
						bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex2,0);
					}
					else
					{
						bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnTargetPosIndex,0);
					}

					if (FALSE == bSearchTarget)
					{
						ReportPLCFunc(eCodeNG);

						/*m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
						m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		*/

						return FALSE;  
					}
					m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
				}

				//对象拍照
				{
					SetDefaultObjectSearchMarkPos(TRUE);
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;

					if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
					{
						int nObjectVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
						BOOL bEnableObjectPositionVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;

						if (bEnableObjectPositionVirtualMode)
						{
							switch(nObjectVirtualMode)
							{
							case 0:
								{
									// 位置1、位置2
									vnPosIndex.push_back(0);
									vnPosIndex.push_back(1);

								}
								break;

							case 1:
								{
									// 位置3、位置4
									vnPosIndex.push_back(2);
									vnPosIndex.push_back(3);
								}
								break;

							case 2:
								{
									// 位置1、位置3
									vnPosIndex.push_back(0);
									vnPosIndex.push_back(2);
								}
								break;

							case 3:
								{
									// 位置2、位置4
									vnPosIndex.push_back(1);
									vnPosIndex.push_back(3);
								}
								break;

							default:
								{
									return FALSE;
								}
								break;
							}
						}
						else
						{
							vnPosIndex.push_back(0);
							vnPosIndex.push_back(1);
							vnPosIndex.push_back(2);
							vnPosIndex.push_back(3);
						}
					}
					else
					{
						for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
						{
							vnPosIndex.push_back(nTempPosIndex);
						}
					}

					//对象光源切换
					if (optionInfo.m_bEnablePhotoLightCtrl)
					{
						if (!OnLightOnOff(0))
						{
							ReportPLCFunc(eCodeNG);
							m_nCurAlignerTimesIndex = 0;
							m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = _T("拍照失败：");
							m_strStatusBarInfo2 = _T("光源切换失败！");
							SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
							PostMessageUpdateStatusBarInfo();
							return FALSE;
						}
					}

					BOOL bSearchObject = FALSE;
					if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
					{
						std::vector<int> vnPosIndex2;
						vnPosIndex2.push_back(0);
						bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
					}
					else
					{
						bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);
					}

					if (FALSE == bSearchObject)
					{
						ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

						//m_bStatusBar = FALSE;
						//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
						//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
						//if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
						//{
						//	m_strStatusBarInfo1 = m_strStatusBarInfo1 + _T(",") + m_strSnapImgStatuString;
						//}
						//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo1);
						//PostMessageUpdateStatusBarInfo();		
						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
						return FALSE;     			
					}
				}
			}
			else
			{
				//FAAL 目标对象都拍照
				if(m_nCurAlignerTimesIndex==0 && optionInfo.m_bFAALAlignOneTimeTargetSeachSnap)
				{				
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}

					if(m_nCurAlignerTimesIndex==0)
					{
						//目标光源切换
						if (optionInfo.m_bEnablePhotoLightCtrl)
						{
							if (!OnLightOnOff(1))
							{
								ReportPLCFunc(eCodeNG);
								m_nCurAlignerTimesIndex = 0;
								m_bStatusBar = FALSE;
								m_strStatusBarInfo1 = _T("拍照失败：");
								m_strStatusBarInfo2 = _T("光源切换失败！");
								SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
								PostMessageUpdateStatusBarInfo();
								return FALSE;
							}
						}

						BOOL bSearchTarget = FALSE;
						if (optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜目标搜后一半的第一个位置。
						{
							std::vector<int> vnPosIndex2;
							vnPosIndex2.push_back(0);
							bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex2,0);
						}
						else
						{
							bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnTargetPosIndex,0);
						}
						if (FALSE == bSearchTarget)
						{
							ReportPLCFunc(eCodeNG);

							/*m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
							m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
							SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		*/

							return FALSE;  
						}
						m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
					}

					//对象光源切换
					if (optionInfo.m_bEnablePhotoLightCtrl 
						&& (m_nCurAlignerTimesIndex==0 || m_bTagetLightFinish))
					{
						if (!OnLightOnOff(0))
						{
							ReportPLCFunc(eCodeNG);
							m_nCurAlignerTimesIndex = 0;
							m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = _T("拍照失败：");
							m_strStatusBarInfo2 = _T("光源切换失败！");
							SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
							PostMessageUpdateStatusBarInfo();
							return FALSE;
						}	
						m_bTagetLightFinish = FALSE;
					}

					BOOL bSearchObject = FALSE;
					if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
					{
						std::vector<int> vnPosIndex2;
						vnPosIndex2.push_back(0);
						bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
					}
					else
					{
						bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);
					}
					if (FALSE == bSearchObject)
					{
						ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

						//m_bStatusBar = FALSE;
						//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
						//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
						//if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
						//{
						//	m_strStatusBarInfo1 = m_strStatusBarInfo1 + _T(",") + m_strSnapImgStatuString;
						//}
						//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo2,m_strStatusBarInfo1);
						//PostMessageUpdateStatusBarInfo();		
						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
						return FALSE;     			
					}

				}
				else//
				{
					SetDefaultObjectSearchMarkPos(TRUE);
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;

					if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
					{
						int nObjectVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode;
						BOOL bEnableObjectPositionVirtualMode = pCurProductData->m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode;

						if (bEnableObjectPositionVirtualMode)
						{
							switch(nObjectVirtualMode)
							{
							case 0:
								{
									// 位置1、位置2
									vnPosIndex.push_back(0);
									vnPosIndex.push_back(1);

								}
								break;

							case 1:
								{
									// 位置3、位置4
									vnPosIndex.push_back(2);
									vnPosIndex.push_back(3);
								}
								break;

							case 2:
								{
									// 位置1、位置3
									vnPosIndex.push_back(0);
									vnPosIndex.push_back(2);
								}
								break;

							case 3:
								{
									// 位置2、位置4
									vnPosIndex.push_back(1);
									vnPosIndex.push_back(3);
								}
								break;

							default:
								{
									return FALSE;
								}
								break;
							}
						}
						else
						{
							vnPosIndex.push_back(0);
							vnPosIndex.push_back(1);
							vnPosIndex.push_back(2);
							vnPosIndex.push_back(3);
						}
					}
					else
					{
						for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
						{
							vnPosIndex.push_back(nTempPosIndex);
						}
					}

					//对象光源切换
					if (optionInfo.m_bEnablePhotoLightCtrl 
						&& (m_nCurAlignerTimesIndex == 0 || m_bTagetLightFinish))
					{
						if (!OnLightOnOff(0))
						{
							ReportPLCFunc(eCodeNG);
							m_nCurAlignerTimesIndex = 0;
							m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = _T("拍照失败：");
							m_strStatusBarInfo2 = _T("光源切换失败！");
							SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
							PostMessageUpdateStatusBarInfo();
							return FALSE;
						}
						m_bTagetLightFinish = FALSE;
					}

					BOOL bSearchObject = FALSE;
					if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
					{
						std::vector<int> vnPosIndex2;
						vnPosIndex2.push_back(0);
						bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
					}
					else
					{
						bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);

					}

					if (FALSE == bSearchObject)
					{
						ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

						//m_bStatusBar = FALSE;
						//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
						//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
						//if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
						//{
						//	m_strStatusBarInfo1 = m_strStatusBarInfo1 + _T(",") + m_strSnapImgStatuString;
						//}
						//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo1);
						//PostMessageUpdateStatusBarInfo();		
						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
						return FALSE;     			
					}
				}

			dObjectSearchTime = stObjectSearchTimer.GetTimeMilli(FALSE);
		}
		}

		//刷新SN、平台号信息
		{
			vector<CString> vSearchResult;
			vector<BOOL> vbResult;
			CString m_StrTextID;
			vSearchResult.clear();
			if (strProductID != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("SN:%s"),m_strProductID);
					vSearchResult.push_back(m_StrTextID);
				}
			}
			if (strProductSTATION != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,strProductSTATION);
					vSearchResult.push_back(m_StrTextID);
				}
			}
			if (strInspectStation != "")
			{
				for (int i = 0; i < 2; i++)
				{
					vbResult.push_back(1);
					m_StrTextID.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
					vSearchResult.push_back(m_StrTextID);
				}
			}

			if (vSearchResult.size())
			{
				std::vector<int> vnPosIndex; 
				vnPosIndex.push_back(0);
				vnPosIndex.push_back(1);
				m_cVisionAlignGui.SetAllPosText(vbResult,vSearchResult);
				m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
			}
		}

		VisionAlignLogRecord("D:\\TimeLog.txt", "执行对位计算");
		// 执行基准对位
		if(optionInfo.m_bEnableAlignBenchCenter == TRUE)
		{
			if (i == 0)
			{
				// 设置图像中心十字基准目标
				SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
				m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

				// 执行对位
				m_pAlignerTool->SetInnerAlignBench(TRUE);	// 设置为内部图像十字中心基准对位有效
				if(FALSE == ExecuteAlign(i))
				{
					m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效
					// 设置实时的目标
					m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
					m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
					m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();	
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
					return FALSE;     
				}

				// 获取对位结果
				cpBenchOffset = GetAlignPlatformOffset();
				CPlatformXYDAxisPos* pAlignPlatformMovementResults = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
				benchPlatformXYDAxisPos = *pAlignPlatformMovementResults;
				m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效

				// 设置实时的目标
				m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}
		}
		// 执行对位计算
		stCaluteAlignTimer.Reset();
		if(FALSE == ExecuteAlign(i))
		{
			ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
			m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
			return FALSE;     
		}
		dCaluteAlignTime = stCaluteAlignTimer.GetTimeMilli(FALSE); 

		// 执行一次对位
		if (TRUE == optionInfo.m_bAlignOneTime)
		{
			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i+1;
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
			}
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cpProduct.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cpProduct.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();

			}
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;
			PostMessageUpdateAlignerProcessInfo();
			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);
			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
			// 当启用目标补偿，且不发送目标补偿到PLC时，在MMVA指令中增加目标补偿运动量，否则在报告对位成功指令中将目标补偿绝对位置放在备用轴位置寄存器中
			if (pAlignerParam->GetEnableAlnTargetOffset() && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{
				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
			}
			scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
			stCommunicateTimer.Reset();


			m_nCurAlignerTimesIndex=0;
			// 启用中心对位，将中心对位结果写入D20地址
			if(optionInfo.m_bEnableAlignBenchCenter)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
				pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosY();
				pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
				//ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				ReportPLC_OffsetAndBenchCenter(cp, eCodeOK, pPlatformXYDAxisPos,pPlatformXYDAxisPosMove1);
				if (pPlatformXYDAxisPosMove1!=NULL)
				{
					delete pPlatformXYDAxisPosMove1;
					pPlatformXYDAxisPosMove1 = NULL;					
				}
			}
			else
			{
				ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
			}
			
			
			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
					//D角度不累加

				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

				}
				//D角度不累加
				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}
			dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);
			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), m_apAlignerProcesInfo.dTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);

				double m_distance;
				m_distance = sqrt(pow(m_apAlignerProcesInfo.dYMove,2)+pow(m_apAlignerProcesInfo.dXMove,2));
				strOut.Format(",%.4f,%.4f,%.4f",m_apAlignerProcesInfo.dXMove,m_apAlignerProcesInfo.dYMove,m_distance);
				VisionAlignLogRecordCsv(m_strProductdistanceData, strOut);

			}
			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}
			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}
			return TRUE;

		}
		VisionAlignLogRecord("D:\\TimeLog.txt", "对位计算结束");
		// 判断是否执行对位动作
		if (/*i!=0 && */TRUE == IsAlignFinished(i+1))
		{
			dTotalAlignTime = stTotalAlignTimer.GetTimeMilli(FALSE);

			// 更新主界面对位精度、次数、对位总时间
			CCoordPos cp = GetAlignPlatformOffset();
			m_apAlignerProcesInfo.nTimes  = i;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				cpProduct = GetAlignProductOffsetXYPD();
			}
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;

			PostMessageUpdateAlignerProcessInfo();
			m_bStatusBar = IsAlignSucceed();

			

			if (pAlignerParam->GetEnableAlnTargetOffset() && IsAlignSucceed() && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{				
				CPlatformOptionInfo optionInfo;
				GetPlatformOptionInfo(optionInfo);
				CCoordPos cp = GetAlignPlatformOffset();
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
				if(IsAxisMoveOverRun(cp))
				{

					ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
					m_strStatusBarInfo2.Format(_T(""));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;
				}
				// 执行本次对位操作
				// 获取平台对位应移动到的绝对位置
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
				pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();			

				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);

				scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
				stCommunicateTimer.Reset();

				if (!m_bStatusBar)
				{
					ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
					
				}
				else
				{
					ReportPLC_Offset(cp, eCodeNG, pPlatformXYDAxisPos);
				}

				if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
				{
					m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
					m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
					if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
						//D角度不累加

					}
					else
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

					}
					//D角度不累加
					if (m_pSystempOptionConfig->m_bShowSepTargetPos)
					{
						UpdateTargetPosAfterRoate();
					}
				}
				else
				{
					m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
				}	
				dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);

			}

			if (m_bStatusBar)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
				pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);

				// 启用中心对位，将中心对位结果写入D20地址
				if(optionInfo.m_bEnableAlignBenchCenter)
				{
					CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
					pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
					pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosY();
					pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
					//ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
					ReportPLC_OffsetAndBenchCenter(cp, eCodeOK, pPlatformXYDAxisPos,pPlatformXYDAxisPosMove1);
					if (pPlatformXYDAxisPosMove1!=NULL)
					{
						delete pPlatformXYDAxisPosMove1;
						pPlatformXYDAxisPosMove1 = NULL;					
					}
				}
				else
				{
					ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
				}
				m_nCurAlignerTimesIndex = 0;
				
			}

			if (!m_bStatusBar) 
			{
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
			}

			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), i, stTotalAlignTimer.GetTimeMilli(FALSE));
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CString strOut;					
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;	
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CString strOut;						
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), i, dTotalAlignTime);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			return m_bStatusBar;  
		}
		else
		{
			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i+1;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;

			PostMessageUpdateAlignerProcessInfo();

			CPlatformOptionInfo optionInfo;
			GetPlatformOptionInfo(optionInfo);

			if(IsAxisMoveOverRun(cp))
			{

				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}


			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();

			scTimer stCommunicateTimer;   double dCommunicateTime(0.0);
			stCommunicateTimer.Reset();

			// 打包回复偏差量到D10-14
			ReportPLC_Offset(cp, eCodeContinued, pPlatformXYDAxisPos);
			m_nCurAlignerTimesIndex = i+1;


			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;
				}
				//D角度不累加
				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}
			dCommunicateTime =stCommunicateTimer.GetTimeMilli(FALSE);		

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), m_apAlignerProcesInfo.dTime);
			m_strStatusBarInfo2+= _T("精度未达到，继续对位");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

		} // else
	}// for	
	return TRUE;
}

//折弯专用FAAL
BOOL vcXYDVisionAlign::OnlineAlignExFAAL_BendAlign_Ex(int nOffsetIndex, int nReservedIndex/* = 10*/, int nBenchOffsetIndex/* = -1*/, int nExProductIndex/* = 0*/,int nTarFixOffsetIndex /*= 0*/)
{
	scTimer stTotalAlignTimer;double dTotalAlignTime(0.0);
	stTotalAlignTimer.Reset();
	std::vector<BOOL> m_bResult;
	std::vector<CString> m_vSearchResult;

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

	CString strCmd = _T("VAAL:");
	CString strTempWarningType = _T("");
	CString strlog;
	strlog.Format("start m_nCurAlignerTimesIndex=%d",m_nCurAlignerTimesIndex);
	VisionAlignLogRecord("D:\\TimeLog.txt", strlog);

	// 避免没有进入产品设置，导致对位工具类为空时的崩溃
	if (m_pAlignerTool == NULL) 
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
	    m_bALLBendRecord = _T("");
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();	
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	GetCommCommandName(strCmd);
	if (!CheckPlatformInfo(strCmd) || !CheckVisionASMConfig(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bALLBendRecord = _T("");
		return FALSE;
	}	

	// 检查对象定位工具是否有效
	if (!CheckObjectSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查对位工具是否有效
	if (!CheckAlignerSearchTool(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查通信工具是否有效
	if (!CheckCommuciationValid(strCmd))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		return FALSE;
	}

	// 检查所有相机是否均处于连续采集状态	
	int i=0;
	for(i=0; i<m_pPlatformInfo->m_nCamNum; i++)
	{		 
		if (FALSE == m_vbCameraLiveGrab[i])
		{
			ReportPLCFunc(eCodeNG);
			m_nCurAlignerTimesIndex = 0;
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings, IDS_SB_ALIGN_FAILED);
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			return FALSE;
		}            
	}    

	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	// 设置补偿量索引
	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	if (pCurProductData->m_pAlignerParam== NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}


	SetAlignReservedIndex(nReservedIndex);
	SetAlignBenchOffsetIndex(nBenchOffsetIndex);
	SetAlignExProductIndex(nExProductIndex);

	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);

	// start 直接根据searchdata中图像坐标和轴位置 设置进对位工具中；
	{
		int nSize1 = m_vvpTargetPlatformAxisPosMultiEx.size();
		int nSize2 = m_vvpObjectPlatformAxisPosMultiEx.size();
		int nSize3 = m_vmpObjectMarkImagePos_MultiEX.size();
		int nSize4 = m_vmpTargetMarkImagePosMultiEx.size();
		bool bTmp = (nExProductIndex >=0) &&  (nExProductIndex <nSize1) &&  (nExProductIndex <nSize2) &&  (nExProductIndex <nSize3)  &&  (nExProductIndex <nSize4);
		if (bTmp)
		{
			vector<CPlatformAxisPos*> tmpObjectPlatformAxisPos;
			vector<CPlatformAxisPos*> tmpTargetPlatformAxisPos;
			tmpObjectPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);
			tmpTargetPlatformAxisPos.resize(m_pPlatformInfo->m_nPositionNum);

			if (m_pPlatformInfo->m_bEnableMultiCalibExtension)
			{
				if (optionInfo.m_bObjectSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{
						tmpObjectPlatformAxisPos.at(jt) = m_vvpObjectPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}
					m_mpObjectMarkImagePos = m_vmpObjectMarkImagePos_MultiEX.at(nExProductIndex);
				}
				else
				{
					tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				}

				if (optionInfo.m_bTargetSearchExProduct)
				{
					for (int jt = 0 ;jt<m_pPlatformInfo->m_nPositionNum;jt++)
					{					
						tmpTargetPlatformAxisPos.at(jt) = m_vvpTargetPlatformAxisPosMultiEx.at(nExProductIndex).at(jt);
					}		
					m_mpTargetMarkImagePos = m_vmpTargetMarkImagePosMultiEx.at(nExProductIndex);
				}
				else
				{
					tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
				}

			}
			else
			{
				tmpObjectPlatformAxisPos = m_vpObjectPlatformAxisPos;
				tmpTargetPlatformAxisPos = m_vpTargetPlatformAxisPos;
			}
			m_pAlignerTool->SetTargetPlatformPos(tmpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			m_pAlignerTool->SetObjectPlatformPos(tmpObjectPlatformAxisPos);
			m_pAlignerTool->SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
			m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
			m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();

		}
	}
	// end  直接根据searchdata中图像坐标和轴位置 设置进对位工具中；

	if (optionInfo.m_bEnableMutiTarget)
	{
		m_bTargetMarkSearchSucceed=m_vbMutiTargetMarkOK.at(m_nMutiTargetCurIndex);
		if (m_bTargetMarkSearchSucceed)
		{
			SetTargetMarkSearchResults(m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex));
			m_mpTargetMarkImagePos=m_vMutiTargetMarkImagePos.at(m_nMutiTargetCurIndex);
			int nPosNum = m_pPlatformInfo->m_nPositionNum;
			for (int i=0;i<nPosNum;i++)
			{
				SetTargetPlatformPos(m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i),i);
				SetTempTargetPlatformAxisPos(i,m_vpMutiTargetPlatformAxisPos.at(m_nMutiTargetCurIndex).at(i));
			}
			PostMessageShowMutiTargetImagePosToDisplay(m_bTargetMarkSearchSucceed);	
		}	
	}


	EnterCriticalSection(&m_csAlignerOffset);
	pCurProductData->m_pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerBenchOffsetIndex(nBenchOffsetIndex);
	pCurProductData->m_pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);

	CAlignerParam* pAlignerParam = m_pAlignerTool->GetProductAlignerParam();
	pAlignerParam->SetCurAlignerOffsetIndex(nOffsetIndex);
	pAlignerParam->SetCurAlignerBenchOffsetIndex(nBenchOffsetIndex);
	pAlignerParam->SetCurAlignerTarFixOffsetIndex(nTarFixOffsetIndex);
	LeaveCriticalSection(&m_csAlignerOffset);
	PostMessageUpdateAlignOffsetInfo();

	

	m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
	//// 检查目标Mark是否已成功定位
	//if (eTargetOnline == pCurProductData->m_pAlignerParam->m_eTargetMarkType)
	//{
	//	if (FALSE == m_bTargetMarkSearchSucceed)
	//	{
	//		ReportPLCFunc(eCodeNG);
	//		m_nAlignTime = 0;
	//		m_bStatusBar = FALSE;
	//		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
	//		m_strStatusBarInfo2 = _T("");
	//		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	//		PostMessageUpdateStatusBarInfo();		
	//		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
	//		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
	//		return FALSE;
	//	}
	//}

	if (!pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendAlignInspect)
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
		m_strStatusBarInfo2 = _T("折弯对位并检测功能未开启");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();		
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	SysPlatformInfo sysPlatformInfo;
	GetSysPlatformInfo(sysPlatformInfo);

	CBendInfo BendInfo = pCurProductData->m_pAlignerParam->GetBendInfo();
	m_pAlignerTool->SetBendAlignProcess(TRUE);

	// 获取当前产品最大对位次数
	int nMaxAlignTime = pCurProductData->m_pAlignerParam->m_nAlignerMaxTime;

	scTimer stObjectSearchTimer; double dObjectSearchTime(0.0);
	scTimer stCaluteAlignTimer;  double dCaluteAlignTime(0.0);
	scTimer stAlignTimer;   double dAlignTime(0.0);

	CCoordPos cpBenchOffset;
	CPlatformXYDAxisPos benchPlatformXYDAxisPos;

	// for (i=0; i<nMaxAlignTime+1; i++)
	i = m_nCurAlignerTimesIndex;
	if((i<-1) || (i>=nMaxAlignTime))
	{
		ReportPLCFunc(eCodeNG);
		m_nCurAlignerTimesIndex = 0;
		m_bALLBendRecord = _T("");
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("");
		m_strStatusBarInfo2.Format(_T("对位次数%d（最大次数:%d）超限"),i,nMaxAlignTime);
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
		return FALSE;
	}

	// 当i=0时，将m_apAlignerProcesInfo复位
	if(i==0)
	{
		m_apAlignerProcesInfo.nTimes  = 0;
		m_apAlignerProcesInfo.dXMove = 0.0;
		m_apAlignerProcesInfo.dYMove  = 0.0;
		m_apAlignerProcesInfo.dDMove  = 0.0;

		m_apAlignerProcesInfo.dXOffset = 0.0;
		m_apAlignerProcesInfo.dYOffset = 0.0;
		m_apAlignerProcesInfo.dDOffset = 0.0;

		//复位数据
		m_dLastLy = 0.0;
		m_dLastRy = 0.0;
		m_bisFirstAlign = TRUE;
	}

	m_nAlignTime = i+1;

	{
		stAlignTimer.Reset();
		stObjectSearchTimer.Reset();
		//m_nCurAlignerTimesIndex = i+1;/
		// 定义位置序列
		std::vector<int> vnTargetPosIndex; // 目标拍照位置序列
		std::vector<int> vnObjectPosIndex; // 对象拍照位置序列
		int nTempPosIndex = 0;
		// 对象目标相机分离 目标后一半位置拍照 、对象前一半位置
		if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
		{
			for(nTempPosIndex = GetPosNum()/2; nTempPosIndex < GetPosNum(); nTempPosIndex++)
			{
				vnTargetPosIndex.push_back(nTempPosIndex);
			}

			for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
			{
				vnObjectPosIndex.push_back(nTempPosIndex);
			}
		}
		else //对象目标相机不分离 目标全部位置拍照
		{
			for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
			{
				vnTargetPosIndex.push_back(nTempPosIndex);
				vnObjectPosIndex.push_back(nTempPosIndex);
			}		
		}
		// 获取当前产品信息
		int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		CProductData curProductData = m_pVisionASMConfig->m_TotalProductData.GetItem(nCurProductIndex);

		//读码
		CString strProductID = _T("");
		//多次对位只有第0次才获取SN码
		if((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendInspectGetSnOnce&&m_nCurAlignerTimesIndex==0)
			||(!pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendInspectGetSnOnce)
			)
		if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
		{
			if (!CommunicateGetAlignProcessProductID(strProductID))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品ID读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
			m_BendInspectResultInfo.m_strProductID = strProductID;
			m_strProductID = strProductID;
		}

		//平台号读取
		CString strProductSTATION = _T("");
		CString strInspectStation = _T("");
		CString strStartIime = _T("");
		CString strEndIime = _T("");
		CString strProsessingIime = _T("");
		if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
			||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo))
		{
			if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
			{
				ReportPLCFunc(eCodeNG);m_nCurAlignerTimesIndex = 0;
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("产品工位读取失败");
				m_strStatusBarInfo2 = _T("检测失败");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
				return FALSE;
			}
		}
		m_strProductSTATION = strProductSTATION;
		m_strInspectStation = strInspectStation;
		m_BendInspectResultInfo.m_strProductSTATION = strProductSTATION;
		m_BendInspectResultInfo.m_strInspectStation = strInspectStation;

		// 获取轴位置
		{
			VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴位置");
			CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
			BOOL bCommuciateGetPlatformAxisAbsPos = FALSE; 
			// 直接读取寄存器地址轴数据
			bCommuciateGetPlatformAxisAbsPos = CommuciateGetPlatformAxisAbsPosEx(pPlatformAxisPos);

			if (FALSE == bCommuciateGetPlatformAxisAbsPos)
			{
				if (pPlatformAxisPos!=NULL)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;					
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
				m_bALLBendRecord = _T("");
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_SYS_CALIB_FAILED);
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PLATFORM_POS_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
				return FALSE;
			}
			else
			{
				for (int k=0;k<vnObjectPosIndex.size();k++)
				{
					int nPosIndex = vnObjectPosIndex[k];
					SetObjectPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
					SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
					SetTempObjectPlatformAxisPos(nPosIndex,pPlatformAxisPos);
				}

				for (int k=0;k<vnTargetPosIndex.size();k++)
				{
					int nPosIndex = vnTargetPosIndex[k];
					SetTargetPlatformPos_MultiEx(pPlatformAxisPos,nPosIndex);
					SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
					SetTempTargetPlatformAxisPos(nPosIndex,pPlatformAxisPos);
				}

				m_AlignerAxisInfo.m_vdAxis.at(0) = pPlatformAxisPos->m_dPosX;
				m_AlignerAxisInfo.m_vdAxis.at(1) = pPlatformAxisPos->m_dPosY;
				m_AlignerAxisInfo.m_vdAxis.at(2) = pPlatformAxisPos->m_dAngle;
				// 记录当前平台各个轴的绝对位置
				if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
				{
					CString strTemp;
					CString strInfo;
					strInfo.Format(	GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_OBJECT_SEARCH_PLATFORM_POS), 
						pPlatformAxisPos->m_dPosX, pPlatformAxisPos->m_dPosY, pPlatformAxisPos->m_dAngle);
					strInfo = strTemp + strInfo;	
					VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
					VisionAlignLogRecord(m_strAlignProcessData, strInfo);
				}


				if (pPlatformAxisPos)
				{
					delete pPlatformAxisPos;
					pPlatformAxisPos = NULL;
				}
			}
			VisionAlignLogRecord("D:\\TimeLog.txt", "获取轴结束");
		}
		 
		// 目标对象拍照
		{
			int nStartPatIndex = 0;

			// 目标拍照
			if((optionInfo.m_bFAALAlignOneTimeTargetSeachSnap && m_nCurAlignerTimesIndex==0)
				|| optionInfo.m_bTargetSeachSnapEverytime)
			{
				//目标光源切换
				m_bTagetLightFinish = FALSE;
				if (optionInfo.m_bEnablePhotoLightCtrl)
				{
					if (!OnLightOnOff(1))
					{
						ReportPLCFunc(eCodeNG);
						m_nCurAlignerTimesIndex = 0;
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = _T("拍照失败：");
						m_strStatusBarInfo2 = _T("光源切换失败！");
						SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();
						return FALSE;
					}	
					m_bTagetLightFinish = TRUE;
				}

				BOOL bSearchTarget = FALSE;
				if (optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜目标搜后一半的第一个位置。
				{
					std::vector<int> vnPosIndex2;
					vnPosIndex2.push_back(GetPosNum()/2);
					bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex2,nStartPatIndex);
				}
				else
				{
					bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnTargetPosIndex,nStartPatIndex);
				}
				//if (FALSE == OnTargetSearchCandidateExFTGC_Ex(vnTargetPosIndex,nStartPatIndex))
				if (FALSE == bSearchTarget)
				{
					ReportPLCFunc(eCodeNG);

					/*m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_SEARCH_FAILED);
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	*/	

					return FALSE;  
				}
				m_bTargetMarkSearchSucceed = m_mpTargetMarkImagePos.IsAllMarkImagePosOK();
			}
			

			// 对象拍照光源切换
			if (optionInfo.m_bEnablePhotoLightCtrl && 
				(m_nCurAlignerTimesIndex == 0 || m_bTagetLightFinish)) //仅第一次切换或者目标进行切换时切换
			{
				if (!OnLightOnOff(0))
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("拍照失败：");
					m_strStatusBarInfo2 = _T("光源切换失败！");
					SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					m_nCurAlignerTimesIndex = 0;
					ReportPLCFunc(eCodeNG);
					return FALSE;
				}	
				m_bTagetLightFinish = FALSE;
			}

			// 对象拍照
			if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
			{
				SetDefaultObjectSearchMarkPos(TRUE);
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum()/2; nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}
				BOOL bSearchObject = FALSE;
				if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
				{
					std::vector<int> vnPosIndex2;
					vnPosIndex2.push_back(0);
					bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
				}
				else
				{
					bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);
				}

				//if (FALSE == OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0))
				if (FALSE == bSearchObject)
				{
					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					//m_bStatusBar = FALSE;
					//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
					//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					//if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
					//{
					//	m_strStatusBarInfo1 = m_strStatusBarInfo1 + _T(",") + m_strSnapImgStatuString;
					//}
					//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo1);
					//PostMessageUpdateStatusBarInfo();		
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;     			
				}
			}
			else
			{
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}

				BOOL bSearchObject = FALSE;
				if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
				{
					std::vector<int> vnPosIndex2;
					vnPosIndex2.push_back(0);
					bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
				}
				else
				{
					bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);	 
				}

				//if (FALSE == OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0))
				if (FALSE == bSearchObject)
				{
					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					//m_bStatusBar = FALSE;
					//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_SEARCH_FAILED);
					//m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					//if (!m_boIsSnapImgSucceed)	//  [5/8/2020 zzc]
					//{
					//	m_strStatusBarInfo1 = m_strStatusBarInfo1 + _T(",") + m_strSnapImgStatuString;
					//}
					//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo2,m_strStatusBarInfo1);
					//PostMessageUpdateStatusBarInfo();		
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;     			
				}
				m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "检查目标对象Mark是否已成功定位");
			m_bObjectMarkSearchSucceed = m_mpObjectMarkImagePos.IsAllMarkImagePosOK();
			{
				m_pAlignerTool->SetIsReadTargetPlatformPos(FALSE);
				// 检查目标Mark是否已成功定位
				if (eTargetOnline == curProductData.m_pAlignerParam->m_eTargetMarkType)
				{
					if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
					{
						if (FALSE == m_bTargetMarkSearchSucceed)
						{
							ReportPLCFunc(eCodeNG);
							m_nCurAlignerTimesIndex = 0;
							m_bStatusBar = FALSE;
							m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_KEEP_TARGET_SEARCH_SUCCEED_FIRST);
							m_strStatusBarInfo2 = _T("");
							SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
							PostMessageUpdateStatusBarInfo();		
							strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
							AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
							return FALSE;
						}
					}
				}
			}
			// 检查对象Mark是否已成功定位
			if (eObjectBench != curProductData.m_pAlignerParam->m_eObjectMarkType)
			{
				if (!m_bIsExecuteFixTargetCheck && !m_bIsExecuteSizeCheck)  //执行标定关联验证过程时
				{
					if (FALSE == m_bObjectMarkSearchSucceed)
					{
						ReportPLCFunc(eCodeNG);
						m_nCurAlignerTimesIndex = 0;
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CONFIRM_OBJECT_SEARCH_SUCCESS);//_T("请先保证实时对象Mark定位成功" );
						m_strStatusBarInfo2 = _T("");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();	
						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
						return FALSE;
					}	
				}

				if (!m_bIsExecuteFixTargetCheck) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
				{
					//			CalculateObjectDistance(); 
					if (!CheckObjectDistance())
					{
						ReportPLCFunc(eCodeNG);
						m_nCurAlignerTimesIndex = 0;
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_DIS_OUT_RANGE);
						m_strStatusBarInfo2 = _T("");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();
						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
						return FALSE;
					}


					if (!CheckObjectAngle())
					{
						ReportPLCFunc(eCodeNG);
						m_nCurAlignerTimesIndex = 0;
						m_bStatusBar = FALSE;
						//				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OBJECT_ANGLE_OUT_RANGE);
						m_strStatusBarInfo1 = _T("对象角度超过设定标准，对象定位结果可能出错");
						m_strStatusBarInfo2 = _T("");
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();

						strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
						AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

						return FALSE;
					}


				}
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "尺寸检查");
			if (!m_bIsExecuteFixTargetCheck && m_nCurAlignerTimesIndex==0) // 固定目标对位 执行标定检查时  对位时不进行对象目标尺寸检测 
			{
				//		CalculateTargetDistance(); 
				if (!CheckTargetDistance())
				{
					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_DIS_OUT_RANGE);
					m_strStatusBarInfo2 = _T("");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;
				}

				if (!CheckTargetAngle())
				{
					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TARGET_ANGLE_OUT_RANGE);
					m_strStatusBarInfo2 = _T("");
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;
				}
			}
		}


		dObjectSearchTime = stObjectSearchTimer.GetTimeMilli(FALSE);

		// 执行基准对位
		VisionAlignLogRecord("D:\\TimeLog.txt", "执行基准对位");
		if( (TRUE == optionInfo.m_bEnableAlignBenchCenter) && (0 == i))
		{
			// 设置图像中心十字基准目标
			SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
			m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

			// 执行对位
			m_pAlignerTool->SetInnerAlignBench(TRUE);	// 设置为内部图像十字中心基准对位有效
			if(FALSE == ExecuteAlign(i))
			{
				m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效

				// 设置实时的目标
				m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
				m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
				m_nCurAlignerTimesIndex = 0;
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
				m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
				return FALSE;     
			}

			// 获取对位结果
			cpBenchOffset = GetAlignPlatformOffset();
			CPlatformXYDAxisPos* pAlignPlatformMovementResults = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
			benchPlatformXYDAxisPos = *pAlignPlatformMovementResults;
			m_pAlignerTool->SetInnerAlignBench(FALSE);	// 设置为内部图像十字中心基准对位无效

			// 设置实时的目标
			m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
		}

		// 折弯上料判定
		VisionAlignLogRecord("D:\\TimeLog.txt", "折弯上料判定");
		if((BendInfo.m_bBendLoadThre) && (0 == i))
		{
			// 设置图像中心十字基准目标
			SetDefaultCrossBenchTargetSearchMarkPos(TRUE);
			m_pAlignerTool->SetTargetPlatformPos(m_vpCrossBenchPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpCrossBenchMarkImagePos);				

			AlignerProcessInfo TempAlignerProcesInfo;
			m_apAlignerProcesInfo.nTimes  = TempAlignerProcesInfo.nTimes;
			m_apAlignerProcesInfo.dTime   = TempAlignerProcesInfo.dTime;
			m_apAlignerProcesInfo.dXMove  = TempAlignerProcesInfo.dXMove;  
			m_apAlignerProcesInfo.dYMove  = TempAlignerProcesInfo.dYMove;
			m_apAlignerProcesInfo.dDMove  = TempAlignerProcesInfo.dDMove;

			m_apAlignerProcesInfo.dXOffset = TempAlignerProcesInfo.dXOffset;
			m_apAlignerProcesInfo.dYOffset = TempAlignerProcesInfo.dYOffset;
			m_apAlignerProcesInfo.dDOffset = TempAlignerProcesInfo.dDOffset;

			CString strBendInsError = _T("");
			//BendingAlignProcessInfo(strBendInsError);
			BOOL bBendInspect  = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
			m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
			m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

			//if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
			//{
			//	std::vector<int> vnPosIndex;
			//	int nTempPosIndex = 0;
			//	for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
			//	{
			//		vnPosIndex.push_back(nTempPosIndex);
			//	}

			//	ExeSaveVDBFile(vnPosIndex,eObjectSearch);
			//}

			if ( !(m_BendInspectResultInfo.m_dLCenterx < fabs(BendInfo.m_scBendLoadThre.GetX())) )
			{
				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,左侧X上料超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

			if ( !(m_BendInspectResultInfo.m_dLCentery < fabs(BendInfo.m_scBendLoadThre.GetY())) )
			{
				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,左侧Y上料超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

			if ( !(m_BendInspectResultInfo.m_dRCenterx < fabs(BendInfo.m_scBendLoadThre.GetX())) )
			{
				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,右侧X上料超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

			if ( !(m_BendInspectResultInfo.m_dRCentery < fabs(BendInfo.m_scBendLoadThre.GetY())) )
			{
				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,右侧Y上料超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				return FALSE;
			}

			// 设置实时的目标
			m_pAlignerTool->SetTargetPlatformPos(m_vpTargetPlatformAxisPos);
			m_pAlignerTool->SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

			BendInspectResultInfo TempBendInspectResultInfo;
			TempBendInspectResultInfo.m_dLCenterx = m_BendInspectResultInfo.m_dLCenterx;
			TempBendInspectResultInfo.m_dLCentery = m_BendInspectResultInfo.m_dLCentery;
			TempBendInspectResultInfo.m_dRCenterx = m_BendInspectResultInfo.m_dLCenterx;
			TempBendInspectResultInfo.m_dRCentery = m_BendInspectResultInfo.m_dLCentery;

			m_BendInspectResultInfo = TempBendInspectResultInfo;
			PostMessageUpdateBendingDetalInfo();
			//20230704
			//VisionInspectBendRecord(TRUE, TRUE, FALSE);	 //csv保存数据

		}


		CString strBendInsError = _T("");
		BOOL bBendInspect = FALSE;

		// 说明：执行对位计算和折弯检测需成对出现，目的是将两者的结果记录在一个csv文件中
		VisionAlignLogRecord("D:\\TimeLog.txt", "执行对位计算和折弯检测需成对出现，目的是将两者的结果记录在一个csv文件中");
		if ( (BendInfo.m_bBendPressPara) && (0 == i) )
		{
			// 执行对位计算
			stCaluteAlignTimer.Reset();
			if(FALSE == ExecuteAlign(i))
			{
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
				m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));

				return FALSE;     
			}
			dCaluteAlignTime = stCaluteAlignTimer.GetTimeMilli(FALSE); 

			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;
			PostMessageUpdateAlignerProcessInfo();


			//bBendInspect = BendingAlignProcessInfo(strBendInsError);
			//注释掉防止重复存储数据和图片,压合成功存储数据图片
			bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
			m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
			m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

			//VisionInspectBendRecord(TRUE, TRUE, FALSE);

			//int nStatus = -1;
			//if (IsAlignSucceed())	//对位完成
			//{
			//	nStatus = 1;		//OK
			//}
			//else
			//{
			//	if ( m_nAlignTime < m_pAlignerTool->GetProductAlignerParam()->GetAlignerMaxTime())
			//	{
			//		nStatus = 2;	//Retry
			//	}
			//	else
			//	{
			//		nStatus = 0;	//NG
			//	}
			//}
			
			
			

			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

				VisionInspectBendRecord(TRUE,TRUE,FALSE,0);
				PostMessageUpdateBendingProcessInfo();

				return FALSE;
			}


			if( (m_BendInspectResultInfo.m_dLy < BendInfo.m_dBendPressNGYmin) || (m_BendInspectResultInfo.m_dLy > BendInfo.m_dBendPressNGYmax) )
			{

				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}
				/////////////////////////////////////

				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,左侧Y向距离超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				VisionInspectBendRecord(TRUE,TRUE,FALSE,0);
				PostMessageUpdateBendingProcessInfo();

				return FALSE;
			}


			if( (m_BendInspectResultInfo.m_dRy < BendInfo.m_dBendPressNGYmin) || (m_BendInspectResultInfo.m_dRy > BendInfo.m_dBendPressNGYmax) )
			{

				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}
				/////////////////////////////////////



				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("第一次对位时,右侧Y向距离超出范围！");
				m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

				VisionInspectBendRecord(TRUE,TRUE,FALSE,0);
				PostMessageUpdateBendingProcessInfo();

				return FALSE;
			}

			if((m_BendInspectResultInfo.m_dLy > BendInfo.m_dBendPressOKYmin) && (m_BendInspectResultInfo.m_dLy < BendInfo.m_dBendPressOKYmax) 
				&& (m_BendInspectResultInfo.m_dRy > BendInfo.m_dBendPressOKYmin) && (m_BendInspectResultInfo.m_dRy < BendInfo.m_dBendPressOKYmax) && bBendInspect)
			{

				
				//存储VDB图片
				if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
				{
					std::vector<int> vnPosIndex;
					int nTempPosIndex = 0;
					for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
					{
						vnPosIndex.push_back(nTempPosIndex);
					}
					ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
				}
				/////////////////////////////////////

				// 压合阈值范围内对位成功；
				ReportPLCFunc(eCodeOK);
				m_nCurAlignerTimesIndex = 0;

				m_bStatusBar = TRUE;
				m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
				m_strStatusBarInfo2 = _T("压合阈值范围内对位成功！");
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

				VisionInspectBendRecord(TRUE,TRUE,FALSE,1);
				PostMessageUpdateBendingProcessInfo();

				return TRUE;
			}

			VisionInspectBendRecord(TRUE, TRUE, FALSE , 2);
			PostMessageUpdateBendingProcessInfo();

		}

		// 执行一次对位
		VisionAlignLogRecord("D:\\TimeLog.txt", "执行一次对位");
		if ( (TRUE == optionInfo.m_bAlignOneTime) && (0 == i) )
		{
			// 执行对位计算
			stCaluteAlignTimer.Reset();
			if(FALSE == ExecuteAlign(i))
			{
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
				m_bALLBendRecord = _T("");
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
				m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();	
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));
				return FALSE;     
			}
			dCaluteAlignTime = stCaluteAlignTimer.GetTimeMilli(FALSE); 

			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i+1;
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
			}
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			if (pAlignerParam->GetEnableAlnTargetOffset())
			{
				cpProduct.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cpProduct.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();
			}

			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;
			PostMessageUpdateAlignerProcessInfo();

			//bBendInspect = BendingAlignProcessInfo(strBendInsError);
			bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
			m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
			m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);
		//	VisionInspectBendRecord(TRUE, TRUE, FALSE);

			VisionInspectBendRecord(TRUE,TRUE,FALSE,(!IsAxisMoveOverRun(cp)));
			PostMessageUpdateBendingProcessInfo();
			
			//存储VDB图片
			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
			{
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}
				ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
			}

			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
			    m_bALLBendRecord = _T("");
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
				return FALSE;
			}

			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();
			// 当启用目标补偿，且不发送目标补偿到PLC时，在MMVA指令中增加目标补偿运动量，否则在报告对位成功指令中将目标补偿绝对位置放在备用轴位置寄存器中
			if (pAlignerParam->GetEnableAlnTargetOffset() && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{
				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);
			}

			// 启用中心对位，将中心对位结果写入D20地址
			if(optionInfo.m_bEnableAlignBenchCenter)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
				pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosY();
				pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
				//ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				ReportPLC_OffsetAndBenchCenter(cp, eCodeOK, pPlatformXYDAxisPos,pPlatformXYDAxisPosMove1);
				if (pPlatformXYDAxisPosMove1!=NULL)
				{
					delete pPlatformXYDAxisPosMove1;
					pPlatformXYDAxisPosMove1 = NULL;					
				}
			}
			else
			{
				// 打包偏差量结果并给出对位OK信号
				ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
			}
			
			// 通信驱动平台运动到指定位置

			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

				}

				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), m_apAlignerProcesInfo.dTime);
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

			return TRUE;

		}

		// 执行对位计算
		VisionAlignLogRecord("D:\\TimeLog.txt", "执行对位计算");
		stCaluteAlignTimer.Reset();
		if(FALSE == ExecuteAlign(i))
		{
			ReportPLCFunc(eCodeNG);
			m_nCurAlignerTimesIndex = 0;
			m_bALLBendRecord = _T("");
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_EXCUTE_ALIGN_CALCULATE_FAILED);
			//			m_strStatusBarInfo2 = _T("");
			m_strStatusBarInfo2 = m_pAlignerTool->GetAlignErrInfo();
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();	

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1 + m_strStatusBarInfo2));

			return FALSE;     
		}
		dCaluteAlignTime = stCaluteAlignTimer.GetTimeMilli(FALSE); 

		VisionAlignLogRecord("D:\\TimeLog.txt", "对位结束判定");
		BOOL bIsAlignFinished = FALSE;
		//leep(20);
		if(optionInfo.m_nBendFIsAlignFinished==0)
		{
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定开始------");
			// bIsAlignFinished = (i!=0) && (TRUE == IsAlignFinished(i+1));
			bIsAlignFinished = (TRUE == IsAlignFinished(i+1));

			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定结束------");
		}
		else if(optionInfo.m_nBendFIsAlignFinished==1) //同时OK
		{
			VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定开始------");
			BOOL bInspectCheck = FALSE;
			if ((BendInfo.m_bBendPressPara) && (0 == i))
			{
				bInspectCheck = bBendInspect;
			} 
			else
			{
				m_apAlignerProcesInfo.nTimes  = i;
				//bInspectCheck = BendingAlignProcessInfo(strBendInsError);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定BendingAlignProcessInfo------");
				bInspectCheck = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定SetAllPosText------");
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定SetAllPosSearchResult------");
				m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

				int nStatus = -1;
				if (!IsAlignFinished(i+1) || !bInspectCheck)
				{
					if (m_nAlignTime < m_pAlignerTool->GetProductAlignerParam()->GetAlignerMaxTime())
					{
						nStatus = 2;
					}
					else
						nStatus = 0;
				}
				else
					nStatus = 1;
				
				VisionInspectBendRecord(TRUE, TRUE, FALSE , nStatus);

				VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定PostMessageUpdateBendingProcessInfo------");
				PostMessageUpdateBendingProcessInfo();
				//Sleep(10);
			}
			
			bBendInspect = bInspectCheck;
			// bIsAlignFinished = (i!=0) && (TRUE == IsAlignFinished(i+1)) && bInspectCheck;
			bIsAlignFinished = (TRUE == IsAlignFinished(i+1)) && bInspectCheck;
			VisionAlignLogRecord("D:\\TimeLog.txt", "------进行对位&检测判定结束------");
		}
		else if(optionInfo.m_nBendFIsAlignFinished==2) // 只进行折弯检测达到
		{
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定开始------");
			BOOL bInspectCheck = FALSE;
			// 需要执行一次，但是不进行判断
			IsAlignFinished(i+1);
			m_apAlignerProcesInfo.nTimes  = i;
			//bInspectCheck = BendingAlignProcessInfo(strBendInsError);
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定BendingAlignProcessInfo------");
			bInspectCheck = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定SetAllPosText------");
			m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定SetAllPosSearchResult------");
			m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

			if ((BendInfo.m_bBendPressPara) && (0 == i) )
			{

			}
			else
			{
				int nStatus = -1;
				if(!bInspectCheck)
				{
					if ( m_nAlignTime < m_pAlignerTool->GetProductAlignerParam()->GetAlignerMaxTime())
					{
						nStatus = 2;	//Retry
					}
					else
					{
						nStatus = 0;
					}
				}
				else
					nStatus = 1;

				VisionInspectBendRecord(TRUE, TRUE, FALSE , nStatus);
			//	VisionInspectBendRecord(TRUE, TRUE, FALSE , TRUE);
			}

			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定PostMessageUpdateBendingProcessInfo------");
			PostMessageUpdateBendingProcessInfo();
			//Sleep(10);
			
			bBendInspect = bInspectCheck;
			// bIsAlignFinished = (i!=0) && bInspectCheck;
			bIsAlignFinished = bInspectCheck;
			VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行检测判定结束------");
		}
		////存储CSV固定路径
		//if (m_pSystempOptionConfig->m_bAllSteoage)
		//{
		//	SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,bSearchSucceed, vdDistance, TRUE);
		//}
		//如果是零次对位或者最后一次对位写入日志文件中
		/*if ((0 == i) && m_pSystempOptionConfig->m_bAllSteoage)
		{  
		   VisionInspectBendRecordFixed(m_bALLBendRecord, FALSE,TRUE, TRUE, FALSE);
		}*/

		

		// 判断是否执行对位动作
		VisionAlignLogRecord("D:\\TimeLog.txt", "判断是否执行对位动作");
		//if (i!=0 && TRUE == IsAlignFinished(i+1))
		if (bIsAlignFinished)
		{

			/*if (m_pSystempOptionConfig->m_bAllSteoage)
			{  
			VisionInspectBendRecordFixed(m_bALLBendRecord, TRUE,TRUE, TRUE, FALSE);
			}*/

			dTotalAlignTime = stTotalAlignTimer.GetTimeMilli(FALSE);

			// 更新主界面对位精度、次数、对位总时间
			CCoordPos cp = GetAlignPlatformOffset();
			m_apAlignerProcesInfo.nTimes  = i;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				cpProduct = GetAlignProductOffsetXYPD();
			}
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;
			PostMessageUpdateAlignerProcessInfo();
			
			if(optionInfo.m_nBendFIsAlignFinished==0)
			{
				//bBendInspect = BendingAlignProcessInfo(strBendInsError);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定BendingAlignProcessInfo------");
				bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定SetAllPosText------");
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定SetAllPosSearchResult------");
				m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

				if ((BendInfo.m_bBendPressPara) && (0 == i) )
				{

				}
				else
				{
					VisionInspectBendRecord(TRUE, TRUE, FALSE,IsAlignSucceed());
				}

				VisionAlignLogRecord("D:\\TimeLog.txt", "------仅进行对位判定PostMessageUpdateBendingProcessInfo------");
				PostMessageUpdateBendingProcessInfo();
				

			}

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
			{
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}
				ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
			}

			m_bStatusBar = IsAlignSucceed();
			if(optionInfo.m_nBendFIsAlignFinished==2)m_bStatusBar=TRUE;//模式2的时候不判断对位是否成功
			// 启用中心对位，将中心对位结果写入D20地址
			if(m_bStatusBar && optionInfo.m_bEnableAlignBenchCenter)
			{
				CPlatformXYDAxisPos* pPlatformXYDAxisPosMove1 = new CPlatformXYDAxisPos();
				pPlatformXYDAxisPosMove1->m_dPosX = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dPosY = cpBenchOffset.GetPosX();
				pPlatformXYDAxisPosMove1->m_dAngle = cpBenchOffset.GetAngle();
				ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPosMove1,10); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				if (pPlatformXYDAxisPosMove1!=NULL)
				{
					delete pPlatformXYDAxisPosMove1;
					pPlatformXYDAxisPosMove1 = NULL;					
				}
			}
			auto IsAlignSu= IsAlignSucceed();
			if(optionInfo.m_nBendFIsAlignFinished==2)IsAlignSu=TRUE;//模式2的时候不判断对位是否成功
			if (pAlignerParam->GetEnableAlnTargetOffset() && IsAlignSu && FALSE == pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
			{
				CCoordPos cp = GetAlignPlatformOffset();
				cp.m_dPosX+=pAlignerParam->GetAlnTargetOffsetX();
				cp.m_dPosY+=pAlignerParam->GetAlnTargetOffsetY();

				if(IsAxisMoveOverRun(cp))
				{
					ReportPLCFunc(eCodeNG);
					m_nCurAlignerTimesIndex = 0;
					m_bALLBendRecord = _T("");
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
					m_strStatusBarInfo2.Format(_T(""));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);

					PostMessageUpdateStatusBarInfo();
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

				// 执行本次对位操作
				// 获取平台对位应移动到的绝对位置
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
				pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();			

				AddLastTargetOffsetForAln(pPlatformXYDAxisPos);

				// 打包偏差量结果D10-14
				//ReportPLC_WriteAbsPosEx(pPlatformXYDAxisPos); // 暂时将居中对位结果写入（D10+10）D20-D24 XYD
				BOOL bStatusBar = FALSE;
				bStatusBar = (m_bStatusBar && bBendInspect);
				// 检测失败
				if (!bStatusBar)
				{
					ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos);
				}
				else
				{
					ReportPLC_Offset(cp, eCodeNG, pPlatformXYDAxisPos);
				}

				if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
				{
					m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
					m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
					if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;
					}
					else
					{
						m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;

					}

					if (m_pSystempOptionConfig->m_bShowSepTargetPos)
					{
						UpdateTargetPosAfterRoate();
					}
				}
				else
				{
					m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
				}			

				return bStatusBar;
			}

			if(optionInfo.m_nBendFIsAlignFinished==0)bBendInspect=TRUE;//模式0的时候不判断检测是否成功
			if (m_bStatusBar)
			{

				BOOL bStatusBar = FALSE;
				bStatusBar = (m_bStatusBar && bBendInspect);

				CPlatformXYDAxisPos* pPlatformXYDAxisPos2 = NULL;
				pPlatformXYDAxisPos2 = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();	

				// 检测失败
				if (!bStatusBar)
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					
					// 打包回复偏差量到D10-14，完成对位，回复OK
					ReportPLC_Offset(cp, eCodeNG, pPlatformXYDAxisPos2);
					// 打包回复检测结果D30
					ReportPLC_Inspect(m_apAlignerProcesInfo.nTimes);
					m_nCurAlignerTimesIndex = 0;
					m_bALLBendRecord = _T("");
				}
				else
				{
					m_bStatusBar = TRUE;

					
					// 打包回复偏差量到D10-14，完成对位，回复OK
					ReportPLC_Offset(cp, eCodeOK, pPlatformXYDAxisPos2);
					// 打包回复检测结果D30
					ReportPLC_Inspect(m_apAlignerProcesInfo.nTimes);
					m_nCurAlignerTimesIndex = 0;
					m_bALLBendRecord = _T("");

					//m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_SUCCEED);
					m_strStatusBarInfo1= _T("折弯对位OK");
					m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), i, stTotalAlignTimer.GetTimeMilli(TRUE));

					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
				}
			}
			else
			{
				ReportPLCFunc(eCodeNG);
				m_nAlignTime = 0;

				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_FAILED);
				m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIMES_AND_TIME), i, dTotalAlignTime);
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			}

			// 记录对位精度
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;
				CString strOut;					
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_FINAL_ALIGN_ACCURACY), m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;	
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;
				CString strOut;						
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_ALIGN_ALL_TIMES_AND_ALL_TIME), i, dTotalAlignTime);
				strOut = strTemp + strOut;
				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			return m_bStatusBar;  
		}
		else
		{
			CCoordPos cp = GetAlignPlatformOffset(); // 目标Mark和对象Mark二者之间的

			m_apAlignerProcesInfo.nTimes  = i;
			m_apAlignerProcesInfo.dTime   = stAlignTimer.GetTimeMilli(FALSE);
			m_apAlignerProcesInfo.dXMove  = cp.m_dPosX;  
			m_apAlignerProcesInfo.dYMove  = cp.m_dPosY;
			m_apAlignerProcesInfo.dDMove  = cp.m_dAngle;

			CCoordPos cpProduct = GetAlignProductOffset();
			m_apAlignerProcesInfo.dXOffset = cpProduct.m_dPosX;
			m_apAlignerProcesInfo.dYOffset = cpProduct.m_dPosY;
			m_apAlignerProcesInfo.dDOffset = cpProduct.m_dAngle;

			if(BendInfo.m_bEnableAxisMoveLimit)
			{
				CCoordPos cpTmp = cp;
				if(abs(cpTmp.m_dPosX)>abs(BendInfo.m_dAxisAllowMoveMaxX))
				{
					if(cpTmp.m_dPosX<0)
					{
						cp.m_dPosX = -abs(BendInfo.m_dAxisAllowMoveMaxX);
					}
					else
					{
						cp.m_dPosX = abs(BendInfo.m_dAxisAllowMoveMaxX);
					}

				}
				if(abs(cpTmp.m_dPosY)>abs(BendInfo.m_dAxisAllowMoveMaxY))
				{
					if(cpTmp.m_dPosY<0)
					{
						cp.m_dPosY = -abs(BendInfo.m_dAxisAllowMoveMaxY);
					}
					else
					{
						cp.m_dPosY = abs(BendInfo.m_dAxisAllowMoveMaxY);
					}
				}
				if(abs(cpTmp.m_dAngle)>abs(BendInfo.m_dAxisAllowMoveMaxD))
				{
					if(cpTmp.m_dAngle<0)
					{
						cp.m_dAngle = -abs(BendInfo.m_dAxisAllowMoveMaxD);
					}
					else
					{
						cp.m_dAngle = abs(BendInfo.m_dAxisAllowMoveMaxD);
					}

				}
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = _T("平台轴移动量超过允许折弯移动量");
				m_strStatusBarInfo2.Format(_T("采用分步偏差量发送"));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();
			}

			PostMessageUpdateAlignerProcessInfo();

			if(optionInfo.m_nBendFIsAlignFinished==0)
			{
				//bBendInspect = BendingAlignProcessInfo(strBendInsError);
				bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,strProductSTATION,strInspectStation);
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				//m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();
				m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);

				if ((BendInfo.m_bBendPressPara) && (0 == i) )
				{

				}
				else
				{
					int nStatus = -1;
					if (m_nAlignTime >= m_pAlignerTool->GetProductAlignerParam()->GetAlignerMaxTime())
					{
						if (IsAlignSucceed())
						{
							nStatus = 1;
						}
						else
						{
							nStatus = 0;
						}
					}
					else
						nStatus = 2;
					
						
					VisionInspectBendRecord(TRUE, TRUE, FALSE,nStatus);
					PostMessageUpdateBendingProcessInfo();
				}
			}

			//if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile && (0 == i))
			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile&&m_pVisionASMConfig->m_DataRecord.m_bSaveProcessSnapVDBFile)
			{
				std::vector<int> vnPosIndex;
				int nTempPosIndex = 0;
				for(nTempPosIndex = 0; nTempPosIndex < GetPosNum(); nTempPosIndex++)
				{
					vnPosIndex.push_back(nTempPosIndex);
				}
				ExeSaveVDBFile(vnPosIndex,eObjectSearch,bBendInspect);
			}


			if(IsAxisMoveOverRun(cp))
			{
				ReportPLCFunc(eCodeNG);
				m_nCurAlignerTimesIndex = 0;
			    m_bALLBendRecord = _T("");
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_PLATFORM_MOVE_OVERFLOW);//.Format(_T("平台轴移动量超过设定的最大可移动量"));
				m_strStatusBarInfo2.Format(_T(""));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
				PostMessageUpdateStatusBarInfo();

				strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
				AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

				return FALSE;
			}

			// 执行本次对位操作
			// 获取平台对位应移动到的绝对位置
			CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL;
			pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)GetAlignPlatformMovementResults();

			
			// 打包回复偏差量到D10-14
			ReportPLC_Offset(cp, eCodeContinued, pPlatformXYDAxisPos);
			// 打包回复检测结果D30
			ReportPLC_Inspect(m_apAlignerProcesInfo.nTimes);

			m_nCurAlignerTimesIndex = i+1;
			if (ePlatformXYPD == sysPlatformInfo.m_eMidPlatformType)
			{
				m_CurPlatformAxisPos.m_dPosX = pPlatformXYDAxisPos->m_dPosX;
				m_CurPlatformAxisPos.m_dPosY = pPlatformXYDAxisPos->m_dPosY;
				if (m_pPlatformInfo->m_bTargetObjectCamSeparate)
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()/2-1))->m_dAngle;

				}
				else
				{
					m_CurPlatformAxisPos.m_dAngle = ((CPlatformXYDAxisPos*)m_vpObjectPlatformAxisPos.at(m_vpObjectPlatformAxisPos.size()-1))->m_dAngle;
				}
				//D角度不累加
				if (m_pSystempOptionConfig->m_bShowSepTargetPos)
				{
					UpdateTargetPosAfterRoate();
				}
			}
			else
			{
				m_CurPlatformAxisPos = *pPlatformXYDAxisPos;
			}

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CERTAIN_ALIGN_SUCCEED), m_apAlignerProcesInfo.nTimes);

			if(!bBendInspect)
			{
				m_strStatusBarInfo1 +=  strBendInsError;
			}
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_ALIGN_TIME), stTotalAlignTimer.GetTimeMilli(TRUE)/*m_apAlignerProcesInfo.dTime*/);
			m_strStatusBarInfo2+= _T("精度未达到，继续对位");

			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	

			dAlignTime = m_apAlignerProcesInfo.dTime;

			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerPrecision)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_OFFSET), 
					i+1, m_apAlignerProcesInfo.dXMove, m_apAlignerProcesInfo.dYMove, m_apAlignerProcesInfo.dDMove);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录对位时间
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerTime)
			{
				CString strTemp;

				CString strOut;
				strOut.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_CERTAIN_ALIGN_TIME_OBJECT_SEARCH_TIME_ALIGN_TIME), i+1, dAlignTime, dObjectSearchTime, dCaluteAlignTime);
				strOut = strTemp + strOut;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strOut);
			}

			// 记录当前平台各个轴的绝对位置
			if (m_pVisionASMConfig->m_DataRecord.m_bAlignerSearchPltfmInfo)
			{
				CString strTemp;
				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_DR_XYD_EXCUTE_ALIGN_ACTION_THEN_PLATFORM_POS),
					i+1, pPlatformXYDAxisPos->m_dPosX, pPlatformXYDAxisPos->m_dPosY, pPlatformXYDAxisPos->m_dAngle);
				strInfo = strTemp + strInfo;

				VisionAlignLogRecord(m_strAlignProcessData, _T("\n"), FALSE);
				VisionAlignLogRecord(m_strAlignProcessData, strInfo);
			}

		} // else

	}// for	

	return TRUE;
}
BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFCHB_Ex(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex)
{
	// vnPatIndex表示模板组合，大小等于5，数组中从0到4分别代表标准0和候补1/2/3/4模板，哪位置1则哪个模板选中
	// 在处理多模板vnPatIndex时，与定位搜索时的候补策略不同（先从第一个开始搜索，当前失败后转下一个，成功后不再继续），本函数要求选中的所有模板都成功才算成功，只要有一个失败就算失败，相当于与操作
	// “是否候补搜索”和“指定候补模板是否选中”有效，如果不开启则无法对候补模板进行检测，但是标准0模板不受限制
	// “是否成对搜索”无效，与操作默认对选中位置的选中模板都要求检测成功

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();

	CString strCmd = _T("VCHB:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");

	VisionAlignLogRecord("D:\\TimeLog.txt", "start");


	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	if (vnPatIndex.size() != 5)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		ReportPLCFunc(eCodeNG);
		return FALSE;
	}

	for(int i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			ReportPLCFunc(eCodeNG);
			return FALSE;
		}    	
	}


	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		ReportPLCFunc(eCodeNG);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}

	CString strProductID = _T("");
	if ( (pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
	{
		if (!CommunicateGetAlignProcessProductID(strProductID))
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品ID读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}
	m_BendInspectResultInfo.m_strProductID = strProductID;
	m_strProductID = strProductID;

	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			ReportPLCFunc(eCodeNG);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nStartPatIndex = 0;
		for(int it=0;it<vnPatIndex.size();it++)
		{
			if(vnPatIndex.at(it)==1)
			{
				nStartPatIndex = it;
				break;
			}
		}

		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate*/ && bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	
			int i = 0;
			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}
			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}



	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;
	for(int i = 0; i < nPosNum; i++)
	{
		vsmSearchMode.push_back(eInspectSearch);
	}


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);

	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 开始");
	m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eInspectSearch);
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		ReportPLCFunc(eCodeNG);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 结束");


	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();

	BOOL bSearchSucceed = TRUE;
	std::vector<BOOL> vbSearchSucceed;
	std::vector<int> vnSearchIndex;

	if (bInspectSearchCandidate == FALSE)
	{
		// 如果模板参数选中中候补1/2/3/4模板，但是界面设置中没有选中，则认为检测失败
		for (int j=1; j<5; j++)
		{
			if (vnPatIndex.at(j) == 1)
			{
				bSearchSucceed = FALSE;
				break;
			}
		}

		// 如果不启用候补模板，则只能检测标准0模板，直接跳过候补1/2/3/4模板
		if (vnPatIndex.at(0) == 1)
		{
			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = 0;
				m_vnInspectSearchEndPatIndex[nPosIndex] = 0;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);
			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(0);
		}
	}
	else
	{
		for (int j=0; j<5; j++)
		{
			// 如果模板参数没有选中该模板，则跳过
			if (vnPatIndex.at(j) == 0)
			{
				continue;
			}

			// 如果候补1/2/3/4模板在界面设置中没有被选中，则认为检测失败
			if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
			{
				bSearchSucceed = FALSE;
				break;
			}

			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = j;
				m_vnInspectSearchEndPatIndex[nPosIndex] = j;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);
			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(j);
		}
	}


	std::vector<double> vdDistance;
	std::vector<BOOL> vbAreaInspectSubResult;

	std::vector<double> vdCircleRadius;


	std::vector<BOOL> vbBendTwoModelSearchTool;
	std::vector<BOOL> vbBendCircleSearchTool;

	// FALSE包含两种情况：1）候补功能未启用、但是指令参数选中候补模板，2）候补功能打开、指令参数选中候补模板、但是界面上候补模板未选中
	if (TRUE == bSearchSucceed)
	{
		// 等于0包含两种情况：1）候补功能未启用、并且指令参数没有选中标准0模板，2）候补功能打开、界面上候补模板选中、但是参数一个都没有选中
		if (vbSearchSucceed.size() == 0)
		{
			bSearchSucceed = FALSE;
		}
		else
		{
			// 对所有参与检测的模板进行遍历
			for (int j=0; j<vbSearchSucceed.size(); j++)
			{
				// 将当前检测结果与总结果进行与操作
				bSearchSucceed &= vbSearchSucceed.at(j);

				// 获取当前检测结果对应模板索引
				int nPatIndex = vnSearchIndex.at(j);

				// 遍历所有位置
				for (int i = 0; i<nPosNum; i++)
				{
					// 获取当前位置
					nPosIndex = vnPosIndex[i];

					// 获取指定位置、指定候补模板的定位工具
					CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, nPatIndex);

					if (pSearchTool != NULL)
					{
						CSearchToolType eType = pSearchTool->GetSearchToolType();
						switch (eType)
						{
						case ePatternPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else
								{
									// 不等于0：如果找线工具1成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eQuickPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);
									
								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
									
								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else
								{
									// 不等于0：如果找线工具1成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eAreaInspectTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);
							}
							break;
						case eAreaInspectToolEx:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的子检测结果发送PLC时，先发送第一个模板的第一个位置的所有子结果，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								for (int n=0; n<result.m_resultAreaInspectToolEx.m_nValidResultNum; n++)
								{
									BOOL bTemp = result.m_resultAreaInspectToolEx.m_vnResult.at(n) == 1;
									vbAreaInspectSubResult.push_back(bTemp);
								}
							}
							break;

						case eCornerSearchCheckInspectTool:
							{

							}
							break;

						case eACFInspectTool:
							{

							}
							break;

							// 两几何定位工具
						case eTwoQuickModelSearchTool:
						case eTwoPatModelSearchTool:
							{
								vbBendTwoModelSearchTool.push_back(TRUE);

								CSearchResult result;
								pSearchTool->GetResult(0, result);

								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(0), result.m_vdAuxiliaryPosY.at(0), 0));
								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(1), result.m_vdAuxiliaryPosY.at(1), 0));

								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}

							}
							break;

						case eCircleSearchTool:
						case ePatternCircleSearchTool:
							{
								vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
								vdCircleRadius.push_back(dCircleRadius);
							}
							break;

						default:
							break;
						}
					}
				}
			}
		}
	} 


	CString strBendInsError = _T("");
	BOOL bBendInspect = FALSE;

	if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		// 手动弹窗功能，只针对折弯检测使用的工具，其它定位工具不支持
		if (FALSE == bSearchSucceed)
		{
			if (optionInfo.m_bManualAfObjSearchFail && (2 == vbBendTwoModelSearchTool.size()))
			{
				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉弹出手动搜索框
				// 	SetPlcManualSearch(TRUE);
				// }

				if (FALSE ==ExcuteManualSearch(2,vnPosIndex))
				{
					if (optionInfo.m_bSetVisionManualSearchToPLC)
					{
						// 通知PLC，视觉关闭手动搜索框
						//SetPlcManualSearch(FALSE);
					}
				}
				else
				{
					bSearchSucceed = TRUE;
				}

				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉关闭手动搜索框
				// 	SetPlcManualSearch(FALSE);
				// }

				if (TRUE == bSearchSucceed)
				{
					if ( 2 == vbBendTwoModelSearchTool.size())
					{
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

						m_pAlignerTool->SetBendAlignProcess(FALSE);
						bBendInspect = BendingAlignProcessInfo(strBendInsError);
						m_pAlignerTool->SetBendAlignProcess(TRUE);

						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingProcessInfo();

						VisionInspectBendRecord(FALSE, TRUE, FALSE);

					}

					if ( 2 == vbBendCircleSearchTool.size() )
					{
						bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingCircleInfo();

						VisionInspectBendRecord(FALSE, FALSE, TRUE);
					}


					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}

					// 打包回复PLC成功
					ReportPLCFunc(eCodeOK);

					m_bStatusBar = TRUE;
					m_strStatusBarInfo1 = _T("检测成功");
					m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return TRUE;

				}
				else
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}

					// 打包回复PLC NG
					ReportPLCFunc(eCodeNG);

					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

			}
		}


		if (TRUE == bSearchSucceed)
		{
			if ( 2 == vbBendTwoModelSearchTool.size())
			{
				SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
				SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

				m_pAlignerTool->SetBendAlignProcess(FALSE);
				bBendInspect = BendingAlignProcessInfo(strBendInsError);
				m_pAlignerTool->SetBendAlignProcess(TRUE);

				bSearchSucceed = bBendInspect;
				PostMessageUpdateBendingProcessInfo();

				VisionInspectBendRecord(FALSE, TRUE, FALSE);

			}

			if ( 2 == vbBendCircleSearchTool.size() )
			{
				bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
				bSearchSucceed = bBendInspect;
				PostMessageUpdateBendingCircleInfo();

				VisionInspectBendRecord(FALSE, FALSE, TRUE);
			}

			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}

			ReportPLCFunc(eCodeOK);

			m_bStatusBar = TRUE;
			m_strStatusBarInfo1 = _T("检测成功");
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

			return TRUE;

		}
		else
		{
			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}

			ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = strBendInsError;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			return FALSE;
		}

	}

	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);

	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch);
	}

	// 使用ACF检测工具、ACF检测工具EX和区域点线工具时，要求平台选项界面必须不启用线距检测，否则在检测成功和平台选项界面启用发送线距结果时，会因为获取距离结果错误造成软件崩溃
	// 只有使用角定位检查测量工具时，才能在平台选项界面启用线距检测和发送线距结果

	// 使用区域点线工具测量点距时，无论检测结果整体成功/失败，均将距离发送到PLC备用轴位置寄存器(距离1/2/3/.../n)，无协议也发送
	// 使用ACF检测工具EX时，如果平台选项界面启用发送子结果，则无论检测结果整体成功/失败，均将子结果发送到PLC备用寄存器（数量+子结果1/2/3/.../n），无协议不发送
	// 使用角定位检查测量工具测量线距时，要求平台选项界面必须启用线距检测，在同时满足检测成功和平台选项界面启用发送线距结果时，才将距离发送到PLC备用轴位置寄存器(距离1/2/12结果/距离3/4/34结果)，无协议不发送


	if (TRUE == bSearchSucceed)
	{
		ReportPLCFunc(eCodeOK);
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = _T("检测成功");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		ReportPLCFunc(eCodeNG);

		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = strBendInsError;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	return TRUE;
}

//检测工位用的 使用扩展指令 候补方式
BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFCHB_Ex3(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex)
{
	// vnPatIndex表示模板组合，大小等于5，数组中从0到4分别代表标准0和候补1/2/3/4模板，哪位置1则哪个模板选中
	// 在处理多模板vnPatIndex时，与定位搜索时的候补策略不同（先从第一个开始搜索，当前失败后转下一个，成功后不再继续），本函数要求选中的所有模板都成功才算成功，只要有一个失败就算失败，相当于与操作
	// “是否候补搜索”和“指定候补模板是否选中”有效，如果不开启则无法对候补模板进行检测，但是标准0模板不受限制
	// “是否成对搜索”无效，与操作默认对选中位置的选中模板都要求检测成功
	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();


	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");

	CString strCmd = _T("VCHB:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	vector<double> m_vdDetectionResult1;
	for (int ii=0;ii<optionInfo.m_nNumberOfInspectResults;ii++)
	{
		m_vdDetectionResult1.push_back(0);
	}

	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	if (vnPatIndex.size() != 5)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

		return FALSE;
	}

	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	for(int i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

			return FALSE;
		}    	
	}


	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}

	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nStartPatIndex = 0;
		for(int it=0;it<vnPatIndex.size();it++)
		{
			if(vnPatIndex.at(it)==1)
			{
				nStartPatIndex = it;
				break;
			}
		}

		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate*/ && bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	
			int i = 0;
			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}
			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}



	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;
	for(int i = 0; i < nPosNum; i++)
	{
		vsmSearchMode.push_back(eInspectSearch);
	}


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);

	m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eInspectSearch);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 开始");
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 结束");
	ReportPLCFunc(eCodeSnapOK);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照回复PLC成功 结束");

	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();

	BOOL bInspectSingleSearch = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectSingleSearchEnable();

	BOOL bSearchSucceed = TRUE;
	std::vector<BOOL> vbSearchSucceed;
	std::vector<int> vnSearchIndex;

	std::vector<BOOL> vnPosSearchSucceed;

	CString strErrorInfo;	// 检测工具失败提示
	CString strPatNum;   // 模板序号
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 开始");

	if (bInspectSearchCandidate == FALSE)
	{
		// 如果模板参数选中中候补1/2/3/4模板，但是界面设置中没有选中，则认为检测失败
		/*for (int j=1; j<5; j++)   //注释后组合模板参数不进行报警
		{
			if (vnPatIndex.at(j) == 1)
			{
				bSearchSucceed = FALSE;
				strErrorInfo = "检测模板候选搜索未开. ";
				break;
			}
		}*/

		// 如果不启用候补模板，则只能检测标准0模板，直接跳过候补1/2/3/4模板
		if (vnPatIndex.at(0) == 1)
		{
			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = 0;
				m_vnInspectSearchEndPatIndex[nPosIndex] = 0;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strErrorInfo += m_StrSearchErrorInfo + "模板搜索失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(0);
		}
	}
	else
	{
		for (int j=0; j<5; j++)
		{
			// 如果模板参数没有选中该模板，则跳过
			if (vnPatIndex.at(j) == 0)
			{
				continue;
			}

			// 如果候补1/2/3/4模板在界面设置中没有被选中，则认为检测失败
			if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
			{
				bSearchSucceed = FALSE;
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += "检测模板" + strPatNum + "未开启. ";
				break;
			}


			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = j;
				m_vnInspectSearchEndPatIndex[nPosIndex] = j;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			for (int m = 0; m<nPosNum; m++)
			{
				vnPosSearchSucceed.push_back(m_vbSearchSucceed.at(m));
			}

			if (bTmep == FALSE)
			{
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += m_StrSearchErrorInfo + "模板" + strPatNum + "失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(j);

			if (bTmep == TRUE)
			{
				break;
			}
		}
	}

	std::vector<BOOL> vbResult;
	std::vector<int> vbResultIndex;
	BOOL vbResult1;
	int vbResultIndex1;
	//判定哪个模板成功
	if ( 1 == bInspectSingleSearch && 1 == bInspectSearchCandidate)
	{
		if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
		{
			for (int i = 0;i<nPosNum;i++)
			{
				for (int j= 0; j<vbSearchSucceed.size();j++)
				{
					if (1 == vnPosSearchSucceed.at(nPosNum*j + i))
					{
						vbResult1 = TRUE;
						vbResultIndex1 = j;
						break;
					}
					else
					{
						vbResult1 = FALSE;
						vbResultIndex1 = 0;
					}
				}

				vbResult.push_back(vbResult1);
				vbResultIndex.push_back(vbResultIndex1);
			}
		}	
	}
	else if (1 != bInspectSingleSearch && 1 == bInspectSearchCandidate)
	{
		if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
		{
			if (vnSearchIndex.size() > 1)
			{
				int bSearchSucceed = vbSearchSucceed[vnSearchIndex.size()-1];
				int nSearchIndex = vnSearchIndex[vnSearchIndex.size()-1];
				for (int i = 0;i < vnSearchIndex.size();++i)
				{
					if (vbSearchSucceed[i])
					{
						bSearchSucceed = vbSearchSucceed[i];
						nSearchIndex = vnSearchIndex[i];
						break;
					}
				}
				vbSearchSucceed.clear(); vbSearchSucceed.push_back(bSearchSucceed);
				vnSearchIndex.clear(); vnSearchIndex.push_back(nSearchIndex);
			}
		}
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 结束");


	std::vector<double> vdDistance;
	std::vector<BOOL> vbAreaInspectSubResult;

	std::vector<double> vdCircleRadius;


	std::vector<BOOL> vbBendTwoModelSearchTool;
	std::vector<BOOL> vbBendCircleSearchTool;
	std::vector<BOOL> vbBendCircleSearchToolXY;

	//double dDetectionResult[6] = {0, 0, 0, 0,0,0};	//初始化检测结果

	std::vector<BOOL> vbBendCornerSearchTool;

	// FALSE包含两种情况：1）候补功能未启用、但是指令参数选中候补模板，2）候补功能打开、指令参数选中候补模板、但是界面上候补模板未选中

	std::vector<BOOL> PosResult1;

	if (TRUE == bSearchSucceed)
	{
		// 等于0包含两种情况：1）候补功能未启用、并且指令参数没有选中标准0模板，2）候补功能打开、界面上候补模板选中、但是参数一个都没有选中
		if (vbSearchSucceed.size() == 0)
		{
			bSearchSucceed = FALSE;
		}
		else
		{
			//是否全部模板均需要成功
			if (1 == bInspectSingleSearch && 1 == bInspectSearchCandidate)
			{		
				// 遍历所有位置
				for (int i = 0; i<nPosNum; i++)
				{
					// 获取当前位置
					nPosIndex = vnPosIndex[i];

					bSearchSucceed &= vbResult.at(i);
					CBaseSearchTool* pSearchTool; 

					if (TRUE == vbResult.at(i))
					{
						// 获取指定位置、指定候补模板的定位工具
						pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, vbResultIndex.at(i));
					}
					else
					{
						// 获取指定位置、指定候补模板的定位工具
						pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, 0);
					}
					//// 获取指定位置、指定候补模板的定位工具
					//CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, vbResultIndex.at(i));

					if (pSearchTool != NULL)
					{
						CSearchToolType eType = pSearchTool->GetSearchToolType();
						switch (eType)
						{
						case ePatternPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
								{


								}else
								{
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eQuickPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);

								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
								{
								}
								else
								{
									// 不等于0：如果找线工具1成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eAreaInspectTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);
							}
							break;
						case eAreaInspectToolEx:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的子检测结果发送PLC时，先发送第一个模板的第一个位置的所有子结果，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								for (int n=0; n<result.m_resultAreaInspectToolEx.m_nValidResultNum; n++)
								{
									BOOL bTemp = result.m_resultAreaInspectToolEx.m_vnResult.at(n) == 1;
									vbAreaInspectSubResult.push_back(bTemp);
								}
							}
							break;

						case eCornerSearchCheckInspectTool:
							{
								vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								CResultL2LInspectDistance* tmpL2LReslut=&result.m_ResultL2LInspectDistance;
								//获取找线结果
								for(int lineindex=0;lineindex<3;lineindex++)
								{
									if(tmpL2LReslut->m_nResult[lineindex]!=0)//找线OK
									{
										//vdDistance.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
										vdCircleRadius.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
									}
									else
									{
										//vdDistance.push_back(0);
										vdCircleRadius.push_back(0);

									}
								}

							}
							break;
						case eCircleCornerSearchCheckInspectTool:
							{
								vbBendCircleSearchToolXY.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推

								if (result.m_CircleCornerDistance.m_vDistMM[0] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[0]);

								}


								if (result.m_CircleCornerDistance.m_vDistMM[1] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[1]);

								}

								if (result.m_CircleCornerDistance.m_vSizeMM[0] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vSizeMM[0]);

								}



							}
							break;

						case eACFInspectTool:
							{

							}
							break;

							// 两几何定位工具
						case eTwoQuickModelSearchTool:
						case eTwoPatModelSearchTool:
							{
								vbBendTwoModelSearchTool.push_back(TRUE);

								CSearchResult result;
								pSearchTool->GetResult(0, result);

								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(0), result.m_vdAuxiliaryPosY.at(0), 0));
								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(1), result.m_vdAuxiliaryPosY.at(1), 0));

								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}

							}
							break;
						case ePatternCornerSearchTool:
						case eCornerSearchTool:
							{
								vbBendCornerSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));

								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));




								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}
							}
							break;
						case eCircleSearchTool:
							{
								//vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
								vdCircleRadius.push_back(dCircleRadius);
							}
							break;

						case ePatternCircleSearchTool:
							{
								//vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
								vdCircleRadius.push_back(dCircleRadius);
							}
							break;

						case eCombinationCornerSearchTool:
							{
								vbBendTwoModelSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(1, result);

								/*CSearchResult resultt;
								pSearchTool->GetResult(1, result);*/

								/*m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));
								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));*/

								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));
								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));

								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}
							}
							break;
						default:
							break;
						}
					}
				}
			} 
			else
			{
				{
					// 对所有参与检测的模板进行遍历
					for (int j=0; j<vbSearchSucceed.size(); j++)
					{
						// 将当前检测结果与总结果进行与操作
						bSearchSucceed &= vbSearchSucceed.at(j);

						// 获取当前检测结果对应模板索引
						int nPatIndex = vnSearchIndex.at(j);

						// 遍历所有位置
						for (int i = 0; i<nPosNum; i++)
						{
							// 获取当前位置
							nPosIndex = vnPosIndex[i];

							// 获取指定位置、指定候补模板的定位工具
							CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, nPatIndex);

							if (pSearchTool != NULL)
							{
								CSearchToolType eType = pSearchTool->GetSearchToolType();
								switch (eType)
								{
								case ePatternPosLineDistanceTool:
									{
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
										if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
										{
											// 等于0：如果找线工具0失败，也将数值0传出
											vdDistance.push_back(0.0);
										}
										else
										{
											// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
											vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
										}

										if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
										{
											// 等于0：如果找线工具1失败，也将数值0传出
											vdDistance.push_back(0.0);
										}
										else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
										{


										}else
										{
											vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
										}
									}
									break;
								case eQuickPosLineDistanceTool:
									{
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
										if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
										{
											// 等于0：如果找线工具0失败，也将数值0传出
											vdDistance.push_back(0.0);

										}
										else
										{
											// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
											vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);

										}

										if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
										{
											// 等于0：如果找线工具1失败，也将数值0传出
											vdDistance.push_back(0.0);
										}else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
										{
										}
										else
										{
											// 不等于0：如果找线工具1成功，无论检测结果成功或失败，均将实际结果传出
											vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
										}
									}
									break;
								case eAreaInspectTool:
									{
										CSearchResult result;
										pSearchTool->GetResult(0, result);
									}
									break;
								case eAreaInspectToolEx:
									{
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										// 多模板的多位置的子检测结果发送PLC时，先发送第一个模板的第一个位置的所有子结果，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
										for (int n=0; n<result.m_resultAreaInspectToolEx.m_nValidResultNum; n++)
										{
											BOOL bTemp = result.m_resultAreaInspectToolEx.m_vnResult.at(n) == 1;
											vbAreaInspectSubResult.push_back(bTemp);
										}
									}
									break;

								case eCornerSearchCheckInspectTool:
									{
										vbBendCircleSearchTool.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										CResultL2LInspectDistance* tmpL2LReslut=&result.m_ResultL2LInspectDistance;
										//获取找线结果
										for(int lineindex=0;lineindex<3;lineindex++)
										{
											if(tmpL2LReslut->m_nResult[lineindex]!=0)//找线OK
											{
												//vdDistance.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
												vdCircleRadius.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
											}
											else
											{
												//vdDistance.push_back(0);
												vdCircleRadius.push_back(0);

											}
										}

									}
									break;
								case eCircleCornerSearchCheckInspectTool:
									{
										vbBendCircleSearchToolXY.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推

										if (result.m_CircleCornerDistance.m_vDistMM[0] == 0)
										{
											// 等于0：如果找线工具0失败，也将数值0传出
											vdCircleRadius.push_back(0.0);

										}
										else
										{
											// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
											vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[0]);

										}


										if (result.m_CircleCornerDistance.m_vDistMM[1] == 0)
										{
											// 等于0：如果找线工具0失败，也将数值0传出
											vdCircleRadius.push_back(0.0);

										}
										else
										{
											// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
											vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[1]);

										}

										if (result.m_CircleCornerDistance.m_vSizeMM[0] == 0)
										{
											// 等于0：如果找线工具0失败，也将数值0传出
											vdCircleRadius.push_back(0.0);

										}
										else
										{
											// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
											vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vSizeMM[0]);

										}



									}
									break;

								case eACFInspectTool:
									{

									}
									break;

									// 两几何定位工具
								case eTwoQuickModelSearchTool:
								case eTwoPatModelSearchTool:
									{
										vbBendTwoModelSearchTool.push_back(TRUE);

										CSearchResult result;
										pSearchTool->GetResult(0, result);

										m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(0), result.m_vdAuxiliaryPosY.at(0), 0));
										m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(1), result.m_vdAuxiliaryPosY.at(1), 0));

										CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
										SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
										SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

										if (pPlatformAxisPos)
										{
											delete pPlatformAxisPos;
											pPlatformAxisPos = NULL;
										}

									}
									break;
								case ePatternCornerSearchTool:
								case eCornerSearchTool:
									{
										vbBendCornerSearchTool.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));

										m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));




										CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
										SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
										SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

										if (pPlatformAxisPos)
										{
											delete pPlatformAxisPos;
											pPlatformAxisPos = NULL;
										}
									}
									break;
								case eCircleSearchTool:
									{
										//vbBendCircleSearchTool.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
										vdCircleRadius.push_back(dCircleRadius);
									}
									break;

								case ePatternCircleSearchTool:
									{
										//vbBendCircleSearchTool.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(0, result);

										double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
										vdCircleRadius.push_back(dCircleRadius);
									}
									break;

								case eCombinationCornerSearchTool:
									{
										vbBendTwoModelSearchTool.push_back(TRUE);

										vbBendCornerSearchTool.push_back(TRUE);
										CSearchResult result;
										pSearchTool->GetResult(1, result);

										/*CSearchResult resultt;
										pSearchTool->GetResult(1, result);*/

										m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));
										m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
										CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
										SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
										SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
										if (pPlatformAxisPos)
										{
											delete pPlatformAxisPos;
											pPlatformAxisPos = NULL;

										}
									}
									break;
								default:
									break;
								}
							}
						}
					}
				} 
			}

		}
	} 

	//检测读码
	CString strProductID = _T("");
	//检测工位
	CString strProductSTATION = _T("");
	//本压工位
	CString strInspectStation = _T("");

	CString strStartIime = _T("");
	CString strEndIime = _T("");
	CString strProsessingIime = _T("");
	if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
	{
		if (!CommunicateGetAlignProcessProductID(strProductID))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品ID读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
	{
		if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品工位读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	m_BendInspectResultInfo.m_strProductID = strProductID;
	m_strProductID = strProductID;
	m_strProductSTATION = strProductSTATION;
	m_strInspectStation = strInspectStation;
	m_BendInspectResultInfo.m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strInspectStation = strInspectStation;

	CString strBendInsError = _T("");
	BOOL bBendInspect = FALSE;

	std::vector<BOOL> m_bResult;
	std::vector<CString> m_vSearchResult;
	CString	 m_StrText;

	CMarkImagePos       m_mpInsectTargetMarkImagePos;
	m_mpInsectTargetMarkImagePos = GetTargetMarkSearchResults();

	//这里去掉，折弯检测用不了手动弹窗
	if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		m_apAlignerProcesInfo.nTimes = 0;
		// 手动弹窗功能，只针对折弯检测使用的工具，其它定位工具不支持
		if (FALSE == bSearchSucceed/*FALSE*//*&& 1 == bInspectSearchCandidate && 1 == bInspectSingleSearch*/)
		{
			if (optionInfo.m_bManualAfObjSearchFail && 2 == vbBendTwoModelSearchTool.size())
			{
				if (optionInfo.m_bSetVisionManualSearchToPLC)
				{
					// 通知PLC，视觉弹出手动搜索框
					//SetPlcManualSearch(TRUE);
					SetPlcManualSearch_EX(TRUE);
				}

				if (1 == bInspectSingleSearch && 1 == bInspectSearchCandidate)
				{
					if (TRUE ==ExcuteManualSearch_EX(2,vnPosIndex,vbResult))
					{
						bSearchSucceed = TRUE;
					}
				} 
				else
				{
					if (TRUE ==ExcuteManualSearch(2,vnPosIndex))
					{
						bSearchSucceed = TRUE;
					}
				}

				if (optionInfo.m_bSetVisionManualSearchToPLC)
				{
					// 通知PLC，视觉关闭手动搜索框
					//SetPlcManualSearch(FALSE);
					SetPlcManualSearch_EX(FALSE);
				}


				if (TRUE == bSearchSucceed)
				{
					if ( 2 == vbBendTwoModelSearchTool.size())
					{
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

						m_pAlignerTool->SetBendAlignProcess(FALSE);
						//bBendInspect = BendingAlignProcessInfo(strBendInsError);
						bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
						m_pAlignerTool->SetBendAlignProcess(TRUE);

						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingProcessInfo();

						m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
					}

					m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
					m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
					m_cVisionAlignGui.SetPosObjectToTargetResultLine(vnPosIndex);

					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
					}

					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);

					// 存储csv
					SaveResultToCsv(m_strProductID, m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						if (2 == vbBendCircleSearchTool.size())
						{
							vdDistance.clear();
							vdDistance.push_back(vdCircleRadius.at(2));
							vdDistance.push_back(vdCircleRadius.at(5));
						}
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}
					if (bSearchSucceed)
					{
						// 打包回复PLC成功
						ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

						m_bStatusBar = TRUE;
						m_strStatusBarInfo1 = _T("检测成功");
						m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();

					} 
					else
					{
						// 打包回复PLC成功
						ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);

						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = _T("检测失败");
						m_strStatusBarInfo2 = strBendInsError;
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();
					}
					//// 打包回复PLC成功
					//ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

					/*m_bStatusBar = TRUE;
					m_strStatusBarInfo1 = _T("检测成功");
					m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();*/

					return TRUE;

				}
				else
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
					}


					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);


					// 打包回复PLC NG
					ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
					// 存储csv
					SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

			}
		}

		if (TRUE == bSearchSucceed)
		{
			//if ( 2 == vbBendTwoModelSearchTool.size())

			if(TRUE ==pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableMoveSnap)
			{

				if(pCurProductData->m_pAlignerParam->GetBendInfo().m_nPosIndexFisrt !=vnPosIndex[0] && 1 == vbBendTwoModelSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					bBendInspect = BendingAlignProcessInfo(strBendInsError);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

			}
			else
			{
				if ( 2 == vbBendTwoModelSearchTool.size())
				{

					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

				if ( 2 == vbBendCornerSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);

					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

					//添加 2022/4/1  
					//m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

				// 折弯弧度检测
				if ( 2 == vbBendCircleSearchTool.size() )
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
							m_vSearchResult.push_back(m_StrText);
						}
					}

					//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);

					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				}
				else if ( 1 == vbBendCircleSearchTool.size()) //单侧检测模式
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
							m_vSearchResult.push_back(m_StrText);
						}
					}

					//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
					std::vector<double> vdTempCircleRadius;
					for (int j = 0; j < 2; j++)
					{
						for (int i = 0; i < vdCircleRadius.size(); i++)
						{
							vdTempCircleRadius.push_back(vdCircleRadius[i]);
						}
					}
					vdCircleRadius = vdTempCircleRadius;
					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdTempCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				}

				// 检测XY距离检测
				if ( 1 == vbBendCircleSearchToolXY.size())
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
							m_vSearchResult.push_back(m_StrText);
						}
					}

					//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
					std::vector<double> vdCircleRadiusXY;
					for (int j = 0; j < 2; j++)
					{
						for (int i = 0; i < vdCircleRadius.size(); i++)
						{
							vdCircleRadiusXY.push_back(vdCircleRadius[i]);
						}
					}

					vdCircleRadius = vdCircleRadiusXY;
					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				}
				// 检测XY距离检测
				if ( 2 == vbBendCircleSearchToolXY.size() )
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				}

			}



			if (1 == bInspectSingleSearch && 1 == bInspectSearchCandidate)
			{
				m_cVisionAlignGui.SetPosSearchResult_TX(vnPosIndex, vbResultIndex,eInspectSearch);
			} 
			else
			{
				m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			}


			if (2 == vbBendCircleSearchTool.size() || 1 == vbBendCircleSearchTool.size())
			{
				vdDistance = vdCircleRadius;
			} 
			else if (1 == vbBendCircleSearchToolXY.size() || 2 == vbBendCircleSearchToolXY.size())
			{
				vdDistance = vdCircleRadius;
			}
			else
			{
				// 获取LX LY RX RY
				BendInspectResultInfo BendInspectResult;
				GetBendInspectResult(BendInspectResult);
				vdDistance.push_back( BendInspectResult.m_dLx);
				vdDistance.push_back( BendInspectResult.m_dLy);
				vdDistance.push_back( BendInspectResult.m_dRx);
				vdDistance.push_back( BendInspectResult.m_dRy);
			}

			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
			m_cVisionAlignGui.SetPosObjectToTargetResultLine(vnPosIndex);

			// 检测结果判定
			if(bSearchSucceed)
			{
				//ReportPLCFunc(eCodeOK);
				ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = _T("检测成功");
				m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}
			else
			{
				//ReportPLCFunc(eCodeNG);
				ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("检测失败");
				m_strStatusBarInfo2 = strBendInsError;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}


			// 存储csv
			SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				if (2 == vbBendCircleSearchTool.size() || 1 == vbBendCircleSearchTool.size())
				{
					vdDistance.clear();
					vdDistance.push_back(vdCircleRadius.at(2));
					vdDistance.push_back(vdCircleRadius.at(5));
				}
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}


			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				//Sleep(10);
				ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
			}

			m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
			SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			return TRUE;

		}
		else
		{

			// 折弯弧度检测
			if ( 2 == vbBendCircleSearchTool.size() )
			{

				//添加变量用于前端GUI显示
				//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
				for (int i = 0; i < 2; i++)
				{
					m_bResult.push_back(1);
					m_StrText.Format(_T("SN:%s"),m_strProductID);
					//vSearchResult.push_back(m_StrText);
					m_vSearchResult.push_back(m_StrText);
				}
				if (m_strProductSTATION != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
						m_vSearchResult.push_back(m_StrText);
					}
				}
				if (strInspectStation != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
						m_vSearchResult.push_back(m_StrText);
					}
				}

				bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
				//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
				bSearchSucceed = bBendInspect;
				PostMessageUpdateBendingCircleInfo();
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				//VisionInspectBendRecord(FALSE, FALSE, TRUE);
				//m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			}
			else if( 1 == vbBendCircleSearchTool.size() )
			{

				//添加变量用于前端GUI显示
				//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
				for (int i = 0; i < 2; i++)
				{
					m_bResult.push_back(1);
					m_StrText.Format(_T("SN:%s"),m_strProductID);
					//vSearchResult.push_back(m_StrText);
					m_vSearchResult.push_back(m_StrText);
				}
				if (m_strProductSTATION != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName1,m_strProductSTATION);
						m_vSearchResult.push_back(m_StrText);
					}
				}
				if (strInspectStation != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("%s:%s"),pCurProductData->m_pAlignerParam->GetBendInfo().m_strPlatformName2,strInspectStation);
						m_vSearchResult.push_back(m_StrText);
					}
				}

				std::vector<double> vdTempCircleRadius;
				for (int j = 0; j < 2; j++)
				{
					for (int i = 0; i < vdCircleRadius.size(); i++)
					{
						vdTempCircleRadius.push_back(vdCircleRadius[i]);
					}
				}
				vdCircleRadius = vdTempCircleRadius;

				bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
				//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
				bSearchSucceed = bBendInspect;
				PostMessageUpdateBendingCircleInfo();
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				//VisionInspectBendRecord(FALSE, FALSE, TRUE);
				//m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			}


			if (2 == vbBendCircleSearchTool.size() || 1 == vbBendCircleSearchTool.size())
			{
				vdDistance = vdCircleRadius;
			} 
			else
			{
				// 获取LX LY RX RY
				//BendInspectResultInfo BendInspectResult;
				//GetBendInspectResult(BendInspectResult);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
			}
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);


			//// 获取LX LY RX RY
			//BendInspectResultInfo BendInspectResult;
			//GetBendInspectResult(BendInspectResult);
			//vdDistance.push_back( BendInspectResult.m_dLx);
			//vdDistance.push_back( BendInspectResult.m_dLy);
			//vdDistance.push_back( BendInspectResult.m_dRx);
			//vdDistance.push_back( BendInspectResult.m_dRy);



			// 存储csv


			ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);

			SaveResultToCsv(m_strProductID,m_strProductSTATION, strInspectStation,bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				if (2 == vbBendCircleSearchTool.size() || 1 == vbBendCircleSearchTool.size())
				{
					vdDistance.clear();
					vdDistance.push_back(vdCircleRadius.at(2));
					vdDistance.push_back(vdCircleRadius.at(5));
				}
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}

			//ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = strErrorInfo;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
			}
			m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
			SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			return FALSE;
		}

	}


	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);


	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
	}
	m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
	SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

	// 使用ACF检测工具、ACF检测工具EX和区域点线工具时，要求平台选项界面必须不启用线距检测，否则在检测成功和平台选项界面启用发送线距结果时，会因为获取距离结果错误造成软件崩溃
	// 只有使用角定位检查测量工具时，才能在平台选项界面启用线距检测和发送线距结果

	// 使用区域点线工具测量点距时，无论检测结果整体成功/失败，均将距离发送到PLC备用轴位置寄存器(距离1/2/3/.../n)，无协议也发送
	// 使用ACF检测工具EX时，如果平台选项界面启用发送子结果，则无论检测结果整体成功/失败，均将子结果发送到PLC备用寄存器（数量+子结果1/2/3/.../n），无协议不发送
	// 使用角定位检查测量工具测量线距时，要求平台选项界面必须启用线距检测，在同时满足检测成功和平台选项界面启用发送线距结果时，才将距离发送到PLC备用轴位置寄存器(距离1/2/12结果/距离3/4/34结果)，无协议不发送

	SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, FALSE);
	//存储CSV固定路径
	if (m_pSystempOptionConfig->m_bAllSteoage)
	{
		SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, FALSE);
	}


	if (TRUE == bSearchSucceed)
	{
		//ReportPLCFunc(eCodeOK);
		ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = _T("检测成功");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = strBendInsError;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	return TRUE;
}

//检测工位用的 使用扩展指令 候补方式
BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFCHB_Ex2(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex)
{
	// vnPatIndex表示模板组合，大小等于5，数组中从0到4分别代表标准0和候补1/2/3/4模板，哪位置1则哪个模板选中
	// 在处理多模板vnPatIndex时，与定位搜索时的候补策略不同（先从第一个开始搜索，当前失败后转下一个，成功后不再继续），本函数要求选中的所有模板都成功才算成功，只要有一个失败就算失败，相当于与操作
	// “是否候补搜索”和“指定候补模板是否选中”有效，如果不开启则无法对候补模板进行检测，但是标准0模板不受限制
	// “是否成对搜索”无效，与操作默认对选中位置的选中模板都要求检测成功

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");

	CString strCmd = _T("VCHB:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	vector<double> m_vdDetectionResult1;
	for (int ii=0;ii<optionInfo.m_nNumberOfInspectResults;ii++)
	{
		m_vdDetectionResult1.push_back(0);
	}
	
	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	if (vnPatIndex.size() != 5)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

		return FALSE;
	}

	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	for(int i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

			return FALSE;
		}    	
	}


	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}
	
	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nStartPatIndex = 0;
		for(int it=0;it<vnPatIndex.size();it++)
		{
			if(vnPatIndex.at(it)==1)
			{
				nStartPatIndex = it;
				break;
			}
		}

		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate*/ && bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	
			int i = 0;
			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}
			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;
	for(int i = 0; i < nPosNum; i++)
	{
		vsmSearchMode.push_back(eInspectSearch);
	}


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);

	m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eInspectSearch);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 开始");
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 结束");
	ReportPLCFunc(eCodeSnapOK);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照回复PLC成功 结束");

	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();

	BOOL bSearchSucceed = TRUE;
	std::vector<BOOL> vbSearchSucceed;
	std::vector<int> vnSearchIndex;

	CString strErrorInfo;	// 检测工具失败提示
	CString strPatNum;   // 模板序号
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 开始");

	if (bInspectSearchCandidate == FALSE)
	{
		// 如果模板参数选中中候补1/2/3/4模板，但是界面设置中没有选中，则认为检测失败
		//for (int j=1; j<5; j++)
		//{
		//	if (vnPatIndex.at(j) == 1)
		//	{
		//		bSearchSucceed = FALSE;
		//		strErrorInfo = "检测模板候选搜索未开. ";
		//		break;
		//	}
		//}

		// 如果不启用候补模板，则只能检测标准0模板，直接跳过候补1/2/3/4模板
		if (vnPatIndex.at(0) == 1)
		{
			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = 0;
				m_vnInspectSearchEndPatIndex[nPosIndex] = 0;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strErrorInfo += m_StrSearchErrorInfo + "模板搜索失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(0);
		}
	}
	else
	{
		for (int j=0; j<5; j++)
		{
			// 如果模板参数没有选中该模板，则跳过
			if (vnPatIndex.at(j) == 0)
			{
				continue;
			}

			// 如果候补1/2/3/4模板在界面设置中没有被选中，则认为检测失败
			if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
			{
				bSearchSucceed = FALSE;
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += "检测模板" + strPatNum + "未开启. ";
				break;
			}

			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = j;
				m_vnInspectSearchEndPatIndex[nPosIndex] = j;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += m_StrSearchErrorInfo + "模板" + strPatNum + "失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(j);
		}
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 结束");


	std::vector<double> vdDistance;
	std::vector<BOOL> vbAreaInspectSubResult;

	std::vector<double> vdCircleRadius;


	std::vector<BOOL> vbBendTwoModelSearchTool;
	std::vector<BOOL> vbBendCircleSearchTool;
	std::vector<BOOL> vbBendCircleSearchToolXY;

	//double dDetectionResult[6] = {0, 0, 0, 0,0,0};	//初始化检测结果

	std::vector<BOOL> vbBendCornerSearchTool;

	// FALSE包含两种情况：1）候补功能未启用、但是指令参数选中候补模板，2）候补功能打开、指令参数选中候补模板、但是界面上候补模板未选中
	if (TRUE == bSearchSucceed)
	{
		// 等于0包含两种情况：1）候补功能未启用、并且指令参数没有选中标准0模板，2）候补功能打开、界面上候补模板选中、但是参数一个都没有选中
		if (vbSearchSucceed.size() == 0)
		{
			bSearchSucceed = FALSE;
		}
		else
		{
			// 对所有参与检测的模板进行遍历
			for (int j=0; j<vbSearchSucceed.size(); j++)
			{
				// 将当前检测结果与总结果进行与操作
				bSearchSucceed &= vbSearchSucceed.at(j);

				// 获取当前检测结果对应模板索引
				int nPatIndex = vnSearchIndex.at(j);

				// 遍历所有位置
				for (int i = 0; i<nPosNum; i++)
				{
					// 获取当前位置
					nPosIndex = vnPosIndex[i];

					// 获取指定位置、指定候补模板的定位工具
					CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, nPatIndex);

					if (pSearchTool != NULL)
					{
						CSearchToolType eType = pSearchTool->GetSearchToolType();
						switch (eType)
						{
						case ePatternPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}
								else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
								{
									
									
								}else
								{
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eQuickPosLineDistanceTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								if (result.m_resultPosLineDistanceTool.m_nResult0 == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdDistance.push_back(0.0);
									
								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM0);
									
								}

								if (result.m_resultPosLineDistanceTool.m_nResult1 == 0)
								{
									// 等于0：如果找线工具1失败，也将数值0传出
									vdDistance.push_back(0.0);
								}else if(result.m_resultPosLineDistanceTool.m_nResult1 == -2)//不启用找线工具2，
								{
								}
								else
								{
									// 不等于0：如果找线工具1成功，无论检测结果成功或失败，均将实际结果传出
									vdDistance.push_back(result.m_resultPosLineDistanceTool.m_dDistanceMM1);
								}
							}
							break;
						case eAreaInspectTool:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);
							}
							break;
						case eAreaInspectToolEx:
							{
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的子检测结果发送PLC时，先发送第一个模板的第一个位置的所有子结果，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
								for (int n=0; n<result.m_resultAreaInspectToolEx.m_nValidResultNum; n++)
								{
									BOOL bTemp = result.m_resultAreaInspectToolEx.m_vnResult.at(n) == 1;
									vbAreaInspectSubResult.push_back(bTemp);
								}
							}
							break;

						case eCornerSearchCheckInspectTool:
							{
							   vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								CResultL2LInspectDistance* tmpL2LReslut=&result.m_ResultL2LInspectDistance;
								//获取找线结果
								for(int lineindex=0;lineindex<3;lineindex++)
								{
									if(tmpL2LReslut->m_nResult[lineindex]!=0)//找线OK
									{
										//vdDistance.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
										vdCircleRadius.push_back((tmpL2LReslut->m_dDistanceMM[lineindex]));
									}
									else
									{
										//vdDistance.push_back(0);
										vdCircleRadius.push_back(0);
										
									}
								}

							}
							break;
						case eCircleCornerSearchCheckInspectTool:
							{
								vbBendCircleSearchToolXY.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								// 多模板的多位置的距离结果发送PLC时，先发送第一个模板的第一个位置的所有距离，把第一个模板的所有位置发送完后，再发送第二个模板的所有位置，以此类推
										
								if (result.m_CircleCornerDistance.m_vDistMM[0] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[0]);

								}


								if (result.m_CircleCornerDistance.m_vDistMM[1] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vDistMM[1]);

								}

								if (result.m_CircleCornerDistance.m_vSizeMM[0] == 0)
								{
									// 等于0：如果找线工具0失败，也将数值0传出
									vdCircleRadius.push_back(0.0);

								}
								else
								{
									// 不等于0：如果找线工具0成功，无论检测结果成功或失败，均将实际结果传出
									vdCircleRadius.push_back(result.m_CircleCornerDistance.m_vSizeMM[0]);

								}

								

							}
							break;

						case eACFInspectTool:
							{

							}
							break;

							// 两几何定位工具
						case eTwoQuickModelSearchTool:
						case eTwoPatModelSearchTool:
							{
								vbBendTwoModelSearchTool.push_back(TRUE);

								CSearchResult result;
								pSearchTool->GetResult(0, result);

								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(0), result.m_vdAuxiliaryPosY.at(0), 0));
								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(1), result.m_vdAuxiliaryPosY.at(1), 0));

								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}

							}
							break;
						case ePatternCornerSearchTool:
						case eCornerSearchTool:
							{
								vbBendCornerSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));
							
								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));




								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);

								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}
							}
							break;
						case eCircleSearchTool:
							{
								//vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
								vdCircleRadius.push_back(dCircleRadius);
							}
							break;

						case ePatternCircleSearchTool:
							{
								//vbBendCircleSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								double dCircleRadius = result.m_resultCircleSearchTool.m_dRadius;
								vdCircleRadius.push_back(dCircleRadius);
							}
							break;

						case eCombinationCornerSearchTool:
							{
								vbBendCornerSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(1, result);

								/*CSearchResult resultt;
								pSearchTool->GetResult(1, result);*/

								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_vdAuxiliaryPosX.at(3), result.m_vdAuxiliaryPosY.at(3), 0));
								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
	} 



	//检测读码
	CString strProductID = _T("");
	//检测工位
	CString strProductSTATION = _T("");
	//本压工位
	CString strInspectStation = _T("");

	CString strStartIime = _T("");
	CString strEndIime = _T("");
	CString strProsessingIime = _T("");
	if ((pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID)|| m_XYDCommProtocol.GetIsEnableIDRead())
	{
		if (!CommunicateGetAlignProcessProductID(strProductID))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品ID读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
	{
		if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品工位读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	m_BendInspectResultInfo.m_strProductID = strProductID;
	m_strProductID = strProductID;
	m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strInspectStation = strInspectStation;

	CString strBendInsError = _T("");
	BOOL bBendInspect = FALSE;

	std::vector<BOOL> m_bResult;
	std::vector<CString> m_vSearchResult;
	CString	 m_StrText;

	CMarkImagePos       m_mpInsectTargetMarkImagePos;
	m_mpInsectTargetMarkImagePos = GetTargetMarkSearchResults();
	
	//这里去掉，折弯检测用不了手动弹窗
	if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		// 手动弹窗功能，只针对折弯检测使用的工具，其它定位工具不支持
		if (FALSE)
		{
			if (optionInfo.m_bManualAfObjSearchFail && ((2 == vbBendTwoModelSearchTool.size()) || (2 == vbBendCornerSearchTool.size())))
				//if (optionInfo.m_bManualAfObjSearchFail)
			{
				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉弹出手动搜索框
				// 	SetPlcManualSearch(TRUE);
				// }

				if (FALSE ==ExcuteManualSearch(2,vnPosIndex))
				{
					if (optionInfo.m_bSetVisionManualSearchToPLC)
					{
						// 通知PLC，视觉关闭手动搜索框
						//SetPlcManualSearch(FALSE);
					}
				}
				else
				{
					bSearchSucceed = TRUE;
				}

				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉关闭手动搜索框
				// 	SetPlcManualSearch(FALSE);
				// }

				if (TRUE == bSearchSucceed)
				{
					if ( 2 == vbBendTwoModelSearchTool.size())
					{
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

						m_pAlignerTool->SetBendAlignProcess(FALSE);
						bBendInspect = BendingAlignProcessInfo(strBendInsError);
						m_pAlignerTool->SetBendAlignProcess(TRUE);

						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingProcessInfo();

						//VisionInspectBendRecord(FALSE, TRUE, FALSE);

					}

					if ( 2 == vbBendCircleSearchTool.size() )
					{
						bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingCircleInfo();

						//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					}


					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}

					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);

					// 存储csv
					SaveResultToCsv(m_strProductID, m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						if (2 == vbBendCircleSearchTool.size())
						{
							vdDistance.clear();
							vdDistance.push_back(vdCircleRadius.at(2));
							vdDistance.push_back(vdCircleRadius.at(5));
						}
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}
					


					// 打包回复PLC成功
					ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

					m_bStatusBar = TRUE;
					m_strStatusBarInfo1 = _T("检测成功");
					m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return TRUE;

				}
				else
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}


					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);
					
					
					// 打包回复PLC NG
					ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
					// 存储csv
					SaveResultToCsv(m_strProductID,m_strProductSTATION, strInspectStation,bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

			}
		}


		if (TRUE == bSearchSucceed)
		{
			//if ( 2 == vbBendTwoModelSearchTool.size())

			if(TRUE ==pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableMoveSnap)
			{

				if(pCurProductData->m_pAlignerParam->GetBendInfo().m_nPosIndexFisrt !=vnPosIndex[0] && 1 == vbBendTwoModelSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					bBendInspect = BendingAlignProcessInfo(strBendInsError);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

			}
			else
			{
				if ( 2 == vbBendTwoModelSearchTool.size())
				{
					
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

				if ( 2 == vbBendCornerSearchTool.size())
				{

					

					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
					
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

					//添加 2022/4/1  
					//m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();
					
					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

				// 折弯弧度检测
				if ( 2 == vbBendCircleSearchTool.size() )
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("工位:%s"),m_strProductSTATION);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("检测平台:%s"),strInspectStation);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}

					//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);

				    bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				}

				// 检测XY距离检测
				if ( 1 == vbBendCircleSearchToolXY.size() )
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("工位:%s"),m_strProductSTATION);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("检测平台:%s"),strInspectStation);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}

					//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
					std::vector<double> vdCircleRadiusXY;
					for (int j = 0; j < 2; j++)
					{
						for (int i = 0; i < vdCircleRadius.size(); i++)
						{
							vdCircleRadiusXY.push_back(vdCircleRadius[i]);
						}
					}
					
					vdCircleRadius = vdCircleRadiusXY;
					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				}
				// 检测XY距离检测
				if ( 2 == vbBendCircleSearchToolXY.size() )
				{
					//添加变量用于前端GUI显示
					//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("SN:%s"),m_strProductID);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
					if (m_strProductSTATION != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("工位:%s"),m_strProductSTATION);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					if (strInspectStation != "")
					{
						for (int i = 0; i < 2; i++)
						{
							m_bResult.push_back(1);
							m_StrText.Format(_T("检测平台:%s"),strInspectStation);
							//vSearchResult.push_back(m_StrText);
							m_vSearchResult.push_back(m_StrText);
						}
					}
					bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingCircleInfo();

					//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

				}
			
			}


			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			
			

			if (2 == vbBendCircleSearchTool.size())
			{
				vdDistance = vdCircleRadius;
			} 
			else if (1 == vbBendCircleSearchToolXY.size() || 2 == vbBendCircleSearchToolXY.size())
			{
				vdDistance = vdCircleRadius;
			}
			else
			{
				// 获取LX LY RX RY
				BendInspectResultInfo BendInspectResult;
				GetBendInspectResult(BendInspectResult);
				vdDistance.push_back( BendInspectResult.m_dLx);
				vdDistance.push_back( BendInspectResult.m_dLy);
				vdDistance.push_back( BendInspectResult.m_dRx);
				vdDistance.push_back( BendInspectResult.m_dRy);
			}

			
			
			// 检测结果判定
			if(bSearchSucceed)
			{
				//ReportPLCFunc(eCodeOK);
				ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = _T("检测成功");
				m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}
			else
			{
				//ReportPLCFunc(eCodeNG);
				ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("检测失败");
				m_strStatusBarInfo2 = strBendInsError;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}


			// 存储csv
			SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				if (2 == vbBendCircleSearchTool.size())
				{
					vdDistance.clear();
					vdDistance.push_back(vdCircleRadius.at(2));
					vdDistance.push_back(vdCircleRadius.at(5));
				}
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}


			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				//Sleep(10);
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}

			m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
			SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			return TRUE;

		}
		else
		{
			


			// 折弯弧度检测
			if ( 2 == vbBendCircleSearchTool.size() )
			{

				//添加变量用于前端GUI显示
				//std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult
				for (int i = 0; i < 2; i++)
				{
					m_bResult.push_back(1);
					m_StrText.Format(_T("SN:%s"),m_strProductID);
					//vSearchResult.push_back(m_StrText);
					m_vSearchResult.push_back(m_StrText);
				}
				if (m_strProductSTATION != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("工位:%s"),m_strProductSTATION);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
				}
				if (strInspectStation != "")
				{
					for (int i = 0; i < 2; i++)
					{
						m_bResult.push_back(1);
						m_StrText.Format(_T("检测平台:%s"),strInspectStation);
						//vSearchResult.push_back(m_StrText);
						m_vSearchResult.push_back(m_StrText);
					}
				}

				bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius,m_bResult,m_vSearchResult);
				//bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
				bSearchSucceed = bBendInspect;
				PostMessageUpdateBendingCircleInfo();
				m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
				//VisionInspectBendRecord(FALSE, FALSE, TRUE);
				//m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			}


			if (2 == vbBendCircleSearchTool.size())
			{
				vdDistance = vdCircleRadius;
			} 
			else
			{
				// 获取LX LY RX RY
				//BendInspectResultInfo BendInspectResult;
				//GetBendInspectResult(BendInspectResult);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
			}
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			

			//// 获取LX LY RX RY
			//BendInspectResultInfo BendInspectResult;
			//GetBendInspectResult(BendInspectResult);
			//vdDistance.push_back( BendInspectResult.m_dLx);
			//vdDistance.push_back( BendInspectResult.m_dLy);
			//vdDistance.push_back( BendInspectResult.m_dRx);
			//vdDistance.push_back( BendInspectResult.m_dRy);

			

			// 存储csv
			

			ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);

			SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				if (2 == vbBendCircleSearchTool.size())
				{
					vdDistance.clear();
					vdDistance.push_back(vdCircleRadius.at(2));
				    vdDistance.push_back(vdCircleRadius.at(5));
				}
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}

			//ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = strErrorInfo;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			
			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}
			m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
			SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			return FALSE;
		}

	}
	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch);
	}
	m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
	SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

	// 使用ACF检测工具、ACF检测工具EX和区域点线工具时，要求平台选项界面必须不启用线距检测，否则在检测成功和平台选项界面启用发送线距结果时，会因为获取距离结果错误造成软件崩溃
	// 只有使用角定位检查测量工具时，才能在平台选项界面启用线距检测和发送线距结果

	// 使用区域点线工具测量点距时，无论检测结果整体成功/失败，均将距离发送到PLC备用轴位置寄存器(距离1/2/3/.../n)，无协议也发送
	// 使用ACF检测工具EX时，如果平台选项界面启用发送子结果，则无论检测结果整体成功/失败，均将子结果发送到PLC备用寄存器（数量+子结果1/2/3/.../n），无协议不发送
	// 使用角定位检查测量工具测量线距时，要求平台选项界面必须启用线距检测，在同时满足检测成功和平台选项界面启用发送线距结果时，才将距离发送到PLC备用轴位置寄存器(距离1/2/12结果/距离3/4/34结果)，无协议不发送

	SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, FALSE);
	//存储CSV固定路径
	if (m_pSystempOptionConfig->m_bAllSteoage)
	{
		SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, FALSE);
	}
	

	if (TRUE == bSearchSucceed)
	{
		//ReportPLCFunc(eCodeOK);
		ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = _T("检测成功");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = strBendInsError;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	return TRUE;
}

//检测工位用的 使用扩展指令 候补方式
BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFBIS_Ex3(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex,int nTarFixOffsetIndex /*= 0*/)
{
	// vnPatIndex表示模板组合，大小等于5，数组中从0到4分别代表标准0和候补1/2/3/4模板，哪位置1则哪个模板选中
	// 在处理多模板vnPatIndex时，与定位搜索时的候补策略不同（先从第一个开始搜索，当前失败后转下一个，成功后不再继续），本函数要求选中的所有模板都成功才算成功，只要有一个失败就算失败，相当于与操作
	// “是否候补搜索”和“指定候补模板是否选中”有效，如果不开启则无法对候补模板进行检测，但是标准0模板不受限制
	// “是否成对搜索”无效，与操作默认对选中位置的选中模板都要求检测成功
	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");

	CString strCmd = _T("VBIS:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	vector<double> m_vdDetectionResult1;
	for (int ii=0;ii<optionInfo.m_nNumberOfInspectResults;ii++)
	{
		m_vdDetectionResult1.push_back(0);
	}

	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	if (vnPatIndex.size() != 5)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

		return FALSE;
	}

	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	for(int i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

			return FALSE;
		}    	
	}


	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}
	//检测读码
	CString strProductID = _T("");
	//检测工位
	CString strProductSTATION = _T("");
	//本压工位
	CString strInspectStation = _T("");
	CString strStartIime = _T("");
	CString strEndIime = _T("");
	CString strProsessingIime = _T("");
	if ( (pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID) || m_XYDCommProtocol.GetIsEnableIDRead())
	{
		if (!CommunicateGetAlignProcessProductID(strProductID))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品ID读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
	{
		if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品工位读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	m_BendInspectResultInfo.m_strProductID = strProductID;
	m_strProductID = strProductID;
	m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strInspectStation = strInspectStation;

	//进行同时拍照(拍目标或者对象模板),仅支持2位置配置的同时检测
	if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableFbisTarObc
		&& vnPosIndex.size() == 2 && GetPosNum() == 2)
	{
		if (nTarFixOffsetIndex)//拍目标
		{
			//目标光源切换
			if (optionInfo.m_bEnablePhotoLightCtrl)
			{
				if (!OnLightOnOff(1))
				{
					ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("拍照失败：");
					m_strStatusBarInfo2 = _T("光源切换失败！");
					SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					return FALSE;
				}	
			}

			// 目标拍照
			m_bTargetSearchTest = FALSE;
			{
				BOOL bSearchTarget = FALSE;
				if (optionInfo.m_bEnableMultiMarkMode)//如果启用多标记点功能，默认只搜目标搜后一半的第一个位置。
				{
					std::vector<int> vnPosIndex2;
					vnPosIndex2.push_back(0);
					bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex2,0);
				}
				else
				{
					bSearchTarget = OnTargetSearchCandidateExFTGC_Ex(vnPosIndex,0);
				}
				if (FALSE == bSearchTarget)
				{
					ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1)); 
				}
			}
		}
		else//拍对象
		{
			// 对象拍照光源切换
			if (optionInfo.m_bEnablePhotoLightCtrl)
			{
				if (!OnLightOnOff(0))
				{
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("拍照失败：");
					m_strStatusBarInfo2 = _T("光源切换失败！");
					SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
					return FALSE;
				}	
			}

			// 对象拍照
			m_bObjectSearchTest = FALSE;
			{
				BOOL bSearchObject = FALSE;
				if (optionInfo.m_bEnableMultiMarkMode && !optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)//如果启用多标记点功能，
				{
					std::vector<int> vnPosIndex2;
					vnPosIndex2.push_back(0);
					bSearchObject = OnObjectSearchCandidateExFOBC_Ex(vnPosIndex2,0);
				}
				else
				{
					bSearchObject =OnObjectSearchCandidateExFOBC_Ex(vnPosIndex,0);	 
				}
				if (FALSE == bSearchObject)
				{
					ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
					return FALSE;     			
				}
			}
		}
	}

	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nStartPatIndex = 0;
		for(int it=0;it<vnPatIndex.size();it++)
		{
			if(vnPatIndex.at(it)==1)
			{
				nStartPatIndex = it;
				break;
			}
		}

		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate*/ && bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	
			int i = 0;
			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}
			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}

	

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;
	for(int i = 0; i < nPosNum; i++)
	{
		vsmSearchMode.push_back(eInspectSearch);
	}


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);

	//m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eInspectSearch);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 开始");
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 结束");


	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();
	BOOL bInspectSingleSearch = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectSingleSearchEnable();

	BOOL bSearchSucceed = TRUE;
	std::vector<BOOL> vbSearchSucceed;
	std::vector<int> vnSearchIndex;

	std::vector<BOOL> vnPosSearchSucceed;
	CString strErrorInfo;	// 检测工具失败提示
	CString strPatNum;   // 模板序号
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 开始");

	if (bInspectSearchCandidate == FALSE)
	{
		// 如果模板参数选中中候补1/2/3/4模板，但是界面设置中没有选中，则认为检测失败
		//for (int j=1; j<5; j++)  //注释后组合模板参数不进行报警
		//{
		//	if (vnPatIndex.at(j) == 1)
		//	{
		//		bSearchSucceed = FALSE;
		//		strErrorInfo = "检测模板候选搜索未开. ";
		//		break;
		//	}
		//}

		// 如果不启用候补模板，则只能检测标准0模板，直接跳过候补1/2/3/4模板
		if (vnPatIndex.at(0) == 1)
		{
			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = 0;
				m_vnInspectSearchEndPatIndex[nPosIndex] = 0;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strErrorInfo += m_StrSearchErrorInfo + "模板搜索失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(0);
		}
	}
	else
	{
		for (int j=0; j<5; j++)
		{
			// 如果模板参数没有选中该模板，则跳过
			if (vnPatIndex.at(j) == 0)
			{
				continue;
			}

			// 如果候补1/2/3/4模板在界面设置中没有被选中，则认为检测失败
			if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
			{
				bSearchSucceed = FALSE;
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += "检测模板" + strPatNum + "未开启. ";
				break;
			}

			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = j;
				m_vnInspectSearchEndPatIndex[nPosIndex] = j;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			for (int m = 0; m<nPosNum; m++)
			{
				vnPosSearchSucceed.push_back(m_vbSearchSucceed.at(m));
			}

			if (bTmep == FALSE)
			{
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += m_StrSearchErrorInfo + "模板" + strPatNum + "失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(j);

			if (bTmep == TRUE)
			{
				break;
			}
		}
	}

	std::vector<BOOL> vbResult;
	std::vector<int> vbResultIndex;
	BOOL vbResult1;
	int vbResultIndex1;
	//判定哪个模板成功
	if ( 1 == bInspectSingleSearch && bInspectSearchCandidate == TRUE)
	{
		if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
		{
			for (int i = 0;i<nPosNum;i++)
			{
				for (int j= 0; j<vbSearchSucceed.size();j++)
				{
					if (1 == vnPosSearchSucceed.at(nPosNum*j + i))
					{
						vbResult1 = TRUE;
						vbResultIndex1 = j;
						break;
					}
					else
					{
						vbResult1 = FALSE;
						vbResultIndex1 = 0;
					}
				}

				vbResult.push_back(vbResult1);
				vbResultIndex.push_back(vbResultIndex1);
			}
		}
	}
	else if (1 != bInspectSingleSearch && 1 == bInspectSearchCandidate)
	{
		if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
		{
			if (vnSearchIndex.size() > 1)
			{
				int bSearchSucceed = vbSearchSucceed[vnSearchIndex.size()-1];
				int nSearchIndex = vnSearchIndex[vnSearchIndex.size()-1];
				for (int i = 0;i < vnSearchIndex.size();++i)
				{
					if (vbSearchSucceed[i])
					{
						bSearchSucceed = vbSearchSucceed[i];
						nSearchIndex = vnSearchIndex[i];
						break;
					}
				}
				vbSearchSucceed.clear(); vbSearchSucceed.push_back(bSearchSucceed);
				vnSearchIndex.clear(); vnSearchIndex.push_back(nSearchIndex);
			}
		}
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 结束");


	std::vector<double> vdDistance;
	std::vector<BOOL> vbAreaInspectSubResult;

	std::vector<double> vdCircleRadius;


	std::vector<BOOL> vbBendTwoModelSearchTool;
	std::vector<BOOL> vbBendCircleSearchTool;
	std::vector<BOOL> vbBendCircleSearchToolXY;

	//double dDetectionResult[6] = {0, 0, 0, 0,0,0};	//初始化检测结果

	std::vector<BOOL> vbBendCornerSearchTool;

	// FALSE包含两种情况：1）候补功能未启用、但是指令参数选中候补模板，2）候补功能打开、指令参数选中候补模板、但是界面上候补模板未选中
	if (TRUE == bSearchSucceed)
	{
		if (1 == bInspectSingleSearch && bInspectSearchCandidate == TRUE)
		{
			if (vbSearchSucceed.size() == 0)
			{
				bSearchSucceed = FALSE;
			}
			else
			{
				// 遍历所有位置
				for (int i = 0; i<nPosNum; i++)
				{
					// 获取当前位置
					nPosIndex = vnPosIndex[i];

					bSearchSucceed &= vbResult.at(i);
					CBaseSearchTool* pSearchTool; 

					if (TRUE == vbResult.at(i))
					{
						// 获取指定位置、指定候补模板的定位工具
						pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, vbResultIndex.at(i));
					}
					else
					{
						// 获取指定位置、指定候补模板的定位工具
						pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, 0);
					}
					//// 获取指定位置、指定候补模板的定位工具
					//CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, vbResultIndex.at(i));

					if (pSearchTool != NULL)
					{
						CSearchToolType eType = pSearchTool->GetSearchToolType();
						switch(eType)
						{
						case ePatternPosLineDistanceTool:
							{
							}
							break;
						default:
							vbBendCornerSearchTool.push_back(TRUE);
							CSearchResult result;
							pSearchTool->GetResult(0, result);

							/*CSearchResult resultt;
							pSearchTool->GetResult(1, result);*/

							if (nTarFixOffsetIndex)
							{
								m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
							} 
							else
							{
								m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
							}



							CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
							SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
							SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
							if (pPlatformAxisPos)
							{
								delete pPlatformAxisPos;
								pPlatformAxisPos = NULL;
							}

							break;

						}

					}
				}
			}
		} 
		else
		{
			// 等于0包含两种情况：1）候补功能未启用、并且指令参数没有选中标准0模板，2）候补功能打开、界面上候补模板选中、但是参数一个都没有选中
			if (vbSearchSucceed.size() == 0)
			{
				bSearchSucceed = FALSE;
			}
			else
			{
				// 对所有参与检测的模板进行遍历
				for (int j=0; j<vbSearchSucceed.size(); j++)
				{
					// 将当前检测结果与总结果进行与操作
					bSearchSucceed &= vbSearchSucceed.at(j);

					// 获取当前检测结果对应模板索引
					int nPatIndex = vnSearchIndex.at(j);

					// 遍历所有位置
					for (int i = 0; i<nPosNum; i++)
					{
						// 获取当前位置
						nPosIndex = vnPosIndex[i];

						// 获取指定位置、指定候补模板的定位工具
						CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, nPatIndex);

						if (pSearchTool != NULL)
						{
							CSearchToolType eType = pSearchTool->GetSearchToolType();
							switch(eType)
							{
							case ePatternPosLineDistanceTool:
								{


								}
								break;
							default:
								vbBendCornerSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								/*CSearchResult resultt;
								pSearchTool->GetResult(1, result);*/

								if (nTarFixOffsetIndex)
								{
									m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
								} 
								else
								{
									m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
								}



								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}

								break;

							}

						}
					}
				}
			}
		}

	} 


	CString strBendInsError = _T("");
	BOOL bBendInspect = FALSE;

	std::vector<BOOL> m_bResult;
	std::vector<CString> m_vSearchResult;
	CString	 m_StrText;
	CMarkImagePos       m_mpInsectTargetMarkImagePos;
	if (nTarFixOffsetIndex)
	{
		m_mpInsectTargetMarkImagePos = GetObjectMarkSearchResults();
	} 
	else
	{
		m_mpInsectTargetMarkImagePos = GetTargetMarkSearchResults();
	}

	//这里去掉，折弯检测用不了手动弹窗
	if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		m_apAlignerProcesInfo.nTimes = 0;

		// 手动弹窗功能，只针对折弯检测使用的工具，其它定位工具不支持
		if (FALSE == bSearchSucceed/*&&1==bInspectSingleSearch && bInspectSearchCandidate == TRUE*/)
		{
			if (optionInfo.m_bManualAfObjSearchFail && 2 == vbBendCornerSearchTool.size())
			{
				if (optionInfo.m_bSetVisionManualSearchToPLC)
				{
					// 通知PLC，视觉弹出手动搜索框
					SetPlcManualSearch_EX(TRUE);
				}

				if (1==bInspectSingleSearch && bInspectSearchCandidate == TRUE)
				{
					vector<int> vnTempPosIndex;
					for (int i =0;i<vbResult.size();++i)
					{
						if (vbResult[i] == FALSE) vnTempPosIndex.push_back(i);
					}
					if (vnTempPosIndex.size())
					{
						bSearchSucceed = ExcuteManualSearch_TX(nTarFixOffsetIndex,vnTempPosIndex);
					}	
				}
				else
				{
					bSearchSucceed = ExcuteManualSearch_TX(nTarFixOffsetIndex,vnPosIndex);
				}

				if (optionInfo.m_bSetVisionManualSearchToPLC)
				{
					// 通知PLC，视觉关闭手动搜索框
					SetPlcManualSearch_EX(FALSE);
				}

				if (TRUE == bSearchSucceed)
				{
					if ( 2 == vbBendCornerSearchTool.size())
					{
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

						m_pAlignerTool->SetBendAlignProcess(FALSE);
						bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
						m_pAlignerTool->SetBendAlignProcess(TRUE);

						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingProcessInfo();

						//VisionInspectBendRecord(FALSE, TRUE, FALSE);

						m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);
					}

					m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
					m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
					m_cVisionAlignGui.SetPosObjectToTargetResultLine(vnPosIndex);

					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
					}

					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);

					// 存储csv
					SaveResultToCsv(m_strProductID, m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}

					if (bSearchSucceed)
					{
						// 打包回复PLC成功
						ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

						m_bStatusBar = TRUE;
						m_strStatusBarInfo1 = _T("检测成功");
						m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();
					} 
					else
					{
						// 打包回复PLC NG
						ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
						m_bStatusBar = FALSE;
						m_strStatusBarInfo1 = _T("检测失败");
						m_strStatusBarInfo2 = strBendInsError;
						SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
						PostMessageUpdateStatusBarInfo();
					}
					//// 打包回复PLC成功
					//ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

					//m_bStatusBar = TRUE;
					//m_strStatusBarInfo1 = _T("检测成功");
					//m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					if (nTarFixOffsetIndex)
					{
						m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					} 
					else
					{
						m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
					}

					return TRUE;

				}
				else
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
					}


					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);
					// 存储csv
					SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}

					// 打包回复PLC NG
					ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					if (nTarFixOffsetIndex)
					{
						m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					} 
					else
					{
						m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
					}

					return FALSE;
				}

			}
		}

		if (TRUE == bSearchSucceed)
		{
			//if ( 2 == vbBendTwoModelSearchTool.size())

			if(TRUE ==pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableMoveSnap)
			{

				if(pCurProductData->m_pAlignerParam->GetBendInfo().m_nPosIndexFisrt !=vnPosIndex[0] && 1 == vbBendTwoModelSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					bBendInspect = BendingAlignProcessInfo(strBendInsError);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

			}
			else
			{
				if ( 2 == vbBendCornerSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);

					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

					//添加 2022/4/1  
					//m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}
			}


			//m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);

			{
				// 获取LX LY RX RY
				BendInspectResultInfo BendInspectResult;
				GetBendInspectResult(BendInspectResult);
				vdDistance.push_back( BendInspectResult.m_dLx);
				vdDistance.push_back( BendInspectResult.m_dLy);
				vdDistance.push_back( BendInspectResult.m_dRx);
				vdDistance.push_back( BendInspectResult.m_dRy);
			}


			// 存储csv
			SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}
			// 检测结果判定
			if(bSearchSucceed)
			{
				//ReportPLCFunc(eCodeOK);
				ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = _T("检测成功");
				m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}
			else
			{
				//ReportPLCFunc(eCodeNG);
				ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("检测失败");
				m_strStatusBarInfo2 = strBendInsError;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}

			//if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			//{
			//	//Sleep(10);
			//	ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			//}

			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
			m_cVisionAlignGui.SetPosObjectToTargetResultLine(vnPosIndex);

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
			}

			if (nTarFixOffsetIndex)
			{
				m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
			} 
			else
			{
				m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}



			return TRUE;

		}
		else
		{


			{
				// 获取LX LY RX RY
				//BendInspectResultInfo BendInspectResult;
				//GetBendInspectResult(BendInspectResult);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
			}
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);


			//// 获取LX LY RX RY
			//BendInspectResultInfo BendInspectResult;
			//GetBendInspectResult(BendInspectResult);
			//vdDistance.push_back( BendInspectResult.m_dLx);
			//vdDistance.push_back( BendInspectResult.m_dLy);
			//vdDistance.push_back( BendInspectResult.m_dRx);
			//vdDistance.push_back( BendInspectResult.m_dRy);



			// 存储csv
			//SaveResultToCsv(m_strProductID,m_strProductSTATION, bSearchSucceed, vdDistance, TRUE);

			////存储CSV固定路径
			//if (m_pSystempOptionConfig->m_bAllSteoage)
			//{
			//	SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,bSearchSucceed, vdDistance, TRUE);
			//}
			ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
			//ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = strErrorInfo;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
			}

			if (nTarFixOffsetIndex)
			{
				m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
			} 
			else
			{
				m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}

			return FALSE;
		}
	}

	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);

	if (nTarFixOffsetIndex)
	{
		m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
		SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
	} 
	else
	{
		m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
		SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
	}


	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch,bSearchSucceed);
	}


	// 使用ACF检测工具、ACF检测工具EX和区域点线工具时，要求平台选项界面必须不启用线距检测，否则在检测成功和平台选项界面启用发送线距结果时，会因为获取距离结果错误造成软件崩溃
	// 只有使用角定位检查测量工具时，才能在平台选项界面启用线距检测和发送线距结果

	// 使用区域点线工具测量点距时，无论检测结果整体成功/失败，均将距离发送到PLC备用轴位置寄存器(距离1/2/3/.../n)，无协议也发送
	// 使用ACF检测工具EX时，如果平台选项界面启用发送子结果，则无论检测结果整体成功/失败，均将子结果发送到PLC备用寄存器（数量+子结果1/2/3/.../n），无协议不发送
	// 使用角定位检查测量工具测量线距时，要求平台选项界面必须启用线距检测，在同时满足检测成功和平台选项界面启用发送线距结果时，才将距离发送到PLC备用轴位置寄存器(距离1/2/12结果/距离3/4/34结果)，无协议不发送

	SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, FALSE);
	//存储CSV固定路径
	if (m_pSystempOptionConfig->m_bAllSteoage)
	{
		SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, FALSE);
	}

	if (TRUE == bSearchSucceed)
	{
		//ReportPLCFunc(eCodeOK);
		ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = _T("检测成功");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = strBendInsError;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	return TRUE;
}

//检测工位用的 使用扩展指令 候补方式
BOOL vcXYDVisionAlign::OnSerialCameraACFInspectCandidateExFBIS_Ex2(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex,int nTarFixOffsetIndex /*= 0*/)
{
	// vnPatIndex表示模板组合，大小等于5，数组中从0到4分别代表标准0和候补1/2/3/4模板，哪位置1则哪个模板选中
	// 在处理多模板vnPatIndex时，与定位搜索时的候补策略不同（先从第一个开始搜索，当前失败后转下一个，成功后不再继续），本函数要求选中的所有模板都成功才算成功，只要有一个失败就算失败，相当于与操作
	// “是否候补搜索”和“指定候补模板是否选中”有效，如果不开启则无法对候补模板进行检测，但是标准0模板不受限制
	// “是否成对搜索”无效，与操作默认对选中位置的选中模板都要求检测成功

	m_bStatusBar = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
	PostMessageUpdateStatusBarInfo();
	VisionAlignLogRecord("D:\\TimeLog.txt", "start");

	CString strCmd = _T("VBIS:");
	GetCommCommandName(strCmd);
	CString strTempWarningType = _T("");
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	vector<double> m_vdDetectionResult1;
	for (int ii=0;ii<optionInfo.m_nNumberOfInspectResults;ii++)
	{
		m_vdDetectionResult1.push_back(0);
	}
	
	if (!CheckInspectSearchTool(strCmd) || !CheckPlatformInfo(strCmd)/*m_bValidInspectSearchTool!=TRUE || NULL == m_pPlatformInfo*/)
	{	
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	if (vnPatIndex.size() != 5)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

		return FALSE;
	}

	int nPosIndex = 0;		
	int nPosNum = vnPosIndex.size();

	if (nPosNum<=0)
	{
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_POSNUMERROR);
		SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();
		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_PROCESSING);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo2));
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		return FALSE;
	}

	for(int i=0; i<nPosNum; i++)
	{		 
		nPosIndex = vnPosIndex[i];
		if (FALSE == IsPosCameraGrabbing(nPosIndex))
		{      
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_CAMERA_CONNECTION);
			CString strTemp = _T("");
			strTemp.Format(m_strStatusBarInfo2, i + 1);	//zzc报警 相机x链接失败，1,2,3,4
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, strTemp);
			PostMessageUpdateStatusBarInfo();
			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + strTemp));	
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);

			return FALSE;
		}    	
	}


	// 获取当前产品信息
	int nCurProductIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();

	if (nCurProductIndex<0 || nCurProductIndex>=m_pVisionASMConfig->m_TotalProductData.GetCount())
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}


	CProductData* pCurProductData = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProductIndex);
	if (pCurProductData == NULL)
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_CUR_PRODUCT_ALIGN_INFO_FAILED);
		m_strStatusBarInfo2 = _T("");
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();		
		return FALSE;
	}
	//检测读码
	CString strProductID = _T("");
	//检测工位
	CString strProductSTATION = _T("");
	//本压工位
	CString strInspectStation = _T("");
	CString strStartIime = _T("");
	CString strEndIime = _T("");
	CString strProsessingIime = _T("");
	if ( (pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingProductID)|| m_XYDCommProtocol.GetIsEnableIDRead())
	{
		if (!CommunicateGetAlignProcessProductID(strProductID))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品ID读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	if (pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo
		||pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
	{
		if (!CommunicateGetAlignProcessProductSTATION(strProductSTATION,strStartIime,strEndIime,strProsessingIime,strInspectStation))
		{
			//ReportPLCFunc(eCodeNG);
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("产品工位读取失败");
			m_strStatusBarInfo2 = _T("检测失败");
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();	
			return FALSE;
		}
	}

	m_BendInspectResultInfo.m_strProductID = strProductID;
	m_strProductID = strProductID;
	m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strProductSTATION = strProductSTATION;
	m_BendInspectResultInfo.m_strInspectStation = strInspectStation;

	//检测光源切换
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		if (!OnLightOnOff(2))
		{
			ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("拍照失败：");
			m_strStatusBarInfo2 = _T("光源切换失败！");
			SetStatusBarInfo(m_bStatusBar, m_strStatusBarInfo1, m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();
			return FALSE;
		}
	}

	// 检测模板曝光切换
	{
		int nStartPatIndex = 0;
		for(int it=0;it<vnPatIndex.size();it++)
		{
			if(vnPatIndex.at(it)==1)
			{
				nStartPatIndex = it;
				break;
			}
		}

		int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
		BOOL SystempOptionSearchShutter = optionInfo.m_bEnableSearchShutter;

		CameraParamEnable cameraParamEnable;
		cameraParamEnable =m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetEnableInspectSearchShutter(nStartPatIndex);
		BOOL bInspectShutter = (cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable);

		if (SystempOptionSearchShutter /*&& bInspectCandidate*/ && bInspectShutter/* && (0 == nStartPatIndex)*/)
		{
			std::vector<CameraExposureType> vsmCameraExposureType;	
			int i = 0;
			for(i = 0; i < nPosNum; i++)
			{
				vsmCameraExposureType.push_back(eInspectSearchCameraExposure);
			}
			if (!CameraExposureSetting(vnPosIndex,vsmCameraExposureType,nStartPatIndex))
			{
				OnFailedCameraExposureSetting(strCmd);
			}
		}
	}



	scTimer stInspectSearchTimer; double dInspectSearchTime(0.0);
	stInspectSearchTimer.Reset();

	// 准备开始定位			
	std::vector<SearchMode> vsmSearchMode;
	for(int i = 0; i < nPosNum; i++)
	{
		vsmSearchMode.push_back(eInspectSearch);
	}


	// 等待搜索延时
	int nCurProdcutIndex = m_pVisionASMConfig->m_TotalProductData.GetCurProductIndex();
	int nDelayTime = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetObjectSearchDelayTime();
	Sleep(nDelayTime);

	//m_cVisionAlignGui.SetPosGuiInvisible(vnPosIndex, eInspectSearch);
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 开始");
	// 采集搜索图像
	if (!SnapSearchImage(vnPosIndex))
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,m_vdDetectionResult1);
		OnFailedSnapSearchImage(strCmd);
		return FALSE;
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测拍照 结束");


	// 定位
	BOOL bInspectSearchCandidate = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateEnable();
	BOOL bInspectSearchSync = m_pVisionASMConfig->m_TotalProductData.GetItemPtr(nCurProdcutIndex)->m_pAlignerParam->GetInspectCandidateSyncEnable();

	BOOL bSearchSucceed = TRUE;
	std::vector<BOOL> vbSearchSucceed;
	std::vector<int> vnSearchIndex;

	CString strErrorInfo;	// 检测工具失败提示
	CString strPatNum;   // 模板序号
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 开始");

	if (bInspectSearchCandidate == FALSE)
	{
		// 如果模板参数选中中候补1/2/3/4模板，但是界面设置中没有选中，则认为检测失败
		for (int j=1; j<5; j++)
		{
			if (vnPatIndex.at(j) == 1)
			{
				bSearchSucceed = FALSE;
				strErrorInfo = "检测模板候选搜索未开. ";
				break;
			}
		}

		// 如果不启用候补模板，则只能检测标准0模板，直接跳过候补1/2/3/4模板
		if (vnPatIndex.at(0) == 1)
		{
			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = 0;
				m_vnInspectSearchEndPatIndex[nPosIndex] = 0;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strErrorInfo += m_StrSearchErrorInfo + "模板搜索失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(0);
		}
	}
	else
	{
		for (int j=0; j<5; j++)
		{
			// 如果模板参数没有选中该模板，则跳过
			if (vnPatIndex.at(j) == 0)
			{
				continue;
			}

			// 如果候补1/2/3/4模板在界面设置中没有被选中，则认为检测失败
			if (j > 0 && (FALSE == GetAlnInspectSearchCandidatePatEnable(j)))
			{
				bSearchSucceed = FALSE;
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += "检测模板" + strPatNum + "未开启. ";
				break;
			}

			for (int i = 0; i<nPosNum; i++)
			{
				nPosIndex = vnPosIndex[i];
				m_vnInspectSearchStartPatIndex[nPosIndex] = j;
				m_vnInspectSearchEndPatIndex[nPosIndex] = j;
			}

			BOOL bTmep = SearchMark(vnPosIndex, vsmSearchMode);

			if (bTmep == FALSE)
			{
				strPatNum.Format(_T("%d"), j);   // 模板序号
				strErrorInfo += m_StrSearchErrorInfo + "模板" + strPatNum + "失败. ";
			}

			vbSearchSucceed.push_back(bTmep);
			vnSearchIndex.push_back(j);
		}
	}
	VisionAlignLogRecord("D:\\TimeLog.txt", "检测搜索 结束");


	std::vector<double> vdDistance;
	std::vector<BOOL> vbAreaInspectSubResult;

	std::vector<double> vdCircleRadius;


	std::vector<BOOL> vbBendTwoModelSearchTool;
	std::vector<BOOL> vbBendCircleSearchTool;
	std::vector<BOOL> vbBendCircleSearchToolXY;

	//double dDetectionResult[6] = {0, 0, 0, 0,0,0};	//初始化检测结果

	std::vector<BOOL> vbBendCornerSearchTool;

	// FALSE包含两种情况：1）候补功能未启用、但是指令参数选中候补模板，2）候补功能打开、指令参数选中候补模板、但是界面上候补模板未选中
	if (TRUE == bSearchSucceed)
	{
		// 等于0包含两种情况：1）候补功能未启用、并且指令参数没有选中标准0模板，2）候补功能打开、界面上候补模板选中、但是参数一个都没有选中
		if (vbSearchSucceed.size() == 0)
		{
			bSearchSucceed = FALSE;
		}
		else
		{
			// 对所有参与检测的模板进行遍历
			for (int j=0; j<vbSearchSucceed.size(); j++)
			{
				// 将当前检测结果与总结果进行与操作
				bSearchSucceed &= vbSearchSucceed.at(j);

				// 获取当前检测结果对应模板索引
				int nPatIndex = vnSearchIndex.at(j);

				// 遍历所有位置
				for (int i = 0; i<nPosNum; i++)
				{
					// 获取当前位置
					nPosIndex = vnPosIndex[i];

					// 获取指定位置、指定候补模板的定位工具
					CBaseSearchTool* pSearchTool = GetRepoAlnInspectSearchTool(nCurProdcutIndex, nPosIndex, nPatIndex);

					if (pSearchTool != NULL)
					{
						CSearchToolType eType = pSearchTool->GetSearchToolType();
						switch(eType)
						{
						case ePatternPosLineDistanceTool:
							{
								

							}
							break;
						default:
							   vbBendCornerSearchTool.push_back(TRUE);
								CSearchResult result;
								pSearchTool->GetResult(0, result);

								/*CSearchResult resultt;
								pSearchTool->GetResult(1, result);*/
								
								if (nTarFixOffsetIndex)
								{
									m_mpObjectMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
								} 
								else
								{
									m_mpTargetMarkImagePos.SetMarkImagePos(nPosIndex, TRUE, CCoordPos(result.m_dPosX, result.m_dPosY, 0));
								}
								
								
								
								CPlatformXYDAxisPos* pPlatformAxisPos = new CPlatformXYDAxisPos;
								SetObjectPlatformPos(pPlatformAxisPos,nPosIndex);
								SetTargetPlatformPos(pPlatformAxisPos,nPosIndex);
								if (pPlatformAxisPos)
								{
									delete pPlatformAxisPos;
									pPlatformAxisPos = NULL;
								}

							break;

						}

					}
				}
			}
		}
	} 


	CString strBendInsError = _T("");
	BOOL bBendInspect = FALSE;

	std::vector<BOOL> m_bResult;
	std::vector<CString> m_vSearchResult;
	CString	 m_StrText;
	CMarkImagePos       m_mpInsectTargetMarkImagePos;
	if (nTarFixOffsetIndex)
	{
		m_mpInsectTargetMarkImagePos = GetObjectMarkSearchResults();
	} 
	else
	{
		m_mpInsectTargetMarkImagePos = GetTargetMarkSearchResults();
	}
	
	//这里去掉，折弯检测用不了手动弹窗
	if (TRUE == pCurProductData->m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		// 手动弹窗功能，只针对折弯检测使用的工具，其它定位工具不支持
		if (FALSE)
		{
			if (optionInfo.m_bManualAfObjSearchFail && ((2 == vbBendTwoModelSearchTool.size()) || (2 == vbBendCornerSearchTool.size())))
				//if (optionInfo.m_bManualAfObjSearchFail)
			{
				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉弹出手动搜索框
				// 	SetPlcManualSearch(TRUE);
				// }

				if (FALSE ==ExcuteManualSearch(2,vnPosIndex))
				{
					if (optionInfo.m_bSetVisionManualSearchToPLC)
					{
						// 通知PLC，视觉关闭手动搜索框
						//SetPlcManualSearch(FALSE);
					}
				}
				else
				{
					bSearchSucceed = TRUE;
				}

				// if (optionInfo.m_bSetVisionManualSearchToPLC)
				// {
				// 	// 通知PLC，视觉关闭手动搜索框
				// 	SetPlcManualSearch(FALSE);
				// }

				if (TRUE == bSearchSucceed)
				{
					if ( 2 == vbBendTwoModelSearchTool.size())
					{
						SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
						SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

						m_pAlignerTool->SetBendAlignProcess(FALSE);
						bBendInspect = BendingAlignProcessInfo(strBendInsError);
						m_pAlignerTool->SetBendAlignProcess(TRUE);

						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingProcessInfo();

						//VisionInspectBendRecord(FALSE, TRUE, FALSE);

					}

					if ( 2 == vbBendCircleSearchTool.size() )
					{
						bBendInspect = BendingCircleProcessInfo(strBendInsError, vdCircleRadius);
						bSearchSucceed = bBendInspect;
						PostMessageUpdateBendingCircleInfo();

						//VisionInspectBendRecord(FALSE, FALSE, TRUE);
					}


					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}

					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);

					// 存储csv
					SaveResultToCsv(m_strProductID, m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}

					if (bSearchSucceed)
					{
					// 打包回复PLC成功
					ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

					m_bStatusBar = TRUE;
					m_strStatusBarInfo1 = _T("检测成功");
					m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
					} 
					else
					{
					// 打包回复PLC NG
					ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();
					}
					//// 打包回复PLC成功
					//ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);

					//m_bStatusBar = TRUE;
					//m_strStatusBarInfo1 = _T("检测成功");
					//m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
					//SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();

					return TRUE;

				}
				else
				{
					if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
					{
						ExeSaveVDBFile(vnPosIndex,eInspectSearch);
					}


					// 获取LX LY RX RY
					BendInspectResultInfo BendInspectResult;
					GetBendInspectResult(BendInspectResult);
					vdDistance.push_back( BendInspectResult.m_dLx);
					vdDistance.push_back( BendInspectResult.m_dLy);
					vdDistance.push_back( BendInspectResult.m_dRx);
					vdDistance.push_back( BendInspectResult.m_dRy);
					// 存储csv
					SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, TRUE);
					//存储CSV固定路径
					if (m_pSystempOptionConfig->m_bAllSteoage)
					{
						SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
					}
					
					// 打包回复PLC NG
					ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
					m_bStatusBar = FALSE;
					m_strStatusBarInfo1 = _T("检测失败");
					m_strStatusBarInfo2 = strBendInsError;
					SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
					PostMessageUpdateStatusBarInfo();

					strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
					AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

					return FALSE;
				}

			}
		}


		if (TRUE == bSearchSucceed)
		{
			//if ( 2 == vbBendTwoModelSearchTool.size())

			if(TRUE ==pCurProductData->m_pAlignerParam->GetBendInfo().m_bEnableMoveSnap)
			{

				if(pCurProductData->m_pAlignerParam->GetBendInfo().m_nPosIndexFisrt !=vnPosIndex[0] && 1 == vbBendTwoModelSearchTool.size())
				{
					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					bBendInspect = BendingAlignProcessInfo(strBendInsError);
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

			}
			else
			{

				if ( 2 == vbBendCornerSearchTool.size())
				{

					SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
					SetTargetMarkSearchResults(m_mpTargetMarkImagePos);

					m_pAlignerTool->SetBendAlignProcess(FALSE);
					//bBendInspect = BendingAlignProcessInfo(strBendInsError);
					bBendInspect = BendingAlignProcessInfo(strBendInsError,m_bResult,m_vSearchResult,m_strProductID,m_strProductSTATION,strInspectStation);
					
					m_pAlignerTool->SetBendAlignProcess(TRUE);

					bSearchSucceed = bBendInspect;
					PostMessageUpdateBendingProcessInfo();

					m_cVisionAlignGui.SetAllPosText(m_bResult,m_vSearchResult);

					//添加 2022/4/1  
					//m_cVisionAlignGui.SetAllPosObjectToTargetResultLine();
					
					//VisionInspectBendRecord(FALSE, TRUE, FALSE);
				}

			}


			//m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			
			{
				// 获取LX LY RX RY
				BendInspectResultInfo BendInspectResult;
				GetBendInspectResult(BendInspectResult);
				vdDistance.push_back( BendInspectResult.m_dLx);
				vdDistance.push_back( BendInspectResult.m_dLy);
				vdDistance.push_back( BendInspectResult.m_dRx);
				vdDistance.push_back( BendInspectResult.m_dRy);
			}

			
			// 存储csv
			SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation,bSearchSucceed, vdDistance, TRUE);
			//存储CSV固定路径
			if (m_pSystempOptionConfig->m_bAllSteoage)
			{
				SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, TRUE);
			}
			// 检测结果判定
			if(bSearchSucceed)
			{
				//ReportPLCFunc(eCodeOK);
				ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
				m_bStatusBar = TRUE;
				m_strStatusBarInfo1 = _T("检测成功");
				m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}
			else
			{
				//ReportPLCFunc(eCodeNG);
				ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
				m_bStatusBar = FALSE;
				m_strStatusBarInfo1 = _T("检测失败");
				m_strStatusBarInfo2 = strBendInsError;
				SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
			}

			//if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			//{
			//	//Sleep(10);
			//	ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			//}

			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eTargetSearch);
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, eObjectSearch);
			m_cVisionAlignGui.SetPosObjectToTargetResultLine(vnPosIndex);

			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}

			if (nTarFixOffsetIndex)
			{
				m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
			} 
			else
			{
				m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}

			

			return TRUE;

		}
		else
		{

			
			{
				// 获取LX LY RX RY
				//BendInspectResultInfo BendInspectResult;
				//GetBendInspectResult(BendInspectResult);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
				vdDistance.push_back(0);
			}
			m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
			

			//// 获取LX LY RX RY
			//BendInspectResultInfo BendInspectResult;
			//GetBendInspectResult(BendInspectResult);
			//vdDistance.push_back( BendInspectResult.m_dLx);
			//vdDistance.push_back( BendInspectResult.m_dLy);
			//vdDistance.push_back( BendInspectResult.m_dRx);
			//vdDistance.push_back( BendInspectResult.m_dRy);

			

			// 存储csv
			//SaveResultToCsv(m_strProductID,m_strProductSTATION, bSearchSucceed, vdDistance, TRUE);

			////存储CSV固定路径
			//if (m_pSystempOptionConfig->m_bAllSteoage)
			//{
			//	SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,bSearchSucceed, vdDistance, TRUE);
			//}
			ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
			//ReportPLCFunc(eCodeNG);

			m_bStatusBar = FALSE;
			m_strStatusBarInfo1 = _T("检测失败");
			m_strStatusBarInfo2 = strErrorInfo;
			SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
			PostMessageUpdateStatusBarInfo();

			strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
			AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));
			
			if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
			{
				ExeSaveVDBFile(vnPosIndex,eInspectSearch);
			}

			if (nTarFixOffsetIndex)
			{
				m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
			} 
			else
			{
				m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
				SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
			}

			return FALSE;
		}

	}
	m_cVisionAlignGui.SetPosSearchResult(vnPosIndex, vnSearchIndex, eInspectSearch);
	if (nTarFixOffsetIndex)
	{
	m_mpObjectMarkImagePos = m_mpInsectTargetMarkImagePos;
	SetObjectMarkSearchResults(m_mpObjectMarkImagePos);
	} 
	else
	{
	m_mpTargetMarkImagePos = m_mpInsectTargetMarkImagePos;
	SetTargetMarkSearchResults(m_mpTargetMarkImagePos);
	}


	if (m_pVisionASMConfig->m_DataRecord.m_bSaveVDBFile)
	{
		ExeSaveVDBFile(vnPosIndex,eInspectSearch);
	}


	// 使用ACF检测工具、ACF检测工具EX和区域点线工具时，要求平台选项界面必须不启用线距检测，否则在检测成功和平台选项界面启用发送线距结果时，会因为获取距离结果错误造成软件崩溃
	// 只有使用角定位检查测量工具时，才能在平台选项界面启用线距检测和发送线距结果

	// 使用区域点线工具测量点距时，无论检测结果整体成功/失败，均将距离发送到PLC备用轴位置寄存器(距离1/2/3/.../n)，无协议也发送
	// 使用ACF检测工具EX时，如果平台选项界面启用发送子结果，则无论检测结果整体成功/失败，均将子结果发送到PLC备用寄存器（数量+子结果1/2/3/.../n），无协议不发送
	// 使用角定位检查测量工具测量线距时，要求平台选项界面必须启用线距检测，在同时满足检测成功和平台选项界面启用发送线距结果时，才将距离发送到PLC备用轴位置寄存器(距离1/2/12结果/距离3/4/34结果)，无协议不发送

	SaveResultToCsv(m_strProductID,m_strProductSTATION,strInspectStation, bSearchSucceed, vdDistance, FALSE);
	//存储CSV固定路径
	if (m_pSystempOptionConfig->m_bAllSteoage)
	{
		SaveResultToCsvFixed(m_strProductID, m_strProductSTATION,strInspectStation,strStartIime,strEndIime,strProsessingIime,bSearchSucceed, vdDistance, FALSE);
	}

	if (TRUE == bSearchSucceed)
	{
		//ReportPLCFunc(eCodeOK);
		ReportPLCWithDetectionResultFunc(eCodeOK,vdDistance);
		m_bStatusBar = TRUE;
		m_strStatusBarInfo1 = _T("检测成功");
		m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SEARCH_TIME_COST), stInspectSearchTimer.GetTimeMilli(TRUE));
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);PostMessageUpdateStatusBarInfo();
	}
	else
	{
		//ReportPLCFunc(eCodeNG);
		ReportPLCWithDetectionResultFunc(eCodeNG,vdDistance);
		m_bStatusBar = FALSE;
		m_strStatusBarInfo1 = _T("检测失败");
		m_strStatusBarInfo2 = strBendInsError;
		SetStatusBarInfo(m_bStatusBar,m_strStatusBarInfo1,m_strStatusBarInfo2);
		PostMessageUpdateStatusBarInfo();

		strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_OPERATION_WARNING_CAMERA);
		AddWarningInfo(strTempWarningType, (strCmd + m_strStatusBarInfo1));

		return FALSE;
	}

	return TRUE;
}

BOOL vcXYDVisionAlign::ResetAllIndex()
{
	m_nCurFOBC_Index = 0; //当前对象拍照指令的流程索引
	m_nCurFTGC_Index= 0; //当前对象拍照指令的流程索引
	m_nCurFCBP_Index= 0; //当前关联指令的流程索引
	m_nCurFCBS_Index= 0; //当前关联指令的流程索引
	m_nCurFCHB_Index= 0; //当前检测拍照指令的流程索引
	m_nCurFCLB_Code_Index= 0;//当前标定指令的流程索引
	m_nCurFCBS_XY_Index = 0;
	m_nCurFCBS_Angle_Index = 0;
	m_nCurCalibrateIndex = 0;
	m_nCurAlignerTimesIndex = 0;

	return TRUE;
}
//回复发多个结果 绝对偏差量
BOOL vcXYDVisionAlign::ReportMulAlignPosResultFALP_Ex2(std::vector<double> vdDate,int nResult,int nSearchNumStatus)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "发送拾取 结果 开始");
	
	CPlatformOptionInfo optionInfo;
	GetPlatformOptionInfo(optionInfo);
	BOOL bResult = TRUE;
	if(vdDate.size()<0)
	{
		bResult = FALSE;
	}
	if (m_XYDCommProtocol.IsPlcProtocolType())// 如果是PLC直接读取存储寄存器协议
	{
		CString strSimpleInfo;
		m_XYDCommProtocol.PackRePlcMulResult_SimpleInfo_Ex(strSimpleInfo,vdDate,nResult,nSearchNumStatus);
		AddCommCommandInfo(TRUE,strSimpleInfo,FALSE,FALSE);

		CString strSend;
		BOOL bReceived;
		CString strInfo;
		CommStatus nCommStatus = eCommSucceed;
		int nOffSetMul = 0;

		BOOL bEnableMulReg= m_pPlatformInfo->m_bEnableMulRegTrigger;
		if(bEnableMulReg)
		{
			//nOffSetMul = m_XYDCommProtocol.m_nCurRegTriggerIndex*RREG_KEYENCE_PLC_REG_NUMS;
			nOffSetMul =RREG_KEYENCE_PLC_REG_NUMS*m_XYDCommProtocol.m_nCurRegTriggerIndex;
			if(m_XYDCommProtocol.m_nCurRegTriggerIndex>=0 && m_XYDCommProtocol.m_nCurRegTriggerIndex<optionInfo.m_vnMulRegTriggerSet.size())
			{
				nOffSetMul = optionInfo.m_vnMulRegTriggerSet[m_XYDCommProtocol.m_nCurRegTriggerIndex].m_nMulRegOffset;
			}
		}
		if (bResult)
		{
			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackRePlcMulResult_Ex(strSend,vdDate,nOffSetMul))
			{
				return FALSE;
			}

			//bReceived = CommunicateWriteReg(vnSend,strInfo,nCommStatus,nRegAddr,true,false);
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			// // 打包 D5 产品数目状态 1-贴头1处有产品  2贴头2处有产品  3 都有  0没有
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nSearchNumStatus, atoi(RREG_KEYENCE_VISION_PARAM_RESERVED)+nOffSetMul))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}

			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffSetMul))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
		else
		{
			// 打包 D5 产品数目状态 1-贴头1处有产品  2贴头2处有产品  3 都有  0没有
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,0, atoi(RREG_KEYENCE_VISION_PARAM_RESERVED)+nOffSetMul))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}


			// 打包写寄存器指令
			if (FALSE == m_XYDCommProtocol.PackWritePLCFunc(strSend,nResult, atoi(RREG_KEYENCE_VISION_TRIGGER)+nOffSetMul))
			{
				return FALSE;
			}

			// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
			bReceived = CommunicateSendWriteRegStr(strSend, strInfo, nCommStatus);
			if (bReceived == FALSE || nCommStatus != eCommSucceed)
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}

	VisionAlignLogRecord("D:\\TimeLog.txt", "发送拾取 结果 结束");

	return TRUE;
}
//存CSV
BOOL vcXYDVisionAlign::SaveResultToCsv(CString strSNCode, BOOL bSearchSucceed, vector<double> vdDetectionResult, BOOL bIsBending)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV开始");
	CPlatformOptionInfo optioninfo;
	GetPlatformOptionInfo(optioninfo);

	CStdioFile file;
	CString strcsv;
	CString strSearchSucceed;

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 

	// 时间
	CString strTime;
	strTime.Format("[%02d_%02d_%02d_%02d_%02d_%03d]",sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);

	// BOOL to Cstring（NG/OK）
	if(bSearchSucceed)
	{
		strSearchSucceed = "OK";
	}
	else
	{
		strSearchSucceed = "NG";
	}


	// 判断文件夹是否存在，不存在新建
	CFileFind findFile;
	BOOL IsFild2 = FALSE; 
	BOOL IsDir2 = FALSE;
	CString strDir  = "D:\\VisionASMLog";

	IsFild2 = findFile.FindFile(strDir);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strDir, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strFileName;
	strFileName.Format("[%04d_%02d_%02d]",sys.wYear,sys.wMonth,sys.wDay);
	strFileName = strDir + _T("\\") + strFileName;

	IsFild2 = FALSE;
	IsDir2 = FALSE;

	IsFild2 = findFile.FindFile(strFileName);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strFileName, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strPathFileName;
	strPathFileName.Format(_T("%s_Result.csv"),m_pSystempOptionConfig->m_strPlatformCustomName[m_pPlatformInfo->m_nPlatformIndex]);
	//strPathFileName.Format(_T("Platform%d_Result.csv"),m_pPlatformInfo->m_nPlatformIndex);

	CString strPath;
	CFileStatus rStatus;
	strPath = strFileName + _T("\\") + strPathFileName;

	BOOL bRes = CFile::GetStatus(strPath, rStatus);

	if (!bRes)
	{
		if(!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		{
			return FALSE;
		}
		file.SeekToEnd();

		CString strcsv;
		
		strcsv.Format(optioninfo.m_sFCHBSaveCSVHead+_T("\n"));
		file.WriteString(strcsv);
		file.Close();
	}
	if (!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return FALSE;
	}
	file.SeekToEnd();
	strcsv.Format(_T("%s,%s,%s"), strTime, strSNCode, strSearchSucceed);

	for (int ii=0;ii<vdDetectionResult.size();ii++)
	{
		CString tempstr;
		tempstr.Format(_T(",%lf"),vdDetectionResult.at(ii));
		strcsv+=tempstr;
	}
	strcsv+=_T("\n");
	file.WriteString(strcsv);
	file.Close();
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV结束");

	return TRUE;
}


BOOL vcXYDVisionAlign::SaveResultToCsv(CString strSNCode, CString strSTATIONCode, CString strInspectStation, BOOL bSearchSucceed, vector< double> vdDetectionResult, BOOL bIsBending)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV开始");
	CPlatformOptionInfo optioninfo;
	GetPlatformOptionInfo(optioninfo);

	CStdioFile file;
	CString strcsv;
	CString strSearchSucceed;

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 

	// 时间
	CString strTime;
	strTime.Format("[%02d_%02d_%02d_%02d_%02d_%03d]",sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);

	// BOOL to Cstring（NG/OK）
	if(bSearchSucceed)
	{
		strSearchSucceed = "OK";
	}
	else
	{
		strSearchSucceed = "NG";
	}


	// 判断文件夹是否存在，不存在新建
	CFileFind findFile;
	BOOL IsFild2 = FALSE; 
	BOOL IsDir2 = FALSE;
	CString strDir  = "D:\\VisionASMLog";

	IsFild2 = findFile.FindFile(strDir);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strDir, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strFileName;
	strFileName.Format("[%04d_%02d_%02d]",sys.wYear,sys.wMonth,sys.wDay);
	strFileName = strDir + _T("\\") + strFileName;

	IsFild2 = FALSE;
	IsDir2 = FALSE;

	IsFild2 = findFile.FindFile(strFileName);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strFileName, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strPathFileName;
	strPathFileName.Format(_T("%s_Result.csv"),m_pSystempOptionConfig->m_strPlatformCustomName[m_pPlatformInfo->m_nPlatformIndex]);
	//strPathFileName.Format(_T("Platform%d_Result.csv"),m_pPlatformInfo->m_nPlatformIndex);

	CString strPath;
	CFileStatus rStatus;
	strPath = strFileName + _T("\\") + strPathFileName;

	BOOL bRes = CFile::GetStatus(strPath, rStatus);

	if (!bRes)
	{
		if(!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		{
			return FALSE;
		}
		file.SeekToEnd();

		CString strcsv;

		strcsv.Format(optioninfo.m_sFCHBSaveCSVHead+_T("\n"));
		file.WriteString(strcsv);
		file.Close();
	}
	if (!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return FALSE;
	}
	file.SeekToEnd();
	strcsv.Format(_T("%s,%s,%s,%s,%s"), strTime, strSNCode, strSTATIONCode,strInspectStation,strSearchSucceed);

	for (int ii=0;ii<vdDetectionResult.size();ii++)
	{
		CString tempstr;
		tempstr.Format(_T(",%lf"),vdDetectionResult.at(ii));
		strcsv+=tempstr;
	}
	strcsv+=_T("\n");
	file.WriteString(strcsv);
	file.Close();
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV结束");

	return TRUE;

}

BOOL vcXYDVisionAlign::SaveResultToCsvFixed(CString strSNCode,CString strSTATIONCode,CString strInspectStation,CString strStartIime,CString strEndIime,CString strProsessingIime, BOOL bSearchSucceed, vector< double> vdDetectionResult, BOOL bIsBending)
{
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV固定路径开始");
	CPlatformOptionInfo optioninfo;
	GetPlatformOptionInfo(optioninfo);

	CStdioFile file;
	CString strcsv;
	CString strSearchSucceed;

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 

	// 时间
	CString strTime;
	strTime.Format("[%02d_%02d_%02d_%02d_%02d_%03d]",sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);

	// BOOL to Cstring（NG/OK）
	if(bSearchSucceed)
	{
		strSearchSucceed = "OK";
	}
	else
	{
		strSearchSucceed = "NG";
	}


	// 判断文件夹是否存在，不存在新建
	CFileFind findFile;
	BOOL IsFild2 = FALSE; 
	BOOL IsDir2 = FALSE;
	CString strDir  = "D:\\VisionASMLog";

	IsFild2 = findFile.FindFile(strDir);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strDir, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strFileName;
	strFileName.Format("Platform_Result");
	strFileName = strDir + _T("\\") + strFileName;

	IsFild2 = FALSE;
	IsDir2 = FALSE;

	IsFild2 = findFile.FindFile(strFileName);
	while (IsFild2)
	{
		IsFild2 = findFile.FindNextFile();
		IsDir2 = findFile.IsDirectory();
		if(IsDir2) break;
	}
	if (!IsDir2)
	{
		if (!CreateDirectory(strFileName, NULL))// 创建文件夹
		{
			return FALSE;
		}
	}

	CString strPathFileName;
	strPathFileName.Format("%s_%04d%02d%02d_%02d%02d%02d.dmy",strSNCode,sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);

	CString strPath;
	CFileStatus rStatus;
	strPath = strFileName + _T("\\") + strPathFileName;

	BOOL bRes = CFile::GetStatus(strPath, rStatus);

	if (!bRes)
	{
		if(!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		{
			return FALSE;
		}
		file.SeekToEnd();

		CString strcsv;

		strcsv.Format(optioninfo.m_sFCHBSaveCSVHead+_T("\n"));
		file.WriteString(strcsv);
		file.Close();
	}
	if (!file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return FALSE;
	}
	file.SeekToEnd();
	//strcsv.Format(_T("%s,%s,%s,%s,%s"), strTime, strSNCode, optioninfo.m_StepID,strSTATIONCode,strSearchSucceed);
	strcsv.Format(_T("%s,%s,%s,%s,%s,%s"),optioninfo.m_FieldCount, strSNCode, optioninfo.m_StepID,strSTATIONCode,strInspectStation,optioninfo.m_FoamSize);

	for (int ii=0;ii<vdDetectionResult.size();ii++)
	{
		CString tempstr;
		tempstr.Format(_T(",%lf"),vdDetectionResult.at(ii));
		strcsv+=tempstr;
	}
	CString strcsvv;
	strcsvv.Format(_T("%s,%s,%s,%s,%s,%s\n"),optioninfo.m_BendingR,strSearchSucceed,strStartIime,strEndIime,strProsessingIime,optioninfo.m_Production);
	/*strcsv+=optioninfo.m_BendingR;
	strcsv+=strSearchSucceed;

	strcsv+=strStartIime;
	strcsv+=strEndIime;
	strcsv+=strProsessingIime;
	strcsv+=optioninfo.m_Production;
	strcsv+=_T("\n");*/
	strcsv+=strcsvv;
	file.WriteString(strcsv);
	file.Close();
	VisionAlignLogRecord("D:\\TimeLog.txt", "存CSV固定路径结束");

	return TRUE;

}