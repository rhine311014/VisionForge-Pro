#include "stdafx.h"
#include "LightController.h"


CLightController::CLightController(void)
{
	m_vpLightControl.resize(MAXLightControl, NULL);
}


CLightController::~CLightController(void)
{
	for (int i = 0; i < m_vpLightControl.size(); i++)
	{
		if (m_vpLightControl[i])
		{
			m_vpLightControl[i]->CloseConnect();
			vcLightControlBase::DestroyLightControl(m_vpLightControl[i]);
			m_vpLightControl[i] = NULL;
		}
	}
}


CLightController& CLightController::GetInstance()
{
	static CLightController instance;

	return instance;
}

bool CLightController::GetIntensityRange(LightControlType type, int& nMinIntensity, int& nMaxIntensity)
{
	vcLightControlBase* pLight = vcLightControlBase::CreateLightControl(type);
	
	if (!pLight) return false;

	bool bRet =pLight->GetIntensityRange(nMinIntensity, nMaxIntensity);

	vcLightControlBase::DestroyLightControl(pLight);

	return bRet;
}


bool CLightController::OpenConnect(SystemOptionLightConfig& systemOptionLightConfig)
{
	bool bRet = true;
	for (int i = 0 ; i < MAXLightControl; i++)
	{
		bRet &= OpenConnect(i, systemOptionLightConfig.m_bLightCtrlUsed[i], systemOptionLightConfig.m_lightCtrlParam[i]);
	}

	m_systemOptionLightConfig = systemOptionLightConfig;

	return bRet;
}


bool CLightController::OpenConnect(int nLightIndex, bool bUsed, LightCtrlParam& lightCtrlParam ,int nChannelIndex)
{
	bool bRet = true;

	// 该位置控制器已经使用
	if(bUsed)
	{
		// 1. 光源控制器未被打开
		if (m_vpLightControl[nLightIndex])
		{
			if (m_systemOptionLightConfig.m_lightCtrlParam[nLightIndex] != lightCtrlParam)
			{
				m_vpLightControl[nLightIndex]->CloseConnect();
				vcLightControlBase::DestroyLightControl(m_vpLightControl[nLightIndex]);
				m_vpLightControl[nLightIndex] = NULL;
			}

		}


		if (!m_vpLightControl[nLightIndex])
		{
			vcLightControlBase* pLightControl =	vcLightControlBase::CreateLightControl((LightControlType)lightCtrlParam.m_nLightType);
			if (!pLightControl)
			{
				return false;
			}


			pLightControl->SetConnectTimeOut(lightCtrlParam.m_nConnectTimeOut/1000);
			pLightControl->SetCommTimeOut(lightCtrlParam.m_nCommTimeOut);

			if (lightCtrlParam.m_nCommType == 0)
			{
				// 串口
				if(!pLightControl->OpenConnect(lightCtrlParam.m_nSerialPort, lightCtrlParam.m_nBaundRate))
				{
					vcLightControlBase::DestroyLightControl(pLightControl);
					
					return false;
				}
			}
			else
			{
				// 网口
				if(!pLightControl->OpenConnect(lightCtrlParam.m_szIP, lightCtrlParam.m_nPort))
				{
					vcLightControlBase::DestroyLightControl(pLightControl);
					return false;
				}
			}

			m_vpLightControl[nLightIndex] = pLightControl;
		}

		m_systemOptionLightConfig.m_bLightCtrlUsed[nLightIndex] = bUsed;
		m_systemOptionLightConfig.m_lightCtrlParam[nLightIndex] = lightCtrlParam;

		// 2. 设置光源通道开关
		for (int j = 0; j < lightCtrlParam.m_nMaxChannels; j++)
		{
			if (IsSupportSetChannelSwitchState(nLightIndex))
			{
				SetChannelSwitchState(nLightIndex, j, lightCtrlParam.m_bChannelIsOpen[j]);
			}
		}

		// 3. 设置光源亮度值
		for (int j = 0; j < lightCtrlParam.m_nMaxChannels; j++)
		{
			//Summer0216 nChannelIndex：0 初始化   1~8：对应通道   9：全部通道
			if ((lightCtrlParam.m_bChannelIsOpen[j] && nChannelIndex == 0) || 
			//if ((nChannelIndex == 0) || 
				(nChannelIndex == j+1)||
				(nChannelIndex == 9))//Summer0216
			{
				if (IsSupportSetChannelSwitchState(nLightIndex) 
					&& m_systemOptionLightConfig.m_lightCtrlParam[nLightIndex].m_bChannelIsOpen[j])
				{
					SetChannelIntensityValue(nLightIndex, j, lightCtrlParam.m_nChannelValue[j]);
				}
				else if (!IsSupportSetChannelSwitchState(nLightIndex))
				{
					SetChannelIntensityValue(nLightIndex, j, lightCtrlParam.m_nChannelValue[j]);
				}
			}
		}

	}
	else
	{
		// 释放资源
		if (m_vpLightControl[nLightIndex])
		{
			m_vpLightControl[nLightIndex]->CloseConnect();
			vcLightControlBase::DestroyLightControl(m_vpLightControl[nLightIndex]);
			m_vpLightControl[nLightIndex] = NULL;
		}
	}

	return true;
}


bool CLightController::IsOpen(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl) return false;

	return m_vpLightControl[nLightIndex] == NULL ? false : true;
}

void CLightController::CloseConnect(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl) return;

	if (m_vpLightControl[nLightIndex])
	{
		m_vpLightControl[nLightIndex]->CloseConnect();
		vcLightControlBase::DestroyLightControl(m_vpLightControl[nLightIndex]);
		m_vpLightControl[nLightIndex] = NULL;
	}
}

/**
	*  函数功能: 设置光源控制器的通道开关状态
	*  输入: nLightIndex-光源控制器索引; nChannelIndex-通道索引; bSwitchState-开关状态
	*/
bool CLightController::SetChannelSwitchState(int nLightIndex, int nChannelIndex, bool bSwitchState)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);

	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->SetChannelSwitchState(nChannelIndex+1, bSwitchState);
}

bool CLightController::IsSupportSetChannelSwitchState(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->IsSupportSetChannelSwitchState();

}

bool CLightController::SetAllChannelSwitchState(int nLightIndex, bool bSwitchState)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->SetAllChannelSwitchState(bSwitchState);
}

bool CLightController::IsSupportSetAllChannelSwitchState(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->IsSupportSetAllChannelSwitchState();
}

bool CLightController::SetChannelIntensityValue(int nLightIndex, int nChannelIndex, int nIntensity)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	if (!IsSupportSetChannelSwitchState(nLightIndex) && !m_systemOptionLightConfig.m_lightCtrlParam[nLightIndex].m_bChannelIsOpen[nChannelIndex])
	{
		nIntensity = 0;
	}

	return m_vpLightControl[nLightIndex]->SetChannelIntensityValue(nChannelIndex+1, nIntensity);
}

bool CLightController::IsSupportSetChannelIntensityValue(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);

	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->IsSupportSetChannelIntensityValue();
}

bool CLightController::GetChannelIntensityValue(int nLightIndex, int nChannelIndex, int& nIntensity)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->GetChannelIntensityValue(nChannelIndex+1, nIntensity);
}

bool CLightController::IsSupportGetChannelIntensityValue(int nLightIndex)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->IsSupportGetChannelIntensityValue();
}

bool CLightController::GetIntensityRange(int nLightIndex, int& nMinIntensity, int& nMaxIntensity)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	return m_vpLightControl[nLightIndex]->GetIntensityRange(nMinIntensity, nMaxIntensity);
}

bool CLightController::SetMultiChannelIntensityValue(int nLightIndex, std::vector<int> vnChannelIndex, std::vector<int> vnIntensity)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	for (int  i= 0; i < vnChannelIndex.size(); i++)
	{
		vnChannelIndex[i] += 1;
	}

	return m_vpLightControl[nLightIndex]->SetMultiChannelIntensityValue(vnChannelIndex, vnIntensity);
}

bool CLightController::GetMultiChannelIntensityValue(int nLightIndex, std::vector<int> vnChannelIndex, std::vector<int>& vnIntensity)
{
	if (nLightIndex < 0 || nLightIndex >= MAXLightControl)
	{
		return false;
	}

	vcMutexLock l(&m_vLightOperatorMutex[nLightIndex]);


	if (!m_vpLightControl[nLightIndex])
	{
		return false;
	}

	for (int  i= 0; i < vnChannelIndex.size(); i++)
	{
		vnChannelIndex[i] += 1;
	}

	return m_vpLightControl[nLightIndex]->GetMultiChannelIntensityValue(vnChannelIndex, vnIntensity);
}
