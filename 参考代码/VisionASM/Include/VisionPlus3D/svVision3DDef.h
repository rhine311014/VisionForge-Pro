#pragma once

#ifdef SV_VISION_3D_EXPORTS
#define SV_VISION_3D_API __declspec(dllexport)
#else
#define SV_VISION_3D_API __declspec(dllimport)
#endif

#ifdef SV_CALIBRATE_3D_EXPORTS
#define SV_CALIBRATE_3D_API __declspec(dllexport)
#else
#define SV_CALIBRATE_3D_API __declspec(dllimport)
#endif


enum CalculateMethod            // 轮廓投影计算方式
{
	eAverage = 0,	            // 平均值
	eMax,			            // 最大值
	eMin,			            // 最小值
};

enum ShapeCorrectionType        // 轮廓补正类型
{
	eCorrectionNone = 0,		// 无
	eCorrectionRotation,		// 旋转补正
	eCorrectionRotProjection,	// 旋转+投影补正
};

enum ValidPixelBorderType      // 有效像素边界类型
{
	eBorderNone,               // 无
	eBorderExtendUpward,       // 向上延伸
	eBorderExtendDownward      // 向下延伸
};

// 轮廓检测工具-检测对象类型
enum ProfileInspectObjectType        // 检测对象类型
{
	eObjUnknown  = 0,                // 未知检测对象
	eObjAverage,                     // 平均
	eObjPeak,                        // 峰值
	eObjBottom,                      // 谷值
	eObjInflectionPoint,             // 拐点
	eObjTwoLinesIntersection,        // 两直线交点
	eObjCircleAndLineIntersection,   // 圆与直线交点
	eObjCircleCenter,                // 圆心
	eObjDistanceToReferenceLine,     // 到基准模型线的距离
	eObjEdge,                        // 边缘
	eObjLine,                        // 直线
	eObjCrossSectionArea,            // 截面积
};

// 轮廓检测工具-检测项类型
enum ProfileInspectItemType        // 检测项类型
{
	eItemUnknown = 0,              // 未知检测项
	eItemHeight,                   // 高度
	eItemLevelDiff,                // 段差
	eItemPosition,                 // 位置
	eItemCenterPosition,           // 中心位置
	eItemWidth,                    // 宽度
	eItemCircleRadius,             // 圆的半径
	eItemPointsDistance,           // 2点间的距离
	eItemPointToLineDistance,      // 点与直线的距离
	eItemLineToLineDistance,       // 直线与直线的距离
	eItemLineToCircleDistance,     // 直线与圆的距离
	eItemCircleToCircleDistance,   // 圆与圆的距离
	eItemAngleFromHor,             // 与水平的角度
	eItemAngleFromTwoLines,        // 2直线的交角
	eItemCrossSectionArea,         // 截面积
	eItemOneLineCrossSectionArea,  // 以一条直线包围的截面积
	eItemTwoLineCrossSectionArea,  // 以2条直线包围的截面积
	eItemCount,                    // 计数
	eItemDefectInspect,            // 缺陷检测
};

enum FitMethod3D
{
	eRansac1,				//随机抽样
	eTukey,				    //权重
	eRansacTukey,			//随机抽样权重
};
enum SamplingMethod
{
	eNoSampling,           //不采样
	eIntervalSampling,     //间隔采样
	eAverageSampling,      //均值采样
};

