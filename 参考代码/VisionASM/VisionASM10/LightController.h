#pragma once
#include "SystemOptionConfig.h"
#include "vsLightControlBase.h"
#include "vc_mutex.hpp"

// 单例模式
class CLightController
{
public:
	/**
	 * 析构函数, 释放对象资源
	 */
	~CLightController(void);
	
private:
	CLightController(void);  
public:

	/**
	 * 获取光源控制对象
	 */

	static CLightController& GetInstance();

	static bool GetIntensityRange(LightControlType type, int& nMinIntensity, int& nMaxIntensity);


	/**
	 *  函数功能: 根据配置从新配置光源控制器是否进行打开和关闭
	 *  输入: 光源控制器配置参数
	 */
	bool OpenConnect(SystemOptionLightConfig& systemOptionLightConfig);
	bool OpenConnect(int nLightIndex, bool bUsed, LightCtrlParam& lightCtrlParam,int nChannelIndex=0);//Summer0216
	void CloseConnect(int nLightIndex);

	bool IsOpen(int nLightIndex);


	/**
	 *  函数功能: 设置光源控制器的通道开关状态
	 *  输入: nLightIndex-光源控制器索引; nChannelIndex-通道索引; bSwitchState-开关状态
	 */
    bool SetChannelSwitchState(int nLightIndex, int nChannelIndex, bool bSwitchState); 

	bool IsSupportSetChannelSwitchState(int nLightIndex);

    bool SetAllChannelSwitchState(int nLightIndex, bool bSwitchState); 

	bool IsSupportSetAllChannelSwitchState(int nLightIndex);


	/**
	 *  函数功能: 设置光源通道的亮度值
	 *  输入: nLightIndex-光源控制器索引; nChannelIndex-通道索引; nIntensity-亮度值
	 */
	bool SetChannelIntensityValue(int nLightIndex, int nChannelIndex, int nIntensity); 

	bool IsSupportSetChannelIntensityValue(int nLightIndex);

	bool GetChannelIntensityValue(int nLightIndex, int nChannelIndex, int& nIntensity); 	   

	bool IsSupportGetChannelIntensityValue(int nLightIndex);

	/**
	 *  函数功能: 获取光源控制器通道调整亮度的最大/最小值
	 */
	bool GetIntensityRange(int nLightIndex, int& nMinIntensity, int& nMaxIntensity);	 

	/**
	 *  同时设置/获取多个通道的光源值
	 */
	bool SetMultiChannelIntensityValue(int nLightIndex, std::vector<int> vnChannelIndex, std::vector<int> vnIntensity); 

	bool GetMultiChannelIntensityValue(int nLightIndex, std::vector<int> vnChannelIndex, std::vector<int>& vnIntensity); 



private:
	SystemOptionLightConfig	m_systemOptionLightConfig;	// 光源控制器配置参数
	vcMutex m_vLightOperatorMutex[MAXLightControl];			// 光源控制器操作线程安全锁
	std::vector<vcLightControlBase*> m_vpLightControl;  // 光源控制器对象集合
};

