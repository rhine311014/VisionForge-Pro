// VisionASMMessage.h 


#pragma once


#define  xxx   WM_USER + 100
// 类型定义


// 枚举视觉工具类型
typedef enum tagVBVisionTool
{
	eVBTool		= 0x00,
		eVBCamera	= 0x01,
		eVBCalibrate = 0x2,
		eVBSearch = 0x03,
		eVBPatternInsp = 0x04,
		eVBDistanceInsp = 0x05
} VBVisionTool;

// 枚举相机类型
typedef enum tagVBCamera
{
	eVBBaseCamera = 0x00,
		eVBPTCamera = 0x01,
}VBCamera; 

// 枚举标定类型
typedef enum tageVBCalibrate
{
	eVBBaseCali = 0x00,
		eVBDotCali = 0x01,
		eVBChessCali = 0x02,
		eVBScaleCali = 0x03
}VBCalibrate;

// 枚举定位工具类型
typedef enum tagVBSearch
{
	eVBBaseSearch = 0x00,
		eVBQuickSearch = 0x01,
		eVBFineSearch  = 0x02
}VBSearchType;

// 枚举图案检测工具
typedef enum tagVBPatternInsp
{
	eVBBasePatternInsp = 0x00,
		eVBGreyInsp	=	0x01,
		eVBBlankInsp =	0x02
}VBInspectType;

// 枚举位置检测工具
typedef enum tagVBDistanceInsp
{
	eVBBaseDistanceInsp = 0x00,
		eVBEdgeToEdge = 0x01,
		eVBEdgeToPat  = 0x02,
		eVBPatToPat	  = 0x03
}VBDistanceInsp;




/************************************************************************/
/* 0x00-00-0000											   				*/
/* 对象编号规则(视觉工具类型编号)-（细分工具类型编号）-（对象序号）		*/
/* 视觉工具类型编号：范围00~FF，VBVisionTool类型						
/* 细分工具类型编号: 范围00~FF，如标志定位工具类型、检测工具类型等
/* 对象序号：序号范围0000~FFFF，对象的类型由视觉工具类型和细分工具类型共同确定
/************************************************************************/

#define sfGenVBObjectType(ToolT, SubT) (((ToolT)<<24)|((SubT)<<16))
#define sfGenVBObjectKey(Tp, SubN) ((Tp)|(SubN))

#define sfIsVBCamera(Key) (((Key)>>24) == eVBCamera)
#define sfIsVBSearch(Key) (((Key)>>24) == eVBSearch)
#define sfIsVBPatternInsp(Key) (((Key)>>24) == eVBPatternInsp)
#define sfIsVBDistanceInsp(Key) (((Key)>>24) == eVBDistanceInsp)

#define sfGetVBObjectType(Key) ((Key)&& 0xFFFF0000)
#define sfGetVBObjectToolType(Key) ((Key)>>24)
#define sfGetVBObjectSubType(Key) (((Key)>>16) & 0x00FF)
#define sfGetVBObjectSubNum(Key) ((Key) & 0x0000FFFF)