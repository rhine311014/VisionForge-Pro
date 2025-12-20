#pragma once

#include "vpImage.h"
#include "svShape.h"
#include "svVision3DDef.h"

#pragma pack(push,8)


enum ThreMethod;

typedef struct tagAbsolute
{
	int nThreLow;
	int nThreHigh;

	tagAbsolute()
	{
		nThreLow = 0;
		nThreHigh = 0;
	}
}Absolute;

typedef struct tagRelative
{

	double dTailLow;
	double dTailHigh;
	double dThreLow;
	double dThreHigh;

	tagRelative()
	{
		dTailLow = 0;
		dTailHigh = 0;
		dThreLow = 0;
		dThreHigh = 0;
	}
}Relative;

typedef struct tagStaticGlobal
{
	ThreMethod method;
	int nOffsetUp;
	int nOffsetDown;

	tagStaticGlobal()
	{
		method = (ThreMethod)0;
		nOffsetUp = 0;
		nOffsetDown = 0;
	}
}StaticGlobal;

typedef struct tagDynamicGlobal
{
	ThreMethod method;
	int nOffsetUp;
	int nOffsetDown;

	tagDynamicGlobal()
	{
		method = (ThreMethod)0;
		nOffsetUp = 0;
		nOffsetDown = 0;
	}
}DynamicGlobal;

typedef struct tagLocal
{
	ThreMethod method;
	int nHalfWidth;
	//int nWidthY;
	int nOffsetUp;
	int nOffsetDown;

	tagLocal()
	{
		method = (ThreMethod)0;
		nHalfWidth = 1;
		//nWidthY = 1;
		nOffsetUp = 0;
		nOffsetDown = 0;
	}
}Local;


// 灰度分割工具
class SV_VISION_3D_API scImagePreprocessToolGraySegmenter
{
public:
	scImagePreprocessToolGraySegmenter();
	virtual ~scImagePreprocessToolGraySegmenter();

	enum SegmenterMode
	{
		eSegmenterModeUnknown = 0,
		eSegmenterModeAbsolute,			// 绝对阈值分割
		eSegmenterModeRelative,			// 相对阈值分割
		eSegmenterModeStaticGlobal,		// 静态全局阈值分割
		eSegmenterModeDynamicGlobal,	// 动态全局阈值分割
		eSegmenterModeLocal				// 局部阈值分割
	};

	enum ThreMethod
	{
		eThreMethodUnknown = 0,
		eThreMethodAverage,
		eThreMethodMedian,
		eThreMethodOtsu					 // 大津阈值
	};

	// 设置/获取阈值分割方式
	void SetSegmenterMode(SegmenterMode mode){m_segmenterMode = mode;};
	SegmenterMode GetSegmenterMode()const{return m_segmenterMode;};

	void SetAbsoluteData(Absolute data);
	Absolute GetAbsoluteData()const;

	void SetRelativeData(Relative data);
	Relative GetRelativeData()const;

	void SetStaticGlobalData(StaticGlobal data);
	StaticGlobal GetStaticGlobalData()const;

	void SetDynamicGlobalData(DynamicGlobal data);
	DynamicGlobal GetDynamicGlobalData()const;

	void SetLocalData(Local data);
	Local GetLocalData()const;

	// 设置/获取是否反色
	void EnableInvert(bool bEnable);
	bool IsEnableInvert()const;

	// 获取标准阈值
	int GetStandardThre()const;

	// 静态全局阈值分割输入训练图像
	bool Train(cpImage& src,scShape* rcROI = NULL);

	// 执行阈值分割
	bool Excute(cpImage& src,cpImage& dst,scShape* rcROI = NULL);

private:
	bool Binarization(cpImage& image, long lGrayThreLow, long lGrayThreHigh, bool bInvert);
	bool LocalBinarization(cpImage& image, cpImage& preImage,long lGrayThreLow, long lGrayThreHigh, bool bInvert);

private:
	SegmenterMode	m_segmenterMode;		// 阈值分割方式
	Absolute		m_absoluteData;			
	Relative		m_relativeData;
	StaticGlobal	m_staticGlobalData;
	DynamicGlobal	m_dynamicGlobalData;
	Local			m_localData;
	bool			m_bInvert;				// 是否反色
	int				m_nStandardThre;		// 标准阈值
};

#pragma pack(pop)