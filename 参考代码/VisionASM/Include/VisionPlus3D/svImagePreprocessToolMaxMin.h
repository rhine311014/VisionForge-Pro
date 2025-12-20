#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像差值滤波工具
class SV_VISION_3D_API scImagePreprocessToolMaxMin : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolMaxMin();
	virtual ~scImagePreprocessToolMaxMin();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessMaxMin;}  // 获取预处理类型

	// 设置/获取差值滤波模板大小
	void SetStructSize(int nStructSize);
	unsigned int GetStructSize()const;

	// 设置/获取差值滤波方向
	void SetDirecton(int nDirection);
	int GetDirecton()const;

	// 设置/获取差值滤波次数
	void SetMaxMinTimes(int nMaxMinTimes);
	unsigned int GetMaxMinTimes()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);

private:
	int m_nStructSize;		// 差值滤波模板大小
	int m_nDirection;		// 差值滤波方向
	int m_nMaxMinTimes;		// 差值滤波次数	
};

#pragma pack(pop)