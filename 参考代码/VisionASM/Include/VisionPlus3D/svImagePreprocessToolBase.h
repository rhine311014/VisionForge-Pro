#pragma once

#include "svVision3DDef.h"
#include "vpImage.h"

#pragma pack(push,8)

enum PreprocessType
{
	ePreprocessNone = 0,
	ePreprocessBinary,			// 二值化
	ePreprocessBlobFilter,	    // 斑点过滤
	ePreprocessLinearTransform,	// 对比度变换
	ePreprocessDilate,			// 膨胀
	ePreprocessErode,			// 腐蚀
	ePreprocessClose,			// 黑像素干扰排除
	ePreprocessOpen,			// 白像素干扰排除
	ePreprocessAverage,			// 平均化
	ePreprocessMedian,			// 中间值
	ePreprocessGauss,			// 高斯
	ePreprocessInvalidPixel,	// 无效像素控制
	ePreprocessConvexNoise,		// 凸起状干扰
	ePreprocessMaxMin,			// 差值滤波
	ePreprocessOutlierRemove,	// 离群点去除
	ePreprocessFixedRangeHeightRemove,		// 固定范围高度去除
};

enum MorphStructShape
{
	eMorphStructUnknown = 0,
	eMorphStructSquare,
	eMorphStructCircle
};

enum MorphDirection
{
	eMorphDirectionUnknown = 0,
	eMorphDirectionXY,
	eMorphDirectionX, 
	eMorphDirectionY
};

enum RemoveTarget
{
	eRemoveTargetUnkown = 0,
	eRemoveTargetHigh,
	eRemoveTargetLow,
	eRemoveTargetBoth
};

// 预处理工具基类
class SV_VISION_3D_API scImagePreprocessToolBase
{
public:
	scImagePreprocessToolBase() ;
	virtual ~scImagePreprocessToolBase() {};

	virtual PreprocessType GetPreprocessType() const {return ePreprocessNone;}  // 获取预处理类型

	void SetEdgePreprocessEnabled(bool bEdgePreprocess = true);
	bool GetEdgePreprocessEnabled();

	void SetEdgeValue(int nValue);
	int GetEdgeValue();

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false){return false;}
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false){return false;}

protected:
	bool GetEdgeProcessSpecify(cpImage& src,cpImage& dst,int nValue,int nDirection,int nStructSize)const;
	bool GetEdgeProcessOriginal(cpImage& src,cpImage& dst,int nDirection,int nStructSize)const;

protected:
	bool m_bEdgePreprocess;  // 是否开启边界处理
	int m_nEdgeValue;		// 若不开启边界处理，-1表示采用原图像边界像素的值，其他非负数表示指定值，8位图像边界指定值范围位[0,255]，16位图像边界指定值范围为[0,65535]
};

#pragma pack(pop)