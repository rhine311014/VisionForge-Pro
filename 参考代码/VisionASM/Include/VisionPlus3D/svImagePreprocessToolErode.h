#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像腐蚀
class SV_VISION_3D_API scImagePreprocessToolErode : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolErode();
	virtual ~scImagePreprocessToolErode();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessErode; }  // 获取预处理类型

	// 设置/获取形态学结构形状
	void SetMorphStructShape(int nMorphStructShape);
	int GetMorphStructShape()const;

	// 设置/获取形态学结构大小
	void SetMorphStructSize(int nMorphStructSize);
	int GetMorphStructSize()const;

	// 设置/获取形态学方向
	void SetMorphDirecton(int nMorphDirection);
	int GetMorphDirecton()const;

	// 设置/获取腐蚀次数
	void SetErodeTimes(int nErodeTimes);
	int GetErodeTimes()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	bool GetMinXGray8(cpImage& src,cpImage& dst,int nSize)const;
	bool GetMinXGray16(cpImage& src,cpImage& dst,int nSize)const;
	bool GetMinYGray8(cpImage& src,cpImage& dst,int nSize)const;
	bool GetMinYGray16(cpImage& src,cpImage& dst,int nSize)const;

private:
	int m_nMorphStructShape;		// 形态学结构形状
	int m_nMorphStructSize;			// 形态学结构大小
	int m_nMorphDirection;			// 形态学方向
	int m_nErodeTimes;				// 形态学次数
};

#pragma pack(pop)