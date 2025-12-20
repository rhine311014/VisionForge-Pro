// vsLightControlBase.h
// 光源控制基类
#pragma once
#include <map>
#include <vector>

#ifdef VS_LightControl_EXPORTS 
#define VS_LightControl_API _declspec(dllexport)
#else
#define VS_LightControl_API _declspec(dllimport)
#endif

#pragma pack(push,8)

// 控制器类型
typedef enum tagLightControlType
{
	eLightControlUnknown = -1,	// 未知
	eLightControlLTS = 0,       // 乐视
	eLightControlLTS2 =1,       // 乐视2
	eLightControlCST = 10,       // 康视达
	eLightControlCST2 = 11,      // 康视达2
	eLightControlFST = 20,       // 菲视特
	eLightControlFST2 = 21,      // 菲视特2
    eLightControlFST3 = 22,      // 菲视特3
	eLightControlOPT  = 30,      // 奥普特
	eLightControlOPT2 = 31,      // 奥普特2
	eLightControlLKS  = 40,      // 蓝科胜
}LightControlType;

extern VS_LightControl_API std::map<int,CString> g_mapLightControlType;

class VS_LightControl_API vcLightControlBase
{
public:
	/*******************************************************************
	函数: CreateLightControl;
	描述: 根据光源控制器类型, 创建控制器对象;
	输入: eLightControlType - 控制器对象枚举值;
	输出: 无;
	返回值: 返回控制器对象指针;
	*******************************************************************/
	static vcLightControlBase* CreateLightControl(LightControlType eLightControlType);

	/*******************************************************************
	函数: DestroyLightControl;
	描述: 销毁对应的控制器对象;
	输入: pLightControl - 控制器对象;
	输出: 无;
	返回值: True - 销毁光源控制器成功; False - 销毁光源控制器失败;
	*******************************************************************/
	static void DestroyLightControl(vcLightControlBase* pLightControl);

public:
	vcLightControlBase(void);
	virtual ~vcLightControlBase(void);

public:	
	/*******************************************************************
	函数: GetLightControlType;
	描述: 获取当前光源控制器枚举值;
	输入: 无;
	输出: 无;
	返回值: 当前光源控制器枚举值;
	*******************************************************************/
	virtual LightControlType GetLightControlType();

	/*******************************************************************
	函数: OpenConnect;
	描述: 打开连接;
	输入: uiSerialPort - 串口号; uiBaudRate - 波特率
	输出: 无;
	返回值:True - 打开成功; False - 打开失败
	*******************************************************************/
	virtual bool OpenConnect(unsigned int  uiSerialPort,unsigned int uiBaudRate = 19200);   

	/*******************************************************************
	函数: OpenConnect;
	描述: 打开连接;
	输入: ucAddr - IP地址; uiPort - 端口号
	输出: 无;
	返回值:True - 打开成功; False - 打开失败
	*******************************************************************/
	virtual bool OpenConnect(unsigned char (&ucAddr)[4],unsigned int uiPort);  
     
    /*******************************************************************
	函数: CloseConnect;
	描述: 关闭连接;
	输入: 无; 
	输出: 无;
	返回值:True - 关闭成功; False - 关闭失败
	*******************************************************************/
	virtual bool CloseConnect();  

	/*******************************************************************
	函数: SetChannelSwitchState;
	描述: 打开或关闭控制器对应通道;
	输入: nChannelIndex - 通道索引; bSwitchState - true:打开; false:关闭; 
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
    virtual bool SetChannelSwitchState(int nChannelIndex, bool bSwitchState);  

	/*******************************************************************
	函数: IsSupportSetChannelSwitchState;
	描述: 是否支持获取通道开关状态;
	输入: 无; 
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool IsSupportSetChannelSwitchState();

	/*******************************************************************
	函数: SetAllChannelSwitchState;
	描述: 打开控制器对应的所有通道;
	输入: bSwitchState - true:打开; false:关闭;
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
    virtual bool SetAllChannelSwitchState(bool bSwitchState); 

	/*******************************************************************
	函数: IsSupportSetAllChannelSwitchState;
	描述: 是否支持设置所有通道开关状态;
	输入: 无;
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool IsSupportSetAllChannelSwitchState();


	/*******************************************************************
	函数: SetChannelIntensityValue;
	描述: 设置通道光源亮度值;
	输入: ChannelIndex - 通道索引; nIntensity - 光源亮度值
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool SetChannelIntensityValue(int nChannelIndex, int nIntensity); 

	/*******************************************************************
	函数: IsSupportSetChannelIntensityValue;
	描述: 是否支持设置通道光源亮度值;
	输入: 无;
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool IsSupportSetChannelIntensityValue();

	/*******************************************************************
	函数: GetLightIntensityValue;
	描述: 获取对应通道光源亮度值;
	输入: ChannelIndex - 通道索引; 
	输出: nIntensity - 光源亮度值;
	返回值: True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool GetChannelIntensityValue(int nChannelIndex, int& nIntensity); 	   \

	/*******************************************************************
	函数: IsSupportGetChannelIntensityValue;
	描述: 是否支持获取通道光源亮度值;
	输入: 无; 
	输出: 无;
	返回值: True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool IsSupportGetChannelIntensityValue();

	/*******************************************************************
	函数: GetIntensityRange;
	描述: 获取光源控制器亮度的最大值与最小值;
	输入: 无;
	输出: nMinIntensity - 亮度最小值; nMaxIntensity - 亮度最大值  
	返回值: True - 获取成功; False - 获取失败
	*******************************************************************/
	virtual bool GetIntensityRange(int& nMinIntensity, int& nMaxIntensity);	 

	/*******************************************************************
	函数: SetConnectTimeOut;
	描述: 设置连接超时时间 (针对CST、CST2类型控制器);
	输入: nConnectTimeOut(单位：s);
	输出: 无;  
	返回值: True - 设置成功; False - 设置失败
	*******************************************************************/
	virtual bool SetConnectTimeOut(int nConnectTimeOut);

	/*******************************************************************
	函数: SetCommTimeOut;
	描述: 设置通信超时时间 (针对LTS、OPT、OPT2、FST、FST2、FST3、LKS控制器);
	输入: nTimeOut(单位：ms);
	输出: 无;  
	返回值: True - 设置成功; False - 设置失败
	*******************************************************************/
	virtual bool SetCommTimeOut(int nCommTimeOut);

    /*******************************************************************
	函数: SetMultiChannelIntensityValue;
	描述: 设置控制器多通道光源亮度值;
	输入: vnChannelIndex - 通道索引; vnIntensity - 光源亮度值
	输出: 无;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool SetMultiChannelIntensityValue(std::vector<int> vnChannelIndex, std::vector<int> vnIntensity); 

	 /*******************************************************************
	函数: GetMultiChannelIntensityValue;
	描述: 获取控制器多通道光源亮度值;
	输入: vnChannelIndex - 通道索引; 
	输出: vnIntensity - 光源亮度值;
	返回值:True - 操作成功; False - 操作失败
	*******************************************************************/
	virtual bool GetMultiChannelIntensityValue(std::vector<int> vnChannelIndex, std::vector<int>& vnIntensity); 
};

#pragma pack(pop)