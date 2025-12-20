// vsBaseVisionAlign.h: interface for the scVisionASM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVVisionASM_H__B7EA2F66_DAB1_4018_9973_6538BD23418B__INCLUDED_)
#define AFX_SVVisionASM_H__B7EA2F66_DAB1_4018_9973_6538BD23418B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "SystemOptionConfig.h"
#include "vsVisionAlignDef.h"
#include "vsDisplayModule.h"
#include "SysPlatformInfo.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svTimer.h"
#include "vsSerialComm.h"
#include "vsSocketComm.h"

#include "svGuiCoordCross.h"
#include "svGuiLineSeg.h"
#include "svGuiText.h"
#include "svGuiObround.h"


#include "CommProtocolXYDFast.h"
#include "DmCodeCalibrate.h"
#include "svJpeg.h"
#include "svPng.h"
#include "SystemStatusInfo.h"
#include "VisionAlignGui.h"

#define WM_UPDATE_DISPLAY_IMAGE										WM_USER + 1
#define WM_UPDATE_STATUSBAR_INFO									WM_USER + 2
//#define WM_UPDATE_CALIB_SEARCH_GRAPHIC								WM_USER + 3     
//#define WM_UPDATE_ALIGNER_TARGET_SEARCH_GRAPHIC						WM_USER + 4		//更新定位工具中的搜索结果图形
//#define WM_UPDATE_ALIGNER_OBJECT_SEARCH_GRAPHIC						WM_USER + 5		//更新定位工具中的搜索结果图形
//#define WM_UPDATE_ALIGNER_VIRTUAL_OBJECT_SEARCH_GRAPHIC				WM_USER + 6
#define WM_UPDATE_ALIGNER_PROCESS_INFO								WM_USER + 7
#define WM_CAMERA_ARRIVAL											WM_USER + 8
#define WM_CAMERA_REMOVAL											WM_USER + 9

//#define WM_UPDATE_BENCH_POINT_POS									WM_USER + 10
#define WM_UPDATE_COMM_COMMAND_INFO									WM_USER + 11
//#define WM_UPDATE_DISPLAY_SEARCH_IMAGE								WM_USER + 12
#define WM_UPDATE_CUR_PRODUCT_ALIGN_INFO							WM_USER + 13
#define WM_UPDATE_ALIGN_PRECISION									WM_USER + 14
#define WM_UPDATE_ALIGN_OFFSET										WM_USER + 15
//#define WM_UPDATE_CUR_PLATFORM_AXIS_POS_INFO						WM_USER + 16
#define WM_UPDATE_ALIGNER_TARGET_IMAGE_POS_GRAPHIC					WM_USER + 17		//更新对位流程中定位搜索结果对应的图形 
#define WM_UPDATE_ALIGNER_OBJECT_IMAGE_POS_GRAPHIC					WM_USER + 18		//更新对位流程中定位搜索结果对应的图形
#define WM_UPDATE_ALIGNER_VIRTUAL_OBJECT_IMAGE_POS_GRAPHIC			WM_USER + 19

#define WM_UPDATE_COMM_ROBOT_COMMAND_INFO							 WM_USER + 20
//#define WM_UPDATE_CLEAR_COMM_COMMAND_INFO							 WM_USER + 21
//#define WM_UPDATE_ALIGNER_CALIB_TARGET_SEARCH_GRAPHIC				 WM_USER + 22   //更新定位工具中的搜索结果图形
#define WM_UPDATE_ALIGNER_CALIB_TARGET_IMAGE_POS_GRAPHIC			 WM_USER + 23   //更新对位流程中定位搜索结果对应的图形
#define WM_UPDATE_SINGLE_PLATFORM_INFO								 WM_USER + 24

//#define WM_UPDATE_SHOW_TARGET_IMAGE_POS_TEXT       WM_USER + 25		//更新对位流程中定位搜索结果对应的主界面显示 
//#define WM_UPDATE_SHOW_OBJECT_IMAGE_POS_TEXT       WM_USER + 26		//更新对位流程中定位搜索结果对应的主界面显示
//#define WM_UPDATE_SHOW_VIRTUAL_OBJECT_IMAGE_POS_TEXT       WM_USER + 27
//#define WM_UPDATE_SHOW_CALIB_TARGET_IMAGE_POS_TEXT	     WM_USER + 28

#define WM_UPDATE_ALIGNER_TARGET_ROTATED_IMAGE_POS_GRAPHIC			WM_USER + 29 //XY+D平台 目标旋转后位置显示

#define WM_UPDATE_ALIGNER_OBJECT_LINE_IMAGE_POS_GRAPHIC				WM_USER + 30
#define WM_UPDATE_ALIGNER_TARGET_LINE_IMAGE_POS_GRAPHIC				WM_USER + 31

#define WM_UPDATE_ALIGNER_TARGET_SIZE_INFO							WM_USER + 32	// 更新目标距离信息
#define WM_UPDATE_ALIGNER_OBJECT_SIZE_INFO							WM_USER + 33	// 更新对象距离信息
#define WM_UPDATE_ALIGNER_TARGET_STD_SIZE_INFO						WM_USER + 34	// 更新目标标准距离信息
#define WM_UPDATE_ALIGNER_OBJECT_STD_SIZE_INFO						WM_USER + 35	// 更新对象标准距离信息

//#define WM_UPDATE_ALIGNER_INSPECT_SEARCH_GRAPHIC					WM_USER + 36    //更新定位工具中的搜索结果图形
//#define WM_UPDATE_ALIGNER_INSPECT_IMAGE_POS_GRAPHIC				WM_USER + 37	//更新对位流程中定位搜索结果对应的图形
//#define WM_UPDATE_SHOW_INSPECT_IMAGE_POS_TEXT						WM_USER + 38

//#define WM_UPDATE_ALIGNER_EXTRA_POS_GRAPHIC				WM_USER + 39
//#define WM_UPDATE_ALIGNER_EXTRA_LINE_GRAPHIC			WM_USER + 40
//#define WM_UPDATE_ALIGNER_EXTRA_LINESEG_GRAPHIC			WM_USER + 41
//#define WM_UPDATE_ALIGNER_EXTRA_RECT_GRAPHIC			WM_USER + 42
//#define WM_UPDATE_ALIGNER_EXTRA_CIRCLE_GRAPHIC			WM_USER + 43
//#define WM_UPDATE_ALIGNER_EXTRA_OBROUND_GRAPHIC			WM_USER + 46
//#define WM_UPDATE_ALIGNER_EXTRA_TEXT_GRAPHIC			WM_USER + 44
#define WM_UPDATE_CALIBRATE_TARGET_GRAPHIC							WM_USER + 45


#define WM_UPDATE_ROBOT_UPDATE_DLG_DATA		 WM_USER+50 // 更新机器人所有示教信息
#define WM_UPDATE_ROBOT_MOD					 WM_USER+51 // 更新机器人手动移动速度
#define WM_UPDATE_ROBOT_MSPEED				 WM_USER+52 // 更新机器人手动移动速度
#define WM_UPDATE_ROBOT_CUR_POS				 WM_USER+53 // 更新机器人当前位置信息
#define WM_UPDATE_ROBOT_TEACH_POS			 WM_USER+54 // 更新机器人示教点信息
#define WM_UPDATE_ROBOT_SET_POS				 WM_USER+55 // 更新保存机器人示教点信息
#define WM_UPDATE_ROBOT_SGI					 WM_USER+56 // 更新保存机器人示教常量信息
#define WM_UPDATE_ROBOT_SGR					 WM_USER+57 // 更新保存机器人示教常量信息

//#define WM_UPDATE_ALIGN_PRECISION2						 WM_USER + 60
#define WM_UPDATE_CHANGE_PRODUCT_SET_DISPLAY			WM_USER + 61 // 通信指令切换品种，加载新品种时向定位工具设置GuiDisplay和删除老品种时删除定位工具指针

#define WM_UPDATE_WARNING_INFO							WM_USER + 62	// zzc更新报警信息
#define WM_UPDATE_CLEAR_WARNING_INFO					WM_USER + 63	// zzc清空报警信息
#define WM_SHOW_OPERATION_DLG							WM_USER + 64	// zzc清空报警信息
#define WM_SAVE_VDB_FILE                                WM_USER + 65	//存储VDB文件

//#define WM_UPDATE_ALIGNER_EXTRA_AFFINE_RECT_GRAPHIC			WM_USER + 66
//#define WM_UPDATE_ALIGNER_EXTRA_CONST_TEXT_GRAPHIC			WM_USER + 67

#define WM_UPDATE_ALIGNER_GUI_SHOW							WM_USER+68
#define WM_UPDATE_ALIGNER_COMBINE_GUI_SHOW					WM_USER+69

#define WM_UPDATE_ALIGNER_TARGET_ANGLE_INFO            WM_USER + 70	// 更新目标距离信息
#define WM_UPDATE_ALIGNER_OBJECT_ANGLE_INFO            WM_USER + 71	// 更新对象距离信息
#define WM_UPDATE_ALIGNER_TARGET_STD_ANGLE_INFO        WM_USER + 72	// 更新目标标准距离信息
#define WM_UPDATE_ALIGNER_OBJECT_STD_ANGLE_INFO        WM_USER + 73	// 更新对象标准距离信息

#define WM_UPDATE_BEND_INSPECT_RESULT				   WM_USER + 80
#define WM_UPDATE_BEND_CIRCLE_RESULT				   WM_USER + 81
#define WM_UPDATE_BEND_DETAL_RESULT					   WM_USER + 82

#define  WM_UPDATE_ALINGER_OFFSET_AREA_GUI_SHOW		   (WM_USER+86)


#define MAXWARNINGCOUNT					 1000
#define INITSEARCHTOOLTHREAD_WAITINGTIME 300000 //工具初始化模板线程最长等待时间
#define SAVESEARCHTOOLTHREAD_WAITINGTIME 300000 //工具保存模板线程最长等待时间

#define COMM_WAIT_TIME					 60000
#define WAIT_THREAD_QUIT_TIME			 5000
#define WAIT_THREAD_SUSPEND_TIME		 10000

#define TARGETSIZEMAXNUM				 6		// 目标距离最大的数量
#define OBJECTSIZEMAXNUM				 6		// 对象距离最大的数量
#define MUTITARGETMAXNUM				 8		// 最大多目标数  2018.8.24 qw



#define	 EXTRA_GRAPHICS_NUM_POS			25
#define	 EXTRA_GRAPHICS_NUM_LINE		25
#define	 EXTRA_GRAPHICS_NUM_LINESEG		25
#define	 EXTRA_GRAPHICS_NUM_RECT		25
#define  EXTRA_GRAPHICS_NUM_CIRCLE		25
#define  EXTRA_GRAPHICS_NUM_OBROUND		25
#define	 EXTRA_GRAPHICS_NUM_TEXT		25
#define	 EXTRA_GRAPHICS_NUM_CONST_TEXT	5
#define  EXTRA_GRAPHICS_NUM_AFFINERECT  25


#define  INIT_SUCCEED                         0
#define  INIT_CAMERA_FAILED                   0x80000000             
#define  INIT_CAMERA_PARAM_FAILED             0x40000000 
#define  INIT_COMMU_FAILED                    0x20000000
#define  INIT_VISION_ALIGN_TOOL_FAILED        0x10000000 
#define  INIT_CREATE_THREAD_FAILED            0x08000000

typedef std::vector<bool> VbSearchToolChanged;//工具是否修改的标识符

// 相机像素格式
typedef enum tagCamPixelFormat
{
	eCamColorUnknown = -1,	// 未知类型	
	eCamGray,				// 黑白
	eCamBayer_RG8,				// Bayer
	eCamBayer_GB8,				// Bayer
	eCamBayer_GR8,				// Bayer
	eCamBayer_BG8,				// Bayer
	eCamRGBPacked,			// RGBPacked
}CamPixelFormat;

// 尺寸检测状态
typedef enum eSizeCheckStatus
{
	eSCSUnCheck = 0,		// 未检测
	eSCSCheckOK = 1,		// OK
	eSCSCheckNG = 2,		// NG
} SizeCheckStatus;

typedef enum emnuCSearchMode
{
	eNoSearch     = -1,
	eCalibSearch  = 0, 
	eTargetSearch = 1,
	eBenchSearch  = 2,
	eObjectSearch = 3,
	eVirtualSearch = 4,
	eCalibTargetSearch = 5,
	eInspectSearch = 6,
	eAutoCalibTargetSearch = 7,
	eCalibWholeDmcodeBoard = 8,//全局二维码标定时图像到靶标映射搜索
}SearchMode;

typedef enum emnuCameraExposureType
{
	eNoSearchCameraExposure     = -1,
	eCalibSearchCameraExposure  = 0, 
	eTargetSearchCameraExposure = 1,
	eBenchSearchCameraExposure  = 2,
	eObjectSearchCameraExposure = 3,
	eVirtualSearchCameraExposure = 4,
	eCalibTargetSearchCameraExposure = 5,
	eInspectSearchCameraExposure = 6,
	eDmCodeSearchCameraExposure = 7,//二维码搜索切换曝光

}CameraExposureType;

//检测结果结构体:主界面实时显示
const int nInspectDistNum = 2;
typedef struct tabBendInspectResultInfo
{
	double m_dLx;
	double m_dRx;

	double m_dLy;
	double m_dRy;

	double m_dLCenterx;
	double m_dRCenterx;

	double m_dLCentery;
	double m_dRCentery;

	double m_dDistanceL;
	double m_dDistanceR;

	double m_dDistanceSubX;
	double m_dDistanceSubY;

	double m_dTargetDistance;
	double m_dObjectDistance;

	double m_dCircleRadius[nInspectDistNum];
	double m_dCircleRadiusX[nInspectDistNum];
	double m_dCircleRadiusY[nInspectDistNum];

	CString m_strProductID;
	CString m_strProductSTATION;
	CString m_strInspectStation;
	bool m_strProductOKNG;

	double m_dAxisX;
	double m_dAxisY;
	double m_dAxisD;

	//DeltaY
	double m_dLDeltaY;
	double m_dRDeltaY;
	double m_dDeltaY;

	tabBendInspectResultInfo()
	{
		m_dLx = 0.0;
		m_dRx = 0.0;

		m_dLy = 0.0;
		m_dRy = 0.0;


		m_dLCenterx = 0.0;
		m_dLCentery = 0.0;

		m_dRCenterx = 0.0;
		m_dRCentery = 0.0;


		m_dDistanceL = 0.0;
		m_dDistanceR = 0.0;

		m_dDistanceSubX = 0.0;
		m_dDistanceSubY = 0.0;

		m_dAxisX = 0.0;
		m_dAxisY = 0.0;
		m_dAxisD = 0.0;

		for(int i = 0;i<nInspectDistNum;i++)
		{
			m_dCircleRadius[i] = 0.0;
			m_dCircleRadiusX[i] = 0.0;
			m_dCircleRadiusY[i] = 0.0;
		}

		m_dTargetDistance = 0.0;
		m_dObjectDistance = 0.0;

		m_strProductID = _T("");
		m_strProductSTATION = _T("");
		m_strProductOKNG = false;
		m_strInspectStation = _T("");

		m_dLDeltaY = 0.0;
		m_dRDeltaY = 0.0;
		m_dDeltaY = 0.0;
	}


	tabBendInspectResultInfo& operator= (tabBendInspectResultInfo &inspectResultInfo)
	{
		if (this == &inspectResultInfo)
		{
			return *this;
		}

		m_dLx			= inspectResultInfo.m_dLx;
		m_dRx			= inspectResultInfo.m_dRx;
		m_dLy			= inspectResultInfo.m_dLy;
		m_dRy			= inspectResultInfo.m_dRy;

		m_dLCenterx		= inspectResultInfo.m_dLCenterx;
		m_dLCentery		= inspectResultInfo.m_dLCentery;
		m_dRCenterx		= inspectResultInfo.m_dRCenterx;
		m_dRCentery		= inspectResultInfo.m_dRCentery;

		m_dDistanceL	= inspectResultInfo.m_dDistanceL;
		m_dDistanceR	= inspectResultInfo.m_dDistanceR;

		m_dDistanceSubX	= inspectResultInfo.m_dDistanceSubX;
		m_dDistanceSubY	= inspectResultInfo.m_dDistanceSubY;

		m_dAxisX		= inspectResultInfo.m_dAxisX;
		m_dAxisY		= inspectResultInfo.m_dAxisY;
		m_dAxisD		= inspectResultInfo.m_dAxisD;

		for (int i = 0; i < nInspectDistNum; i++)
		{
			m_dCircleRadius[i]			= inspectResultInfo.m_dCircleRadius[i];
			m_dCircleRadiusX[i]			= inspectResultInfo.m_dCircleRadiusX[i];
			m_dCircleRadiusY[i]			= inspectResultInfo.m_dCircleRadiusY[i];
		}

		m_dTargetDistance = inspectResultInfo.m_dTargetDistance;
		m_dObjectDistance = inspectResultInfo.m_dObjectDistance;

		m_strProductID = inspectResultInfo.m_strProductID;
		m_strProductSTATION = inspectResultInfo.m_strProductSTATION;
		m_strInspectStation = inspectResultInfo.m_strInspectStation;
		m_strProductOKNG = inspectResultInfo.m_strProductOKNG;

		m_dLDeltaY = inspectResultInfo.m_dLDeltaY;
		m_dRDeltaY = inspectResultInfo.m_dRDeltaY;
		m_dDeltaY = inspectResultInfo.m_dDeltaY;

		return *this;
	}


}BendInspectResultInfo;


// 机器人事件类型
typedef enum enumRobotEvent
{
	eRobotIdle = 0,
	eRobotAutoControlRun = 1,		// 1）自动模式运行
	eRobotSetModManual,				// 2）手动模式；
	eRobotGetTeachInfo,				// 3）查询所有示教信息；
	eRobotSetMSpeed,				// 4）机器人手动移动速度设定（mspeed）；
	eRobotInch,						// 5）机器人移动命令（inch X、Y、Z等）；
	eRobotGetPosWHRXY,				// 6）机器人当前位置查询（WHRXY）；	
	eRobotTeachPos,					// 7）设置机器人示教点位（TEACH 200）；
	eRobotReadPos,					// 9）机器人点位查询（Read P200）；
	eRobotMovePos,					// 10）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）；
	eRobotETX,						// 11)机器人中断处理
	eRobotSaveTeachInfo,			// 12) 保存示教信息到机器人；
	eRobotSetPos,					// 13) 保存点位置到机器人；
	eRobotGetPosWHRXYMSpeed,			// 14）查询机器人当前位置和速度；
	eRobotSetSGISGR,					// 15）设置机器人示教点位（SGI、SGR）；
	eRobotGetSGISGR,					// 16）查询机器人示教点位（SGI、SGR）；
} RobotEvent;


//保存产品模板线程的传输参数
typedef struct tagSaveSearchToolRepositoryThreadParam
{
	CString m_strPatternsDir;						// 当前模板的路径
	int  m_nPosIndex;								// 相机位置
	VpSearchTool m_vpSearchTool;					// 当前工具
	VbSearchToolChanged m_vbSearchToolChanged;		// 定位工具是否修改的标识符

private:

	vector<bool> m_vbOK;							// 保存成功标识符 false保存失败，true保存成功
	bool m_bAllOK;									// 当前位置模板是否全部保存成功
	bool m_bTimeOut;								// 是否保存超时

public:

	tagSaveSearchToolRepositoryThreadParam()
	{
		m_strPatternsDir = "";
		m_nPosIndex = 0;
		m_vbOK.resize(0,true);
		m_bAllOK = true;
		m_vpSearchTool.resize(0,NULL);
		m_vbSearchToolChanged.resize(0,true);
		m_bTimeOut = true;
	}

	tagSaveSearchToolRepositoryThreadParam(CString StrPatternsDir,int PosIndex,VpSearchTool vpSearchTool)
	{
		m_strPatternsDir			= StrPatternsDir;
		m_nPosIndex				= PosIndex;
		m_vbOK.resize(vpSearchTool.size(),true);
		m_bAllOK					= true;
		m_vpSearchTool				= vpSearchTool;
		m_vbSearchToolChanged.resize(vpSearchTool.size(),true);
		m_bTimeOut = true;
	}

	tagSaveSearchToolRepositoryThreadParam(CString StrPatternsDir,int PosIndex,VpSearchTool vpSearchTool,VbSearchToolChanged vbSearchToolChanged)
	{
		m_strPatternsDir			= StrPatternsDir;
		m_nPosIndex					= PosIndex;
		m_vbOK.resize(vpSearchTool.size(),true);
		m_bAllOK					= true;
		m_vpSearchTool				= vpSearchTool;
		m_vbSearchToolChanged.resize(vbSearchToolChanged.size());
		for (int i=0;i < vbSearchToolChanged.size();i++)
		{
			m_vbSearchToolChanged[i]= vbSearchToolChanged[i];
		}
		m_bTimeOut = true;
	}

	//设置指定位置模板是否保存成功
	bool SetSearchToolSaveResult(bool bIsSaveSucess,int nCandidateIndex)
	{
		if(nCandidateIndex < 0 || nCandidateIndex >= m_vbOK.size())
		{
			return false;
		}
		m_vbOK[nCandidateIndex]=bIsSaveSucess;
		return true;
	}

	//获取指定位置模板是否保存成功
	bool GetSearchToolSaveResult(int nCandidateIndex)
	{
		if(nCandidateIndex < 0 || nCandidateIndex >= m_vbOK.size())
			return false;
		return m_vbOK[nCandidateIndex];
	}

	//设置指定位置所有候补模板是否保存成功
	void SetAllSearchToolSaveResult(bool bIsSaveAllSucess)
	{
		m_bAllOK = bIsSaveAllSucess;
	}

	//获取指定位置所有候补模板是否保存成功
	bool GetAllSearchToolSaveResult()
	{
		return m_bAllOK;
	}

	//设置当前线程是否保存超时
	void SetIsSaveTimeOut(bool bIsTimeOut)
	{
		m_bTimeOut = bIsTimeOut;
	}

	//获取当前线程是否保存超时
	bool GetIsSaveTimeOut()
	{
		return m_bTimeOut;
	}

}CSaveSearchToolRepositoryThreadParam;

//加载产品模板线程的传输参数
typedef struct tagInitSearchToolThreadParam
{
	vcBaseVisionAlign *m_pBaseVisionAlign;		//当前界面的指针
	CString m_strPatternsDir;					//当前模板的路径
	int m_nCandidateModelsNum;					//候补模板数量
	int m_nPosIndex;							//位置序号
	VpSearchTool m_vpAlnSearchToolRepo;			//暂时存储工具的变量
	VbSearchToolChanged m_vbSearchToolChanged;		// 定位工具是否修改的标识符
private:
	vector<bool> m_vbOK;						//保存成功标识符 false保存失败，true保存成功
	bool m_bTimeOut;							// 是否初始化超时
public:
	tagInitSearchToolThreadParam()
	{
		m_pBaseVisionAlign = NULL;
		m_nCandidateModelsNum = 5;
		m_strPatternsDir = "";
		m_nPosIndex = 0;
		m_vpAlnSearchToolRepo.resize(0);
		m_vbSearchToolChanged.resize(0,true);
		m_vbOK.resize(0);
		m_bTimeOut = true;
	}
	tagInitSearchToolThreadParam(vcBaseVisionAlign *BaseVisionAlign,CString StrPatternsDir,int nCandidateModelsNum,int nPosIndex)
	{
		m_pBaseVisionAlign		= BaseVisionAlign;
		m_strPatternsDir		= StrPatternsDir.GetBuffer();
		m_nCandidateModelsNum	= nCandidateModelsNum;
		m_nPosIndex			= nPosIndex;
		m_vbOK.resize(nCandidateModelsNum,false);
		m_vpAlnSearchToolRepo.resize(nCandidateModelsNum);
		m_vbSearchToolChanged.resize(nCandidateModelsNum,true);
		m_bTimeOut = true;
	}

	tagInitSearchToolThreadParam(vcBaseVisionAlign *BaseVisionAlign,CString StrPatternsDir,int nCandidateModelsNum,int nPosIndex,VbSearchToolChanged vbSearchToolChanged)
	{
		m_pBaseVisionAlign		= BaseVisionAlign;
		m_strPatternsDir		= StrPatternsDir.GetBuffer();
		m_nCandidateModelsNum	= nCandidateModelsNum;
		m_nPosIndex			= nPosIndex;
		m_vbOK.resize(nCandidateModelsNum,false);
		m_vpAlnSearchToolRepo.resize(nCandidateModelsNum);
		m_vbSearchToolChanged.resize(nCandidateModelsNum);
		for (int i=0;i<vbSearchToolChanged.size();i++)
		{
			m_vbSearchToolChanged[i]=vbSearchToolChanged[i];
		}
		m_bTimeOut = true;
	}
	// 增加常量拷贝构造函数
	tagInitSearchToolThreadParam(const tagInitSearchToolThreadParam& rhs)
	{
		m_pBaseVisionAlign = rhs.m_pBaseVisionAlign;
		m_strPatternsDir = rhs.m_strPatternsDir;
		m_nCandidateModelsNum = rhs.m_nCandidateModelsNum;
		m_nPosIndex = rhs.m_nPosIndex;
		m_bTimeOut = rhs.m_bTimeOut;

		m_vbOK.resize(rhs.m_vbOK.size());
		for (int i = 0; i < rhs.m_vbOK.size(); i++)
		{
			m_vbOK[i] = rhs.m_vbOK[i];
		}

		m_vpAlnSearchToolRepo.resize(rhs.m_vpAlnSearchToolRepo.size());
		for (int i = 0; i < rhs.m_vpAlnSearchToolRepo.size(); i++)
		{
			m_vpAlnSearchToolRepo[i] = rhs.m_vpAlnSearchToolRepo[i];
		}
	}
	//设置指定位置模板是否初始化成功
	bool SetSearchToolInitResult(bool bIsSaveSucess,int nCandidateIndex)
	{	
		if(nCandidateIndex < 0 || nCandidateIndex >= m_vbOK.size())
		{
			return false;
		}
		m_vbOK[nCandidateIndex]=bIsSaveSucess;
		return true;
	}
	//获取指定位置模板是否初始化成功
	bool GetSearchToolInitResult(int nCandidateIndex)
	{
		if(nCandidateIndex < 0 || nCandidateIndex >= m_vbOK.size())
			return false;
		return m_vbOK[nCandidateIndex];
	}

	//设置当前线程是否初始化超时
	void SetIsInitTimeOut(bool bIsTimeOut)
	{
		m_bTimeOut = bIsTimeOut;
	}
	//获取当前线程是否初始化超时
	bool GetIsInitTimeOut()
	{
		return m_bTimeOut;
	}

} CInitSearchToolThreadParam;

typedef struct TagAlignerProcessInfo
{
	double dTime;   // 对位时间
	int    nTimes;  // 对位次数
	double dXMove;  // X轴移动量(mm)
	double dYMove;  // Y轴移动量(mm)
	double dDMove;  // D轴移动量(mm)

	double dXOffset;  // 产品X偏移量(mm)
	double dYOffset;  // 产品Y偏移量(mm)
	double dDOffset;  // 产品D偏移量(mm)

	TagAlignerProcessInfo()
	{
		dTime   = 0.0;
		nTimes  = 0;
		dXMove = 0.0;
		dYMove  = 0.0;
		dDMove  = 0.0;

		dXOffset = 0.0;
		dYOffset = 0.0;
		dDOffset = 0.0;
	}

	TagAlignerProcessInfo& operator= (TagAlignerProcessInfo &alignerProcessInfo)
	{
		if (this == &alignerProcessInfo)
		{
			return *this;
		}

		dTime   = alignerProcessInfo.dTime;
		nTimes  = alignerProcessInfo.nTimes;
		dXMove  = alignerProcessInfo.dXMove;
		dYMove  = alignerProcessInfo.dYMove;
		dDMove  = alignerProcessInfo.dDMove;

		dXOffset = alignerProcessInfo.dXOffset;
		dYOffset = alignerProcessInfo.dYOffset;
		dDOffset = alignerProcessInfo.dDOffset;

		return *this;
	}

}AlignerProcessInfo;

// 目标距离信息
typedef struct tagAlignerTargetSizeInfo
{
	std::vector<CString> m_vstrTargetSizeName;
	//	std::vector<CString> m_vstrTargetSizeUnit;
	std::vector<double> m_vdTargetSize;
	std::vector<SizeCheckStatus> m_vnCheckStatus;
	int m_nExProductIndex;

	tagAlignerTargetSizeInfo()
	{
		m_vstrTargetSizeName.resize(TARGETSIZEMAXNUM);
		//		m_vstrTargetSizeUnit.resize(TARGETSIZEMAXNUM);
		m_vdTargetSize.resize(TARGETSIZEMAXNUM, 0);
		m_vnCheckStatus.resize(TARGETSIZEMAXNUM, eSCSUnCheck);
		m_nExProductIndex = 0;
	}
}AlignerTargetSizeInfo;

// 目标角度信息
typedef struct tagAlignerTargetAngleInfo
{
	std::vector<CString> m_vstrTargetAngleName;
	//	std::vector<CString> m_vstrTargetAngleUnit;
	std::vector<double> m_vdTargetAngle;
	std::vector<SizeCheckStatus> m_vnCheckStatus;
	int m_nExProductIndex;

	tagAlignerTargetAngleInfo()
	{
		m_vstrTargetAngleName.resize(TARGETSIZEMAXNUM);
		//		m_vstrTargetAngleUnit.resize(TARGETSIZEMAXNUM);
		m_vdTargetAngle.resize(TARGETSIZEMAXNUM, 0);
		m_vnCheckStatus.resize(TARGETSIZEMAXNUM, eSCSUnCheck);
		m_nExProductIndex = 0;
	}
}AlignerTargetAngleInfo;

// 对象距离信息
typedef struct tagAlignerObjectSizeInfo
{
	std::vector<CString> m_vstrObjectSizeName;
	//	std::vector<CString> m_vdObjectSizeUnit;
	std::vector<double> m_vdObjectSize;
	std::vector<SizeCheckStatus> m_vnCheckStatus;

	tagAlignerObjectSizeInfo()
	{
		m_vstrObjectSizeName.resize(OBJECTSIZEMAXNUM);
		//		m_vdObjectSizeUnit.resize(OBJECTSIZEMAXNUM);
		m_vdObjectSize.resize(OBJECTSIZEMAXNUM, 0);
		m_vnCheckStatus.resize(OBJECTSIZEMAXNUM, eSCSUnCheck);
	}
}AlignerObjectSizeInfo;

// 对象角度信息
typedef struct tagAlignerObjectAngleInfo
{
	std::vector<CString> m_vstrObjectAngleName;
	//	std::vector<CString> m_vdObjectAngleUnit;
	std::vector<double> m_vdObjectAngle;
	std::vector<SizeCheckStatus> m_vnCheckStatus;

	tagAlignerObjectAngleInfo()
	{
		m_vstrObjectAngleName.resize(OBJECTSIZEMAXNUM);
		//		m_vdObjectAngleUnit.resize(OBJECTSIZEMAXNUM);
		m_vdObjectAngle.resize(OBJECTSIZEMAXNUM, 0);
		m_vnCheckStatus.resize(OBJECTSIZEMAXNUM, eSCSUnCheck);
	}
}AlignerObjectAngleInfo;

typedef struct stUploadData
{
	CString strSavePath;
	CString strUploadPath;
	int nTimeInterval;
	CString strWorkType;
	stUploadData()
	{
		nTimeInterval = 5;
		strSavePath = _T("E:\\2DCodeRecord");
		strUploadPath = _T("E:\\UploadData");
		strWorkType = _T("Product");
	}
}STUPLOADDATA;

typedef struct tagAlignerAxisInfo
{
	std::vector<double> m_vdAxis;
	tagAlignerAxisInfo()
	{
		m_vdAxis.resize(3,0.0);
	}
}AlignerAxisInfo;


// typedef enum eSearchAllResultStatus
// {
// 	eSARSResultOK = 0,		// PCB 搜索ok
// 	eSARSResultNG1 = 1,		// PCB 搜索ng=1
// 	eSARSResultNGMore = 2,		// NG>1
// } SearchAllResultStatus;


// 光源切换回调
class	CLightCtrlObject
{
public:
	CLightCtrlObject()
	{
		m_pOwner =NULL;
		m_nLightCtrlIndex = -1;
	};
	~CLightCtrlObject(){};

public:
	void*	m_pOwner;				 // 调用对象
	int		m_nLightCtrlIndex;	     // 控制器的编号
};


// 相机回调对象类
class	CCameraCallbackObject
{
public:
	CCameraCallbackObject(){m_pOwner =NULL;m_nCamIndex=-1;m_nPatIndex=-1;};
	~CCameraCallbackObject(){};

public:
	void*	m_pOwner;				// 调用对象
	int		m_nCamIndex;			// 用于标示相机的序号
	int		m_nPatIndex;			// 用于标识定位模板的序号
};

// 定位搜索回调
class	CSearchObject
{
public:
	CSearchObject(){m_pOwner =NULL;m_nPosIndex = -1; m_nCamIndex=-1;m_nPatIndex=-1;};
	~CSearchObject(){};

public:
	void*	m_pOwner;				// 调用对象
	int		m_nPosIndex;			// 用于位置的序号
	int		m_nCamIndex;			// 用于标示相机的序号
	int		m_nPatIndex;
};

// 相机曝光回调
class	CCameraExposureObject
{
public:
	CCameraExposureObject(){m_pOwner =NULL;m_nPosIndex = -1; m_nCamIndex=-1;m_nPatIndex=-1;};
	~CCameraExposureObject(){};

public:
	void*	m_pOwner;				// 调用对象
	int		m_nPosIndex;			// 用于位置的序号
	int		m_nCamIndex;			// 用于标示相机的序号
	int		m_nPatIndex;
};

// 存图线程参数
class CSaveImageObject
{
public:
	CSaveImageObject(){m_pOwner =NULL;m_nPosIndex = -1; m_nCamIndex=-1;m_nPatIndex=-1;};
	~CSaveImageObject(){};

public:
	void*	m_pOwner;				// 调用对象
	int		m_nPosIndex;			// 用于位置的序号
	int		m_nCamIndex;			// 用于标示相机的序号
	int		m_nPatIndex;
};



//图像存储格式转换类
//*****************
//功能：提供bmp、jpg、png格式图像的读取和存储
//*****************
class CImageFormatTransfer
{
public:
	CImageFormatTransfer(void){}
	virtual ~CImageFormatTransfer(void){}
public:
	BOOL Read(CString strFilePath, scImageGray& image,CImageType eImageType = eBmp);
	BOOL Read(CString strFilePath, scImageRGB& image,CImageType eImageType = eBmp);
	BOOL Read(CString strFilePath, cpImage& image,CImageType eImageType = eBmp);

	BOOL Write(CString strFilePath, const scImageGray& image,CImageType eImageType = eBmp,int nQuality = 100);	
	BOOL Write(CString strFilePath, const scImageRGB& image,CImageType eImageType = eBmp,int nQuality = 100);	
	BOOL Write(CString strFilePath, cpImage& image,CImageType eImageType = eBmp,int nQuality = 100);	

};

#include "DlgVisionRun.h"
#include "DlgVisionRunProductInfo.h"
// 视觉对位处理:基本处理流程
class vcBaseVisionAlign
{
public:
	vcBaseVisionAlign();
	virtual ~vcBaseVisionAlign();
	virtual void DestroyWorkThread();

	BOOL  m_bTagetLightFinish;
	BOOL  m_bFaalBendProcess;

public:
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMinute;

public:
	bool m_bNeedUpdateAddrTime;



public:

	BOOL                               m_bGetAutoCalibrateBaseAxisSucceed;
	int                                m_nCalibTargetMovePosIndex;
	CMarkImagePos                      m_mpBenchMarkImagePos;       // 基准目标定位结果
	
	BOOL							   m_nCheckTargetDistanceIndex;		// 6相机8位置；目标2相机，4位置；检测目标距离；
	BOOL							   m_nCheckObjectDistanceIndex;		// 6相机8位置；对象2相机，4位置；检测对象距离； 


protected:
	std::vector<sc2Vector> m_vBoardPosForTarFixOffset;//目标采用二维码靶标关联且计算目标固定补偿时，各位置MARK的二维码靶标坐标
	HANDLE*							   m_phPosEvent;
	CString m_strProductID;
	CString m_strProductSTATION;
	CString m_strInspectStation;

public:
	CBenchTargetMarkPickInfo*		 m_pBenchMarkPickInfo;
	std::vector<CMarkImagePos*>      m_vpMarkImagePos;  //标定时图像定位结果


protected:
	std::vector<CRITICAL_SECTION>      m_vcsResource;
	CRITICAL_SECTION                   m_csResource;
	CRITICAL_SECTION                   m_csResourceComm;
	CRITICAL_SECTION                   m_csResourceCommName;		// 通信指令
	CRITICAL_SECTION				   m_csResourceWarning;


	CRITICAL_SECTION                   m_csBendInspectResultLine;
	CRITICAL_SECTION                   m_csAlignerProcessInfoLine;

public:
	CRITICAL_SECTION                   m_csAlignerPrecision;
	CRITICAL_SECTION                   m_csAlignerMaxTime;
	CRITICAL_SECTION                   m_csAlignerOffset;
	CRITICAL_SECTION                   m_csAlignCheckInfo;


public:

	BOOL m_bIsSetShutterInMain;		// 是否正在进行主界面上设置曝光
	int	m_nPickMutiExMode;					// 多吸头应用多扩展模式

	// 指令流程
public:	

	//判断是否启用多目标定位	
//	virtual void SetMutiTarget();
	virtual BOOL SetProductCameraShutter(int nIndex) = 0;
	CString GetDataRecordDirectory();//获得存储的文件夹


	virtual void UpdateChangeProductSetDisplay(int nProductIndex, int nToolType);


	virtual BOOL	SetCurProductName(CString strCurProductName);			// 根据名称设置当前产品
	virtual BOOL	SetCurProductIndex(int nCurProductIndex);				// 根据索引设置当前产品
	virtual BOOL	SetCurCalibDataIndex(int nCurProductIndex);				// 根据索引设置当前产品

	virtual BOOL	SetCurCalibDataAndProductIndex(int nCurProductIndex);	// 根据索引设置当前标定产品、当前产品

	virtual BOOL ProductEmpty(int nProductIndex); // 清空产品
	virtual BOOL ProductCopy(int nSrcProductIndex, int nDstProductIndex); // 复制产品
	virtual BOOL ProductNew(int nProductIndexFa, int nProductIndexChd); // 新建产品
	virtual BOOL CopyXMLDataFile(int nSrcProductIndex, int nDstProductIndex); // 复制产品XML文件
	virtual BOOL CopyCalibXMLDataFile(int nSrcProductIndex, int nDstProductIndex); // 复制标定XML文件

	virtual BOOL	SetCurProductVirtualAln(BOOL bVAlnOn);				// 根据索引设置当前产品


	// 进行检测时隐藏对象和目标的GUI
	void HideObjTarGuiResultInSpect(std::vector<int> vnPosIndex, BOOL bShow);

	void SetMultiEXObjectMarkSearchResultsWithCombinationCode(CMarkImagePos ImageResults);//设置扩展产品中带有组合码的对象拍照结果
	void SetMultiEXTargetMarkSearchResultsWithCombinationCode(CMarkImagePos ImageResults);//设置扩展产品中带有组合码的对象拍照结果
	void PostMessageShowMutiTargetImagePosToDisplay(BOOL bSuccess);//多目标对位时，显示目标GUI

	
protected:	

	virtual BOOL ExecuteAlnCrossBenchSearch(int nPosIndex, int nPatIndex, CSearchResult& result,BOOL bZeroAngle = 0);	// 执行实时对象对位定位
	// 通信指令切换品种，加载新品种时向定位工具设置GuiDisplay和删除老品种时删除定位工具指针，从工作线程直接操作改为向主线程发送消息间接操作，避免上述两种操作和主界面连续采集状态刷新GUI并行调用冲突造成软件崩溃
	virtual BOOL SendMessageTimeOutUpdateChangeProductSetDisplay(int nProductIndex, int nToolType, int nTimeOut = 10000);
	virtual BOOL ExecuteAlnCalibTargetAutoSearch(int nIndex);




	// 多目标
public:

	int												m_nMutiTargetCurIndex;				// 多目标定位时当前选择的目标序号 qw
	std::vector<BOOL>								m_vbMutiTargetMarkOK;				// 多目标目标拍照是否OK
	std::vector<CMarkImagePos>						m_vMutiTargetMarkImagePos;			// 多目标定位结果 2018.8.24 qw
	std::vector<std::vector<CPlatformAxisPos*>>		m_vpMutiTargetPlatformAxisPos;		// 多目标定位目标轴位置 2018.8.25 qw



	// 手动定位
public:
	static void CreateManualSearchSection();	// 创建手动搜索临界区
	static void DeleteManualSearchSection();	// 删除手动搜索临界区
	void ExecuteManualSearchMark(SearchMode searchMode);	// 设置手动搜索Mark类型
	BOOL ExcuteManualSearch(int nMarkType,int nPosIndex);
	BOOL ExcuteManualSearch(int nMarkType,int nPosIndex,int nMoveIndex,int nMarkNum = 1);
	BOOL ExcuteManualSearch(int nMarkType,std::vector<int> nPosIndex);

	BOOL ExcuteManualSearch_TX(int nMarkType,std::vector<int> nPosIndex);
	//折弯用手动弹窗
	BOOL ExcuteManualSearch_EX(int nMarkType,std::vector<int> nPosIndex,std::vector<BOOL> nPosIndex1 );

protected:
	BOOL                               m_bExitManualSearchThread;           // 手动搜索Mark线程是否退出的标志
	CWinThread*                        m_pManualSearchThread;   

	SearchMode						   m_nManualSearchMarkMode;				// 手动搜索Mark类型
	int								   m_nManualTargetSearchIndex;			// 手动目标搜索序号
	int								   m_nManualObjectSearchIndex;			// 手动对象搜索序号

public:
	static CRITICAL_SECTION            m_gManualSearchSection;




	// 初始化
public:
	void VisionAlignLogRecord(CString strPathFileName, CString strLog, BOOL bLogTimeEnable = TRUE);	// 日志
	void VisionAlignLogRecordCsv(CString strPathFileName, CString strLog, BOOL bLogTimeEnable = TRUE);	// 存储Csv数据
	void AlignLogRecord(CString strlog);
	void AlignXmlLog(CString strFunc, CString strXmlFunc, BOOL bFuncResult);
	void SearchProcessLogRecord(CString strlog,int nPlatformIndex,int nPosIndex);

	virtual CWnd* SetMainWnd(CWnd* pMainWnd);                       // 设置接收消息的主窗口（实例化对象后，或更换接收消息的窗口时，必须调用） 
	virtual CWnd* SetMainRunWnd(CWnd* pMainWnd);                    // 设置接收消息的主窗口（实例化对象后，或更换接收消息的窗口时，必须调用）         
	virtual CWnd* SetMainFrameWnd(CWnd* pMainWnd);                  // 设置接收消息的主窗口（实例化对象后，或更换接收消息的窗口时，必须调用）         
	virtual CWnd* SetOperatingInfoShowWnd(CWnd* pInfoShowWnd);		// 设置接收消息的窗口，可显示运行信息（报警、操作日志）
	virtual CWnd* GetMainWnd();

	virtual DWORD InitVisionASM();                              // 初始化VisionASM所有工具

	virtual void SetVisionAlignOther(vcBaseVisionAlign *pVisionAlignOther);// 设置其他的视觉对位对象；
	virtual vcBaseVisionAlign* GetVisionAlignOther();// 获取其他的视觉对位对象；

	virtual int GetAlnSearchPatIndex(int nPosIndex, int eSearchMode);
	virtual bool GetAlnSearchResult(int nPosIndex, int eSearchMode);



protected:
	 CWnd*		m_pMainWnd;
	 CWnd*		m_pMainRunWnd;
	 CWnd*		m_pMainFrameWnd;
	 CWnd*		m_pOperatingInfoShowWnd;

public:
	CVisionAlignGui m_cVisionAlignGui;
	vcBaseVisionAlign *m_pVisionAlignOther;


	// 系统变量设置
public:
	virtual int  GetSysPlatformIndex();

	virtual void SetPlatformInfo(CPlatformInfo*	pPlatformInfo);
	virtual CPlatformInfo* GetPlatformInfo();

	virtual void SetCamPlatformInfo(CCamPlatformInfo* camPlatformInfo);
	virtual CCamPlatformInfo* GetCamPlatformInfo();

	virtual void SetMidPlatformInfo(CPlatformInfo*	pPlatformInfo);
	virtual CPlatformInfo* GetMidPlatformInfo();

	virtual void SetDlgSearchToolData(const DlgSearchToolInfo& searchToolData);
	virtual DlgSearchToolInfo& GetDlgSearchToolData() const;

	virtual void SetAlignerTypeInfo(const AlignerTypeInfo& alignerTypeInfo);
	virtual AlignerTypeInfo& GetAlignerTypeInfo() const;

	virtual BOOL SetCommParam(vcCommInfo* pCommInfo);
	virtual vcCommInfo* GetCommParam();

	virtual BOOL SetSysPlatformInfo(SysPlatformInfo platformInfo);
	virtual BOOL GetSysPlatformInfo(SysPlatformInfo& platformInfo);
	virtual SysPlatformInfo& GetSysPlatformInfoEx() const;

	virtual BOOL GetVSconfig(CVisionASMConfig *&vsConfig);

	virtual void SetSysDataRecord(const SysDataRecord& data);
	virtual SysDataRecord& GetSysDataRecord() const;

	virtual BOOL SetPlatformOptionInfo(CPlatformOptionInfo optionInfo);		    
	virtual BOOL GetPlatformOptionInfo(CPlatformOptionInfo& optionInfo);   

	virtual	BOOL SetPlatformLayoutInfo(CPlatformLayoutInfo layoutInfo);
	virtual BOOL GetPlatformLayoutInfo(CPlatformLayoutInfo& layoutInfo);

	virtual BOOL SetCommParamInfo(CCommParamInfo commParamInfo);		    
	virtual BOOL GetCommParamInfo(CCommParamInfo& commParamInfo);  

	virtual BOOL SetCommOptionInfo(CCommOptionInfo commOptionInfo);		    
	virtual BOOL GetCommOptionInfo(CCommOptionInfo& commOptionInfo);  
	virtual BOOL GetCommOptionInfoXML(CCommOptionInfo& commOptionInfo);
	virtual BOOL UpdateCommOptionInfo();

	virtual void SetSystemOptionConfig(CSystemOptionConfig *pSystemOptionConfig);

	virtual void SetParamToXYDCommProtocol();

	virtual CTotalProductData* GetTotalProuductData();              // 获取所有产品的存储地址指针
	virtual CTotalCalibData* GetTotalCalibData();					 // 获取所有产品标定的存储地址指针
	virtual CAlignerParam* GetProductAlignerParam();

	BOOL SetAlignerPrecision(CCoordPos precision);				// 设置对位精度
	BOOL SetAlignerMaxTime(int nTime);							// 设置对位最大次数

	BOOL SetCurAlignerOffsetIndex(int nIndex);					// 设置对位补偿值索引
	BOOL SetCurAlignerOffset(int nIndex, CCoordPos Offset);		// 设置对位补偿值

	virtual void GetAlignerProcessInfo(AlignerProcessInfo& alignerProcessInfo);             // 获取对位过程中对位耗时、次数、平台偏移量信息
	
	virtual void GetAlignerProcessInfoLine(AlignerProcessInfo& alignerProcessInfo);             // 获取对位过程中对位耗时、次数、平台偏移量信息

	CalibTargetMarkPatternMode GetCalibTargetMarkPatternMode();								// 获取标定方式
	CalibTargetMarkPatternMode GetProductCalibTargetMarkPatternMode(int nIndex);			// 获取标定方式
	CalibMarkPatternMode  GetCalibMarkPatternMode();										// 获取标定方式

	virtual BOOL IsSysMemoryUsedRateWarning();
	virtual BOOL IsDiskUsedRateWarning(CString strDriveName);
	void SetSysStatusInfo(CSystemStatusInfo* pSysStatusInfo){m_pSystemStatusInfo = pSysStatusInfo;}

	virtual BOOL UpdateDataRecordTxtAddr();             // 根据是否进行数据合并记录，更新数据记录文本的具体路径      
	virtual BOOL UpdateDataRecordTxtAddrTime();			// 增加时间文件结构，更新m_strDataRecord、m_strAlignProcessData和m_strCommunicationData的具体路径

	virtual void SinglePlatformProductManage(BOOL bExecuteProductManage);

	virtual void CalibrateParam_LogRecord(int nCalibPos);
	virtual void CalibrateResult_LogRecord(int nCalibPos, int nMarkType);

	virtual BOOL IsAxisMoveOverRun(CCoordPos cp);	//判定平台运动量是否超过设置最大轴移动量限制
	virtual BOOL IsReadWriteAxisPosWithCmd();	//是否随指令读写轴位置

	virtual BOOL CheckProductDataCalibDataConsistency();

	virtual void SetMutiTarget();

protected:	

	virtual void DeletePlatformInfo();                              // 释放平台信息占用的内存
	virtual void DeleteCameraPlatformInfo();                        // 释放相机平台信息展占用的内存
	virtual void DeleteMidPlatformInfo();                           // 释放中间平台信息占用的内存
	virtual void DeleteDlgSearchToolInfo();							// 释放定位工具信息占用的内存
	virtual void DeleteAlignerTypeInfo();							
	virtual void DeleteCommInfo();                                  // 释放通信信息占用的内存	
	virtual void DeleteSysPlatformInfo();                           // 释放系统平台信息占用的内存
	virtual void DeleteVisionASMConfig();							// 释放m_pVisionASMConfig所指向的内存

protected:	
	vcXMLConfigurator		m_XMLConfigator;                          // XML配置文件读写工具
	CString					m_strEXEDir;                              // EXE所在的文件夹的路径
	CString					m_strDataRecord;                          // DataRecord\\Platform1数据记录文件夹（与SysConfigs平级，均在Bin文件夹中，其中包括txt文件及图像数据）

	DlgSearchToolInfo*		m_pDlgSearchToolInfo;	
	AlignerTypeInfo*		m_pAlignerTypeInfo;
	SysPlatformInfo*		m_pPlatformInfo;							// 管理员配置的系统平台信息 
	CVisionASMConfig*		m_pVisionASMConfig;							// 参数信息加载、保存
	

public:
	CCommProtocolXYDFast	m_XYDCommProtocol;
	CSystemOptionConfig*	m_pSystempOptionConfig;						// 系统选项
	AlignerProcessInfo      m_apAlignerProcesInfo;						// 对位过程信息
	std::vector<AlignerProcessInfo> m_apAlignerProcesInfoline;          // 对位过程信息数组



	CSystemStatusInfo*		m_pSystemStatusInfo;

	BOOL				  m_bEnableSingleFrameShowMode;		//是否可以启动单帧显示模式
	BOOL				  m_bExecuteProductManage;

	CString               m_strAlignProcessData;                    // DataRecord\\Platform1数据记录文件夹中AlignProcessData数据记录的路径
	CString               m_strCommunicationData;                   // DataRecord\\Platform1数据记录文件夹中CommunicationData数据记录的路径
	CString		          m_strCommunicationSimpleData;				//                                      CommunicationSimpleData数据记录路径
	CString				  m_strCommunicationSimpleShowData;         //                                      界面中"指令显示"中数据记录路径
	CString				  m_strWarningInfoFilePath;					// DataRecord\\Platform1                WarningInfo.txt记录路径 zzc
	CString				  m_strProductSizeData;						// 产品尺寸信息
	CString               m_strProductdistanceData;                    // 第一次对位目标对象偏差信息


	CString               m_strCalibrateProcessPath1;				// 自动九宫格标定过程日志文件
	CString               m_strCalibrateProcessPath2;				// 自动标定验证过程文件,SystemConfig->m_bEnableCalibrateCheckByAxisMove
	CString               m_strCalibrateProcessPath3;				// 关联过程日志文件
	CString               m_strCalibrateProcessPath4;				// 目标、对象尺寸验证日志文件,SystemConfig->m_bEnableCalibrateCheckBySize/SystemConfig->m_bEnableRelevanceCheckBySize
	CString               m_strCalibrateProcessPath5;				// 对位过程验证，SystemConfig->m_bEnableCalibrateCheckByFixedTargetAligner

	int					  m_nOffsetIndex;							// 拍照并对位时的补偿序号
	int					  m_nAlignType;								// 拍照并对位时对位模式：1绝对移动量，2相对移动量
	BOOL				  m_bAutoAlign;
	int					  m_nAlignTime;		
	BOOL				  m_bCmdParamAxisPosEanble;
	BOOL                  m_bSearchWithInspect;						// 拍照时是否进行检测，由指令参数值和平台选项开关值共同决定

	AlignerAxisInfo		  m_AlignerAxisInfo;             //  轴信息
	BOOL				  m_bIsExecuteFixTargetCheck;				// 是否正在进行固定目标验证操作
	BOOL				  m_bIsExecuteSizeCheck;					// 是否正在进行尺寸验证操作
	int					  m_nExecuteCheckTimes;						// 验证次数	
	STUPLOADDATA		  m_stUploadData;							  //上传配置

	std::vector<std::vector<CalibTargetMarkPatternMode>> m_vmmCalibTargetType;   // 各移动位置关联模式
	std::vector<std::vector<CalibTargetMarkPatternMode>> m_vmmPreCalibTargetType;// 各移动位置先前的关联模式



// 二维码
public:
	virtual void AddAllGuiCalibrateGraphicsToDisplay(); 
	virtual void RemoveAllGuiCalibrateGraphicsFromDisplay();

	virtual void AddGuiCalibrateGraphicsToDisplay(int nPosIndex,scGuiDisplay* pGuiDisplay);
	virtual void RemoveGuiCalibrateGraphicsFromDisplay(int nPosIndex,scGuiDisplay* pGuiDisplay);

	virtual void ShowAllGuiCalibrateGraphics(BOOL bShow);
	virtual void ShowGuiCalibrateGraphics(int nPosIndex,BOOL bShow);
	
	virtual void SetCalibrateTargetMarkGuiPos(int nCalibPos);
	virtual void PostMessageUpdateGuiCalibrateTargetGraphics(int nPosIndex, BOOL bShow);

	virtual BOOL ExecuteAlnCalibTargetDmCodeSearch(int nIndex);

public:
	BOOL								m_bIsAutoCalibrating;
	BOOL								m_bIsFirstCalibPos;						// 二维码对象标定时，是否为标定9宫格的第一个标定点
	std::vector<CCoordPos>				m_vDmCodeCalibBoardPos;					// 二维码对象标定时，各位置第一个标定点的靶标坐标
	std::vector<CGuiGraphics*>			m_vpGuiCalibrateGraphics;
	std::vector<CDmCodeCalibrate>		m_vDmCodeCalibTool;
	std::vector<scGuiDisplay*>			m_vpPreGuiCalibrateDisplay;

	
	CMulMarkPos   m_mpDmCodeImagePos;
	CMulMarkPos   m_mpDmCodeBoardPos;



public:
	// 目标
	virtual void	InitTargetPlatformAxisPos();
	virtual CWnd* GetMainFrameWnd();
	virtual void	UnInitTargetPlatformAxisPos();

	virtual void	InitTargetCamAxisPos();
	virtual void    UnInitTargetCamAxisPos();

	virtual void	InitTargetCamAxisPlatformAxisPos();	
	virtual void	UnInitTargetCamAxisPlatformAxisPos();

	virtual void	SetTempTargetPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);
	virtual void    SetTempTargetCamAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);
	virtual void	SetTempTargetCamAxisPlatformAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);

	// 对象
	virtual void	InitObjectPlatformAxisPos();
	virtual void	UnInitObjectPlatformAxisPos();

	virtual void	InitObjectCamAxisPos();
	virtual void    UnInitObjectCamAxisPos();

	virtual void	InitObjectCamAxisPlatformAxisPos();
	virtual void	UnInitObjectCamAxisPlatformAxisPos();

	virtual void	SetTempObjectPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);
	virtual void    SetTempObjectCamAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);
	virtual void	SetTempObjectCamAxisPlatformAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);


	// 基准中心
	virtual void	InitCrossBenchTargetPlatformAxisPos();
	virtual void	UnInitCrossBenchTargetPlatformAxisPos();	
	virtual void	SetTempCrossBenchTargetPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);

                                                                   
public:
	std::vector<CPlatformAxisPos*>	   m_vpTargetPlatformAxisPos;								// 目标平台轴位置	
	std::vector<CPlatformAxisPos*>	   m_vpObjectPlatformAxisPos;								// 对位平台轴位置	
	std::vector<CPlatformAxisPos*>	   m_vpObjectCamAxisPos;									// 对象相机轴位置
	std::vector<CPlatformAxisPos*>	   m_vpTargetCamAxisPos;									// 目标相机轴位置	
	std::vector<CPlatformAxisPos*>	   m_vpObjectCamAxisPlatformAxisPos;						// 对象相机轴所在平台位置
	std::vector<CPlatformAxisPos*>	   m_vpTargetCamAxisPlatformAxisPos;						// 目标相机轴所在平台位置	
	std::vector<CPlatformAxisPos*>	   m_vpCrossBenchPlatformAxisPos;							// 十字中心基准目标平台轴位置

	std::vector<std::vector<CPlatformAxisPos*>> m_vvpTargetCamAxisPosMultiEx;					// 单产品扩展 目标相机实时小轴位置
	std::vector<std::vector<CPlatformAxisPos*>> m_vvpTargetCamAxisPlatformAxisPosMultiEx;		// 单产品扩展 目标相机实时大轴位置
	std::vector<std::vector<CPlatformAxisPos*>> m_vvpObjectCamAxisPosMultiEx;					// 单产品扩展 对象相机实时小轴位置
	std::vector<std::vector<CPlatformAxisPos*>> m_vvpObjectCamAxisPlatformAxisPosMultiEx;		// 单产品扩展 对象相机实时大轴位置

	std::vector<std::vector<CPlatformAxisPos*>> m_vvpTargetPlatformAxisPosMultiEx;				// 单产品扩展 目标定位时目标轴位置
	std::vector<std::vector<CPlatformAxisPos*>> m_vvpObjectPlatformAxisPosMultiEx;				// 单产品扩展 对象定位时对象轴位置;




	// 平台工作线程
protected:
	virtual BOOL CreateWorkThread()= 0;
	static  UINT WorkThread(LPVOID pParam); 

public:		
	virtual BOOL SuspendWorkThread();     // 挂起工作线程
	virtual BOOL ResumeWorkThread();      // 恢复工作线程
	virtual void StopNow(BOOL bStopNow); // 设置退出循环的标志，True退出循环；False不退出循环；

	CWinThread*                        m_pWorkThread;  
	BOOL                               m_bExitWorkThread;           // 工作线程是否退出的标志	  
	BOOL                               m_bExitWorkThreaded;         // 工作线程是否退出的标志
	BOOL                               m_bCurThreadWorkFinished;    // 本次工作线程是否结束工作
	BOOL                               m_bStopNow;                  // 标识是否立即停止工作（主要用于立即停止通信等待）
	BOOL							   m_bSuspendWorkThread;		// 挂起工作线程，让工作线程空跑
	BOOL							   SuspendResumeWorkThreadSucc;


	// 采集
public:
	BOOL SnapSearchImage(std::vector<int> vnPosIndex);

protected:
	std::vector<int>				   m_vnSnapSearchImageIndex;
	std::vector<int>				   m_vnSnapSearchImageStartIndex;
	std::vector<BOOL>				   m_vbSnapSearchImageStart;
	std::vector<BOOL>				   m_vbSnapSearchImageEnd;
	std::vector<HANDLE>				   m_vhSnapSearchImageEndEvent;
	BOOL							   m_boIsSnapImgSucceed;
	CString							   m_strSnapImgStatuString;
	std::vector<cpImage>               m_vImageSearch;


	// 搜索线程
public:
	static  UINT SearchThread(LPVOID pParam);																	// 搜索线程
	virtual void SearchProcess(int nPosIndex);																	// 搜索处理	
	virtual BOOL SearchMark(std::vector<int> vnPosIndex, std::vector<SearchMode> vsmSearchMode);										// 指定相机进行搜索，直到搜索完成退出
	virtual BOOL SaveSearchNGImage(int nPosIndex);
	virtual BOOL SaveSearchAllImage(int nPosIndex);
	virtual BOOL SaveImage(int nIndex,cpImage &imageInput,CString strFilePath);    // 保存各位置的日志图像

protected:
	std::vector<SearchMode>            m_vsmSearchMode;
	std::vector<BOOL>                  m_vbExitSearchThread;
	std::vector<CWinThread*>           m_vpSearchThread;  
	std::vector<BOOL>				   m_vbSearchStart;
	std::vector<BOOL>				   m_vbSearchEnd;
	std::vector<CSearchObject*>		   m_vpSearchObject;
	std::vector<HANDLE>				   m_vhSearchEndEvent;
	std::vector<BOOL>				   m_vbSearchThreadSuspended;
	std::vector<BOOL>                  m_vbSearchSucceed;
	std::vector<BOOL>                  m_vbSearchFinished;
	std::vector<CSearchResult>         m_vSeachResults;
	std::vector<int>				   m_vnProcessType;



	// 相机曝光
public:
	static  UINT CameraExposureThread(LPVOID pParam);												// 相机曝光线程
	virtual void CameraExposureProcess(int nPosIndex, int nCandIndex = 0);												// 相机曝光处理	
	virtual BOOL CameraExposureSetting(std::vector<int> vnPosIndex, std::vector<CameraExposureType> vsmSearchMode, int nCandIndex = 0);	// 指定相机进行曝光，直到曝光完成退出

protected:
	// 相机曝光线程
	std::vector<BOOL>						m_vbExitCameraExposureThread;
	std::vector<CWinThread*>				m_vpCameraExposureThread;  
	std::vector<BOOL>						m_vbCameraExposureStart;
	std::vector<BOOL>						m_vbCameraExposureEnd;
	std::vector<CCameraExposureObject*>		m_vpCameraExposureObject;		
	std::vector<HANDLE>						m_vhCameraExposureEndEvent;
	std::vector<BOOL>						m_vbCameraExposureThreadSuspended; // 挂起状态
	std::vector<CameraExposureType>         m_vsmSearchCameraExposureType;


	// 存图相关
public:
	static UINT SaveImageThread(LPVOID pParam);
	virtual void SaveImageProcess(int nPosIndex);

	void StartSaveImage(int nPosIndex);
	bool WaitSaveImage(int nPosIndex);
	

	//存储VDB文件
	void SaveSearchVDBFile(scGuiDisplay* pGuiDisplay,int nIndex,SearchMode eSearchMode,int nProcessType = 0);//nProcessType:0定位结果存储类型、1流程OK存储类型、2代表NG存储类型
	void SaveVDBFile(scGuiDisplay* pGuiDisplay,CString strPath,int nIndex);
	void SaveVDBFileID(scGuiDisplay* pGuiDisplay,CString ProductID,int nIndex);
	void SaveVDBFileFunc(int nIndex,SearchMode eSearchMode,int nProcessType = 0);

	void ExeSaveVDBFile(std::vector<int> vnPosIndex,SearchMode eSearchMode,int nProcessType = -1);  
	void PostMessageSaveVDBFile(std::vector<int> vnPosIndex,SearchMode eSearchMode,int nProcessType = 0);	

protected:
	std::vector<CWinThread*>			m_vpSaveImageThread;
	std::vector<CSaveImageObject*>		m_vpSaveImageObject;
	std::vector<HANDLE>					m_vhSaveImageEvent;
	std::vector<BOOL>					m_vbExitSaveImageThread;
	std::vector<BOOL>					m_vbSaveImageStart;
	std::vector<BOOL>					m_vbSaveImageFinish;
	std::vector<BOOL>					m_vbSaveImageThreadSuspended;	//现场挂起状态

	std::vector<cpImage>				m_vSaveImage;
	std::vector<CString>                m_vstrTemp;
	std::vector<CImageFormatTransfer>   m_vImageSaveTemp;

	std::vector<int>					m_vSaveCalibSearchImageFailIndex;
	std::vector<int>					m_vSaveAlignerTargetSearchImageFailIndex;
	std::vector<int>					m_vSaveAlignerObjectSearchImageFailIndex;
	std::vector<int>					m_vSaveAlignerVirtualSearchImageFailIndex;
	std::vector<int>					m_vSaveAlignerCalibTargetSearchImageFailIndex;
	std::vector<int>					m_vSaveAlignerInspectSearchImageFailIndex;

	std::vector<int>					m_vSaveCalibSearchImageIndex;
	std::vector<int>					m_vSaveAlignerTargetSearchImageIndex;
	std::vector<int>					m_vSaveAlignerObjectSearchImageIndex;
	std::vector<int>					m_vSaveAlignerVirtualSearchImageIndex;
	std::vector<int>					m_vSaveAlignerCalibTargetSearchImageIndex;
	std::vector<int>					m_vSaveAlignerInspectSearchImageIndex;

	std::vector<int>					m_vSaveAlignerCalibSearchVDBFileIndex;
	std::vector<int>					m_vSaveAlignerTargetSearchVDBFileIndex;
	std::vector<int>					m_vSaveAlignerObjectSearchVDBFileIndex;
	std::vector<int>					m_vSaveAlignerVirtualSearchVDBFileIndex;
	std::vector<int>					m_vSaveAlignerCalibTargetSearchVDBFileIndex;
	std::vector<int>					m_vSaveAlignerInspectSearchVDBFileIndex;


	// 文件夹删除
public:
	static UINT CheckRemoveFolderThread(LPVOID pParam);				// 检查删除文件夹路径线程函数
	BOOL CheckRemoveFolder();										// 检查并删除过时的时间记录文件夹

	BOOL m_bExitCheckRemoveFolderThread;							// 检查删除文件夹路径线程是否退出标志
	CWinThread* m_pCheckRemoveFolderThread;							// 检查删除文件夹路径线程
	BOOL   m_bCheckRemoveFolderThreadSuspend;                       // 线程是否处于暂停状态




	//////////////////////////////////////////////////////////////////////////
	// 相机采集相关
protected:
	static void WINAPI GrabImageFinishedCallback(void* pOwner, PTStatus status, unsigned char* pImage, long lWidth, long lHeight, PTPixelFormat pixelFormat); 
	virtual void ImageProcess(int nIndex, const cpImage& srcImage);

	virtual BOOL SetCameraCallback(void* pOwner);							// 设置相机的索引值及相关调用所有者
	virtual BOOL GetCameraCallback(void* pOwner);							// 根据相机的索引值，获取相机的调用所有者

	virtual void UninitCamera(int nIndex);								    // 释放相机
	virtual void UninitPos(int nIndex);										// 释放与位置相关的一些信息

	// 图像GuiDisplay显示
public:
	virtual void PostMessageUpdateDisplayImage(int nIndex);					// 发送更新Display显示图像的消息给主线程
	virtual void UpdateDisplayImage(int nIndex);							// 更新Display显示的图像，该函数被系统的主线程调用

	virtual void SetCameraDisplay(int nIndex, scGuiDisplay* pDisplay);		// 设置用于显示各个相机采集图像的Display
	virtual scGuiDisplay* GetCameraDisplay(int nIndex);						// 获取用于显示单个相机采集图像的Display

	virtual void SetPosDisplay(int nPosIndex, scGuiDisplay* pDisplay);		// 设置用于显示各位置相机采集图像的Display
	virtual scGuiDisplay* GetPosDisplay(int nPosIndex);						// 获取用于显示单个位置相机采集图像的Display

	virtual void SetPosSearchToolDisplay(int nProductIndex, int nPosIndex, scGuiDisplay* pDisplay);		// 设置用于显示各位置相机采集图像的Display

public:	
	virtual int  GetCameraNum();		                                    // 获取相机数量
	virtual int  GetPosNum();		                                    // 获取位置数量

	virtual int  GetTargetPosNum();											// 获取目标位置数量
	virtual int  GetObjectPosNum();											// 获取对象位置数量

	virtual BOOL InitCamera(int nIndex);											// 初始化相机
	virtual BOOL DestroyCamera(int nIndex);											// 将scPTGrab对象与硬件相机断开（以备下一次的初始化）

	virtual BOOL GrabAllCameras();													// 采集所有相机的图像
	virtual BOOL FreezeAllCameras();												// 暂停所有相机的采集

	virtual BOOL GrabCamera(int nIndex);											// 采集单个相机的图像
	virtual BOOL FreezeCamera(int nIndex);											// 暂停单个相机的图像

	virtual BOOL SnapCamera(int nIndex);											// 采集单个相机的单帧图像

	virtual BOOL IsCameraOnline(int nIndex);										// 判断某个相机是否在线	
	virtual BOOL IsCameraGrabbing(int nIndex);										// 判断某个相机是否正在采集

	virtual BOOL SetCameraParam(int nIndex, CameraParam camParam);					// 设置相机参数（与主界面交互）
	virtual BOOL GetCameraParam(int nIndex, CameraParam& cameParam);				// 获取相机参数（与主界面交互）	
	virtual BOOL GetCameraShareSrcParam(int nIndex, CameraParam& cameParam);        // 获取共享相机时，主相机参数（与主界面交互）         	                                                                       

	virtual BOOL SetCameraVideoMode(int nIndex, int nCurVideoMode, sc2Vector vStartPt, sc2Vector vCustomWH);     
	virtual BOOL GetCameraVideoMode(int nIndex, VideoMode& vmCameraMode);
	virtual BOOL UpdateCameraVideo(int nIndex);

	virtual BOOL SetImageROIParam(int nIndex, ImageRoi imageRoi);											// 设置图像显示参数（与主界面交互）
	virtual BOOL GetImageROIParam(int nIndex, ImageRoi& imageRoi);											// 获取图像显示参数（与主界面交互）

	virtual BOOL SetCameraFrameRate(int nIndex, double dCurFrameRate);										// 设置相机的帧率（与主界面交互）
	virtual BOOL GetCameraFrameRate(int nIndex, double& dCurFR, double& dMinFR, double& dMaxFR); 
	virtual BOOL UpdateCameraFrameRate(int nIndex);

	virtual BOOL SetCameraShutter(int nIndex, double dCurShutter);											// 设置相机的曝光时间（与主界面交互）
	virtual BOOL GetCameraShutter(int nIndex, double& dCurShutter, double& dMinShutter, double& dMaxShutter); 
	virtual BOOL UpdateCameraShutter(int nIndex);

	virtual BOOL SetCameraGain(int nIndex, double dCurGain);												// 设置相机的增益（与主界面交互）	
	virtual BOOL GetCameraGain(int nIndex, double& dCurGain, double& dMinGain, double& dMaxGain);   
	virtual BOOL UpdateCameraGain(int nIndex);

	virtual BOOL SetCameraGamma(int nIndex, bool bEnable, double dCurGamma);								// 设置相机的Gmma（与主界面交互）	
	virtual BOOL GetCameraGamma(int nIndex, bool& bEnable, double& dCurGamma, double& dMinGamma, double& dMaxGamma);   
	virtual BOOL UpdateCameraGamma(int nIndex);

	virtual BOOL SetCameraWhiteBalanceRatio(int nIndex,double dR,double dG,double dB);						// 设置单个相机的白平衡参数
	virtual BOOL GetCameraWhiteBalanceRatio(int nIndex,double &dR,double &dG,double &dB);					// 获得单个相机的白平衡参数
	virtual BOOL UpdateCameraWhiteBalance(int nIndex);

	virtual BOOL AutoBalance(int nIndex);									// 自动白平衡
	virtual BOOL SetCameraWhiteBalanceRatioDefault(int nIndex);				// 设置默认白平衡

	virtual BOOL GetCameraPixelFormat(int nIndex,CamPixelFormat& pixelFormat);								// 获取相机像素格式
	virtual BOOL UpdateCameraStatus(int nIndex);															// 根据相机参数更新相机所有状态
	virtual BOOL UpdateCameraToCeartainStatus( int nIndex, CameraParam cameraParam );						// 更新相机本身到指定的视频模式、帧率、曝光时间、增益（注：该函数并不考虑各参数下状态之间是否合理）





	virtual BOOL GrabPosCamera(int nPosIndex);																// 采集单个相机的图像
	virtual BOOL FreezePosCamera(int nPosIndex);															// 暂停单个相机的图像

	virtual BOOL SnapPosCamera(int nPosIndex);																// 采集单个相机的单帧图像

	virtual BOOL IsPosCameraOnline(int nPosIndex);															// 判断某个相机是否在线	
	virtual BOOL IsCameraOnlineStatus(int nIndex);									// 判断某个相机是否在线
	virtual BOOL IsPosCameraGrabbing(int nPosIndex);														// 判断某个相机是否正在采集

	virtual BOOL SetPosCameraParam(int nIndex, CameraParam camParam);										// 设置相机参数（与主界面交互）
	virtual BOOL GetPosCameraParam(int nIndex, CameraParam& cameParam);										// 获取相机参数（与主界面交互）	

	virtual BOOL SetPosInfo(int nIndex, CPosInfo posInfo);													// 设置位置信息（与主界面交互）
	virtual BOOL GetPosInfo(int nIndex, CPosInfo& posInfo);													// 获取位置信息（与主界面交互）

	virtual BOOL SetPosImageROIParam(int nPosIndex, ImageRoi imageRoi);										// 设置图像显示参数（与主界面交互）
	virtual BOOL GetPosImageROIParam(int nPosIndex, ImageRoi& imageRoi);									// 获取图像显示参数（与主界面交互）

	virtual BOOL SetPosCameraFrameRate(int nIndex, double dCurFrameRate);									// 设置相机的帧率（与主界面交互）
	virtual BOOL GetPosCameraFrameRate(int nPosIndex, double& dCurFR, double& dMinFR, double& dMaxFR);
	virtual BOOL UpdatePosCameraFrameRate(int nPosIndex);

	virtual BOOL SetPosCameraShutter(int nIndex, double dCurShutter);
	virtual BOOL GetPosCameraShutter(int nPosIndex, double& dCurShutter, double& dMinShutter, double& dMaxShutter); 
	virtual BOOL UpdatePosCameraShutter(int nPosIndex);

	virtual BOOL SetPosCameraGain(int nIndex, double dCurGain);												// 设置相机的增益（与主界面交互）	
	virtual BOOL GetPosCameraGain(int nPosIndex, double& dCurGain, double& dMinGain, double& dMaxGain);  
	virtual BOOL UpdatePosCameraGain(int nPosIndex);

	virtual BOOL SetPosCameraGamma(int nIndex, bool bEnable, double dCurGamma);								// 设置相机的Gamma（与主界面交互）	
	virtual BOOL GetPosCameraGamma(int nPosIndex, bool& bEnable, double& dCurGamma, double& dMinGamma, double& dMaxGamma); 
	virtual BOOL UpdatePosCameraGamma(int nPosIndex);

	virtual BOOL SetPosCameraVideoMode(int nIndex, int nCurVideoMode, sc2Vector vStartPt, sc2Vector vCustomWH);     
	virtual BOOL GetPosCameraVideoMode(int nPosIndex, VideoMode& vmCameraMode); 
	virtual BOOL UpdatePosCameraVideo(int nPosIndex);

	virtual BOOL SetPosWhiteBalanceRatio(int nIndex,double dR,double dG,double dB);							// 设置单个相机的白平衡参数
	virtual BOOL GetPosWhiteBalanceRatio(int nIndex,double &dR,double &dG,double &dB);						// 获得单个相机的白平衡参数

	virtual BOOL SetPosAutoBalance(int nPosIndex);															// 单个相机执行白平衡
	virtual BOOL SetPosCameraWhiteBalanceRatioDefault(int nPosIndex);										// 设置默认白平衡

	virtual BOOL GetPosCameraPixelFormat(int nPosIndex,CamPixelFormat& pixelFormat);						// 获取相机像素格式
	virtual BOOL UpdatePosCameraStatus(int nPosIndex);														// 根据相机参数更新相机所有状态
	virtual BOOL UpdatePosCameraToCeartainStatus( int nPosIndex, CameraParam cameraParam );					// 更新相机本身到指定的视频模式、帧率、曝光时间、增益（注：该函数并不考虑各参数下状态之间是否合理）

	virtual int  GetPosCameraIndex(int nPosIndex);
	virtual int  GetCameraFirstPosIndex(int nCamIndex);
	virtual BOOL IsCameraFirstPosIndex(int nPosIndex);

	// 设置相机采集模式（软触发模式、硬触发模式、连续采集模式（非触发模式））
	BOOL SetCameraTriggerMode(PTTriggerMode eCameraTriggerMode);


protected:
	std::vector<scPTGrab2*>    			m_vpCamera;					// 相机采集
	std::vector<CCameraCallbackObject*> m_vpCamOwner;				// 相机回调对象数组
	std::vector<BOOL>                   m_vbImageProcessFinished;   // 连续采集时，完成一帧采集及图像处理是否结束的标志（用于采集回调中，TRUE表示已结束，FALSE表未结束）
	std::vector<BOOL>                   m_vbCameraLiveGrab;         // 相机是否正在采集

	std::vector<scGuiDisplay*>          m_vpGuiDisplay;              // 各相机对应的图像显示控件	

	std::vector<cpImage>			    m_vImageTemp1; 
	std::vector<cpImage>			    m_vImageTemp2;
	std::vector<cpImage>				m_vImageInput;               // 旋转镜像后的图像
	std::vector<vcDisplayModule>	    m_vImageTransfer;			 // 图像变换

	std::vector<PTStatus>				m_vPTCameraIsOnLine;         // 相机是否在线



	// 共享相机
public:
	virtual cpImage& GetTransferredImage(int nPosIndex);					// 获取指定位置旋转镜像后的图像
	virtual CShareCamType GetCameraShareType(int nIndex);                   // 相机共享类型
	virtual BOOL InitShareTargetCamera(int nIndex);                         // 初始化共享目标相机
	virtual BOOL ReCreateInitCameras();                                     // 重新构造相机对象并初始化相机
	virtual BOOL UninitPreShareCamera(int nIndex,BOOL bShareTarget);        // 释放先前的相机对象，用于重新配置相机序列号时
	virtual BOOL DestroyPreShareCamera(int nIndex,BOOL bShareTarget);       // 断开先前的相机，用于重新配置相机序列号时
	virtual BOOL IsShareCameraPlatform();                                   // 平台是否共享了相机
	virtual BOOL SetShareCameraLiveGrab(int nIndex,BOOL bGrab);             // 设置与该相机共享的相机是否在线
	virtual BOOL IsShareCamTransferSame(int nIndex);                        // 判断共享主相机与副相机的图像变换是否相同
	virtual BOOL UpdateShareCamTransferSame();                              // 更新所有共享相机与副相机图像变换是否相同
	virtual BOOL UpdateShareCamTransferSame(int nIndex);                    // 更新单个共享相机与副相机图像变换是否相同

protected:
	std::vector<BOOL>                  m_vbShareSrcCamTransferFinished;		// 共享相机功能下，源相机的旋转镜像是否完成
	std::vector<BOOL>                  m_vbShareCamTransferSame;			// 共享主相机是否与副相机图像变换一致


	// 定位切换相机曝光
public:	
	BOOL SetCameraShutterByProduct(const CProductData& product);
	BOOL SwitchToSearchShutter(int nPosIndex, double dShutter);
	BOOL SwitchToTargetSearchShutter(int nPosIndex, int nCandPatIndex = 0);
	BOOL SwitchToObjectSearchShutter(int nPosIndex, int nCandPatIndex = 0);
	BOOL SwitchToCalibTargetSearchShutter(int nPosIndex, int nCandPatIndex = 0);
	BOOL SwitchToVirtualObjectSearchShutter(int nPosIndex, int nCandPatIndex = 0);
	BOOL SwitchToInspectSearchShutter(int nPosIndex, int nCandPatIndex = 0);
	BOOL SwitchToDmCodeSearchShutter(int nPosIndex);//切换到二维码搜素曝光

protected:
	std::vector<std::vector<CameraParamSet>> m_vvdTargetSearchShutter;				// 目标模板相机曝光时间
	std::vector<std::vector<CameraParamSet>> m_vvdObjectSearchShutter;				// 对象模板相机曝光时间
	std::vector<std::vector<CameraParamSet>> m_vvdCalibTargetSearchShutter;			// 关联标定模板相机曝光时间
	std::vector<std::vector<CameraParamSet>> m_vvdVirtualObjectSearchShutter;		// 虚拟对象模板相机曝光时间
	std::vector<std::vector<CameraParamSet>> m_vvdInspectSearchShutter;				// 检测模板相机曝光时间



	// 配置文件加载
public:
	// S0：加载/保存视觉对位处理所有信息（含初始化数据）
	virtual BOOL LoadVisionASMConfig();
	virtual BOOL SaveVisionASMConfig();

	// S1：加载/保存平台信息（平台类型及平台坐标系类型）
	virtual BOOL LoadPlatformInfo();
	virtual BOOL SavePlatformInfo();

	// S2：加载/保存平台信息（平台类型及平台坐标系类型）
	virtual BOOL LoadMidPlatformInfo();
	virtual BOOL SaveMidPlatformInfo();

	virtual BOOL LoadMidPlatformXYDInfo();	// XYD
	virtual BOOL SaveMidPlatformXYDInfo();	// XYD

	virtual BOOL LoadMidPlatformXY1Y2Info();	// XY1Y2
	virtual BOOL SaveMidPlatformXY1Y2Info();	// XY1Y2 

	virtual BOOL LoadMidPlatformX1X2YInfo();	// X1X2Y
	virtual BOOL SaveMidPlatformX1X2YInfo();	// X1X2Y

	// S3：加载/保存相机平台信息（相机数量、安装方式及各轴方向）
	virtual BOOL LoadCamPlatformInfo();
	virtual BOOL SaveCamPlatformInfo();

	// S4：加载/保存通信信息（串口、网口通信用相关信息）
	virtual BOOL LoadCommInfo();
	virtual BOOL SaveCommInfo();

	// S5：加载/保存通信选项信息
	virtual BOOL LoadCommOptionInfo();
	virtual BOOL SaveCommOptionInfo();

	// S6：加载/保存相机信息（序列号、视频模式、帧率、曝光、增益）
	virtual BOOL LoadCameraInfo();
	virtual BOOL SaveCameraInfo();

	// S7：加载/保存位置信息（相机索引、位置是否有效）
	virtual BOOL LoadPosInfo();
	virtual BOOL SavePosInfo();

	// S8：加载/保存图像显示信息（旋转、镜像）
	virtual BOOL LoadImageRoiInfo();
	virtual BOOL SaveImageRoiInfo();

	// S9：加载/保存中间数据记录信息
	virtual BOOL LoadDataRecordInfo();
	virtual BOOL SaveDataRecordInfo();

	// S10：对位方式记录信息
	virtual BOOL LoadAlignerTypeInfo();
	virtual BOOL SaveAlignerTypeInfo();


	// S11：加载/保存中间定位工具记录信息
	virtual BOOL LoadSearchToolDataInfo();
	virtual BOOL SaveSearchToolDataInfo();

	// S12：加载/保存平台选项信息
	virtual BOOL LoadPlatformOptionInfo();
	virtual BOOL SavePlatformOptionInfo();

	// S13：加载/保存布局信息
	virtual BOOL LoadPlatformLayoutInfo();
	virtual BOOL SavePlatformLayoutInfo();

	// S14：加载/保存权限配置项
	virtual BOOL LoadAuthorizeManagement();
	virtual BOOL SaveAuthorizeManagement();

	virtual BOOL LoadJobCameraInfo();
	virtual BOOL SaveJobCameraInfo();

	virtual BOOL LoadCalibMoveImagePosInfo();
	virtual BOOL SaveCalibMoveImagePosInfo();

	virtual BOOL LoadImageIndexInfo();					// 加载存图序号信息
	virtual BOOL SaveImageIndexInfo();					// 存储存图序号信息

	
	virtual BOOL LoadTargetPosInfo(int nIndex);	
	virtual BOOL SaveTargetPosInfo(int nIndex);

	virtual BOOL LoadObjectPosInfo(int nIndex);
	virtual BOOL SaveObjectPosInfo(int nIndex);

	virtual void LoadQRCodeOption();					// 读取存储二维码配置信息
	virtual void SaveQRCodeOption();					// 存储二维码配置信息


	virtual BOOL LoadTotalCalibDataInfo();
	virtual BOOL SaveTotalCalibDataInfo(BOOL bSaveDetails = TRUE);

	virtual BOOL LoadTotalProductDataInfo();
	virtual BOOL SaveTotalProductDataInfo(BOOL bSaveDetails = TRUE);

	virtual BOOL LoadCalibDataInfo(int nIndex, LPCTSTR lpszProductName);
	virtual BOOL SaveCalibDataInfo(int nIndex);

	virtual BOOL LoadProductDataInfo(int nIndex, LPCTSTR lpszProductName);
	virtual BOOL SaveProductDataInfo(int nIndex);

	// 按照大小品种版本之前的方式，加载所有产品信息（单个产品的信息包括：单个产品的所有对位信息、单个产品对应的相机标定参数信息）
	virtual BOOL LoadTotalProductDataInfoOld();
	// 按照大小品种版本之前的方式，加载所有标定信息（单个标定的信息包括：单个标定的所有对位信息、单个标定对应的相机标定参数信息）
	virtual BOOL LoadTotalCalibDataInfoOld();

	virtual BOOL IsSaveTotalCalibDataInfoOK();
	virtual BOOL IsSaveTotalProductDataInfoOK();



	// 输入模式、检测模式
public:

	virtual BOOL SetAlignCheckInfo(CAlignCheckInfo checkInfo);			// 设置对位检查信息
	virtual BOOL SetAlignCheckInfo(CAlignCheckInfo checkInfo,const int Index);			// 设置对位检查信息

	void InitAlignerSizeInfo();

	virtual void PostMessageUpdateAlignerTargetSizeInfo();                                      // 发送更新主界面目标距离信息的消息给主线程
	virtual void GetAlignerTargetSizeInfo(AlignerTargetSizeInfo& alignerTargetSizeInfo);		// 获取对位过程中目标距离信息

	virtual void PostMessageUpdateAlignerObjectSizeInfo();                                      // 发送更新主界面对象距离信息的消息给主线程
	virtual void GetAlignerObjectSizeInfo(AlignerObjectSizeInfo& alignerObjectSizeInfo);		// 获取对位过程中对象距离信息

	virtual void PostMessageUpdateAlignerTargetStdSizeInfo();                                   // 发送更新主界面目标标准距离信息的消息给主线程
	virtual void GetAlignerTargetStdSizeInfo(CAlignCheckInfo& checkInfo);						// 获取对位过程中目标标准距离信息

	virtual void PostMessageUpdateAlignerObjectStdSizeInfo();                                   // 发送更新主界面对象距离信息的消息给主线程
	virtual void GetAlignerObjectStdSizeInfo(CAlignCheckInfo& checkInfo);						// 获取对位过程中对象标准距离信息

	// 角度
	virtual void PostMessageUpdateAlignerTargetAngleInfo();                                      // 发送更新主界面目标距离信息的消息给主线程
	virtual void GetAlignerTargetAngleInfo(AlignerTargetAngleInfo& alignerTargetAngleInfo);		// 获取对位过程中目标距离信息

	virtual void PostMessageUpdateAlignerObjectAngleInfo();                                      // 发送更新主界面对象距离信息的消息给主线程
	virtual void GetAlignerObjectAngleInfo(AlignerObjectAngleInfo& alignerObjectAngleInfo);		// 获取对位过程中对象距离信息

	virtual void PostMessageUpdateAlignerTargetStdAngleInfo();                                   // 发送更新主界面目标标准距离信息的消息给主线程
	virtual void GetAlignerTargetStdAngleInfo(CAlignCheckInfo& checkInfo);						// 获取对位过程中目标标准距离信息

	virtual void PostMessageUpdateAlignerObjectStdAngleInfo();                                   // 发送更新主界面对象距离信息的消息给主线程
	virtual void GetAlignerObjectStdAngleInfo(CAlignCheckInfo& checkInfo);						// 获取对位过程中对象标准距离信息

	AlignerTargetSizeInfo		   m_AlignerTargetSizeInfo;		// 对位目标距离信息
	AlignerObjectSizeInfo		   m_AlignerObjectSizeInfo;		// 对位对象距离信息
	AlignerTargetAngleInfo		   m_AlignerTargetAngleInfo;	// 对位目标距离信息
	AlignerObjectAngleInfo		   m_AlignerObjectAngleInfo;	// 对位对象距离信息



	// 定位工具初始化
public:
	static DWORD WINAPI InitSearchToolRepositoryThread(LPVOID lpParam);									 // 加载模板线程
	static DWORD WINAPI SaveSearchToolRepositoryThread(LPVOID lpParam);									 // 保存模板线程
	virtual BOOL ProductSearchToolLoadState(int nProductIndex);											 // 判断产品模板是否加载

	virtual BOOL UpdateCurVisionASMTool(BOOL bUpdateCalibSearchTool = TRUE, BOOL bUpdateAlignTool = FALSE);
	virtual BOOL UpdateVisionASMTool(int nProdcutIndex, BOOL bUpdateCalibSearchTool = TRUE, BOOL bUpdateAlignTool = FALSE);

	virtual BOOL CreateSearchTool(CSearchToolType cSearchToolType, CBaseSearchTool** ppNewBaseSearchTool);
	virtual BOOL CloneSearchTool(CBaseSearchTool* pBaseSearchTool, CBaseSearchTool** ppNewBaseSearchTool);

	//////////////////////////////////////////////////////////////////////////
	// 对位对象的定位工具对象
public:
	virtual BOOL InitAlnObjectSearchToolRepository(int nProductIndex);										// 初始化对象定位工具库 
	virtual	BOOL SaveAlnObjectSearchToolRepository(int nProductIndex);										// 保存对象定位工具库到文件
	
	virtual BOOL AddRepoAlnObjectSearchTool(int nProductIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoAlnObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitAlnObjectSearchToolRepository(int nProductIndex);

	virtual BOOL SetRepoAlnObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);
	virtual CBaseSearchTool* GetRepoAlnObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoAlnObjectSearchTool(int nPosIndex, int nPatIndex);

	virtual int GetRepoAlnObjectSearchToolCount(int nProductIndex, int nPosIndex);

	virtual CameraParamEnable GetAlnObjectSearchCandidatePatShutterEnable(int nPatIndex);	
	virtual BOOL GetAlnObjectSearchCandidatePatEnable(int nPatIndex);

	virtual BOOL ExecuteAlnObjectSearch(int nPosIndex, int nPatIndex);
	virtual CSearchResult GetAlnObjectSearchResult(int nPosIndex, int nPatIndex);	
	virtual std::vector<CSearchResult> GetAlnObjectSearchResults(int nPosIndex, int nPatIndex);

	virtual int GetAlnObjectSearchResultNumber(int nPosIndex, int nPatIndex);

public:
	BOOL				m_bValidObjectSearchTool;
	BOOL				m_bIsRepositoryInitAlnObjectSearch;
	
	BOOL                m_bObjectSearchTest;         // 实时对象搜索测试标志
	BOOL                m_bObjectMarkSearchSucceed;
	CMarkImagePos       m_mpObjectMarkImagePos;
	CMarkImagePos       m_mpFixObjectMarkImagePos;

	std::vector<CMarkImagePos> m_vmpObjectMarkImagePos_MultiEX;

protected:
	std::vector<int>				   m_vnObjectSearchStartPatIndex;
	std::vector<int>				   m_vnObjectSearchEndPatIndex;
	std::vector<int>				   m_vnObjectSearchSuccessPatIndex;
	std::vector<int>				   m_vnObjectSearchResultNumber;
	std::vector<CSearchResult>		   m_vObjectSearchedSeachResults;	// 已经找到的产品位置
	//////////////////////////////////////////////////////////////////////////


	
	//////////////////////////////////////////////////////////////////////////
	// 对位虚拟对象的定位工具对象
public:
	virtual BOOL InitAlnVirtualObjectSearchToolRepository(int nProductIndex);
	virtual	BOOL SaveAlnVirtualObjectSearchToolRepository(int nProductIndex);

	virtual BOOL AddRepoAlnVirtualObjectSearchTool(int nProductIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoAlnVirtualObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitAlnVirtualObjectSearchToolRepository(int nProductIndex);

	virtual BOOL SetRepoAlnVirtualObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);	
	virtual CBaseSearchTool* GetRepoAlnVirtualObjectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoAlnVirtualObjectSearchTool(int nPosIndex, int nPatIndex);

	virtual int GetRepoAlnVirtualObjectSearchToolCount(int nProductIndex, int nPosIndex);	

	virtual BOOL ExecuteAlnVirtualObjectSearch(int nPosIndex, int nPatIndex);					                        // 执行虚拟对象对位定位
	virtual CSearchResult GetAlnVirtualObjectSearchResult(int nPosIndex, int nPatIndex);			                    // 获取虚拟对象对位定位的结果

public:
	BOOL				m_bValidVirtualSearchTool;
	BOOL				m_bIsRepositoryInitAlnVirtualObjectSearch;
	CMarkImagePos       m_mpVirtualMarkImagePos;						// 虚拟对象定位结果

protected:
	std::vector<int>				   m_vnVirtualObjectSearchStartPatIndex;
	std::vector<int>				   m_vnVirtualObjectSearchEndPatIndex;
	std::vector<int>				   m_vnVirtualObjectSearchSuccessPatIndex;
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
public:
	// 目标定位工具
	virtual BOOL InitAlnTargetSearchToolRepository(int nProductIndex);
	virtual	BOOL SaveAlnTargetSearchToolRepository(int nProductIndex);

	virtual BOOL AddRepoAlnTargetSearchTool(int nProductIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoAlnTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitAlnTargetSearchToolRepository(int nProductIndex);

	virtual BOOL SetRepoAlnTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);
	virtual CBaseSearchTool* GetRepoAlnTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoAlnTargetSearchTool(int nPosIndex, int nPatIndex);

	virtual int GetRepoAlnTargetSearchToolCount(int nProductIndex, int nPosIndex);	
	
	virtual CameraParamEnable GetAlnTargetSearchCandidatePatShutterEnable(int nPatIndex);	
	virtual BOOL GetAlnTargetSearchCandidatePatEnable(int nPatIndex);											// 获取对位定位的候补模板是否有效

	virtual BOOL ExecuteAlnTargetSearch(int nPosIndex, int nPatIndex);								            // 执行对位定位
	virtual CSearchResult GetAlnTargetSearchResult(int nPosIndex, int nPatIndex);						        // 获取对位定位的结果
	virtual std::vector<CSearchResult> GetAlnTargetSearchResults(int nPosIndex, int nPatIndex);

	virtual int GetAlnTargetSearchResultNumber(int nPosIndex, int nPatIndex);									// 获取目标定位结果的数量


public:	
	BOOL				m_bValidTargetSearchTool;
	BOOL				m_bIsRepositoryInitAlnTargetSearch;

	BOOL				m_bTargetSearchTest;
	BOOL                m_bTargetMarkSearchSucceed;
	CMarkImagePos       m_mpTargetMarkImagePos;	
	CMarkImagePos       m_mpFixTargetMarkImagePos;

	CMarkImagePos       m_mpTargetRotatedMarkImagePos;
	CMarkImagePos       m_mpCrossBenchMarkImagePos;

	std::vector<CMarkImagePos>	m_vmpTargetMarkImagePosMultiEx;

protected:
	std::vector<int>				   m_vnTargetSearchStartPatIndex;
	std::vector<int>				   m_vnTargetSearchEndPatIndex;
	std::vector<int>				   m_vnTargetSearchSuccessPatIndex;
	std::vector<int>				   m_vnTargetSearchResultNumber;
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// 关联标定的定位工具
public:
	virtual BOOL InitAlnCalibTargetSearchToolRepository(int nProductIndex);
	virtual	BOOL SaveAlnCalibTargetSearchToolRepository(int nProductIndex);

	virtual BOOL AddRepoAlnCalibTargetSearchTool(int nProductIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoAlnCalibTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitAlnCalibTargetSearchToolRepository(int nProductIndex);

	virtual BOOL SetRepoAlnCalibTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);
	virtual CBaseSearchTool* GetRepoAlnCalibTargetSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoAlnCalibTargetSearchTool(int nPosIndex, int nPatIndex);

	virtual int GetRepoAlnCalibTargetSearchToolCount(int nProductIndex, int nPosIndex);

	virtual BOOL ExecuteAlnCalibTargetSearch(int nPosIndex, int nPatIndex);
	virtual CSearchResult GetAlnCalibTargetSearchResult(int nPosIndex, int nPatIndex);
	virtual CSearchResult GetCalibSearchResult(int nPosIndex, int nPatIndex);

public:
	BOOL							m_bValidCalibTargetSearchTool;
	BOOL							m_bIsRepositoryInitAlnCalibTargetSearch;
	CMarkImagePos					m_mpCalibTargetMarkImagePos;					// 关联标定定位结果
	std::vector<CMarkImagePos*>		m_vpmpCalibMarkImagePos;						// 标定所有的定位结果

protected:
	std::vector<int>				   m_vnCalibTargetSearchStartPatIndex;
	std::vector<int>				   m_vnCalibTargetSearchEndPatIndex;
	std::vector<int>				   m_vnCalibTargetSearchSuccessPatIndex;
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 检测定位工具
public:
	virtual BOOL InitAlnInspectSearchToolRepository(int nProductIndex);	
	virtual	BOOL SaveAlnInspectSearchToolRepository(int nProductIndex);	
	
	virtual BOOL AddRepoAlnInspectSearchTool(int nProductIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoAlnInspectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitAlnInspectSearchToolRepository(int nProductIndex);

	virtual BOOL SetRepoAlnInspectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);
	virtual CBaseSearchTool* GetRepoAlnInspectSearchTool(int nProductIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoAlnInspectSearchTool(int nPosIndex, int nPatIndex);	

	virtual int GetRepoAlnInspectSearchToolCount(int nProductIndex, int nPosIndex);	

	virtual BOOL GetAlnInspectSearchCandidatePatEnable(int nPatIndex);											// 获取检测定位的候补模板是否有效
	virtual BOOL ExecuteAlnInspectSearch(int nPosIndex, int nPatIndex);								            // 执行检测定位
	virtual CSearchResult GetAlnInspectSearchResult(int nPosIndex, int nPatIndex);						        // 获取检测定位的结果

public:
	BOOL				m_bValidInspectSearchTool;
	BOOL				m_bIsRepositoryInitAlnInspectSearch;
	CMarkImagePos       m_mpInspectMarkImagePos;						// 检测对象定位结果

protected:
	std::vector<int>				   m_vnInspectSearchStartPatIndex;
	std::vector<int>				   m_vnInspectSearchEndPatIndex;
	std::vector<int>				   m_vnInspectSearchSuccessPatIndex;
	std::vector<int>				   m_vnInspectSearchResultNumber;

	//////////////////////////////////////////////////////////////////////////


	// 标定用的定位工具对象
public:
	virtual BOOL InitCalibObjectSearchToolRepository(int nCalibIndex);
	virtual	BOOL SaveCalibObjectSearchToolRepository(int nProductIndex);
	
	virtual BOOL AddRepoCalibObjectSearchTool(int nCalibIndex, int nPosIndex, CBaseSearchTool* pBaseSearchTool);
	virtual BOOL DeleteRepoCalibObjectSearchTool(int nCalibIndex, int nPosIndex, int nPatIndex);
	virtual BOOL UninitCalibObjectSearchToolRepository(int nCalibIndex);

	virtual BOOL SetRepoCalibObjectSearchTool(int nCalibIndex, int nPosIndex, int nPatIndex, CBaseSearchTool* pBaseSearchTool);
	virtual CBaseSearchTool* GetRepoCalibObjectSearchTool(int nCalibIndex, int nPosIndex, int nPatIndex);
	virtual CBaseSearchTool* GetRepoCalibObjectSearchTool(int nPosIndex, int nPatIndex);

	virtual int GetRepoCalibObjectSearchToolCount(int nCalibIndex, int nPosIndex);

	virtual BOOL ExecuteCalibTargetSearch(int nPosIndex, int nPatIndex);
	virtual CSearchResult GetCalibTargetSearchResult(int nPosIndex, int nPatIndex);

public:
	BOOL				m_bValidCalibSearchTool;     // 标定定位工具是否有效
	CMarkImagePos       m_mpCalibMarkImagePos;       // 标定定位结果

	BOOL                m_bCalibSearchTest;          // 标定搜索测试标志

protected:
	std::vector<int>				   m_vnCalibSearchStartPatIndex;
	std::vector<int>				   m_vnCalibSearchEndPatIndex;
	std::vector<int>				   m_vnCalibSearchSuccessPatIndex;
	//////////////////////////////////////////////////////////////////////////



	// 对位工具
public:
	// 初始化对位工具（定义new AlignerTool+初始化标定时的nIndex个定位工具对象+下面三个参数设置+初始化对位用的nIndex个定位工具对象+下面两个参数设置）
	virtual BOOL InitAlignTool() = 0;	
	virtual void UnInitAlignTool(); 

	virtual BOOL InitAlignTool(int nProductIndex);
	virtual void UnInitAlignTool(int nProductIndex); 

	vcBaseAlignTool* GetAlignTool() { return m_pAlignerTool;};//获取对位工具

	int GetWholeCalibCacuPos();												// 获取全局靶标标定方式计算旋转中心位置
	BOOL GetWholeCalibCacuPosEnable();										// 获取是否启用全局靶标标定方式选择位置自动计算旋转中心

	// XYD/XD/YD平台,全局二维码标定时，对位工具需要用到的二维码数据
	virtual BOOL ExecuteCalibrateWholeDmcodeMarkPosInfo(std::vector<CMulMarkPos*> vpDmCodeImagePos, std::vector<CMulMarkPos*> vpDmCodeBoardPos, std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos);

	// XYPD平台,全局二维码标定时，对位工具需要用到的二维码数据
	virtual BOOL ExecuteCalibrateWholeDmcodeMarkPosInfo(std::vector<CMulMarkPos*> vpDmCodeImagePos, std::vector<CMulMarkPos*> vpDmCodeBoardPos, std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType);

	// 全局二维码标定时，建立二维码图像到靶标的映射关系
	virtual BOOL ExecuteCalibWholeDmCodeBoardImgToBoard(int nPosIndex);		

	// 执行全局二维码靶标标定时，建立靶标坐标与平台坐标之间的关系
	virtual BOOL ExecuteCalibrateWholeDmcodeBoardBoardToPlat(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType);  

	virtual BOOL ExecuteCalibrateWholeDmcodeBoardBoardToPlat(int nCalibPos, int nMarkType);  

	// 执行全局二维码标定时，计算旋转中心位置
	virtual BOOL ExecuteCalibWholeDmCodeBoard(int nCalibPos);

	virtual BOOL CalculateObjectToVirtualMarkOffset(std::vector<CCoordPos>& vObjectToVirtualOffsets, int nExProductIndex = 0);	                                                                                           // 设置对象Mark到虚拟Mark之间的位置关系
	virtual void SetObjectMarkToVirtualMarkOffset(std::vector<CCoordPos> vObjectToVirtualOffsets);

	virtual void SetAlignReservedIndex(int nAlignReservedIndex);// 保留序号	
	virtual void SetAlignBenchOffsetIndex(int nAlignBenchOffsetIndex);// 基准居中对位补偿序号	
	virtual void SetAlignExProductIndex(int nAlignExProductIndex);// 扩展产品序号

	virtual BOOL ExecuteAlign(int nAlnIndex = 0, BOOL bInspect = TRUE);									// 执行对位偏差计算
	virtual BOOL ExecuteAlignExHLLayer(int nAlnIndex, std::vector<CCoordPos> vcpCurCrossPos);			// 叠片机执行对位


	virtual void SetCalibPlatformPos(CPlatformAxisPos* pPlatformAxisPos/*, int nIndex = 0, BOOL bTarCamWithDiffObjCam = FALSE*/);                      // 设置标定时平台基准位置   	

	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformAxisMovePos(int nMarkType);                      // 获取平移标定和旋转标定时平台各轴的移动位置
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformAxisMovePos(int nPosIndex,int nMarkType);                      // 获取平移标定和旋转标定时平台各轴的移动位置
	virtual std::vector<CPlatformAxisPos*> GetCalibCenterPlatformAxisMovePos(int nPosIndex); // 获取平移标定和旋转标定时平台各轴的移动位置
	virtual BOOL ExecuteCalibrateTarget(int nExProductIndex=0);//执行关联 nExProductIndex:扩展产品序号

	//功能：执行二维码靶标标定：根据传入的指定位置的若干图像坐标和靶标坐标建立标定映射关系
	//参数：位置序号，若干图像坐标，若干靶标坐标
	virtual BOOL ExecuteCalibrateDmCodeBoard(int nCalibPos,std::vector<sc2Vector> &vImagePos,std::vector<sc2Vector> &vBoardPos,int nMarkType);

	//功能：执行二维码关联
	//参数：关联位置序号，对应基准关联位置序号
	virtual BOOL ExecuteCalibrateDmCode(int nCalibPos,int nBasePos);

	//功能：执行二维码关联：利用传入的关联位置的图像坐标和对应基准关联位置的图像坐标建立映射关系
	//参数：关联位置序号，对应基准关联位置序号，关联位置若干图像坐标，对应基准关联位置若干图像坐标
	virtual BOOL ExecuteCalibrateDmCode(int nCalibPos,int nBasePos,std::vector<sc2Vector>& vCalibImagePos,std::vector<sc2Vector>& vBaseImagePos);
	virtual BOOL ExecuteCalibrate(std::vector<CMarkImagePos*> pMarkImagePos);                  // 执行标定（计算）
	virtual BOOL ExecuteCalibrate(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos);  
	virtual BOOL ExecuteCopyCalibrateData(int nType,int nExProductIdx = 0);											// 执行标定从位置拷贝到其他位置(nType = 1 对象，nType = 2 目标)
	virtual BOOL ExecuteCalibrate(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType);  
	virtual BOOL ExecuteCalibrateFCBS(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType);  

	//XYPD平台执行全局二维码靶标标定时建立靶标坐标与平台坐标之间的关系
	virtual BOOL ExecuteCalibrateWholeDmcodeBoardBoardToPlatFCBS(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType); 
	virtual BOOL ExecuteCalibrateBenchMarkRgisterPlace(int nCalibPos, int nMarkType);			// X+XYD上料平台执行基准注册标定计算，保存X轴放料轴位置
	virtual BOOL ExecuteCalibrateBenchMarkRgister(std::vector<CMarkImagePos*> pMarkImagePos,int nCalibPos, int nMarkType);			// X+XYD上料平台执行基准注册标定计算，拍照并保存X轴拍照轴位置

	// 执行基本靶标的关联映射标定
	virtual BOOL ExecuteCalibrateBoard(std::vector<CMarkImagePos*> pMarkImagePos, std::vector<CMarkImagePos*> pMarkBoardPos, sc2Vector vBoardSize, int nCalibPos, int nMarkType); 

	//	virtual BOOL IsExcutePlatformCalibrateRefine();                                            // 判断平台是否求精

	virtual BOOL ExcutePlatformCalibrateRefine(int nIndex) = 0;                                // 平台标定求精    

	virtual void SetCalibCamPlatformPos(std::vector<CPlatformXYAxisPos*> vpPlatformXYAxisPos); // 设置标定时相机平台的基准位置

	// 获取标定时,指定相机需要移动的所有位置
	virtual void GetCalibCamPlatformMovePos(int nIndex, std::vector<CPlatformXYAxisPos*>& vpCameraMoveAxisPos); 

	virtual void SetCalibCameraMoveSearchResult(std::vector<CMarkImagePos*> vpImageResults);   // 设置所有相机平台在所有位置的定位结果

	virtual BOOL ExecuteCamPlatformCalibrate();                                                // 执行相机平台的标定
	virtual BOOL ExecuteCamPlatformCalibrate(int nCalibPos);
	virtual std::vector<CCalibratedInfo*> GetAllCalibratedInfo();                              // 获取所有相机标定结果

	virtual BOOL SetSameCalibrateInfo(int nSrcPos,std::vector<int>vnPos);							//将nSrc位置的标定信息拷贝到其他位置中


	//功能：执行目标固定补偿计算（目标采用靶标标定时使用）
	//参数：目标固定补偿值（输出）
	virtual BOOL CaculateTargetFixOffset(CCoordPos &offset);	

	virtual BOOL IsAlignFinished(int nAlignTime, BOOL bMulAligner = FALSE);                    // 判断整个对位过程是否结束（TRUE标结束，FALSE标未结束）（nAlignTime为已经执行的对位动作次数）
	virtual BOOL IsAlignSucceed();                                                             // 判断整个对位过程是否成功（TRUE标成功，FALSE标未成功）


	virtual CPlatformAxisPos* GetAlignPlatformMovementResults(BOOL bUseOffset = FALSE);                               // 对位偏差计算成功后，获取平台对位时要移动的绝对位置
	virtual CPlatformAxisPos* GetAdjustAlignPlatformMovementResults(double dratioX,double dratioY,double dratioD =1.0);                               // 对位偏差计算成功后，获取平台对位时要移动的绝对位置							
	virtual CCoordPos GetAlignProductOffset();												   // 获取对位产品的偏移值，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignProductOffsetXYPD();												   // 获取对位产品的偏移值，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignPlatformOffset();                                                // 获取对位各轴偏差量计算结果，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignPlatformPosOffset();                                             // 获取对位各轴偏差量计算结果，用于输出控制轴运动
	virtual CCoordPos GetAlignPlatformPickRevise();	
	virtual std::vector<CPlatformAxisPos*> GetAlignCameraMovementResults();                    // 中间对位过程，当相机为独立安装时，获取下次定位前各相机要移动到的绝对位置


	virtual void SetCurCalibratedInfo(std::vector<CCalibratedInfo*> vCalibratedInfo);          // 设置当前平台用到的标定后信息（设置好后，并保存到m_pVisionASMConfig->m_vpCalibratedInfo及配置文件中）
	virtual void SaveCalibratedInfoToCurProduct(std::vector<CCalibratedInfo*> vCalibratedInfo);// 保存标定后信息到当前产品中及配置文件中
	virtual BOOL ExecuteBenchMarkFixed(std::vector<CCoordPos> vPtMarkImageBench) = 0;          // 执行基准目标Mark修正    


protected:
	virtual void SetTargetPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);	                   // 设置实时目标定位时平台绝对位置
	virtual void SetTargetCamPlatformPos(std::vector<CPlatformXYAxisPos*> vpCamPltfmAxisPos);  // 设置实时目标定位时相机平台绝对位置
	virtual void SetTargetCamPlatformPos(std::vector<CPlatformXYAxisPos*> vpCamPltfmAxisPos,int nCamPos);  // 设置实时目标定位时相机平台绝对位置
	virtual void SetMutiTargetPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);	        
	virtual void SetTargetCamPlatformPos(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);
	virtual void SetTargetCamPlatformPos_MultiEx(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);//设置扩展产品目标相机轴位置
	virtual void SetTargetMarkSearchResults(CMarkImagePos ImageResults);		               // 设置目标Mark的图像搜索结果
	virtual void SetTargetCamAxisPlatformAxisPos(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);
	virtual void SetTargetCamAxisPlatformAxisPos_MultiEx(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);//扩展产品目标相机平台轴设置

	virtual void SetObjectPlatformPos(std::vector<CPlatformAxisPos*> pPlatformAxisPos);	                   // 设置实时对象定位时平台绝对位置
	virtual void SetObjectPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);	                   // 设置实时对象定位时平台绝对位置
	virtual void SetObjectCamPlatformPos(std::vector<CPlatformXYAxisPos*> vpCamPltfmAxisPos);  // 设置实时对象定位时相机平台绝对位置
	virtual void SetObjectMarkSearchResults(CMarkImagePos ImageResults);		               // 设置实时对象Mark的图像搜索结果

	virtual void SetObjectCamPlatformPos(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);
	virtual void SetObjectCamAxisPlatformAxisPos(CPlatformAxisPos* pCamAxisPlatformAxisPos, int nIndex);

	virtual void SetObjectCamPlatformPos_MultiEx(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);//设置扩展产品对象相机轴
	virtual void SetObjectCamAxisPlatformAxisPos_MultiEx(CPlatformAxisPos* pCamAxisPlatformAxisPos, int nIndex);//设置扩展产品对象相机平台轴
	virtual void SetTargetPlatformPos_MultiEx(CPlatformAxisPos* pPlatformAxisPos,int nIndex);//设置目标拍照实时轴位置
	virtual void SetObjectPlatformPos_MultiEx(CPlatformAxisPos* pPlatformAxisPos,int nIndex);//设置对象拍照实时轴位置

	virtual void SetVirtualPlatformPos(CPlatformAxisPos* pPlatformAxisPos);	                   // 设置虚拟对象定位时平台绝对位置
	virtual void SetVirtualPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);	       // 设置虚拟对象定位时平台绝对位置
	virtual void SetVirtualCamPlatformPos(std::vector<CPlatformXYAxisPos*> vpCamPltfmAxisPos); // 设置虚拟对象定位时相机平台绝对位置
	virtual void SetVirtualMarkSearchResults(CMarkImagePos ImageResults);		               // 设置虚拟对象Mark的图像搜索结果

	virtual CMarkImagePos GetTargetMarkSearchResults();		               // 获取目标Mark的图像搜索结果
	virtual CMarkImagePos GetObjectMarkSearchResults();		               // 获取目标Mark的图像搜索结果


public:

	vcBaseAlignTool*				   m_pAlignerTool;				// 对位工具    
	BOOL                               m_bValidAlignerTool;         // 对位工具是否有效



	/* 函数功能：调用脚本文件Git_Test.bat执行git add . 和 git commit -m 动作
	 功能：在Bin/x64内创建了git仓库的前提下：视觉软件执行了保存磁盘后，调用ExecuteGitCommit函数
	 将软件VSConfig内的配置文件改变提交到git仓库进行备份;
	 这样VSConfig的各个文件的发生改变的记录均可通过git log进行查看；并配合操作日志便于分析配置文件
	 何时、并且基于什么情况发生的改变
	 */
public:
	int ExecuteGitCommit(CString strCommit);
	BOOL m_bExecuteGitCommit; // 开始执行GitCommit


// 状态栏
public:
	virtual void PostMessageUpdateStatusBarInfo();
	virtual void SetStatusBarInfo(BOOL bStatus, CString strText1, CString strText2);
	virtual void GetStatusBarInfo(BOOL& bStatus, CString& strText1, CString& strText2);


	virtual	void ClearCommCommandInfo();
	virtual	void AddCommCommandInfo(BOOL bSend, CString strCmd, BOOL bTimeExist = FALSE, BOOL bAddDetailInfo = TRUE);                                                                                  

	virtual void GetCommCommandInfo(deque<CString>& saCommCommandSimple, deque<CString>& saCommCommandDetail);
	virtual void SetCommCommandName(CString strCommCommandName, SysWorkCmdType eCommandType);
	virtual void GetCommCommandName(CString& strCommCommandName);

	virtual CString GetCmdWarningString(CString strCmd);
	virtual void PackAndAddCommSimpleInfo(CString & strInfo,CString strCmd,std::vector<int> nParam,std::vector<double> dParam);
	virtual void PostMessageUpdateCommCommandInfo();	

	virtual	void AddWarningInfo(CString strWarningType, CString strWarningMsg);

	virtual void PostMessageUpdateAlignerWarningInfo();
	virtual void GetWarningInfo(deque<CString>& saWarningInfo);
	virtual void ClearWarningInfo();	
	virtual CString	GetWarningInfoFilePath();

	virtual void PostMessageUpdateAlignerProcessInfo();                                        // 发送更新主界面对位过程信息的消息给主线程
	virtual void PostMessageUpdateCurProductAlignInfo();			                        // 发送更新当前产品
	virtual void PostMessageUpdateAlignPrecisionInfo();			                            // 发送更新对位精度
	virtual void PostMessageUpdateAlignOffsetInfo();			                            // 发送更新对位补偿	                                                                                   
//	virtual void PostMessageUpdateCurPlatformAxisPosInfo();			                        // 发送更新当前位置

	// 设置系统揭示信息字符串数组
	virtual void SetSysInfoStrings(CStringArray *pSysInfoStrings);

protected:	
	BOOL                               m_bStatusBar;                // 状态栏的状态
	CString                            m_strStatusBarInfo1;         // 状态栏提示内容
	CString                            m_strStatusBarInfo2;         // 状态栏提示内容

	BOOL                               m_bTempStatusBar;            // 临时状态栏的状态
	CString                            m_strTempStatusBarInfo1;     // 临时状态栏提示内容
	CString                            m_strTempStatusBarInfo2;     // 临时状态栏提示内容

	BOOL                               m_bCommSend;                //  通信状态
	CString                            m_strCommCommandName;		// 通信指令-zzc
	SysWorkCmdType					   m_cmdTypeCommandName;		// 通信指令对应的枚举值-zzc
	CString                            m_strCommParam;				// 通信参数
	std::map<CString, SysWorkCmdType>  m_mapCmdNameValue;			// 指令名-指令值对应的map
	deque<CString>					   m_saCommCommandSimple;
	deque<CString>					   m_saCommCommandDetail;	
	deque<CString>					   m_saWarningInfo;  
	scTimer                            m_tTimer;

public:
	CString								m_StrSearchErrorInfo;
	CStringArray*						m_psaSysInfoStrings;		// 存放系统提示信息的字符串数组的地址，由主对话框传入有效地址值
	

	//////////////////////////////////////////////////////////////////////////
	// 通信相关
protected:	
	virtual BOOL InitComm();								            // 初始化通信接口
	virtual void UninitComm();								            // 释放通信接口

	virtual int  WaitCommInfo(int time, CString& strInfo);              // 在规定时间（ms）内等待指令响应(0: 接收失败 1：接收成功 2：超时)	 

public:	
	virtual BOOL UpdateCommTool();                                      // 根据通信接口参数更新通信工具

	virtual BOOL ChangeCommType(CommType eCommType);					// 改变通信连接类型
	virtual BOOL ReInitComm();											// 改变通信连接类型

public:	   
	virtual	void SetComm(vcComm *pComm);								// 设置通信指针
	virtual vcComm* GetComm();	
	virtual BOOL OpenComm();								            // 打开通信接口
	virtual BOOL ReOpenComm();								            // 重新打开通信接口
	virtual BOOL CloseComm();								            // 关闭通信接口
	virtual BOOL StartReceiveComm();									// 开始接收数据
	virtual BOOL StopReceiveComm();										// 停止接收数据
	virtual BOOL ReceiveCommInfo(CString& strInfo, BOOL bReset = FALSE, BOOL bLog = TRUE);// 接收通信接口命令信息
	virtual BOOL ReceiveCommInfo2(CString& strInfo, CString& strFrontInfo, BOOL bReset = FALSE, BOOL bLog = TRUE); // 接收通信接口命令信息
	virtual BOOL SendCommInfo(CString strInfo, BOOL bLog = TRUE, CString strShow = _T(""));				            // 向通信接口发送命令信息
	virtual BOOL IsCommOpen();                                          // 判断通信接口是否已经打开
	virtual BOOL  CommShakeHands();


public:	   
	// 添加平台共享通信信息，即设置所有的共享通信信息
	static CShareCommInfoArray* GetShareCommInfoArray();
	static void AddPlatShareComm(int nPlatformIndex, int nShareCommIndex, BOOL bShareCommEnable);

	// 查询当前平台通信接口共享类型，是否为源通信接口
	ShareCommType GetShareCommType();

	// 查询当前平台通信接口，是否为共享通信接口，且为并行通信方式（共享通信时以源通信的设置为准）
	BOOL IsShareCommPlatformParalle();

	// 共享通信时，非并行通信情况下，获取当前平台是否为可以通信的平台，是否得到通信令牌
	BOOL IsShareCommCurPlatform();	

	// 共享通信时，非并行通信情况下，将通信令牌转移给下一个待通信的平台
	BOOL ShareCommNextPlatform();	

	// 共享通信时，并行通信情况下，加锁当前共享通信平台的通信
	BOOL LockShareComm();

	// 共享通信时，并行通信情况下，解锁当前共享通信平台的通信
	BOOL UnlockShareComm();


protected:
	vcComm*				        m_pComm;				// 通信
	BOOL                        m_bValidComm;           // 通信是否正常
	CommStatus					m_CommStatus;			// 通信状态

public:
	static CShareCommInfoArray	m_ShareCommInfoArray;	//所有的共享通信信息，有N个用于共享的通信接口，则有N组共享通信信息，每组共享通信信息包括与其对应的平台列表





public:

	// 折弯																															m_strProductSTATION
	virtual	BOOL BendingAlignProcessInfo(CString &strInfo);			// 折弯检测
	virtual	BOOL BendingAlignProcessInfo(CString &strInfo,std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult,CString m_strProductID,CString m_strProductSTATION = _T(""),CString strInspectStation = _T(""));			// 折弯检测

	virtual	BOOL BendingCircleProcessInfo(CString &strInfo, std::vector<double> vdCircleRadius);		// 折弯圆检测
	virtual	BOOL BendingCircleProcessInfo(CString &strInfo, std::vector<double> vdCircleRadius,std::vector<BOOL> &bResult,std::vector<CString> &vSearchResult);		// 折弯圆检测
	virtual void GetBendInspectResult(BendInspectResultInfo &BendInspectResult);
	virtual void GetBendInspectResultLine(BendInspectResultInfo &BendInspectResult);
//	virtual BOOL BendingProductID(CString& strProductID);												// 读取二维码
	virtual void PostMessageUpdateBendingProcessInfo(); 
	virtual void PostMessageUpdateBendingCircleInfo(); 
	virtual void PostMessageUpdateBendingDetalInfo(); 
	virtual void VisionInspectBendRecord(BOOL bAlignProcess = TRUE, BOOL bBendProcess = TRUE, BOOL bBendCircle = TRUE, int nAlignStatus = 0 );	// 日志
	//参数1传递的文件数据，参数2传递的是否生成文件
	virtual void VisionInspectBendRecordFixed(CString &strInfo,BOOL bAllSteoage,BOOL bAlignProcess = TRUE, BOOL bBendProcess = TRUE, BOOL bBendCircle = TRUE );	// 日志


public:
	double m_dLastLy;
	double m_dLastRy;
	BOOL   m_bisFirstAlign;
	BendInspectResultInfo m_BendInspectResultInfo;
	std::vector<BendInspectResultInfo> m_BendInspectResultInfoline;          // 检测数据信息组


	//////////////////////////////////////////////////////////////////////////
	//	机器人相关
	//	加载/保存机器人示教范围信息
	virtual BOOL LoadRobotTeachRangeInfo();
	virtual BOOL SaveRobotTeachRangeInfo();

	// 设置、获取机器人示教范围信息
	virtual void SetRobotTeachRangeInfo(CRobotTeachRangeInfo robotTeachRangeInfo);
	virtual void GetRobotTeachRangeInfo(CRobotTeachRangeInfo& robotTeachRangeInfo);

	CRobotTeachRangeInfo m_RobotTeachRangeInfo;

	std::vector< std::vector<CMarkImagePos*> > m_vvpMarkImagePos;  // 存储所有位置标定时图像坐标


protected:	
	std::vector<std::vector<int>>	   m_vvnChannelIndex;       //需进行切换通道
	std::vector<std::vector<int>>	   m_vvnIntensity;          //对应通道的亮度值
	std::vector<CLightCtrlObject*>	   m_vpLightCtrlObject;      //对象资源
	std::vector<BOOL>                  m_vbExitLightCtrlThread;   //线程退出
	std::vector<CWinThread*>           m_vpLightCtrlThread;       //线程句柄
	std::vector<BOOL>				   m_vbLightCtrlSuspended;    //线程挂起
	std::vector<BOOL>				   m_vbLightCtrlStart;       //开始光源切换
	std::vector<BOOL>				   m_vbLightCtrlEnd;          //光源切换结束
	std::vector<BOOL>                  m_vbLightCtrSucceed;       //光源切换成功失败状态
	std::vector<CString>               m_vstrLightCtrlInfo;       //光源切换成功失败状态
	static  UINT LightCtrlThread(LPVOID pParam);																	// 搜索线程
	virtual void LightCtrlProcess(int nPosIndex);																	// 搜索处理	
};

#endif // !defined(AFX_SVVisionASM_H__B7EA2F66_DAB1_4018_9973_6538BD23418B__INCLUDED_)
