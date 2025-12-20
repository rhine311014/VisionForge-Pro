#pragma once

#include "svImagePreprocessToolBase.h"

#pragma pack(push, 8)

// 图像中间值
class SV_VISION_3D_API scImagePreprocessToolMedian : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolMedian();
	virtual ~scImagePreprocessToolMedian();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessMedian;}  // 获取预处理类型

	// 设置/获取中间值模板形状
	void SetStructShape(int nStructShape);
	unsigned int GetStructShape()const;

	// 设置/获取中间值模板大小
	void SetStructSize(int nStructSize);
	unsigned int GetStructSize()const;

	// 设置/获取中间值次数
	void SetMedianTimes(int nMedianTimes);
	unsigned int GetMedianTimes()const;

	// 设置/获取中值滤波方向
	void SetDirecton(int nDirection);
	int GetDirecton()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	template <typename T>
	void GetMedianValue(const unsigned int* pHist, int nSize, int nThreshold, T* pValue);
	
private:
	int m_nStructShape;		// 中值模板形状
	int m_nStructSize;		// 中值模板大小
	int m_nMedianTimes;		// 中值次数
	int m_nDirection;		// 中值滤波方向
};
#pragma pack(pop)