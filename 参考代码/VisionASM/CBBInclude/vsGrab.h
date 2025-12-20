// vsGrab.h: interface for the vcGrab class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "VSGrabDef.h"
#include <vector>

#ifdef VS_GRAB_EXPORTS 
#define VS_GRAB_API _declspec(dllexport)
#else
#define VS_GRAB_API _declspec(dllimport)
#endif

#pragma pack(push,8)

typedef struct tagGrabSystemInfo
{
	int								m_nCameraTypeIndex;					// 相机清单序号
	CString							m_strSystemInfo;					// 相机系统信息
	int								m_nCheckDelayTime;					// 查询相机延时
	HINSTANCE						m_hInstance;						// 句柄
	BOOL							m_bInited;							// 是否初始化过系统
	vfGrabInitSystem				m_fGrabInitSystem;					// 初始化系统
	vfGrabFreeSystem				m_fGrabFreeSystem;					// 释放系统
	vfGrabGetBusCameraCount			m_fGrabGetBusCameraCount;			// 获取总线上相机的数量
	vfGrabEnumerateBusCameras		m_fGrabEnumerateBusCameras;			// 枚举总线上的可用相机及其信息

	vfGrabCheckSystemCameraType		m_fGrabCheckSystemCameraType;		// 检测相机类型
	vfGrabGetSystemCameraValid		m_fGrabGetSystemCameraValid;		// 查询相机是否存在
	vfGrabCreateCamera				m_fGrabCreateCamera;				// 创建相机
	vfGrabDeleteCamera				m_fGrabDeleteCamera;				// 析构相机

	tagGrabSystemInfo()
	{
		m_nCameraTypeIndex = -1;					// 相机清单序号
		m_strSystemInfo	= _T("");					// 相机系统信息
		m_nCheckDelayTime = 0;						// 查询相机延时
		m_hInstance	= NULL;							// 句柄
		m_bInited = FALSE;							// 是否初始化过系统
		m_fGrabInitSystem = NULL;					// 初始化系统
		m_fGrabFreeSystem = NULL;					// 释放系统
		m_fGrabGetBusCameraCount = NULL;			// 获取总线上相机的数量
		m_fGrabEnumerateBusCameras = NULL;			// 枚举总线上的可用相机及其信息

		m_fGrabCheckSystemCameraType = NULL;		// 检测相机类型
		m_fGrabGetSystemCameraValid = NULL;			// 查询相机是否存在
		m_fGrabCreateCamera = NULL;					// 创建相机
		m_fGrabDeleteCamera = NULL;					// 析构相机
	}

}GrabSystemInfo;

class cpPointCloudGenerateToolBase;
class VS_GRAB_API vcGrab
{
public:
	vcGrab(void);
	~vcGrab(void);

public:
	//////////////////////////////////////公共函数//////////////////////////////////
	static bool InitSystem(bool bHigherLevelXmlFilePath = false, bool bBslCameraValid = false);		// 初始化系统
	static bool FreeSystem();																		// 释放系统
	static bool GetGrabSystemNames(std::vector<CString>& vstrSystemNames);							// 获取系统名称
	static int	GetCheckDelayTime();																// 获取查询相机延时		
	static bool GetBusCameraCount(unsigned int* pCamCount);											// 获取总线上相机的数量
	static bool EnumerateBusCameras(PTCameraInfo* pCameraInfo, unsigned int* pSize);				// 枚举总线上的可用相机及其信息

	static void EnableCameraOfflineMode(bool bEnabled = true);										// 开启相机离线模式

public:	
	////////////////////////////////////设置回调函数////////////////////////////////
	virtual void SetOwner(void* pOwner, PTGrabbedCallback funcCallback);							// 设置调用者和回调函数
	virtual void SetSendCallbackEnable(bool bEnable);												// 设置是否调用回调函数
	virtual bool GetSendCallbackEnable();															// 获取是否调用回调函数

	////////////////////////////////////相机初始化/////////////////////////////////
	virtual bool InitFromSerial(CString strDeviceID);												// 根据相机序列号初始化相机
	virtual bool InitFromIndex(unsigned int nCamIndex);												// 根据索引号初始化相机
	virtual bool IsOnline();																		// 是否连接相机
	virtual bool Destroy();																			// 断开相机连接
	
	////////////////////////////////////获取相机信息///////////////////////////////
	virtual bool GetCameraInfo(PTCameraInfo* pCamInfo);												// 获取相机的相关信息
	virtual bool GetGrabSystemName(CString& strGrabSystemName);										// 获取采集系统名称

	virtual bool SetFeature();																		//设置相机参数
	////////////////////////////////////采集操作///////////////////////////////////
	virtual bool Grab();						
	virtual bool Freeze();																			// 结束连续采集，需要已经创建采集线程
	virtual bool IsGrabbing();																		// 获取是否处于连续采集状态
	virtual bool SetGrabTimeout(unsigned long ulTimeout);											// 设置采集延时
	// 采集单帧图像（只支持FreeRun模式）。注：需要已经创建采集线程；
	// 如果bWaitSnapped=true, 等待直到采集完成才返回；
	// 如果bWaitSnapped=false, 调用后不等待采集完直接返回；
	virtual bool Snap(bool bWaitSnapped = false);
	virtual bool IsAcquireSuccess();																// 图像是否采集成功
	virtual bool Start();																			// 控制相机开始采集图像，不需要创建采集线程
	virtual bool Stop();																			// 控制相机停止采集图像，不需要创建采集线程
	//////////////////////////////////////////////////////////////////////////
	// 采集单帧图像；
	// 该采集函数不能与上述其他采集函数同时调用；
	// 该采集函数直到采集完成后退出；
	// 该函数不依赖于采集线程；
	virtual bool SnapImage();																		// 直接采集一帧图像

	/////////////////////////////////触发设置//////////////////////////////////////
	// 设置相机的触发模式(软触发、硬触发、或是非触发)。注：当设置相机为软触发和硬触发时，需要已经创建采集线程
	virtual bool SetTriggerMode(PTTriggerMode mode, unsigned long ulTimeout = 2000, PTTriggerSource source = ePTLine1, PTTriggerActivation act = ePTRisingEdge, unsigned long ulTriggerDelay = 1);                   
	virtual PTTriggerMode  GetTriggerMode();														// 获取当前的触发模式	
	virtual bool CloseTriggerMode();																// 关闭触发模式
	virtual bool IsTrigger();																		// 是否以触发方式（软触发或硬触发）采集图像
	// 发送软触发信号采集单帧图像，利用连续采集获取单帧图像
	// 发送完软触发信号后，该函数立即返回
	// 采集一帧图像成功后，调用回调函数
	virtual bool SendSoftTrigger();																	// 发送软触发信号
	// 发送软触发信号采集单帧图像，利用连续采集获取单帧图像
	// 发送完软触发信号后，该函数立即返回
	// 采集一帧图像成功后，调用回调函数
	virtual bool TriggerSnap();																		// 触发采集一帧图像
// 	virtual bool CheckTriggerReady();																// 查询并等待，直到相机可以进行触发
// 	virtual bool CheckSoftwareTriggerPresence(); 													// 查询相机是否有软触发采集

	///////////////////////////////相机图像属性相关////////////////////////////////
	virtual PTPixelFormat GetImagePixelFormat();													// 获取图像的像素格式
	virtual long GetImageWidth();																	// 获取图像的宽
	virtual long GetImageHeight();																	// 获取图像的高
	virtual long GetImageDataSize();																// 获取存储图像所需的内存大小（字节）	
	virtual bool GetImageData(unsigned char* pImage);												// 获取图像数据，要求外部已分配好内存， 内存的大小可由GetImageDataSize()获得；
	virtual bool GetImage(scImageGray& image, long alignModulus = 1);								// 获取灰度图像，如果像素格式为ePTRGB24则返回false
	virtual bool GetImage(scImageRGB& image, long alignModulus = 1);								// 获取RGB图像，如果像素格式为ePTGrey8则返回false
	virtual bool GetImage(cpImage& image, long alignModulus = 1);								    // 获取灰度图像，如果像素格式为ePTRGB24则返回false

	/////////////////////////////////相机属性相关//////////////////////////////////
	// 帧率设置
	bool SetCurFrameRate(float fFrameRate);															// 设置帧率，可能需要改变最大最小曝光时间
	bool GetCurFrameRate(float *pfFrameRate);														// 获取当前帧率
	bool GetFrameRateRange(float *fMinFrameRate, float *fMaxFrameRate);								// 获取相机帧率范围
	// 曝光时间设置
	virtual bool SetCurShutter(float fShutter);														// 设置曝光时间
	virtual bool GetCurShutter(float* pfShutter);													// 获取当前的曝光时间
	virtual bool GetShutterRange(float *pfMinShutter, float *pfMaxShutter);							// 获取曝光时间范围
	// 增益设置
	virtual bool SetCurGain(float fGain);															// 设置增益
	virtual bool GetCurGain(float* pfGain);															// 获取当前增益
	virtual bool GetGainRange(float *pfMinGain, float *pfMaxGain);									// 获取增益范围
	//设置自动白平衡
	virtual bool AutoBalance(double &dR,double &dG,double &dB);										//设置自动白平衡      
	virtual bool GetWhiteBalanceRatioDefault(double &dR,double &dG,double &dB);						//获得默认白平衡参数系数
	virtual bool SetWhiteBalanceRatioDefault(double &dR,double &dG,double &dB);						//恢复默认白平衡参数系数
	virtual bool GetWhiteBalanceRatio(double &dR,double &dG,double &dB);							//获得白平衡参数系数
	virtual bool SetWhiteBalanceRatio(double dR,double dG,double dB);								//设置白平衡参数系数
	//获取像素格式(Mono8=0, BayerRG8=1, BayerGB8=2, BayerGR8=3, BayerBG8=4, RGB8=5)
	virtual bool GetCamPixelFormat(int &nFormat);													//获取当前像素格式
	//设置ROI
	virtual bool GetSensorSizeMax(int &nSensorWidthMax, int &nSensorHeightMax);						//获取最大图像尺寸（芯片相关的固定值）    
	virtual bool GetImageWidthRange(int &nWidthMin, int &nWidthMax, int &nWidthIncrement, int &nWidth);				//获取图像宽度范围，步长    
	virtual bool GetImageHeightRange(int &nHeightMin, int &nHeightMax, int &nHeightIncrement, int &nHeight);		//获取图像高度范围，步长    
	virtual bool GetImageOffsetXRange(int &nOffsetXMin, int &nOffsetXMax, int &nOffsetXIncrement, int &nOffsetX);	//获取图像OffsetX范围，步长   
	virtual bool GetImageOffsetYRange(int &nOffsetYMin, int &nOffsetYMax, int &nOffsetYIncrement, int &nOffsetY);	//获取图像OffsetY范围，步长     
	virtual bool SetImageRoi(bool bEnable, int nOffsetX=0, int nOffsetY=0, int nWidth=640, int nHeight=480);	//设置ROI    

	// Gamma设置
	virtual bool SetGammaEnable(bool bEnable);														// 设置Gamma是否启用
	virtual bool GetGammaEnable(bool* pbEnable);													// 获取Gamma是否启用
	virtual bool SetCurGamma(float fGamma);															// 设置Gamma
	virtual bool GetCurGamma(float *pfGamma);														// 获取当前的Gamma
	virtual bool GetGammaRange(float *pfMinGamma, float *pfMaxGamma);								// 获取Gamma范围

	cpPointCloudGenerateToolBase* GetPointCloudGenerateTool();                                      // 获取点云生成工具指针

	// 自由采集模式
	virtual bool SnapStart();                                                                       // 开始采集
	virtual bool SnapEnd();                                                                         // 采集结束

	virtual bool SetCameraFreeCollectionMode(bool bMode);                                           // 设置自由采集模式(相机)
	virtual bool GetXmlConfigFreeCollectionMode();                                                  // 获取自由采集模式(xml配置)

	// 相机类型
	virtual CameraType CheckSystemCameraType();
	virtual CameraType GetSystemCameraType();

	// 线阵相机行频
	virtual bool SetLineRate(long nLineRate);
	virtual bool GetLineRate(long &nLineRate);

	// 帧率开关
	virtual bool EnableFrameRateSet(bool bEnable);

	// 最大帧率
	virtual bool GetFrameRateMax(float &f);

	// 是否支持速度等级
	virtual bool IsSupportSpeedLevel();

	// 统一参数设置接口
	virtual bool SetCameraPara(CString strFeatureName, bool bFeatureValue);
	virtual bool SetCameraPara(CString strFeatureName, int nFeatureValue);
	virtual bool SetCameraPara(CString strFeatureName, double dFeatureValue);
	virtual bool SetCameraPara(CString strFeatureName, CString strFeatureValue);

	// 统一参数获取接口
	virtual bool GetCameraPara(CString strFeatureName, bool &bFeatureValue);
	virtual bool GetCameraPara(CString strFeatureName, int &nFeatureValue);
	virtual bool GetCameraPara(CString strFeatureName, double &dFeatureValue);
	virtual bool GetCameraPara(CString strFeatureName, CString &strFeatureValue);

protected:
	bool CreateCamera(CString strDeviceID);															// 创建相机采集对象指针
	bool CreateSingleTypeCamera();																	// 创建相机采集对象指针(只支持单采集系统)
	void DeleteCamera(void* pGrabBase);																// 释放相机采集对象指针

protected:
	bool IsCameraPointerValid();
	static void RecordGrabLog(CString strlog);

	BOOL FindSub(CString strSrc, CString strSub);		// 在字符串中查找子字符串，返回TRUE：找到；FALSE：未找到；

protected:
	void*								m_pGrabBase;
	void*								m_pOwner;							// 回调参数
	PTGrabbedCallback					m_funcCallback;						// 回调
	bool								m_bSendCallbackEnable;				// 是否允许回调	
	CString								m_strCurSystemInfo;					// 当前相机系统信息
	int									m_nCurGrabSystemIndex;				// 当前相机系统序号

	static int							m_nCheckDelayTime;					// 查询相机延时
	static BOOL							m_bInited;							// 是否初始化过系统
	static std::vector<GrabSystemInfo>	m_vGrabSystemInfos;					// 采集系统信息

	static BOOL							m_bCameraOfflineModeEnabled;		// 相机离线模式开启
};


#pragma pack(pop)

