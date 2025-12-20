// vsGrabDef.h
// VS camera grab base definition

#pragma once

#include <memory>
#include <map>
#include "svImage.h"
#include "vpImage.h"


#pragma pack(push,8)

// 相机接口类型
typedef enum tagCameraInterfaceType 
{
	eInterfaceUnknow = 0,					// 未知接口
	eInterfaceIEEE1394 = 1,                 // 1394接口
	eInterfaceGigE,						    // GigE接口
	eInterfaceUSB2,						    // USB2.0接口
	eInterfaceUSB3,						    // USB3.0接口
	eInterfaceCamLink						// CamLink接口
}CameraInterfaceType;


// 相机厂家及接口类型
typedef enum tagCameraType 
{
	eCameraTypeUnknow = 0,		    // 未知接口
	eCreIEEE1394 = 1,               // Crevis 1394接口
	eCreGigE	 = 2,				// Crevis GigE接口
	eCreUSB2	 = 3,				// Crevis USB2.0接口
	eCreUSB3	 = 4,				// Crevis USB3.0接口
	eVsaIEEE1394 = 5,				// Vsa 1394接口
	eVsaGigE	 = 6,				// Vsa GigE接口
	eVsaUSB2	 = 7,				// Vsa USB2.0接口
	eVsaUSB3	 = 8,				// Vsa USB3.0接口
	eVsbIEEE1394 = 9,				// Vsb 1394接口
	eVsbGigE	 = 10,				// Vsb GigE接口
	eVsbUSB2	 = 11,				// Vsb USB2.0接口
	eVsbUSB3	 = 12,				// Vsb USB3.0接口
	eVscIEEE1394 = 13,				// Vsc 1394接口
	eVscGigE	 = 14,				// Vsc GigE接口
	eVscUSB2	 = 15,				// Vsc USB2.0接口
	eVscUSB3	 = 16,				// Vsc USB3.0接口
	eBasIEEE1394 = 17,				// Bas 1394接口
	eBasGigE	 = 18,				// Bas GigE接口
	eBasUSB2	 = 19,				// Bas USB2.0接口
	eBasUSB3	 = 20,				// Bas USB3.0接口
	eDalsaGige	 = 21,				// Dalsa GigE接口
	eLJX8IFGige  = 22,              // 3D keyence Gige接口
	eGocatorGige = 23,              // 3D lmi Gige接口
	eEccoGige	 = 24,				// 3D SmartRay Gige接口
	eSSSRGige    = 25,				// 3D SSSR Gige接口
	eHikGige	 = 26,				// 海康线阵Gige接口
	eFocalSpecGige = 27,			// FocalSpec光谱共焦Gige接口
	eHuarayUSB3	 = 28,				// 华睿USB3接口
	eHuarayGige	 = 29,				// 华睿Gige接口
	eHuarayCam	 = 30,				// 华睿相机(USB3+Gige)
	eGocatorSTGige = 31,			// LMI双目结构光相机
	eFlirCam	 = 32,				// FLIR相机
	eFlirUSB3	 = 33,				// FLIR USB3接口相机
	eFlirGige	 = 34,				// FLIR Gige接口相机
	eAikCamLinkCard = 35,			// 埃科 CamLink接口相机
	eHikCamLink	 = 36,				// 海康CamLink接口相机
	eHikCamLinkCard = 37,			// 海康CamLink卡
	eMPCamSUSB3	= 38,				// 盛相3D相机S系列
	eGocator2DGige = 39,			// LMI相机的2D模式
	eHikCamStereo3D = 40 ,			// 海康立体相机3D模式
	eHikCamStereo2D = 41 ,			// 海康立体相机2D模式
	eHikCam	 = 42,				    // 海康相机
	eHikUSB3 = 43,				    // 海康USB3相机
	eHikGigE = 44,				    // 海康GigE相机
	eBslCam  = 45,                  // Bsl相机
	eDaHengUSB2 = 46,               // 大恒USB2相机
	eDaHengUSB3 = 47,               // 大恒USB3相机
	eDaHengGige = 48,               // 大恒Gige相机
	eDaHengCam = 49,                // 大恒相机
	eDalsaCamLinkCard = 50,         // Dalsa CameraLink采集卡
	eHuarayGigE = 51,				// 华睿线扫

}CameraType;


// 相机信息
typedef struct tagPTCameraInfo
{
	unsigned int m_uiIndex;						// 序号
	unsigned int m_uiSerialNum;					// 序列号
	CameraInterfaceType m_CameraInterfaceType;  // 相机接口类型
	CameraType			m_CameraType;			// 相机厂家及接口类型
	char		 m_cVendorName[512];// 相机厂家
	char		 m_cModelName[512];	// 相机型号
	char		 m_cResolution[512];// 相机分辨率
	char		 m_cDeviceID[512];	// 相机ID
	int			 m_nImageWidthMax;	// 图像最大宽度
	int			 m_nImageHeightMax; // 图像最大高度

	tagPTCameraInfo()
	{
		m_uiIndex = -1;
		m_uiSerialNum = 0;
		memset(m_cVendorName, 0, 512);
		memset(m_cModelName, 0, 512);
		memset(m_cResolution, 0, 512);
		memset(m_cDeviceID, 0, 512);
		m_nImageWidthMax = 0;
		m_nImageHeightMax = 0;
	}
}PTCameraInfo, *pPTCameraInfo ;

/** DCAM video modes. */
enum VideoMode
{        
	VIDEOMODE_160x120YUV444, /**< 160x120 YUV444. */
	VIDEOMODE_320x240YUV422, /**< 320x240 YUV422. */
	VIDEOMODE_640x480YUV411, /**< 640x480 YUV411. */
	VIDEOMODE_640x480YUV422, /**< 640x480 YUV422. */
	VIDEOMODE_640x480RGB, /**< 640x480 24-bit RGB. */
	VIDEOMODE_640x480Y8, /**< 640x480 8-bit. */
	VIDEOMODE_640x480Y16, /**< 640x480 16-bit. */
	VIDEOMODE_800x600YUV422, /**< 800x600 YUV422. */
	VIDEOMODE_800x600RGB, /**< 800x600 RGB. */
	VIDEOMODE_800x600Y8, /**< 800x600 8-bit. */
	VIDEOMODE_800x600Y16, /**< 800x600 16-bit. */
	VIDEOMODE_1024x768YUV422, /**< 1024x768 YUV422. */
	VIDEOMODE_1024x768RGB, /**< 1024x768 RGB. */
	VIDEOMODE_1024x768Y8, /**< 1024x768 8-bit. */
	VIDEOMODE_1024x768Y16, /**< 1024x768 16-bit. */
	VIDEOMODE_1280x960YUV422, /**< 1280x960 YUV422. */
	VIDEOMODE_1280x960RGB, /**< 1280x960 RGB. */
	VIDEOMODE_1280x960Y8, /**< 1280x960 8-bit. */
	VIDEOMODE_1280x960Y16, /**< 1280x960 16-bit. */
	VIDEOMODE_1600x1200YUV422, /**< 1600x1200 YUV422. */
	VIDEOMODE_1600x1200RGB, /**< 1600x1200 RGB. */
	VIDEOMODE_1600x1200Y8, /**< 1600x1200 8-bit. */
	VIDEOMODE_1600x1200Y16, /**< 1600x1200 16-bit. */
	VIDEOMODE_FORMAT7, /**< Custom video mode for Format7 functionality. */
	NUM_VIDEOMODES, /**< Number of possible video modes. */
};

// Format7格式
// typedef struct tagPTFormat7Info
// {
// 	Mode         m_mode;			// 模式
// 	unsigned int m_uiStartX;        // 起始X
// 	unsigned int m_uiStartY;		// 起始Y
// 	unsigned int m_uiWidth;         // 宽度
// 	unsigned int m_uiHeight;		// 高度
// 	float        m_fBusSpeed;       // 数据率
// 	PixelFormat  m_pixelFormat;     // 像素格式
// 
// 	tagPTFormat7Info()
// 	{
// 		m_mode			= MODE_0;		
// 		m_uiStartX		= 0;       
// 		m_uiStartY		= 0;		
// 		m_uiWidth		= 0;        
// 		m_uiHeight		= 0;		
// 		m_fBusSpeed		= 100.0;       
// 		m_pixelFormat	= PIXEL_FORMAT_MONO8;   
// 	}
// }PTFormat7Info, *pPTFormat7Info ;

// 采集图像模式
typedef enum tagGrabImageMode
{
	eWaitEvent = 0,                 // 内部事件触发
	eGrabThread					    // 线程方式	
}PTGrabImageMode;

// 相机触发模式
typedef enum tagPTTriggerMode
{
	ePTNotTrigger		= -1,		// 非触发模式
	ePTSoftwareTrigger	= 0,		// 软件触发模式 
	ePTHardwareTrigger	= 1			// 硬件触发模式	
} PTTriggerMode;

typedef enum tagPTTriggerSource
{
	ePTLine0		= 0,			// Line1
	ePTLine1		= 1,			// Line1
	ePTLine2		= 2,			// Line2
	ePTLine3		= 3,			// Line3
	ePTLine4		= 4,			// Line4
} PTTriggerSource;

typedef enum tagPTTriggerActivation
{
	ePTLevelLow		= 0,			// 低电平
	ePTLevelHigh	= 1,			// 高电平
	ePTRisingEdge	= 2,			// 上升沿
	ePTFallingEdge	= 3,			// 下降沿
	ePTAnyEdge		= 4,			// 上升沿或下降沿
} PTTriggerActivation;

// 像素格式
typedef enum tagPTPixelFormat
{
	ePTUnknown	= -1,				// 未知
	ePTGrey8	= 0,				// 8位灰度图像
	ePTRGB24	= 1,				// 24位彩色图像
	ePTRGBA32	= 2					// 32位彩色图像
} PTPixelFormat;

// 相机采集是否成功的状态标志
typedef enum tagPTStatus
{
	ePTGrabOK		= 0,			// 采集成功
	ePTGrabFailed	= 1,			// 采集失败
	ePTGrabTimeout	= 2,			// 采集超时
	ePTOffline		= 3				// 相机掉线
} PTStatus;

// 相机采集线程工作模式
typedef enum tagPTGrabThreadStatus
{
	ePTIdle = -1,					// 空闲
	ePTSnap = 0,					// 单帧
	ePTGrab							// 连续
} PTGrabThreadStatus;


//////////////////////////////////////////////////////////////////////////
// 函数名称：PTGrabbedCallback
// 函数功能: 回调函数类型声明，当相机采集完成后调用该回调函数；
// 参数说明：
//		pOwner:		主调用对象的指针，与SetOwner的输入参数pOwner一致；
//		status:		采集状态枚举：成功、出错或超时
//		pImage:		图像数据指针
//		lWidth:		图像的宽
//		lHeight:	图像的高
//		pixelFormat:图像的像素格式
// 注：
//		如果采集图像成功，则pImage、lWidth、lHeight和pixelFormat为图像相关的有效数据；
//		如果采集图像出错，则pImage、lWidth、lHeight和pixelFormat为无效值，一般为0；
//		如果采集图像超时，则pImage、lWidth、lHeight和pixelFormat为无效值，一般为0；

// typedef void (WINAPI *PTGrabbedCallback)(int nCamIndex, void* pOwner, PTStatus status, unsigned char* pImage 					
// 	,long lWidth, long lHeight, PTPixelFormat pixelFormat);
typedef void (WINAPI *PTGrabbedCallback)(void* pOwner, PTStatus status, unsigned char* pImage 					
	,long lWidth, long lHeight, PTPixelFormat pixelFormat);

class vcGrabBase;
typedef bool (WINAPI *vfGrabInitSystem)(bool bHigherLevelXmlFilePath);			// 初始化系统
typedef bool (WINAPI *vfGrabFreeSystem)();			// 释放系统
typedef bool (WINAPI *vfGrabGetBusCameraCount)(unsigned int* pCamCount);								// 获取总线上相机的数量
typedef bool (WINAPI *vfGrabEnumerateBusCameras)(PTCameraInfo* pCameraInfo, unsigned int* pSize);		// 枚举总线上的可用相机及其信息

typedef CameraType (WINAPI *vfGrabCheckSystemCameraType)(bool bHigherLevelXmlFilePath);			// 检测相机类型
typedef bool (WINAPI *vfGrabGetSystemCameraValid)(bool bHigherLevelXmlFilePath);				// 查询相机是否存在
typedef vcGrabBase*	(WINAPI *vfGrabCreateCamera)();					// 创建相机
typedef void (WINAPI *vfGrabDeleteCamera)(vcGrabBase* pGrabBase);	// 析构相机

#define  CAMERAGRABOKTIME	2000
#define  WAITGRABOKTIME		(CAMERAGRABOKTIME+2000)

#define  AutoBalanceCycleNumber	100
#define  AutoBalanceCycleTimer	20

#pragma pack(pop)