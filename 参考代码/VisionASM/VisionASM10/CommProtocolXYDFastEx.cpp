// CommProtocolXYDFast.cpp: implementation of the CCommProtocolXYD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisionASM.h"
#include "CommProtocolXYDFast.h"
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SysWorkCmdType CCommProtocolXYDFast::AnalyzePlcSysWorkCmdReceivedEx( CString strCmd, BOOL &bTriggered)
{
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		bTriggered = FALSE;
		return eCmdType;
	}

	//双字和单字
	{
		// 解析字符串
		int  nDataNum = GetReadDataNum();												// 数据量
		std::vector<int> vnReceivedData;
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strCmd, nDataNum, vnReceivedData,m_bRegType))
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.size() != nDataNum)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.at(0) == PLC_CMD_VAL_NOT_TRIGGERED)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

        SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);
		
		
		double dBase = 10;
		bTriggered = TRUE;
	
		//标定过程
		if(vnReceivedData.at(0)==eKeyenceCmdCalibrate)
		{
			return eCmdCalibrateFCLB;
		}
		else if (vnReceivedData.at(0)==eKeyenceCmdFACO)
		{
			// 对位指令
			m_FACOParam.P1 = 1;
			return eCmdGetTargetObjectOffsetFACO;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFOBC)
		{
			return eCmdObjectSearchFOBC;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFPMI)
		{
			return eCmdObjectSearchFPMI;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFBIS)
		{
			return eCmdACFInspectFBIS;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFTGC)
		{
			return eCmdTargetSearchFTGC;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFSPC)
		{
			m_FSPCParam.P1 = vnReceivedData[1];
			m_FSPCParam.P2 = vnReceivedData[2];

			return eCmdSetProductIndexFSPC;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFCHB)
		{
			return eCmdACFInspectFCHB;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFALP)
		{
			return eCmdOnceAlignMovePickFALP;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFAAC)
		{
			return eCmdAutoAlignFAAC;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFCBP)
		{
			return eCmdCalibrateFCBP;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFCBS)
		{
			return eCmdCalibrateFCBS;
		}
		else if(vnReceivedData.at(0)==eKeyenceCmdFAAL)
		{
			return eCmdAutoAlignFAAL;
		}
		else
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		return eCmdUnknow;
	}
}

SysWorkCmdType CCommProtocolXYDFast::AnalyzePlcSysWorkCmdReceivedEx_FixCode( CString strCmd, BOOL &bTriggered)
{
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		bTriggered = FALSE;
		return eCmdType;
	}

	//双字和单字
	{
		// 解析字符串
		int  nDataNum = GetReadDataNum();												// 数据量
		std::vector<int> vnReceivedData;
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strCmd, nDataNum, vnReceivedData,m_bRegType))
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.size() != nDataNum)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.at(0) == PLC_CMD_VAL_NOT_TRIGGERED)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);

		int nCode = vnReceivedData.at(0);



		for(int it=0;it<m_vKeyenceCodeParam.size();it++)
		{
			if(nCode==m_vKeyenceCodeParam.at(it).m_nCmdVal)
			{
				bTriggered = TRUE;
				int nIndex = it+1;
				m_nCurCodeIndex = it;
				if(nIndex==eKeyenceCmdFACO)
				{
					// 对位指令
					return eCmdGetTargetObjectOffsetFACO;
				}
				else if (nIndex==eKeyenceCmdCalibrate)
				{
					// 标定指令
					return eCmdCalibrateFCLB;
				}
				else if(nIndex==eKeyenceCmdFOBC)
				{
					return eCmdObjectSearchFOBC;
				}
				else if(nIndex==eKeyenceCmdFPMI)
				{
					return eCmdObjectSearchFPMI;
				}
				else if(nIndex==eKeyenceCmdFBIS)
				{
					return eCmdACFInspectFBIS;
				}
				else if(nIndex==eKeyenceCmdFTGC)
				{
					return eCmdTargetSearchFTGC;
				}
				else if(nIndex==eKeyenceCmdFSPC)
				{
					return eCmdSetProductIndexFSPC;
				}
				else if(nIndex==eKeyenceCmdFCHB)
				{
					return eCmdACFInspectFCHB;
				}
				else if(nIndex==eKeyenceCmdFALP)
				{
					return eCmdOnceAlignMovePickFALP;
				}
				else if(nIndex==eKeyenceCmdFAAC)
				{
					return eCmdAutoAlignFAAC;
				}
				else if(nIndex==eKeyenceCmdFMCO)
				{
					return eCmdGetTargetObjectOffsetFMCO;
				}
				else if(nIndex==eKeyenceCmdFCBP)
				{
					return eCmdCalibrateFCBP;
				}
				else if(nIndex==eKeyenceCmdFCBS)
				{
					return eCmdCalibrateFCBS;
				}
				break;
			}
		}

		return eCmdUnknow;
	}
}

SysWorkCmdType CCommProtocolXYDFast::AnalyzePlcSysWorkCmdReceivedEx_FixCode( CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam)
{
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		bTriggered = FALSE;
		return eCmdType;
	}

	//双字和单字
	{
		// 解析字符串
		int  nDataNum = GetReadDataNumEx();												// 数据量
		std::vector<int> vnReceivedData;
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strCmd, nDataNum, vnReceivedData,m_bRegType))
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.size() != nDataNum)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.at(0) == PLC_CMD_VAL_NOT_TRIGGERED)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);
		// 解析随轴一并读写
		AnalyzeAxisInfoInCmdParamEx(vnReceivedData);
		int nCode = vnReceivedData.at(0);

		if(vnReceivedData.at(0)==3 && m_eExProtocolType>0)
		{
			// 扩展时 3 指令强制转换为FCLB
			nCode = 2;
		}

		for(int it=0;it<m_vKeyenceCodeParam.size();it++)
		{
			if(nCode==m_vKeyenceCodeParam.at(it).m_nCmdVal)
			{
				bTriggered = TRUE;
				int nIndex = it+1;
				m_nCurCodeIndex = it;
				if(nIndex==eKeyenceCmdFACO)
				{
					// 对位指令
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdGetTargetObjectOffsetFACO;
				}
				else if (nIndex==eKeyenceCmdCalibrate)
				{
					// 标定指令
					if(m_eExProtocolType>0)
					{
						if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
						{
							if(vnReceivedData.at(0)==3)
							{
								cCodeParam.P1.at(0)= 1;
								cCodeParam.P2.at(0)= 2;
							}
							else if(vnReceivedData.at(0)==2)
							{
								cCodeParam.P1.at(0)= 1;
								cCodeParam.P2.at(0)= 1;
							}
						}
					}
					else
					{
						if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
						{

							cCodeParam.P1.at(0)= vnReceivedData.at(1);
							cCodeParam.P2.at(0)= vnReceivedData.at(2);
							cCodeParam.nP3.at(0)= vnReceivedData.at(3);
							cCodeParam.nP4.at(0)= vnReceivedData.at(4);
							cCodeParam.nP5.at(0)= vnReceivedData.at(5);
						}
					}

					return eCmdCalibrateFCLB;
				}
				else if(nIndex==eKeyenceCmdFOBC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdObjectSearchFOBC;
				}
				else if(nIndex==eKeyenceCmdFPMI)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return  eCmdObjectSearchFPMI;
				}
				else if(nIndex==eKeyenceCmdFBIS)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return  eCmdACFInspectFBIS;
				}
				else if(nIndex==eKeyenceCmdFTGC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdTargetSearchFTGC;
				}
				else if(nIndex==eKeyenceCmdFSPC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdSetProductIndexFSPC;
				}
				else if(nIndex==eKeyenceCmdFCHB)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdACFInspectFCHB;
				}
				else if(nIndex==eKeyenceCmdFALP)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdOnceAlignMovePickFALP;
				}
				else if(nIndex==eKeyenceCmdFAAC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdAutoAlignFAAC;
				}
				else if(nIndex==eKeyenceCmdFMCO)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdGetTargetObjectOffsetFMCO;
				}
				else if(nIndex==eKeyenceCmdFCBP)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdCalibrateFCBP;
				}
				else if(nIndex==eKeyenceCmdFCBS)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdCalibrateFCBS;
				}
				else if(vnReceivedData.at(0)==eKeyenceCmdFAAL)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdAutoAlignFAAL;
				}
				break;
			}
		}

		return eCmdUnknow;
	}
}

SysWorkCmdType CCommProtocolXYDFast::AnalyzePlcSysWorkCmdReceivedEx_FixCode2( CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam,int nMulReg)
{
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		bTriggered = FALSE;
		return eCmdType;
	}

	//双字和单字
	{
		// 解析字符串
		int  nDataNum = GetReadDataNumEx();												// 数据量
		std::vector<int> vnReceivedData;
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strCmd, nDataNum, vnReceivedData,m_bRegType))
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.size() != nDataNum)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.at(0) == PLC_CMD_VAL_NOT_TRIGGERED)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);
		// 解析随轴一并读写
		AnalyzeAxisInfoInCmdParamEx(vnReceivedData);
		
		int nCode = vnReceivedData.at(0);
		int ListNumber = 0;
		bool n_ParamNumber = FALSE;
		if(m_bEnableMulRegTrigger)
		{
			//auto iterFind = m_mapMulRegTrigger.find(nMulReg+1);

			//if (iterFind !=m_mapMulRegTrigger.end())
			//{
			//	if(iterFind->second.second == TRUE && nCode== iterFind->second.first.first)
			//	{
			//		nCode = iterFind->second.first.second;
			//	}
			//}

			for(int it1=0;it1< m_vnMulRegTriggerSet.size();it1++)
			{
				if(m_vnMulRegTriggerSet[it1].m_nMulIndex==nMulReg+1)
				{
					if (m_vnMulRegTriggerSet[it1].m_bParamNumber)
					{
						n_ParamNumber = true;
						ListNumber = m_vnMulRegTriggerSet[it1].m_nParamListNumber;
					}
					if(m_vnMulRegTriggerSet[it1].m_bEnable && vnReceivedData.at(0)== m_vnMulRegTriggerSet[it1].m_nCodeTrigger)
					{
						nCode = m_vnMulRegTriggerSet[it1].m_nCodeTrans;
						break;
					}
					
				}
			}

		}

		if(vnReceivedData.at(0)==3 && m_eExProtocolType>0)
		{
			// 扩展时 3 指令强制转换为FCLB
			nCode = 2;
		}

		for(int it=0;it<m_vKeyenceCodeParam.size();it++)
		{
			if(nCode==m_vKeyenceCodeParam.at(it).m_nCmdVal)
			{
				bTriggered = TRUE;
				int nIndex = it+1;
				m_nCurCodeIndex = it;
				if(nIndex==eKeyenceCmdFACO)
				{
					// 对位指令
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdGetTargetObjectOffsetFACO;
				}
				else if (nIndex==eKeyenceCmdCalibrate)
				{
					// 标定指令
					if(m_eExProtocolType>0)
					{
						if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
						{
							if(vnReceivedData.at(0)==3)
							{
								cCodeParam.P1.at(0)= 1;
								cCodeParam.P2.at(0)= 2;
							}
							else if(vnReceivedData.at(0)==2)
							{
								cCodeParam.P1.at(0)= 1;
								cCodeParam.P2.at(0)= 1;
							}
						}
					}
					else
					{
						if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
						{

							cCodeParam.P1.at(0)= vnReceivedData.at(1);
							cCodeParam.P2.at(0)= vnReceivedData.at(2);
							cCodeParam.nP3.at(0)= vnReceivedData.at(3);
							cCodeParam.nP4.at(0)= vnReceivedData.at(4);
							cCodeParam.nP5.at(0)= vnReceivedData.at(5);
						}
					}

					//// 标定指令
					//if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					//{
					//	cCodeParam.P1.at(0)= vnReceivedData.at(1);
					//	cCodeParam.P2.at(0)= vnReceivedData.at(2);
					//	cCodeParam.nP3.at(0)= vnReceivedData.at(3);
					//	cCodeParam.nP4.at(0)= vnReceivedData.at(4);
					//	cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					//}
					return eCmdCalibrateFCLB;
				}
				else if(nIndex==eKeyenceCmdFOBC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdObjectSearchFOBC;
				}
				else if(nIndex==eKeyenceCmdFPMI)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdObjectSearchFPMI;
				}
				else if(nIndex==eKeyenceCmdFBIS)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdACFInspectFBIS;
				}
				else if(nIndex==eKeyenceCmdFTGC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdTargetSearchFTGC;
				}
				else if(nIndex==eKeyenceCmdFSPC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdSetProductIndexFSPC;
				}
				else if(nIndex==eKeyenceCmdFCHB)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode && !n_ParamNumber)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}	  
					else if (ListNumber && ListNumber <m_vKeyenceCodeParam.at(it).P1.size())
					{
						cCodeParam.P1.at(0)= m_vKeyenceCodeParam.at(it).P1[ListNumber];
						cCodeParam.P2.at(0)= m_vKeyenceCodeParam.at(it).P2[ListNumber];
						cCodeParam.nP3.at(0)= m_vKeyenceCodeParam.at(it).nP3[ListNumber];
						cCodeParam.nP4.at(0)= m_vKeyenceCodeParam.at(it).nP4[ListNumber];
						cCodeParam.nP5.at(0)= m_vKeyenceCodeParam.at(it).nP5[ListNumber];
					}
					else
					{
						cCodeParam.P1.at(0)= m_vKeyenceCodeParam.at(it).P1[0];
						cCodeParam.P2.at(0)= m_vKeyenceCodeParam.at(it).P2[0];
						cCodeParam.nP3.at(0)= m_vKeyenceCodeParam.at(it).nP3[0];
						cCodeParam.nP4.at(0)= m_vKeyenceCodeParam.at(it).nP4[0];
						cCodeParam.nP5.at(0)= m_vKeyenceCodeParam.at(it).nP5[0];

					}

					return eCmdACFInspectFCHB;
				}
				else if(nIndex==eKeyenceCmdFALP)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdOnceAlignMovePickFALP;
				}
				else if(nIndex==eKeyenceCmdFAAC)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdAutoAlignFAAC;
				}
				else if(nIndex==eKeyenceCmdFMCO)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdGetTargetObjectOffsetFMCO;
				}
				else if(nIndex==eKeyenceCmdFCBP)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdCalibrateFCBP;
				}
				else if(nIndex==eKeyenceCmdFCBS)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdCalibrateFCBS;
				}
				else if(nIndex==eKeyenceCmdFAAL)
				{
					if(m_vKeyenceCodeParam.at(it).m_bEnableNonFixedCode)
					{
						cCodeParam.P1.at(0)= vnReceivedData.at(1);
						cCodeParam.P2.at(0)= vnReceivedData.at(2);
						cCodeParam.nP3.at(0)= vnReceivedData.at(3);
						cCodeParam.nP4.at(0)= vnReceivedData.at(4);
						cCodeParam.nP5.at(0)= vnReceivedData.at(5);
					}
					return eCmdAutoAlignFAAL;
				}
				break;
			}
		}

		return eCmdUnknow;
	}
}

int CCommProtocolXYDFast::GetReadDataNumEx()
{
	if(m_bRegType)
	{
		if(m_bReadWriteAxisPosWithCmd)
		{
			return 17;
		}
		else
		{
			return 7;
		}
	}
	else
	{
		if(m_bReadWriteAxisPosWithCmd)
		{
			return 16;
		}
		else
		{
			return 6;
		}
	}
}

BOOL CCommProtocolXYDFast::AnalyzeAxisInfoInCmdParamEx(std::vector<int> &vnReceivedData)
{
	if(!m_bReadWriteAxisPosWithCmd)
	{
		return FALSE;
	}
	if(m_bRegType)
	{
		int nDataNum = GetReadDataNumEx();
		if (vnReceivedData.size() >= nDataNum)
		{//如果参数个数大于等于10，解析轴位置
			CPlatformXYDAxisPos platformXYDAixisPos; 		
			std::vector<int> vnAxisPos;
			vnAxisPos.push_back(vnReceivedData[nDataNum - 3]);
			vnAxisPos.push_back(vnReceivedData[nDataNum - 2]);
			vnAxisPos.push_back(vnReceivedData[nDataNum - 1]);
			if (FALSE == ConvertToPlatformXYDAxisPos(vnAxisPos, &platformXYDAixisPos))
			{
				return FALSE;
			}
			m_CurPlatformXYDAxisPos = platformXYDAixisPos;
			m_bCmdAxisPosReady = TRUE;
			return TRUE;
		}
	}
	else
	{
		int nDataNum = GetReadDataNumEx();
		if (vnReceivedData.size() >= nDataNum)
		{//如果参数个数大于等于12，解析轴位置
			CPlatformXYDAxisPos platformXYDAixisPos; 		
			std::vector<int> vnAxisPos;
			vnAxisPos.push_back(WordToDoubleWord(vnReceivedData[nDataNum - 6],vnReceivedData[nDataNum - 5]));
			vnAxisPos.push_back(WordToDoubleWord(vnReceivedData[nDataNum - 4],vnReceivedData[nDataNum - 3]));
			vnAxisPos.push_back(WordToDoubleWord(vnReceivedData[nDataNum - 2],vnReceivedData[nDataNum - 1]));
			if (FALSE == ConvertToPlatformXYDAxisPos(vnAxisPos, &platformXYDAixisPos))
			{
				return FALSE;
			}
			m_CurPlatformXYDAxisPos = platformXYDAixisPos;
			m_bCmdAxisPosReady = TRUE;
			return TRUE;
		}
	}
	return FALSE;

}

SysWorkCmdType CCommProtocolXYDFast::AnalyzePlcSysWorkCmdReceivedEx_CodeWithParam( CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam)
{
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		bTriggered = FALSE;
		return eCmdType;
	}

	//双字和单字
	{
		// 解析字符串
		int  nDataNum = GetReadDataNumEx();												// 数据量
		std::vector<int> vnReceivedData;
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strCmd, nDataNum, vnReceivedData,m_bRegType))
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.size() != nDataNum)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		if (vnReceivedData.at(0) == PLC_CMD_VAL_NOT_TRIGGERED)
		{
			bTriggered = FALSE;
			return eCmdType;
		}

		SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);
		// 解析随轴一并读写
		AnalyzeAxisInfoInCmdParamEx(vnReceivedData);
		int nCode = vnReceivedData.at(0);

		for(int it=0;it<MaxCodeNums;it++)
		{
			if(nCode>=(it+1)*Code_Group_Max_Num && nCode<(it+2)*Code_Group_Max_Num)
			{
				bTriggered = TRUE;
				int nIndex = it+1;
				m_nCurCodeIndex = it;
				if(nIndex==eKeyenceCmdFACO)
				{
					// 对位指令
					
					cCodeParam.m_strCmd = _T("FACO");
					cCodeParam.m_nCmdVal = 1;
					cCodeParam.P1[0] = nCode-eKeyenceCmdGroupFACO;
					cCodeParam.P2[0] = 1;
					return eCmdGetTargetObjectOffsetFACO;
				}
				else if (nIndex==eKeyenceCmdCalibrate)
				{
					// 标定指令
					cCodeParam.m_strCmd = _T("FCLB");
					cCodeParam.m_nCmdVal = 1;
					int nP1Index = nCode-eKeyenceCmdGroupCalibrate;
					cCodeParam.m_nCmdVal = 2;
					
					// 个位数为P2 
					cCodeParam.P2[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P1
					cCodeParam.P1[0] = (nP1Index/1)%10;

					return eCmdCalibrateFCLB;
				}
				else if(nIndex==eKeyenceCmdFOBC)
				{
					cCodeParam.m_strCmd = _T("FOBC");
					cCodeParam.m_nCmdVal = 3;
					int nP1Index = nCode-eKeyenceCmdGroupFOBC;

					// 百十位数为P1  示例：3121；代表对象FOBC 12 1  3051代表FOBC 5 1;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdObjectSearchFOBC;
				}
				else if(nIndex==eKeyenceCmdFPMI)
				{
					cCodeParam.m_strCmd = _T("FPMI");
					cCodeParam.m_nCmdVal = 1;
					int nP1Index = nCode-eKeyenceCmdGroupFPMI;

					// 百十位数为P1  示例：3121；代表对象FOBC 12 1  3051代表FOBC 5 1;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdObjectSearchFPMI;
				}
				else if(nIndex==eKeyenceCmdFBIS)
				{
					cCodeParam.m_strCmd = _T("FBIS");
					cCodeParam.m_nCmdVal = 1;
					int nP1Index = nCode-eKeyenceCmdGroupFBIS;

					// 百十位数为P1  示例：3121；代表对象FOBC 12 1  3051代表FOBC 5 1;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdACFInspectFBIS;
				}
				else if(nIndex==eKeyenceCmdFTGC)
				{
					cCodeParam.m_strCmd = _T("FTGC");
					cCodeParam.m_nCmdVal = 4;
					int nP1Index = nCode-eKeyenceCmdGroupFTGC;
					// 百十位数为P1
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdTargetSearchFTGC;
				}
				else if(nIndex==eKeyenceCmdFSPC)
				{
					cCodeParam.m_strCmd = _T("FSPC");
					cCodeParam.m_nCmdVal = 5;
					int nP1Index = nCode-eKeyenceCmdGroupFSPC;
					// 百十位数为P1
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdSetProductIndexFSPC;
				}
				else if(nIndex==eKeyenceCmdFCHB)
				{
					cCodeParam.m_strCmd = _T("FCHB");
					cCodeParam.m_nCmdVal = 6;
					int nP1Index = nCode-eKeyenceCmdGroupFCHB;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;

					return eCmdACFInspectFCHB;
				}
				else if(nIndex==eKeyenceCmdFALP)
				{
					cCodeParam.m_strCmd = _T("FALP");
					cCodeParam.m_nCmdVal = 7;
					int nP1Index = nCode-eKeyenceCmdGroupFALP;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdOnceAlignMovePickFALP;
				}
				else if(nIndex==eKeyenceCmdFAAC)
				{
					cCodeParam.m_strCmd = _T("FAAC");
					cCodeParam.m_nCmdVal = 8;
					int nP1Index = nCode-eKeyenceCmdGroupFAAC;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdAutoAlignFAAC;
				}
				else if(nIndex==eKeyenceCmdFMCO)
				{
					cCodeParam.m_strCmd = _T("FMCO");
					cCodeParam.m_nCmdVal = 9;
					int nP1Index = nCode-eKeyenceCmdGroupFMCO;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P2
					cCodeParam.P2[0] = (nP1Index/1)%10;
					return eCmdGetTargetObjectOffsetFMCO;
				}
				else if(nIndex==eKeyenceCmdFCBP)
				{
					cCodeParam.m_strCmd = _T("FCBP");
					cCodeParam.m_nCmdVal = 10;
					int nP1Index = nCode-eKeyenceCmdGroupFCBP;
					cCodeParam.P2[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					// 个位数为P1
					cCodeParam.P1[0] = (nP1Index/1)%10;
					return eCmdCalibrateFCBP;
				}
				else if(nIndex==eKeyenceCmdFCBS)
				{
					cCodeParam.m_strCmd = _T("FCBS");
					cCodeParam.m_nCmdVal = 10;
					int nP1Index = nCode-eKeyenceCmdGroupFCBS;
					cCodeParam.P1[0] = ((nP1Index/100)%10)*10+((nP1Index/10)%10);
					return eCmdCalibrateFCBP;
					return eCmdCalibrateFCBS;
				}
				break;
			}
		}

		return eCmdUnknow;
	}
}


BOOL CCommProtocolXYDFast::PackRePLCBaseFunc(CString& strInfo,int nResult)
{
	CString strHead = m_strHead;

	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[1];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = 0;
	nParam = nResult;

	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
	//构造写命令
	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0] + strTail;
	return TRUE;

}

BOOL CCommProtocolXYDFast::PackRePLCBaseFunc2(CString& strInfo,int nResult,vector<int> DetectionResult)
{
	CString strHead = m_strHead;

	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[1];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = 0;
	nParam = nResult;

	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
	//构造写命令
	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0]/* + strTail*/;

	for (int i = 0;i < DetectionResult.size();++i)
	{
		PackParam(strParam[0], DetectionResult[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
		strInfo = strInfo + strBlank + strParam[0];
	}

	strInfo += strTail;

	return TRUE;

}

// 打包获取PLC动作指令或响应
BOOL CCommProtocolXYDFast::PackPlcReadSysWorkCmdEx( CString& strInfo)
{

	// 读取寄存器D50
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}
	CString strPlcRead;
	if (m_bRegType)			// 双字节
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER);		// 寄存器地址
		int  nDataNum = /*7*/GetReadDataNumEx();												// 数据量
		if (!m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr, nDataNum))
		{
			return FALSE;
		}
		SetReadRegTranslateResultInfo(nRegAdrr,nDataNum,TRUE);	
	}
	else					// 单字节
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER);		// 寄存器地址
		int  nDataNum = /*6*/GetReadDataNumEx();												// 数据量
		if (!m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr, nDataNum, FALSE))
		{
			return FALSE;
		}
		SetReadRegTranslateResultInfo(nRegAdrr,nDataNum,FALSE);
	}


	return TRUE;
}

BOOL CCommProtocolXYDFast::PackPlcReadSysWorkCmdEx2( CString& strInfo,int nOffset)
{
	/// 第二段地址读取
	// 读取寄存器D50
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}
	CString strPlcRead;
	if (m_bRegType)			// 双字节
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER) +nOffset;		// 寄存器地址
		int  nDataNum = /*7*/GetReadDataNumEx();												// 数据量
		if (!m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr, nDataNum))
		{
			return FALSE;
		}
		SetReadRegTranslateResultInfo(nRegAdrr,nDataNum,TRUE);	
	}
	else					// 单字节
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER)+nOffset;		// 寄存器地址
		int  nDataNum = /*6*/GetReadDataNumEx();												// 数据量
		if (!m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr, nDataNum, FALSE))
		{
			return FALSE;
		}
		SetReadRegTranslateResultInfo(nRegAdrr,nDataNum,FALSE);
	}


	return TRUE;
}

BOOL CCommProtocolXYDFast::PackPlcWriteSetPlcCmdTriggerEx( CString& strInfo, BOOL bTriggered ,int nOffSet)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}
	// 寄存器D50置0
	if (m_bRegType)
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER) + nOffSet;		// 寄存器地址
		int  nDataNum = 1;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);
		vnWriteData.at(0) = bTriggered ? PLC_CMD_VAL_TRIGGERED : PLC_CMD_VAL_NOT_TRIGGERED;

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData))
		{
			return FALSE;
		}


		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);

	}
	else
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER);		// 寄存器地址
		int  nDataNum = 1;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);
		vnWriteData.at(0) = bTriggered ? PLC_CMD_VAL_TRIGGERED : PLC_CMD_VAL_NOT_TRIGGERED;

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData,FALSE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,FALSE);

	}

	return TRUE;
}

BOOL CCommProtocolXYDFast::PackWriteBendPressPara(CString& strInfo,long lAddre,std::vector<int> vnDtata)	
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, lAddre, vnDtata.size(), vnDtata,TRUE))
	{
		return FALSE;
	}

	SetWriteRegTranslateResultInfo(lAddre,vnDtata.size(),vnDtata,TRUE);

	return TRUE;
}

BOOL CCommProtocolXYDFast::PackWritePLCFunc(CString& strInfo,int nResult,int nOffSet)
{

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	int nOffSetTmp = nOffSet;
	if (m_bRegType && (nOffSet&1))
	{
		// 双字时奇数偏移量处理
		nOffSetTmp=nOffSet+1;
	}
	


	//if (m_bRegType)
	{
		int nParam = 0;
		// D2 + nOffSet 中写结果
		long nRegAdrr = m_nPLCBaseRegAddr + nOffSetTmp;	// 寄存器地址
		int  nDataNum = 1;											// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);

		nParam = nResult;
		PackPlcParam(vnWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData,m_bRegType))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,m_bRegType);

	}
	//else
	//{
	//	int nParam = 0;
	//	// D2 + nOffSet 中写结果 nResult
	//	long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_PLC_CMD_DATA) + nOffSet;	// 寄存器地址
	//	int  nDataNum = 1;												// 数据量
	//	std::vector<int> vnWriteData;
	//	vnWriteData.resize(nDataNum);


	//	nParam = nResult;
	//	PackPlcParam(vnWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

	//	if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData,FALSE))
	//	{
	//		return FALSE;
	//	}
	//	SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,FALSE);


	//}

	return TRUE;

}

//BOOL CCommProtocolXYDFast::PackWritePLCFuncInspect(CString& strInfo,vector<double> nResult,int nOffSet/*=0*/)
//{
//
//	if (m_pCommProRegBase == NULL)
//	{
//		return FALSE;
//	}
//
//	int nOffSetTmp = nOffSet;
//	if (m_bRegType && (nOffSet&1))
//	{
//		// 双字时奇数偏移量处理
//		nOffSetTmp=nOffSet+1;
//	}
//
//	//if (m_bRegType)
//	{
//		double nParam = 0;
//		// D2 + nOffSet 中写结果
//		long nRegAdrr = m_nPLCBaseRegAddr + nOffSetTmp;	// 寄存器地址
//		int  nDataNum = nResult.size()+1;											// 数据量
//		std::vector<int> vnWriteData;
//		vnWriteData.resize(nDataNum);
//
//		for (int i=0;i<nResult.size();i++)
//		{
//			nParam = nResult.at(i);
//			PackPlcParam(vnWriteData[i], nParam, m_CommProtocolParam.m_nCommXUnitExp);
//		}
//
//		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData,m_bRegType))
//		{
//			return FALSE;
//		}
//
//		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,m_bRegType);
//
//	}
//
//	return TRUE;
//
//}

//
BOOL CCommProtocolXYDFast::PackWritePLCWithDetectionResultFunc(CString& strInfo,int nResult,vector<double> DetectionResult,int nOffSet)
{

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	int nOffSetTmp = nOffSet;
	if (m_bRegType && (nOffSet&1))
	{
		// 双字时奇数偏移量处理
		nOffSetTmp=nOffSet+1;
	}
	int nDataNum=DetectionResult.size()+1;
	long nRegAdrr = m_nPLCBaseRegAddr + nOffSetTmp;	// 寄存器地址
	std::vector<int> vnWriteData;
	vnWriteData.resize(nDataNum);
	//数据
	double dParam = 0;
	dParam = nResult;
	PackPlcParam(vnWriteData[0], nResult, m_CommProtocolParam.m_nCommUnitExp);
	for (int i=0;i<DetectionResult.size();i++)
	{
		dParam = DetectionResult.at(i);
		PackPlcParam(vnWriteData[i+1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
	}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData,TRUE))
		{
			return FALSE;
		}

	SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackWritePLCWithDetectionResultFunc2(CString& strInfo,int nResult,vector<int> DetectionResult,int nOffSet)
{

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	int nOffSetTmp = nOffSet;
	if (m_bRegType && (nOffSet&1))
	{
		// 双字时奇数偏移量处理
		nOffSetTmp=nOffSet+1;
	}
	int nDataNum=DetectionResult.size()+1;
	long nRegAdrr = m_nPLCBaseRegAddr + nOffSetTmp;	// 寄存器地址
	std::vector<int> vnWriteData;
	vnWriteData.resize(nDataNum);
	//数据
	int nParam = 0;
	nParam = nResult;
	PackPlcParam(vnWriteData[0], nResult, m_CommProtocolParam.m_nCommUnitExp);
	for (int i=0;i<DetectionResult.size();i++)
	{
		nParam = DetectionResult.at(i);
		PackPlcParam(vnWriteData[i+1], nParam, m_CommProtocolParam.m_nCommUnitExp);
	}

	if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData,FALSE))
	{
		return FALSE;
	}

	SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,FALSE);

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackPlcWriteSetManualSearch(CString& strInfo, BOOL bShow,int nOffSet)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	if (m_bRegType)
	{
		long nRegAdrr = m_nPLCBaseRegAddr + nOffSet;		// 寄存器地址
		int  nDataNum = 1;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);
		vnWriteData.at(0) = bShow ? PLC_CMD_VAL_TRIGGERED : PLC_CMD_VAL_NOT_TRIGGERED;

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);

	}
	else
	{
		long nRegAdrr = m_nPLCBaseRegAddr + nOffSet;		// 寄存器地址
		int  nDataNum = 1;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);
		vnWriteData.at(0) = bShow ? PLC_CMD_VAL_TRIGGERED : PLC_CMD_VAL_NOT_TRIGGERED;

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData,FALSE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,FALSE);

	}

	return TRUE;
}


BOOL CCommProtocolXYDFast::PackReadPLCFunc(CString& strInfo,int nDataNums,int nOffSet,BOOL bAxisGet)
{
	// 读取寄存器 PLC寄存器 +  + nOffSet开始的nDataNums个数据
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}
	
	int nOffSetTmp = nOffSet;
	if (m_bRegType && (nOffSet&1))
	{
		// 双字时奇数偏移量处理
		nOffSetTmp=nOffSet+1;
	}

	long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_PLC_TRIGGER) + nOffSetTmp;		// 寄存器地址
	int  nDataNum = nDataNums;													// 数据量

	m_strCmdName = _T("");

	BOOL bDouble =FALSE;
	// 双字以及读取轴数据时，需要读取双字数据
	bDouble = (m_bRegType || bAxisGet);

	if (!m_pCommProRegBase->PackReadRegInfo(strInfo, nRegAdrr, nDataNum,bDouble))
	{
		return FALSE;
	}

	SetReadRegTranslateResultInfo(nRegAdrr,nDataNum,bDouble);
	
	return TRUE;
}

BOOL CCommProtocolXYDFast::PackPlcWriteDrivePlatformMoveToAbsPosEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,int nOffSet)
{
	if (NULL == vpPlatAxisPos)
	{
		return eAswUnkown;
	}

	if (vpPlatAxisPos->GetPlatformType()!=ePlatformXYD)
	{
		return eAswUnkown;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	
	{
		CString strName = m_MMVAParam.m_strCmd;
		int nCmdVal = m_MMVAParam.m_nCmdVal;
		int nParam = 0;

		m_strCmdName = strName;	
		// 向D10--14中写入
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_VISION_AXIS_READ) + nOffSet;	// 寄存器地址
		int  nDataNum = 3;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);

		m_strCmdName = strName;	

		CPlatformXYDAxisPos* pPlatformXYDAixisPos; 		
		pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);

		std::vector<int> vnUnitExp;
		vnUnitExp.resize(nDataNum);


		std::vector<double> vdParam;
		vdParam.resize(nDataNum);
		//if (eCPOrderYXD == m_eCmdParamOrderType)
		//{
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}
		//else
		//{
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}

		CCoordPos tmpOffset;
		tmpOffset.m_dPosX = pPlatformXYDAixisPos->m_dPosX;
		tmpOffset.m_dPosY = pPlatformXYDAixisPos->m_dPosY;
		tmpOffset.m_dAngle = pPlatformXYDAixisPos->m_dAngle;
		std::vector<int> vnTrans;
		vnTrans.resize(nDataNum);
		ConvertToPlatformXYDAxisPos_Output(tmpOffset,vdParam,vnTrans,vnUnitExp);

		if (m_pMidPlatformInfo == NULL)
		{
			for(int it=0;it<3;it++)
			{
				PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
			}
		}
		else
		{
			switch(m_eMidPlatformType)
			{
			case ePlatformXYD:
			case ePlatformXYPD:
			case ePlatformXD:
			case ePlatformYD:
			case ePlatformXY:
			case ePlatformX:
			case ePlatformXDPY:
			case ePlatformXDPY1:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}
				break;
			case ePlatformXY1Y2:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{
						double dX1 = 0.0;
						double dY1 = 0.0;
						double dY2 = 0.0;

						double dX0 = pPlatformXYDAixisPos->m_dPosX;
						double dY0 = pPlatformXYDAixisPos->m_dPosY;
						double dD0 = pPlatformXYDAixisPos->m_dAngle;
						CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(dX0, dY0, dD0, dY1, dY2, dX1);

						CCoordPos tmpOffsetX1Y1Y2;
						tmpOffsetX1Y1Y2.m_dPosX = dX1;
						tmpOffsetX1Y1Y2.m_dPosY = dY1;
						tmpOffsetX1Y1Y2.m_dAngle = dY2;

						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						//double dParam = 0;
						//dParam = dX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dY1;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//dParam = dY2;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);
					}
					else
					{
						return FALSE;
					}
				}
				break;
			case ePlatformX1X2Y:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{
						double dX1 = 0.0;
						double dX2 = 0.0;
						double dY1 = 0.0;

						double dX0 = pPlatformXYDAixisPos->m_dPosX;
						double dY0 = pPlatformXYDAixisPos->m_dPosY;
						double dD0 = pPlatformXYDAixisPos->m_dAngle;
						CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(dX0, dY0, dD0, dX1, dX2, dY1);

						//dX1 = dX1 + 5.5;
						//dX2 = dX2 + 5.5;
						//dY1 = dY1 + 5.5;
						CCoordPos tmpOffsetX1X2Y1;
						tmpOffsetX1X2Y1.m_dPosX = dX1;
						tmpOffsetX1X2Y1.m_dPosY = dX2;
						tmpOffsetX1X2Y1.m_dAngle = dY1;

						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						//double dParam = 0;
						//dParam = dX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dX2;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dY1;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);
					}
					else
					{
						return FALSE;
					}
				}
				break;
			default:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}					
			}
		}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);
		return TRUE;
	}
	return FALSE;
}

CmdAnsweredStatus CCommProtocolXYDFast::AnalyzeAswPlcDrivePlatformMoveCompletedEx(CString strAsw)
{
	BOOL bTriggered = FALSE;
	SysWorkCmdType eCmdType = eCmdUnknow;
	if (m_pCommProRegBase == NULL)
	{
		return eAswCmdErr;
	}

	int  nDataNum = 1;												// 数据量
	std::vector<int> vnReceivedData;

	if (m_bRegType)
	{
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strAsw, nDataNum, vnReceivedData))
		{
			return eAswCmdErr;
		}
	}
	else
	{
		if (!m_pCommProRegBase->AnalyzeAswReadReg(strAsw, nDataNum, vnReceivedData,FALSE))
		{
			return eAswCmdErr;
		}
	}

	if (vnReceivedData.size() != nDataNum)
	{
		return eAswCmdErr;
	}

	if (vnReceivedData.at(0) != PLC_CMD_VAL_TRIGGERED)
	{
		return eAswCmdErr;
	}
	else if(vnReceivedData.at(0) == PLC_CMD_VAL_TRIGGERED)
	{
		SetAnalyzeAswReadRegTransInfo(nDataNum,vnReceivedData,m_bRegType);
		return eAswSucceed;
	}
	else
	{
		return eAswCmdErr;
	}

	return eAswCmdErr;
}

BOOL CCommProtocolXYDFast::PackReAlignResultInfo_NoProtocol( CString& strInfo, CCoordPos platOffset,int nResult, CPlatformAxisPos* vpPlatAxisPos)
{

	CString strHead = m_strHead;

	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[4];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = nResult;

	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	CPlatformXYDAxisPos* pPlatformXYDAixisPos;
	pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);

	int nDataNum = 3;
	std::vector<int> vnUnitExp;
	vnUnitExp.resize(nDataNum);

	std::vector<int> vnTrans;
	vnTrans.resize(nDataNum);

	std::vector<double> vdParam;
	vdParam.resize(nDataNum);
	//if (eCPOrderYXD == m_eCmdParamOrderType)
	//{
	//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommXUnitExp;
	//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommYUnitExp;
	//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;
	//	
	//	vdParam.at(1)= platOffset.m_dPosX;
	//	vdParam.at(0)= platOffset.m_dPosY;
	//	vdParam.at(2)= platOffset.m_dAngle;
	//}
	//else
	//{
	//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommXUnitExp;
	//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommYUnitExp;
	//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

	//	vdParam.at(0)= platOffset.m_dPosX;
	//	vdParam.at(1)= platOffset.m_dPosY;
	//	vdParam.at(2)= platOffset.m_dAngle;
	//}

	ConvertToPlatformXYDAxisPos_Output(platOffset,vdParam,vnTrans,vnUnitExp);

	if (pPlatformXYDAixisPos == NULL || m_pMidPlatformInfo == NULL )
	{
		for(int it=0;it<3;it++)
		{
			PackParam(strParam[it+1], vdParam[it],m_CommProtocolParam.m_nCommDataLenType,vnUnitExp[it]);
		}

	}
	else
	{
		switch(m_eMidPlatformType)
		{
		case ePlatformXYD:
		case ePlatformXYPD:
		case ePlatformXD:
		case ePlatformYD:
		case ePlatformXY:
		case ePlatformX:
		case ePlatformXDPY:
		case ePlatformXDPY1:
			{
				for(int it=0;it<3;it++)
				{
					PackParam(strParam[it+1], vdParam[it],m_CommProtocolParam.m_nCommDataLenType,vnUnitExp[it]);
				}				
			}
			break;
		case ePlatformXY1Y2:
			{
				if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
				{
					double dToStartX1 = 0.0;
					double dToStartY1 = 0.0;
					double dToStartY2 = 0.0;

					double dToEndX1 = 0.0;
					double dToEndY1 = 0.0;
					double dToEndY2 = 0.0;

					double dFromStartX0 = pPlatformXYDAixisPos->m_dPosX - platOffset.m_dPosX;
					double dFromStartY0 = pPlatformXYDAixisPos->m_dPosY - platOffset.m_dPosY;
					double dFromStartD0 = pPlatformXYDAixisPos->m_dAngle - platOffset.m_dAngle;

					double dFromEndX0 = pPlatformXYDAixisPos->m_dPosX;
					double dFromEndY0 = pPlatformXYDAixisPos->m_dPosY;
					double dFromEndD0 = pPlatformXYDAixisPos->m_dAngle;

					CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

					pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
						dFromStartX0, dFromStartY0, dFromStartD0, dToStartY1, dToStartY2, dToStartX1);

					pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
						dFromEndX0, dFromEndY0, dFromEndD0, dToEndY1, dToEndY2, dToEndX1);

					CCoordPos tmpOffsetX1Y1Y2;
					tmpOffsetX1Y1Y2.m_dPosX = dToEndX1 - dToStartX1;
					tmpOffsetX1Y1Y2.m_dPosY = dToEndY1 - dToStartY1;
					tmpOffsetX1Y1Y2.m_dAngle = dToEndY2 - dToStartY2;

					ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

					for(int it=0;it<3;it++)
					{
						PackParam(strParam[1+it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
					}

					//double dParam = 0;
					//dParam = dToEndX1 - dToStartX1;

					//PackParam(strParam[1], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//dParam = dToEndY1 - dToStartY1;
					//PackParam(strParam[2], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
					//dParam = dToEndY2 - dToStartY2;
					//PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
				}
				else
				{
					return FALSE;
				}
			}
			break;
		case ePlatformX1X2Y:
			{
				if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
				{
					double dToStartX1 = 0.0;
					double dToStartX2 = 0.0;
					double dToStartY1 = 0.0;

					double dToEndX1 = 0.0;
					double dToEndX2 = 0.0;
					double dToEndY1 = 0.0;

					double dFromStartX0 = pPlatformXYDAixisPos->m_dPosX - platOffset.m_dPosX;
					double dFromStartY0 = pPlatformXYDAixisPos->m_dPosY - platOffset.m_dPosY;
					double dFromStartD0 = pPlatformXYDAixisPos->m_dAngle - platOffset.m_dAngle;

					double dFromEndX0 = pPlatformXYDAixisPos->m_dPosX;
					double dFromEndY0 = pPlatformXYDAixisPos->m_dPosY;
					double dFromEndD0 = pPlatformXYDAixisPos->m_dAngle;

					CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

					pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
						dFromStartX0, dFromStartY0, dFromStartD0, dToStartX1, dToStartX2, dToStartY1);
					pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
						dFromEndX0, dFromEndY0, dFromEndD0, dToEndX1, dToEndX2, dToEndY1);

					//dX1 = dX1 + 5.5;
					//dX2 = dX2 + 5.5;
					//dY1 = dY1 + 5.5;

					CCoordPos tmpOffsetX1X2Y1;
					tmpOffsetX1X2Y1.m_dPosX = dToEndX1 - dToStartX1;
					tmpOffsetX1X2Y1.m_dPosY = dToEndX2 - dToStartX2;
					tmpOffsetX1X2Y1.m_dAngle = dToEndY1 - dToStartY1;

					ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

					for(int it=0;it<3;it++)
					{
						PackParam(strParam[1+it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
					}
					//double dParam = 0;
					//dParam = dToEndX1 - dToStartX1;
					//PackParam(strParam[1], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//dParam = dToEndX2 - dToStartX2;
					//PackParam(strParam[2], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//dParam = dToEndY1 - dToStartY1;
					//PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
				}
				else
				{
					return FALSE;
				}
			}
			break;
		default:
			{
				for(int it=0;it<3;it++)
				{
					PackParam(strParam[it+1], vdParam[it],m_CommProtocolParam.m_nCommDataLenType,vnUnitExp[it]);
				}
			}
		}
	}

	//构造写命令
	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0] + strBlank + strParam[1] + strBlank + strParam[2] + strBlank + strParam[3] + strTail;

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackReMulAlignPosInfo_Ex(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult)
{
	int nTempNum = vpPlatformXYDOffset.size();

	CString strHead = m_strHead;

	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[6];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = nResult;

	// strParam[0] : 成功、失败
	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	// strParam[1] : 总数
	nParam = nTempNum;
	PackParam(strParam[1], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	std::vector<CString> vsWriteData[4];
	vsWriteData[0].resize(nTempNum);	// 索引
	vsWriteData[1].resize(nTempNum);	// X数据
	vsWriteData[2].resize(nTempNum);	// Y数据
	vsWriteData[3].resize(nTempNum);	// D数据

	CCoordPos pPlatformXYDAixisOffset; 		

	std::vector<double> vdParam;
	vdParam.resize(3);
	std::vector<int> vnTrans;
	vnTrans.resize(3);
	std::vector<int> vnUnitExp;
	vnUnitExp.resize(3);

	for (int i = 0; i < nTempNum; i++)
	{
		pPlatformXYDAixisOffset = vpPlatformXYDOffset.at(i).first;
		int nPos = vpPlatformXYDOffset.at(i).second;

		ConvertToPlatformXYDAxisPos_Output(pPlatformXYDAixisOffset,vdParam,vnTrans,vnUnitExp);

		if (m_pMidPlatformInfo == NULL)
		{
			//if (eCPOrderYXD == m_eCmdParamOrderType)
			//{
			//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
			//	double dParam;
			//	dParam = pPlatformXYDAixisOffset.m_dPosX;
			//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
			//	dParam = pPlatformXYDAixisOffset.m_dPosY;
			//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
			//	dParam = pPlatformXYDAixisOffset.m_dAngle;
			//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

			//	vsWriteData[0].at(i) = strParam[2];
			//	vsWriteData[2].at(i) = strParam[4];
			//	vsWriteData[1].at(i) = strParam[3];
			//	vsWriteData[3].at(i) = strParam[5];

			//}
			//else
			//{
			//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
			//	double dParam = 0;
			//	dParam = pPlatformXYDAixisOffset.m_dPosX;
			//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
			//	dParam = pPlatformXYDAixisOffset.m_dPosY;
			//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
			//	dParam = pPlatformXYDAixisOffset.m_dAngle;
			//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

			//	vsWriteData[0].at(i) = strParam[2];
			//	vsWriteData[1].at(i) = strParam[3];
			//	vsWriteData[2].at(i) = strParam[4];
			//	vsWriteData[3].at(i) = strParam[5];
			//}

			PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

			PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
			PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
			PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);

			vsWriteData[0].at(i) = strParam[2];
			vsWriteData[1].at(i) = strParam[3];
			vsWriteData[2].at(i) = strParam[4];
			vsWriteData[3].at(i) = strParam[5];
		}
		else
		{
			switch(m_eMidPlatformType)
			{
			case ePlatformXYD:
			case ePlatformXD:
			case ePlatformYD:
			case ePlatformXY:
				{
					//if (eCPOrderYXD == m_eCmdParamOrderType)
					//{
					//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisOffset.m_dPosX;
					//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dPosY;
					//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dAngle;
					//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

					//	vsWriteData[0].at(i) = strParam[2];
					//	vsWriteData[2].at(i) = strParam[4];
					//	vsWriteData[1].at(i) = strParam[3];
					//	vsWriteData[3].at(i) = strParam[5];

					//}
					//else
					//{
					//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisOffset.m_dPosX;
					//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dPosY;
					//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dAngle;
					//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

					//	vsWriteData[0].at(i) = strParam[2];
					//	vsWriteData[1].at(i) = strParam[3];
					//	vsWriteData[2].at(i) = strParam[4];
					//	vsWriteData[3].at(i) = strParam[5];

					//}

					PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

					PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
					PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
					PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);

					vsWriteData[0].at(i) = strParam[2];
					vsWriteData[1].at(i) = strParam[3];
					vsWriteData[2].at(i) = strParam[4];
					vsWriteData[3].at(i) = strParam[5];
				}
				break;
			default:
				{
					//if (eCPOrderYXD == m_eCmdParamOrderType)
					//{
					//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisOffset.m_dPosX;
					//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dPosY;
					//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dAngle;
					//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

					//	vsWriteData[0].at(i) = strParam[2];
					//	vsWriteData[2].at(i) = strParam[4];
					//	vsWriteData[1].at(i) = strParam[3];
					//	vsWriteData[3].at(i) = strParam[5];

					//}
					//else
					//{
					//	PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisOffset.m_dPosX;
					//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dPosY;
					//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisOffset.m_dAngle;
					//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);

					//	vsWriteData[0].at(i) = strParam[2];
					//	vsWriteData[1].at(i) = strParam[3];
					//	vsWriteData[2].at(i) = strParam[4];
					//	vsWriteData[3].at(i) = strParam[5];

					//}

					PackParam(strParam[2], nPos+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

					PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
					PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
					PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);

					vsWriteData[0].at(i) = strParam[2];
					vsWriteData[1].at(i) = strParam[3];
					vsWriteData[2].at(i) = strParam[4];
					vsWriteData[3].at(i) = strParam[5];
				}

			}
		}
	}

	CString strTempInfo = _T("");
	for (int i = 0; i < nTempNum; i++)
	{
		CString strTemp = strBlank + vsWriteData[0].at(i) + strBlank + vsWriteData[1].at(i) + strBlank + vsWriteData[2].at(i) + strBlank + vsWriteData[3].at(i);
		strTempInfo = strTempInfo + strTemp;
	}

	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0] + strBlank + strParam[1] + strTempInfo + strTail;
	return TRUE;

}

BOOL CCommProtocolXYDFast::PackRePlcMulAlignPosResultInfoAxisPos_Ex(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	BOOL bResult = (nResult==1)? TRUE:FALSE;

	// 单字双字
	{
		CString strName= _T("Fxxx");
		if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
		{
			strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
		}

		int nCmdVal = m_FMCOParam.m_nCmdVal;

		m_strCmdName = strName;	
		int nParam = 0;

		int nTempDataNum = vpPlatformXYDOffset.size();
		long nRegAdrr = m_nPLCTempBaseRegAddr;

		int  nDataNum = (nTempDataNum * 4) + 1;	

		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);

		// 总数
		nParam = nTempDataNum;
		PackPlcParam(vnWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

		std::vector<double> vdParam;
		vdParam.resize(3);
		std::vector<int> vnTrans;
		vnTrans.resize(3);
		std::vector<int> vnUnitExp;
		vnUnitExp.resize(3);

		for (int i = 0; i < nTempDataNum; i++)
		{
			CCoordPos pPlatformXYDAixisOffset = vpPlatformXYDOffset.at(i).first;	
			int nPos = vpPlatformXYDOffset.at(i).second;

			ConvertToPlatformXYDAxisPos_Output(pPlatformXYDAixisOffset,vdParam,vnTrans,vnUnitExp);

			if (bResult == FALSE || m_pMidPlatformInfo == NULL)
			{
				//if (eCPOrderYXD == m_eCmdParamOrderType)
				//{
				//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
				//	double dParam = 0;
				//	dParam = pPlatformXYDAixisOffset.m_dPosX;
				//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
				//	dParam = pPlatformXYDAixisOffset.m_dPosY;
				//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
				//	dParam = pPlatformXYDAixisOffset.m_dAngle;
				//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

				//}
				//else
				//{
				//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
				//	double dParam = 0;
				//	dParam = pPlatformXYDAixisOffset.m_dPosX;
				//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
				//	dParam = pPlatformXYDAixisOffset.m_dPosY;
				//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
				//	dParam = pPlatformXYDAixisOffset.m_dAngle;
				//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
				//}

				PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
				PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
				PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
				PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);

			}
			else
			{
				switch(m_eMidPlatformType)
				{
				case ePlatformXYD:
				case ePlatformXD:
				case ePlatformYD:
				case ePlatformXY:
					{
						//if (eCPOrderYXD == m_eCmdParamOrderType)
						//{
						//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisOffset.m_dPosX;
						//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dPosY;
						//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dAngle;
						//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

						//}
						//else
						//{
						//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisOffset.m_dPosX;
						//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dPosY;
						//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dAngle;
						//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

						//}
						PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
						PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
						PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);
					}
					break;

				default:
					{
						//if (eCPOrderYXD == m_eCmdParamOrderType)
						//{
						//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisOffset.m_dPosX;
						//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dPosY;
						//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dAngle;
						//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
						//}
						//else
						//{
						//	PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisOffset.m_dPosX;
						//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dPosY;
						//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisOffset.m_dAngle;
						//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

						//}
						PackPlcParam(vnWriteData[1 + i*4], nPos+1, m_CommProtocolParam.m_nCommUnitExp);
						PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
						PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
						PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);
					}
				}
			}

		}
		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData, TRUE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);

		return TRUE;
	}

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackReMulAlignPosInfoFALP_Ex(CString& strInfo,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult,int nPickMutiExMode/* = 0*/)
{
	int nExNum = vvPlatformXYDAxisPos.size();
	int nTempNum = 0;
	int nRealTempNum;
	if(nExNum > 0)
	{
		nTempNum = vvPlatformXYDAxisPos[0].size();
		nRealTempNum = nTempNum;
	}

	CString strHead = m_strHead;
	CString strName= m_FALPParam.m_strCmd;
	CString strParam[6];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = 0;
	nParam = nResult;

	// strParam[0] : 成功、失败
	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	// strParam[1] : 总数
	nParam = nTempNum;
	PackParam(strParam[1], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	std::vector<std::vector<CString>> vvsWriteData[4];
	vvsWriteData[0].resize(nExNum);	// 索引
	vvsWriteData[1].resize(nExNum);	// X数据
	vvsWriteData[2].resize(nExNum);	// Y数据
	vvsWriteData[3].resize(nExNum);	// D数据

	std::vector<double> vdParam;
	vdParam.resize(3);
	std::vector<int> vnTrans;
	vnTrans.resize(3);
	std::vector<int> vnUnitExp;
	vnUnitExp.resize(3);

	CPlatformXYDAxisPos pPlatformXYDAixisPos; 		
	for(int nExIndex=0; nExIndex < nExNum; nExIndex++)
	{
		vvsWriteData[0][nExIndex].resize(nTempNum);
		vvsWriteData[1][nExIndex].resize(nTempNum);
		vvsWriteData[2][nExIndex].resize(nTempNum);
		vvsWriteData[3][nExIndex].resize(nTempNum);
		if(nPickMutiExMode > 200 && nPickMutiExMode < 300)
		{
			int nRealTempNum = nPickMutiExMode % 100;
			vvsWriteData[0][nExIndex].resize(nRealTempNum, _T("0"));
			vvsWriteData[1][nExIndex].resize(nRealTempNum, _T("0"));
			vvsWriteData[2][nExIndex].resize(nRealTempNum, _T("0"));
			vvsWriteData[3][nExIndex].resize(nRealTempNum, _T("0"));
		}
		for (int i = 0; i < nTempNum; i++)
		{
			pPlatformXYDAixisPos = (CPlatformXYDAxisPos)(vvPlatformXYDAxisPos.at(nExIndex).at(i));	

			CCoordPos pPlatformXYDAixisOffset;
			pPlatformXYDAixisOffset.m_dPosX = pPlatformXYDAixisPos.m_dPosX;
			pPlatformXYDAixisOffset.m_dPosY = pPlatformXYDAixisPos.m_dPosY;
			pPlatformXYDAixisOffset.m_dAngle = pPlatformXYDAixisPos.m_dAngle;

			ConvertToPlatformXYDAxisPos_Output(pPlatformXYDAixisOffset,vdParam,vnTrans,vnUnitExp);

			if (m_pMidPlatformInfo == NULL)
			{
				//if (eCPOrderYXD == m_eCmdParamOrderType)
				//{
				//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
				//	double dParam;
				//	dParam = pPlatformXYDAixisPos.m_dPosX;
				//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
				//	dParam = pPlatformXYDAixisPos.m_dPosY;
				//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
				//	dParam = pPlatformXYDAixisPos.m_dAngle;
				//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
				//	vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
				//	vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
				//	vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
				//	vvsWriteData[3].at(nExIndex).at(i) = strParam[5];

				//}
				//else
				//{
				//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
				//	double dParam = 0;
				//	dParam = pPlatformXYDAixisPos.m_dPosX;
				//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
				//	dParam = pPlatformXYDAixisPos.m_dPosY;
				//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
				//	dParam = pPlatformXYDAixisPos.m_dAngle;
				//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
				//	vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
				//	vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
				//	vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
				//	vvsWriteData[3].at(nExIndex).at(i) = strParam[5];
				//}

				PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
				PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
				PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
				PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);
				
				vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
				vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
				vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
				vvsWriteData[3].at(nExIndex).at(i) = strParam[5];

			}
			else
			{
				switch(m_eMidPlatformType)
				{
				case ePlatformXYD:
				case ePlatformXD:
				case ePlatformYD:
				case ePlatformXY:
					{
						//if (eCPOrderYXD == m_eCmdParamOrderType)
						//{
						//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisPos.m_dPosX;
						//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dPosY;
						//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dAngle;
						//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
						//	vvsWriteData[0][nExIndex].at(i) = strParam[2];
						//	vvsWriteData[2][nExIndex].at(i) = strParam[4];
						//	vvsWriteData[1][nExIndex].at(i) = strParam[3];
						//	vvsWriteData[3][nExIndex].at(i) = strParam[5];

						//}
						//else
						//{
						//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisPos.m_dPosX;
						//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dPosY;
						//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dAngle;
						//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
						//	vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
						//	vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
						//	vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
						//	vvsWriteData[3].at(nExIndex).at(i) = strParam[5];

						//}

						PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
						PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
						PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);
						vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
						vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
						vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
						vvsWriteData[3].at(nExIndex).at(i) = strParam[5];
					}
					break;
				default:
					{
						//if (eCPOrderYXD == m_eCmdParamOrderType)
						//{
						//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisPos.m_dPosX;
						//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dPosY;
						//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dAngle;
						//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
						//	vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
						//	vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
						//	vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
						//	vvsWriteData[3].at(nExIndex).at(i) = strParam[5];

						//}
						//else
						//{
						//	PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						//	double dParam = 0;
						//	dParam = pPlatformXYDAixisPos.m_dPosX;
						//	PackParam(strParam[3], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dPosY;
						//	PackParam(strParam[4], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
						//	dParam = pPlatformXYDAixisPos.m_dAngle;
						//	PackParam(strParam[5], dParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
						//	vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
						//	vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
						//	vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
						//	vvsWriteData[3].at(nExIndex).at(i) = strParam[5];
						//}

						PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
						PackParam(strParam[3], vdParam[0], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[0]);
						PackParam(strParam[4], vdParam[1], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[1]);
						PackParam(strParam[5], vdParam[2], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[2]);

						vvsWriteData[0].at(nExIndex).at(i) = strParam[2];
						vvsWriteData[1].at(nExIndex).at(i) = strParam[3];
						vvsWriteData[2].at(nExIndex).at(i) = strParam[4];
						vvsWriteData[3].at(nExIndex).at(i) = strParam[5];

					}

				}
			}
		}

	}

	CString strTempInfo = _T("");
	for(int nExIndex=0; nExIndex < nExNum; nExIndex++)
	{
		strTempInfo = strTempInfo + strBlank + strParam[1];
		for (int i = 0; i < vvsWriteData[0][nExIndex].size(); i++)
		{
			CString strTemp = strBlank + vvsWriteData[0][nExIndex].at(i) + strBlank + vvsWriteData[1][nExIndex].at(i) + strBlank + vvsWriteData[2][nExIndex].at(i) + strBlank + vvsWriteData[3][nExIndex].at(i);
			strTempInfo = strTempInfo + strTemp;
		}
	}

	strInfo.Empty();

	strInfo = strHead + strName + strBlank + strParam[0] + strTempInfo + strTail;

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackRePlcMulAlignPosResultInfoFALPAxisPos_Ex(CString& strInfo,std::vector<int> &vnInfo,int &nRegAddr,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult,int nPickMutiExMode/* = 0*/)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	int nExNum = vvPlatformXYDAxisPos.size();
	{
		CString strName = m_FALPParam.m_strCmd;

		int nCmdVal = m_FALPParam.m_nCmdVal;
		m_strCmdName = strName;	
		int nParam = 0;

		int nTempDataNum = 0;
		if(nExNum > 0)
		{
			nTempDataNum = vvPlatformXYDAxisPos[0].size();
		}
		int  nDataNum = (nTempDataNum * 4) + 1;	
		int nTotalDataNum = nDataNum * nExNum;
		std::vector<int> vnTotalWriteData;//全部扩展的写入数据
		/*vnTotalWriteData.resize(nTotalDataNum);*/
		// 寄存器首地址
		long nRegAddress = m_nPLCTempBaseRegAddr /*+ atoi(RREG_VISION_PLATFORM_TEMP_POS)*/;
		nRegAddr = nRegAddress;

		BOOL bResult=(nResult==1)?TRUE:FALSE;

		std::vector<double> vdParam;
		vdParam.resize(3);
		std::vector<int> vnTrans;
		vnTrans.resize(3);
		std::vector<int> vnUnitExp;
		vnUnitExp.resize(3);

		for(int nExIndex = 0;nExIndex <vvPlatformXYDAxisPos.size();nExIndex++)
		{
			std::vector<int> vnWriteData;//单个扩展的写入数据
			vnWriteData.resize(nDataNum);
			if(nPickMutiExMode > 200 && nPickMutiExMode<300 && nTempDataNum < nPickMutiExMode%100)
			{
				vnWriteData.resize(1 + 4 * (nPickMutiExMode%100),0);
			}
			// 总数
			nParam = nTempDataNum;
			PackPlcParam(vnWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

			for (int i = 0; i < nTempDataNum; i++)
			{

				CPlatformXYDAxisPos pPlatformXYDAixisPos; 		
				pPlatformXYDAixisPos = (CPlatformXYDAxisPos)(vvPlatformXYDAxisPos[nExIndex].at(i));	
				CCoordPos pPlatformXYDAixisOffset;
				pPlatformXYDAixisOffset.m_dPosX = pPlatformXYDAixisPos.m_dPosX;
				pPlatformXYDAixisOffset.m_dPosY = pPlatformXYDAixisPos.m_dPosY;
				pPlatformXYDAixisOffset.m_dAngle= pPlatformXYDAixisPos.m_dAngle;

				ConvertToPlatformXYDAxisPos_Output(pPlatformXYDAixisOffset,vdParam,vnTrans,vnUnitExp);

				if (bResult == FALSE || m_pMidPlatformInfo == NULL)
				{
					//if (eCPOrderYXD == m_eCmdParamOrderType)
					//{
					//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisPos.m_dPosX;
					//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisPos.m_dPosY;
					//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisPos.m_dAngle;
					//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

					//}
					//else
					//{
					//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
					//	double dParam = 0;
					//	dParam = pPlatformXYDAixisPos.m_dPosX;
					//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
					//	dParam = pPlatformXYDAixisPos.m_dPosY;
					//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
					//	dParam = pPlatformXYDAixisPos.m_dAngle;
					//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
					//}

					PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);

					PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
					PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
					PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);

				}
				else
				{
					switch(m_eMidPlatformType)
					{
					case ePlatformXYD:
					case ePlatformXD:
					case ePlatformYD:
					case ePlatformXY:
						{
							//if (eCPOrderYXD == m_eCmdParamOrderType)
							//{
							//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
							//	double dParam = 0;
							//	dParam = pPlatformXYDAixisPos.m_dPosX;
							//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dPosY;
							//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dAngle;
							//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
							//}
							//else
							//{
							//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
							//	double dParam = 0;
							//	dParam = pPlatformXYDAixisPos.m_dPosX;
							//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dPosY;
							//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dAngle;
							//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
							//}

							PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);

							PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
							PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
							PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);
						}
						break;

					default:
						{
							//if (eCPOrderYXD == m_eCmdParamOrderType)
							//{
							//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
							//	double dParam = 0;
							//	dParam = pPlatformXYDAixisPos.m_dPosX;
							//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dPosY;
							//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dAngle;
							//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);
							//}
							//else
							//{
							//	PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);
							//	double dParam = 0;
							//	dParam = pPlatformXYDAixisPos.m_dPosX;
							//	PackPlcParam(vnWriteData[2 + i*4], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dPosY;
							//	PackPlcParam(vnWriteData[3 + i*4], dParam, m_CommProtocolParam.m_nCommYUnitExp);
							//	dParam = pPlatformXYDAixisPos.m_dAngle;
							//	PackPlcParam(vnWriteData[4 + i*4], dParam, m_CommProtocolParam.m_nCommDUnitExp);

							//}
							PackPlcParam(vnWriteData[1 + i*4], i+1, m_CommProtocolParam.m_nCommUnitExp);

							PackPlcParam(vnWriteData[2 + i*4], vdParam[0], vnUnitExp[0]);
							PackPlcParam(vnWriteData[3 + i*4], vdParam[1], vnUnitExp[1]);
							PackPlcParam(vnWriteData[4 + i*4], vdParam[2], vnUnitExp[2]);
						}
					}
				}
			}
			vnTotalWriteData.insert(vnTotalWriteData.end(),vnWriteData.begin(),vnWriteData.end());
		}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAddress, vnTotalWriteData.size(), vnTotalWriteData),TRUE)
		{
			return FALSE;
		}		
		vnInfo = vnTotalWriteData;
	}

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackReMulRowColsSimpleInfo_Ex(CString& strInfo,int nResult,int nSize,std::vector<int> vnRows,std::vector<int> vnCols)
{
	int nTempNum = nSize;

	CString strHead = m_strHead;

	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[6];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	int nParam = nResult;

	// strParam[0] : 成功、失败
	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	// strParam[1] : 总数
	nParam = nTempNum;
	PackParam(strParam[1], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	std::vector<CString> vsWriteData[4];
	vsWriteData[0].resize(nTempNum);	// 索引
	vsWriteData[1].resize(nTempNum);	// Row数据
	vsWriteData[2].resize(nTempNum);	// Clo数据


	CCoordPos pPlatformXYDAixisOffset; 		

	for (int i = 0; i < nTempNum; i++)
	{
		PackParam(strParam[2], i+1, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
		PackParam(strParam[3], vnRows[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
		PackParam(strParam[4], vnCols[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
		vsWriteData[0].at(i) = strParam[2];
		vsWriteData[2].at(i) = strParam[3];
		vsWriteData[1].at(i) = strParam[4];
	}

	CString strTempInfo = _T("");
	for (int i = 0; i < nTempNum; i++)
	{
		CString strTemp = strBlank + vsWriteData[0].at(i) + strBlank + vsWriteData[1].at(i) + strBlank + vsWriteData[2].at(i);
		strTempInfo = strTempInfo + strTemp;
	}

	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0] + strBlank + strParam[1] + strTempInfo + strTail;
	return TRUE;

}

BOOL CCommProtocolXYDFast::PackReMulRowCols_Ex(CString& strInfo,int nSize,std::vector<int> vnRows,std::vector<int> vnCols,int nOffSet)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	// 单字双字
	{

		int nParam = 0;

		int nTempDataNum = nSize;

		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_VISION_AXIS_READ) + nOffSet;

		int nDataNum = (nTempDataNum * 3) + 1;	

		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);

		// 总数
		nParam = nTempDataNum;
		PackPlcParam(vnWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

		for (int i = 0; i < nTempDataNum; i++)
		{
			nParam = i+1;
			PackPlcParam(vnWriteData[1 + i*3], nParam, m_CommProtocolParam.m_nCommUnitExp);
			nParam = vnRows[i];
			PackPlcParam(vnWriteData[2 + i*3], nParam, m_CommProtocolParam.m_nCommUnitExp);
			nParam = vnCols[i];
			PackPlcParam(vnWriteData[3+ i*3], nParam, m_CommProtocolParam.m_nCommUnitExp);
		}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData, TRUE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);

		return TRUE;
	}

	return TRUE;

}

BOOL CCommProtocolXYDFast::PackRePlcMulResult_Ex(CString& strInfo, std::vector<double> vdDate,int nOffset)
{
	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	int nDataNum = vdDate.size();
	//nDataNum=1;
	if (nDataNum < 0)
	{
		return FALSE;
	}

	if (1)	
	{
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_VISION_AXIS_READ) + nOffset; 	// 寄存器地址

		int nWriteData = 0;
		std::vector<int> vnWriteData;
		for (int i=0; i<nDataNum; i++)
		{
			if(i%3==0)
			{
				PackPlcParam(nWriteData, vdDate.at(i), m_CommProtocolParam.m_nCommXUnitExp);
			}
			else if(i%3==1)
			{
				PackPlcParam(nWriteData, vdDate.at(i), m_CommProtocolParam.m_nCommYUnitExp);
			}
			else if(i%3==2)
			{
				PackPlcParam(nWriteData, vdDate.at(i), m_CommProtocolParam.m_nCommDUnitExp);
			}

			vnWriteData.push_back(nWriteData);
		}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, nDataNum, vnWriteData))
		{
			return FALSE;
		}
		SetWriteRegTranslateResultInfo(nRegAdrr,nDataNum,vnWriteData,TRUE);
	}


	return TRUE;
}

BOOL CCommProtocolXYDFast::PackRePlcMulResult_SimpleInfo_Ex(CString& strInfo, std::vector<double> vdDate,int nResult,int nSearchNum)
{
	CString strHead = m_strHead;
	CString strName= _T("Fxxx");
	if(m_nCurCodeIndex>=0 && m_nCurCodeIndex<m_vKeyenceCodeParam.size())
	{
		strName = m_vKeyenceCodeParam.at(m_nCurCodeIndex).m_strCmd;
	}

	CString strParam[6];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);
	std::vector<CString> vsWriteData;
	vsWriteData.resize(vdDate.size());
	m_strCmdName = strName;

	int nParam = nResult;
	// strParam[0] : 成功、失败
	PackParam(strParam[0], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	// strParam[1] : 总数
	nParam = nSearchNum;
	PackParam(strParam[1], nParam, m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);

	CCoordPos pPlatformXYDAixisOffset; 		

	for (int i = 0; i < vdDate.size(); i++)
	{
		//PackParam(strParam[2], vdDate[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommUnitExp);
		if(i%3==0)
		{
			PackParam(strParam[2], vdDate[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommXUnitExp);
		}
		else if(i%3==1)
		{
			PackParam(strParam[2], vdDate[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommYUnitExp);
		}
		else if(i%3==2)
		{
			PackParam(strParam[2], vdDate[i], m_CommProtocolParam.m_nCommDataLenType, m_CommProtocolParam.m_nCommDUnitExp);
		}
		vsWriteData.at(i) = strParam[2];

	}

	CString strTempInfo = _T("");
	for (int i = 0; i < vdDate.size(); i++)
	{
		CString strTemp = strBlank + vsWriteData.at(i);
		strTempInfo = strTempInfo + strTemp;
	}

	strInfo.Empty();
	strInfo = strHead + strName + strBlank + strParam[0] + strBlank + strParam[1] + strTempInfo + strTail;
	return TRUE;

	return TRUE;
}

BOOL CCommProtocolXYDFast::PackPlcWriteDrivePlatformMoveToAbsPosEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,int nOffSet)
{
	if (NULL == vpPlatAxisPos)
	{
		return eAswUnkown;
	}

	if (vpPlatAxisPos->GetPlatformType()!=ePlatformXYD)
	{
		return eAswUnkown;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}


	{
		CString strName = m_MMVAParam.m_strCmd;
		int nCmdVal = m_MMVAParam.m_nCmdVal;
		int nParam = 0;

		m_strCmdName = strName;	
		// 向D10--14中写入
		long nRegAdrr = m_nPLCBaseRegAddr + atoi(RREG_KEYENCE_VISION_AXIS_READ) + nOffSet;	// 寄存器地址
		int  nDataNum = 3;												// 数据量
		std::vector<int> vnWriteData;
		vnWriteData.resize(nDataNum);

		m_strCmdName = strName;	

		CPlatformXYDAxisPos* pPlatformXYDAixisPos; 		
		pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);

		std::vector<int> vnUnitExp;
		vnUnitExp.resize(nDataNum);

		CPlatformXYDAxisPos* pPlatformXYDAixisPosBase; 		
		pPlatformXYDAixisPosBase = (CPlatformXYDAxisPos*)(vpPlatAxisPosBase);	

		std::vector<double> vdParam;
		vdParam.resize(nDataNum);

		std::vector<int> vnTrans;
		vnTrans.resize(nDataNum);

		CCoordPos tmpOffset;
		tmpOffset.m_dPosX = pPlatformXYDAixisPos->m_dPosX;
		tmpOffset.m_dPosY = pPlatformXYDAixisPos->m_dPosY;
		tmpOffset.m_dAngle = pPlatformXYDAixisPos->m_dAngle;

		ConvertToPlatformXYDAxisPos_Output(tmpOffset,vdParam,vnTrans,vnUnitExp);

		//if (eCPOrderYXD == m_eCmdParamOrderType)
		//{
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}
		//else
		//{
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}

		if (m_pMidPlatformInfo == NULL)
		{
			for(int it=0;it<3;it++)
			{
				PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
			}
		}
		else
		{
			switch(m_eMidPlatformType)
			{
			case ePlatformXYD:
			case ePlatformXYPD:
			case ePlatformXD:
			case ePlatformYD:
			case ePlatformXY:
			case ePlatformX:
			case ePlatformXDPY:
			case ePlatformXDPY1:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}
				break;
			case ePlatformXY1Y2:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{
						double dToStartX1 = 0.0;
						double dToStartY1 = 0.0;
						double dToStartY2 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndY1 = 0.0;
						double dToEndY2 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;

						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;

						CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartY1, dToStartY2, dToStartX1);

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndY1, dToEndY2, dToEndX1);


						double dParam = 0;

						CCoordPos tmpOffsetX1Y1Y2;
						tmpOffsetX1Y1Y2.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1Y1Y2.m_dPosY = dToEndY1 - dToStartY1;
						tmpOffsetX1Y1Y2.m_dAngle = dToEndY2 - dToStartY2;

						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//dParam = dToEndY2 - dToStartY2;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			case ePlatformX1X2Y:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{

						double dToStartX1 = 0.0;
						double dToStartX2 = 0.0;
						double dToStartY1 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndX2 = 0.0;
						double dToEndY1 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;//上一次的绝对量
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;





						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;//当前绝对量
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;


						CString strlog2 = _T("");



						CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartX1, dToStartX2, dToStartY1);


						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndX1, dToEndX2, dToEndY1);

						//dX1 = dX1 + 5.5;
						//dX2 = dX2 + 5.5;
						//dY1 = dY1 + 5.5;

						double dParam = 0;
						CCoordPos tmpOffsetX1X2Y1;
						tmpOffsetX1X2Y1.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1X2Y1.m_dPosY = dToEndX2 - dToStartX2;
						tmpOffsetX1X2Y1.m_dAngle = dToEndY1 - dToStartY1;

						//strlog2.Format(" pPlatformXYDAixisPosBase X=%lf Y%lf D=%lf\r\n pPlatformXYDAixisPos X=%lf Y%lf D%lf \r\n  dFromStart X=%lf Y=%lf D=%lf \r\n dFromEnd X=%lf Y=%lf D=%lf \r\n 终点 X1=%lf X2=%lf Y1=%lf \r\n 起点 X1=%lf X2=%lf Y1=%lf \r\n 偏差 X1=%lf X2=%lf Y1=%lf \r\n "
						//	, pPlatformXYDAixisPosBase->m_dPosX, pPlatformXYDAixisPosBase->m_dPosY, pPlatformXYDAixisPosBase->m_dAngle,
						//	pPlatformXYDAixisPos->m_dPosX, pPlatformXYDAixisPos->m_dPosY, pPlatformXYDAixisPos->m_dAngle,
						//	dFromStartX0,dFromStartY0,dFromStartD0,
						//	dFromEndX0,dFromEndY0,dFromEndD0,
						//	dToEndX1,dToEndX2,dToEndY1,
						//	dToStartX1,dToStartX2,dToStartY1,
						//	tmpOffsetX1X2Y1.m_dPosX,tmpOffsetX1X2Y1.m_dPosY,tmpOffsetX1X2Y1.m_dAngle
						//	);


						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						

						OutputDebugString(strlog2);
						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndX2 - dToStartX2;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			default:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}					
			}
		}

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnWriteData.size(), vnWriteData))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnWriteData.size(),vnWriteData,TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CCommProtocolXYDFast::PackWritePLCFuncWithAxisEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,int nOffSet,int nResult)
{
	if (NULL == vpPlatAxisPos)
	{
		return eAswUnkown;
	}

	if (vpPlatAxisPos->GetPlatformType()!=ePlatformXYD)
	{
		return eAswUnkown;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}


	{
		CString strName = m_MMVAParam.m_strCmd;
		int nCmdVal = m_MMVAParam.m_nCmdVal;
		int nParam = 0;

		m_strCmdName = strName;	
		// 向D10--14中写入
		long nRegAdrr = m_nPLCBaseRegAddr + nOffSet;	// 寄存器地址
		int  nDataNum = 3;												// 数据量
		int  nAllDataNum = 0;
		nAllDataNum = GetReadDataNumEx() - 2;
		std::vector<int> vnWriteData;
		std::vector<int> vnAllWriteData;
		vnWriteData.resize(nDataNum);
		vnAllWriteData.resize(nAllDataNum,0);

		m_strCmdName = strName;	

		CPlatformXYDAxisPos* pPlatformXYDAixisPos; 		
		pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);

		std::vector<int> vnUnitExp;
		vnUnitExp.resize(nDataNum);

		CPlatformXYDAxisPos* pPlatformXYDAixisPosBase; 		
		pPlatformXYDAixisPosBase = (CPlatformXYDAxisPos*)(vpPlatAxisPosBase);	

		std::vector<double> vdParam;
		vdParam.resize(nDataNum);

		std::vector<int> vnTrans;
		vnTrans.resize(nDataNum);

		CCoordPos tmpOffset;
		tmpOffset.m_dPosX = pPlatformXYDAixisPos->m_dPosX;
		tmpOffset.m_dPosY = pPlatformXYDAixisPos->m_dPosY;
		tmpOffset.m_dAngle = pPlatformXYDAixisPos->m_dAngle;

		ConvertToPlatformXYDAxisPos_Output(tmpOffset,vdParam,vnTrans,vnUnitExp);

		//if (eCPOrderYXD == m_eCmdParamOrderType)
		//{
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}
		//else
		//{
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}

		if (m_pMidPlatformInfo == NULL)
		{
			for(int it=0;it<3;it++)
			{
				PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
			}
		}
		else
		{
			switch(m_eMidPlatformType)
			{
			case ePlatformXYD:
			case ePlatformXYPD:
			case ePlatformXD:
			case ePlatformYD:
			case ePlatformXY:
			case ePlatformX:
			case ePlatformXDPY:
			case ePlatformXDPY1:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}
				break;
			case ePlatformXY1Y2:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{
						double dToStartX1 = 0.0;
						double dToStartY1 = 0.0;
						double dToStartY2 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndY1 = 0.0;
						double dToEndY2 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;

						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;

						CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartY1, dToStartY2, dToStartX1);

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndY1, dToEndY2, dToEndX1);


						double dParam = 0;

						CCoordPos tmpOffsetX1Y1Y2;
						tmpOffsetX1Y1Y2.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1Y1Y2.m_dPosY = dToEndY1 - dToStartY1;
						tmpOffsetX1Y1Y2.m_dAngle = dToEndY2 - dToStartY2;

						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//dParam = dToEndY2 - dToStartY2;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			case ePlatformX1X2Y:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{

						double dToStartX1 = 0.0;
						double dToStartX2 = 0.0;
						double dToStartY1 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndX2 = 0.0;
						double dToEndY1 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;//上一次的绝对量
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;





						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;//当前绝对量
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;


						CString strlog2 = _T("");



						CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartX1, dToStartX2, dToStartY1);


						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndX1, dToEndX2, dToEndY1);

						//dX1 = dX1 + 5.5;
						//dX2 = dX2 + 5.5;
						//dY1 = dY1 + 5.5;

						double dParam = 0;
						CCoordPos tmpOffsetX1X2Y1;
						tmpOffsetX1X2Y1.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1X2Y1.m_dPosY = dToEndX2 - dToStartX2;
						tmpOffsetX1X2Y1.m_dAngle = dToEndY1 - dToStartY1;

						//strlog2.Format(" pPlatformXYDAixisPosBase X=%lf Y%lf D=%lf\r\n pPlatformXYDAixisPos X=%lf Y%lf D%lf \r\n  dFromStart X=%lf Y=%lf D=%lf \r\n dFromEnd X=%lf Y=%lf D=%lf \r\n 终点 X1=%lf X2=%lf Y1=%lf \r\n 起点 X1=%lf X2=%lf Y1=%lf \r\n 偏差 X1=%lf X2=%lf Y1=%lf \r\n "
						//	, pPlatformXYDAixisPosBase->m_dPosX, pPlatformXYDAixisPosBase->m_dPosY, pPlatformXYDAixisPosBase->m_dAngle,
						//	pPlatformXYDAixisPos->m_dPosX, pPlatformXYDAixisPos->m_dPosY, pPlatformXYDAixisPos->m_dAngle,
						//	dFromStartX0,dFromStartY0,dFromStartD0,
						//	dFromEndX0,dFromEndY0,dFromEndD0,
						//	dToEndX1,dToEndX2,dToEndY1,
						//	dToStartX1,dToStartX2,dToStartY1,
						//	tmpOffsetX1X2Y1.m_dPosX,tmpOffsetX1X2Y1.m_dPosY,tmpOffsetX1X2Y1.m_dAngle
						//	);


						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}



						OutputDebugString(strlog2);
						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndX2 - dToStartX2;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			default:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}					
			}
		}

		int nLowData,nHighData;
		DoubleWordToWord(vnWriteData[0],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-6] = nLowData;
		vnAllWriteData[nAllDataNum-5] = nHighData;
		DoubleWordToWord(vnWriteData[1],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-4] = nLowData;
		vnAllWriteData[nAllDataNum-3] = nHighData;
		DoubleWordToWord(vnWriteData[2],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-2] = nLowData;
		vnAllWriteData[nAllDataNum-1] = nHighData;

		nParam = nResult;
		PackPlcParam(vnAllWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnAllWriteData.size(), vnAllWriteData, FALSE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnAllWriteData.size(),vnAllWriteData,FALSE);
		return TRUE;
	}
	return FALSE;
}

BOOL CCommProtocolXYDFast::PackWritePLCFuncWithAxisAndBenchCenterEx(CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,CPlatformAxisPos* vpPlatAxisPosCenter,int nOffSet/*=0*/,int nResult /*= 0*/)
{
	if (NULL == vpPlatAxisPos)
	{
		return eAswUnkown;
	}

	if (vpPlatAxisPos->GetPlatformType()!=ePlatformXYD)
	{
		return eAswUnkown;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}


	{
		CString strName = m_MMVAParam.m_strCmd;
		int nCmdVal = m_MMVAParam.m_nCmdVal;
		int nParam = 0;

		m_strCmdName = strName;	
		// 向D10--14中写入
		long nRegAdrr = m_nPLCBaseRegAddr + nOffSet;	// 寄存器地址
		int  nDataNum = 3;												// 数据量
		int  nAllDataNum = 0;
		nAllDataNum = GetReadDataNumEx() - 2 + 10;
		std::vector<int> vnWriteData;
		std::vector<int> vnAllWriteData;
		vnWriteData.resize(nDataNum);
		vnAllWriteData.resize(nAllDataNum,0);
		std::vector<int> vnWriteDataCenter;
		vnWriteDataCenter.resize(nDataNum);

		m_strCmdName = strName;	

		CPlatformXYDAxisPos* pPlatformXYDAixisPos; 		
		pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);

		std::vector<int> vnUnitExp;
		vnUnitExp.resize(nDataNum);

		CPlatformXYDAxisPos* pPlatformXYDAixisPosBase; 		
		pPlatformXYDAixisPosBase = (CPlatformXYDAxisPos*)(vpPlatAxisPosBase);	

		std::vector<double> vdParam;
		vdParam.resize(nDataNum);

		std::vector<int> vnTrans;
		vnTrans.resize(nDataNum);

		CCoordPos tmpOffset;
		tmpOffset.m_dPosX = pPlatformXYDAixisPos->m_dPosX;
		tmpOffset.m_dPosY = pPlatformXYDAixisPos->m_dPosY;
		tmpOffset.m_dAngle = pPlatformXYDAixisPos->m_dAngle;

		ConvertToPlatformXYDAxisPos_Output(tmpOffset,vdParam,vnTrans,vnUnitExp);

		//if (eCPOrderYXD == m_eCmdParamOrderType)
		//{
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}
		//else
		//{
		//	vnUnitExp.at(0)= m_CommProtocolParam.m_nCommXUnitExp;
		//	vnUnitExp.at(1)= m_CommProtocolParam.m_nCommYUnitExp;
		//	vnUnitExp.at(2)= m_CommProtocolParam.m_nCommDUnitExp;

		//	vdParam.at(0)= pPlatformXYDAixisPos->m_dPosX;
		//	vdParam.at(1)= pPlatformXYDAixisPos->m_dPosY;
		//	vdParam.at(2)= pPlatformXYDAixisPos->m_dAngle;
		//}

		if (m_pMidPlatformInfo == NULL)
		{
			for(int it=0;it<3;it++)
			{
				PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
			}
		}
		else
		{
			switch(m_eMidPlatformType)
			{
			case ePlatformXYD:
			case ePlatformXYPD:
			case ePlatformXD:
			case ePlatformYD:
			case ePlatformXY:
			case ePlatformX:
			case ePlatformXDPY:
			case ePlatformXDPY1:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}
				break;
			case ePlatformXY1Y2:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{
						double dToStartX1 = 0.0;
						double dToStartY1 = 0.0;
						double dToStartY2 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndY1 = 0.0;
						double dToEndY2 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;

						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;

						CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartY1, dToStartY2, dToStartX1);

						pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndY1, dToEndY2, dToEndX1);


						double dParam = 0;

						CCoordPos tmpOffsetX1Y1Y2;
						tmpOffsetX1Y1Y2.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1Y1Y2.m_dPosY = dToEndY1 - dToStartY1;
						tmpOffsetX1Y1Y2.m_dAngle = dToEndY2 - dToStartY2;

						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}

						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						//dParam = dToEndY2 - dToStartY2;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			case ePlatformX1X2Y:
				{
					if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
					{

						double dToStartX1 = 0.0;
						double dToStartX2 = 0.0;
						double dToStartY1 = 0.0;

						double dToEndX1 = 0.0;
						double dToEndX2 = 0.0;
						double dToEndY1 = 0.0;

						double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;//上一次的绝对量
						double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
						double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;





						double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;//当前绝对量
						double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
						double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;


						CString strlog2 = _T("");



						CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromStartX0, dFromStartY0, dFromStartD0, dToStartX1, dToStartX2, dToStartY1);


						pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
							dFromEndX0, dFromEndY0, dFromEndD0, dToEndX1, dToEndX2, dToEndY1);

						//dX1 = dX1 + 5.5;
						//dX2 = dX2 + 5.5;
						//dY1 = dY1 + 5.5;

						double dParam = 0;
						CCoordPos tmpOffsetX1X2Y1;
						tmpOffsetX1X2Y1.m_dPosX = dToEndX1 - dToStartX1;
						tmpOffsetX1X2Y1.m_dPosY = dToEndX2 - dToStartX2;
						tmpOffsetX1X2Y1.m_dAngle = dToEndY1 - dToStartY1;

						//strlog2.Format(" pPlatformXYDAixisPosBase X=%lf Y%lf D=%lf\r\n pPlatformXYDAixisPos X=%lf Y%lf D%lf \r\n  dFromStart X=%lf Y=%lf D=%lf \r\n dFromEnd X=%lf Y=%lf D=%lf \r\n 终点 X1=%lf X2=%lf Y1=%lf \r\n 起点 X1=%lf X2=%lf Y1=%lf \r\n 偏差 X1=%lf X2=%lf Y1=%lf \r\n "
						//	, pPlatformXYDAixisPosBase->m_dPosX, pPlatformXYDAixisPosBase->m_dPosY, pPlatformXYDAixisPosBase->m_dAngle,
						//	pPlatformXYDAixisPos->m_dPosX, pPlatformXYDAixisPos->m_dPosY, pPlatformXYDAixisPos->m_dAngle,
						//	dFromStartX0,dFromStartY0,dFromStartD0,
						//	dFromEndX0,dFromEndY0,dFromEndD0,
						//	dToEndX1,dToEndX2,dToEndY1,
						//	dToStartX1,dToStartX2,dToStartY1,
						//	tmpOffsetX1X2Y1.m_dPosX,tmpOffsetX1X2Y1.m_dPosY,tmpOffsetX1X2Y1.m_dAngle
						//	);


						ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
						}



						OutputDebugString(strlog2);
						//dParam = dToEndX1 - dToStartX1;
						//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndX2 - dToStartX2;
						//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
						//dParam = dToEndY1 - dToStartY1;
						//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);

					}
					else
					{
						return FALSE;
					}
				}
				break;
			default:
				{
					for(int it=0;it<3;it++)
					{
						PackPlcParam(vnWriteData[it], vdParam[it],vnUnitExp[it]);
					}
				}					
			}
		}

		//居中对位结果内部函数

		{
			
			CPlatformXYDAxisPos* pPlatformXYDAixisPosCenter; 		
			pPlatformXYDAixisPosCenter = (CPlatformXYDAxisPos*)(vpPlatAxisPosCenter);

			std::vector<int> vnUnitExpCenter;
			vnUnitExpCenter.resize(nDataNum);


			std::vector<double> vdParamCenter;
			vdParamCenter.resize(nDataNum);
			

			CCoordPos tmpOffsetCenter;
			tmpOffsetCenter.m_dPosX = pPlatformXYDAixisPosCenter->m_dPosX;
			tmpOffsetCenter.m_dPosY = pPlatformXYDAixisPosCenter->m_dPosY;
			tmpOffsetCenter.m_dAngle = pPlatformXYDAixisPosCenter->m_dAngle;
			std::vector<int> vnTransCenter;
			vnTransCenter.resize(nDataNum);
			ConvertToPlatformXYDAxisPos_Output(tmpOffsetCenter,vdParamCenter,vnTransCenter,vnUnitExpCenter);

			if (m_pMidPlatformInfo == NULL)
			{
				for(int it=0;it<3;it++)
				{
					PackPlcParam(vnWriteDataCenter[it], vdParamCenter[it],vnUnitExpCenter[it]);
				}
			}
			else
			{
				switch(m_eMidPlatformType)
				{
				case ePlatformXYD:
				case ePlatformXYPD:
				case ePlatformXD:
				case ePlatformYD:
				case ePlatformXY:
				case ePlatformX:
				case ePlatformXDPY:
				case ePlatformXDPY1:
					{
						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteDataCenter[it], vdParamCenter[it],vnUnitExpCenter[it]);
						}
					}
					break;
				case ePlatformXY1Y2:
					{
						if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
						{
							double dX1 = 0.0;
							double dY1 = 0.0;
							double dY2 = 0.0;

							double dX0 = pPlatformXYDAixisPosCenter->m_dPosX;
							double dY0 = pPlatformXYDAixisPosCenter->m_dPosY;
							double dD0 = pPlatformXYDAixisPosCenter->m_dAngle;
							CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

							pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(dX0, dY0, dD0, dY1, dY2, dX1);

							CCoordPos tmpOffsetX1Y1Y2;
							tmpOffsetX1Y1Y2.m_dPosX = dX1;
							tmpOffsetX1Y1Y2.m_dPosY = dY1;
							tmpOffsetX1Y1Y2.m_dAngle = dY2;

							ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParamCenter,vnTransCenter,vnUnitExpCenter);

							for(int it=0;it<3;it++)
							{
								PackPlcParam(vnWriteDataCenter[it], vdParamCenter[it],vnUnitExpCenter[it]);
							}

							//double dParam = 0;
							//dParam = dX1;
							//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//dParam = dY1;
							//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommYUnitExp);
							//dParam = dY2;
							//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						}
						else
						{
							return FALSE;
						}
					}
					break;
				case ePlatformX1X2Y:
					{
						if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
						{
							double dX1 = 0.0;
							double dX2 = 0.0;
							double dY1 = 0.0;

							double dX0 = pPlatformXYDAixisPosCenter->m_dPosX;
							double dY0 = pPlatformXYDAixisPosCenter->m_dPosY;
							double dD0 = pPlatformXYDAixisPosCenter->m_dAngle;
							CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

							pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(dX0, dY0, dD0, dX1, dX2, dY1);

							//dX1 = dX1 + 5.5;
							//dX2 = dX2 + 5.5;
							//dY1 = dY1 + 5.5;
							CCoordPos tmpOffsetX1X2Y1;
							tmpOffsetX1X2Y1.m_dPosX = dX1;
							tmpOffsetX1X2Y1.m_dPosY = dX2;
							tmpOffsetX1X2Y1.m_dAngle = dY1;

							ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParamCenter,vnTransCenter,vnUnitExpCenter);

							for(int it=0;it<3;it++)
							{
								PackPlcParam(vnWriteDataCenter[it], vdParamCenter[it],vnUnitExpCenter[it]);
							}

							//double dParam = 0;
							//dParam = dX1;
							//PackPlcParam(vnWriteData[0], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//dParam = dX2;
							//PackPlcParam(vnWriteData[1], dParam, m_CommProtocolParam.m_nCommXUnitExp);
							//dParam = dY1;
							//PackPlcParam(vnWriteData[2], dParam, m_CommProtocolParam.m_nCommYUnitExp);
						}
						else
						{
							return FALSE;
						}
					}
					break;
				default:
					{
						for(int it=0;it<3;it++)
						{
							PackPlcParam(vnWriteDataCenter[it], vdParamCenter[it],vnUnitExpCenter[it]);
						}
					}					
				}
			}

		}








		int nLowData,nHighData;
		DoubleWordToWord(vnWriteData[0],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-16] = nLowData;
		vnAllWriteData[nAllDataNum-15] = nHighData;
		DoubleWordToWord(vnWriteData[1],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-14] = nLowData;
		vnAllWriteData[nAllDataNum-13] = nHighData;
		DoubleWordToWord(vnWriteData[2],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-12] = nLowData;
		vnAllWriteData[nAllDataNum-11] = nHighData;

		DoubleWordToWord(vnWriteDataCenter[0],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-6] = nLowData;
		vnAllWriteData[nAllDataNum-5] = nHighData;
		DoubleWordToWord(vnWriteDataCenter[1],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-4] = nLowData;
		vnAllWriteData[nAllDataNum-3] = nHighData;
		DoubleWordToWord(vnWriteDataCenter[2],nLowData,nHighData);
		vnAllWriteData[nAllDataNum-2] = nLowData;
		vnAllWriteData[nAllDataNum-1] = nHighData;

		nParam = nResult;
		PackPlcParam(vnAllWriteData[0], nParam, m_CommProtocolParam.m_nCommUnitExp);

		if (!m_pCommProRegBase->PackWriteRegInfo(strInfo, nRegAdrr, vnAllWriteData.size(), vnAllWriteData, FALSE))
		{
			return FALSE;
		}

		SetWriteRegTranslateResultInfo(nRegAdrr,vnAllWriteData.size(),vnAllWriteData,FALSE);
		return TRUE;
	}
	return FALSE;

}


BOOL CCommProtocolXYDFast::ConvertToPlatformXYDAxisPos_Output(CCoordPos platOffset,std::vector<double>& vdParam,std::vector<int>& vnTrans ,std::vector<int>& vnUnitExp)
{
	vdParam.resize(3);
	vnTrans.resize(3);
	vnUnitExp.resize(3);

	switch(m_eCmdParamOrderType)
	{
	case eCPOrderXYD:
		{
			vnTrans[0] = 0;
			vnTrans[1] = 1;
			vnTrans[2] = 2;
		}
		break;
	case eCPOrderYXD:
		{
			vnTrans[0] = 1;
			vnTrans[1] = 0;
			vnTrans[2] = 2;
		}
		break;
	case eCPOrderXDY:
		{
			vnTrans[0] = 0;
			vnTrans[1] = 2;
			vnTrans[2] = 1;
		}
		break;
	case eCPOrderDXY:
		{
			vnTrans[0] = 1;
			vnTrans[1] = 2;
			vnTrans[2] = 0;
		}
		break;
	case eCPOrderYDX:
		{
			vnTrans[0] = 2;
			vnTrans[1] = 0;
			vnTrans[2] = 1;
		}
		break;
	case eCPOrderDYX:
		{
			vnTrans[0] = 2;
			vnTrans[1] = 1;
			vnTrans[2] = 0;
		}
		break;
	default:
		{
			vnTrans[0] = 0;
			vnTrans[1] = 1;
			vnTrans[2] = 2;
		}
		break;
	}

	vdParam[vnTrans[0]] = platOffset.m_dPosX;
	vdParam[vnTrans[1]] = platOffset.m_dPosY;
	vdParam[vnTrans[2]] = platOffset.m_dAngle;


	
	vnUnitExp[0] = m_CommProtocolParam.m_nCommXUnitExp;
	vnUnitExp[1] = m_CommProtocolParam.m_nCommYUnitExp;
	vnUnitExp[2] = m_CommProtocolParam.m_nCommDUnitExp;
	
	if(m_eMidPlatformType== PlatformType::ePlatformX1X2Y)
	{
		vnUnitExp[0] = m_CommProtocolParam.m_nCommXUnitExp;
		vnUnitExp[1] = m_CommProtocolParam.m_nCommXUnitExp;
		vnUnitExp[2] = m_CommProtocolParam.m_nCommYUnitExp;
	}
	else if(m_eMidPlatformType== PlatformType::ePlatformXY1Y2)
	{
		vnUnitExp[0] = m_CommProtocolParam.m_nCommXUnitExp;
		vnUnitExp[1] = m_CommProtocolParam.m_nCommYUnitExp;
		vnUnitExp[2] = m_CommProtocolParam.m_nCommYUnitExp;
	}

	return TRUE;
}




// 打包“指定平台各轴移动到的绝对位置”指令: （[STX]MMVA P1 P2 P3[ETX]）
BOOL CCommProtocolXYDFast::PackDrivePlatformMoveToAbsPosEx(CString& strData, CPlatformAxisPos* vpPlatAxisPos, CPlatformAxisPos* vpPlatAxisPosBase)
{
	if (NULL == vpPlatAxisPos || NULL == vpPlatAxisPosBase)
	{
		return FALSE;
	}

	if (vpPlatAxisPos->GetPlatformType()!=ePlatformXYD)
	{
		return FALSE;
	}

	CString strHead = m_strHead;
	CString strName= m_MMVAParam.m_strCmd;
	CString strParam[3];
	CString strBlank = m_strSeparator;
	CString strTail = m_strTail;	
	CString strCR = char(0x0D);

	m_strCmdName = strName;

	CPlatformXYDAxisPos* pPlatformXYDAixisPos; 		
	pPlatformXYDAixisPos = (CPlatformXYDAxisPos*)(vpPlatAxisPos);	

	CPlatformXYDAxisPos* pPlatformXYDAixisPosBase; 		
	pPlatformXYDAixisPosBase = (CPlatformXYDAxisPos*)(vpPlatAxisPosBase);	
	int nDataNum = 3;
	std::vector<int> vnUnitExp;
	vnUnitExp.resize(nDataNum);

	std::vector<double> vdParam;
	vdParam.resize(nDataNum);

	std::vector<int> vnTrans;
	vnTrans.resize(nDataNum);

	CCoordPos tmpOffset;
	tmpOffset.m_dPosX = pPlatformXYDAixisPos->m_dPosX;
	tmpOffset.m_dPosY = pPlatformXYDAixisPos->m_dPosY;
	tmpOffset.m_dAngle = pPlatformXYDAixisPos->m_dAngle;
	ConvertToPlatformXYDAxisPos_Output(tmpOffset,vdParam,vnTrans,vnUnitExp);

	if (m_pMidPlatformInfo == NULL)
	{
		for(int it=0;it<3;it++)
		{
			PackParam(strParam[it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
		}
	}
	else
	{
		switch(m_eMidPlatformType)
		{
		case ePlatformXYD:
			{
				for(int it=0;it<3;it++)
				{
					PackParam(strParam[it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
				}
			}
			break;
		case ePlatformXY1Y2:
			{
				if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
				{
					double dToStartX1 = 0.0;
					double dToStartY1 = 0.0;
					double dToStartY2 = 0.0;

					double dToEndX1 = 0.0;
					double dToEndY1 = 0.0;
					double dToEndY2 = 0.0;

					double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;
					double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
					double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;

					double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;
					double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
					double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;

					CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pMidPlatformInfo;

					pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
						dFromStartX0, dFromStartY0, dFromStartD0, dToStartY1, dToStartY2, dToStartX1);

					pPlatformXY1Y2Info->ConvertXYDPltfmPosToXY1Y2PltfmPos(
						dFromEndX0, dFromEndY0, dFromEndD0, dToEndY1, dToEndY2, dToEndX1);


					double dParam = 0;

					CCoordPos tmpOffsetX1Y1Y2;
					tmpOffsetX1Y1Y2.m_dPosX = dToEndX1 - dToStartX1;
					tmpOffsetX1Y1Y2.m_dPosY = dToEndY1 - dToStartY1;
					tmpOffsetX1Y1Y2.m_dAngle = dToEndY2 - dToStartY2;

					ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1Y1Y2,vdParam,vnTrans,vnUnitExp);

					for(int it=0;it<3;it++)
					{
						PackParam(strParam[it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
					}


				}
				else
				{
					return FALSE;
				}
			}
			break;

		case ePlatformX1X2Y:
			{
				if (m_SystemOptionCofing.m_pTempPlatformInfo != NULL)
				{

					double dToStartX1 = 0.0;
					double dToStartX2 = 0.0;
					double dToStartY1 = 0.0;

					double dToEndX1 = 0.0;
					double dToEndX2 = 0.0;
					double dToEndY1 = 0.0;

					double dFromStartX0 = pPlatformXYDAixisPosBase->m_dPosX - pPlatformXYDAixisPos->m_dPosX;
					double dFromStartY0 = pPlatformXYDAixisPosBase->m_dPosY - pPlatformXYDAixisPos->m_dPosY;
					double dFromStartD0 = pPlatformXYDAixisPosBase->m_dAngle - pPlatformXYDAixisPos->m_dAngle;

					double dFromEndX0 = pPlatformXYDAixisPosBase->m_dPosX;
					double dFromEndY0 = pPlatformXYDAixisPosBase->m_dPosY;
					double dFromEndD0 = pPlatformXYDAixisPosBase->m_dAngle;

					CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pMidPlatformInfo;

					pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
						dFromStartX0, dFromStartY0, dFromStartD0, dToStartX1, dToStartX2, dToStartY1);
					pPlatformX1X2YInfo->ConvertXYDPltfmPosToX1X2YPltfmPos(
						dFromEndX0, dFromEndY0, dFromEndD0, dToEndX1, dToEndX2, dToEndY1);

					//dX1 = dX1 + 5.5;
					//dX2 = dX2 + 5.5;
					//dY1 = dY1 + 5.5;

					double dParam = 0;
					CCoordPos tmpOffsetX1X2Y1;
					tmpOffsetX1X2Y1.m_dPosX = dToEndX1 - dToStartX1;
					tmpOffsetX1X2Y1.m_dPosY = dToEndX2 - dToStartX2;
					tmpOffsetX1X2Y1.m_dAngle = dToEndY1 - dToStartY1;

					ConvertToPlatformXYDAxisPos_Output(tmpOffsetX1X2Y1,vdParam,vnTrans,vnUnitExp);

					for(int it=0;it<3;it++)
					{
						PackParam(strParam[it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType,vnUnitExp[it]);
					}
				}
				else
				{
					return FALSE;
				}
			}
			break;

		default:
			{
				for(int it=0;it<3;it++)
				{
					PackParam(strParam[it], vdParam[it], m_CommProtocolParam.m_nCommDataLenType, vnUnitExp[it]);
				}
			}
		}
	}

	strData.Empty();
	strData = strHead + strName + strBlank + strParam[0] + strBlank + strParam[1] + strBlank + strParam[2] + strTail;

	return TRUE;


	}