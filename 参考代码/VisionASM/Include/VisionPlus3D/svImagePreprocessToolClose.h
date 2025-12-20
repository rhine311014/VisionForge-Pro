#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像闭运算
class SV_VISION_3D_API scImagePreprocessToolClose : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolClose();
	virtual ~scImagePreprocessToolClose();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessClose;}  // 获取预处理类型

	// 设置/获取形态学结构形状
	void SetMorphStructShape(int nMorphStructShape);
	int GetMorphStructShape()const;

	// 设置/获取形态学结构大小
	void SetMorphStructSize(int nMorphStructSize);
	int GetMorphStructSize()const;

	// 设置/获取形态学方向
	void SetMorphDirecton(int nMorphDirection);
	int GetMorphDirecton()const;

	// 设置/获取闭运算次数
	void SetCloseTimes(int nCloseTimes);
	int GetCLoseTimes()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	int m_nMorphStructShape;
	int m_nMorphStructSize;
	int m_nMorphDirection;
	int m_nCloseTimes;
	bool m_bReference;
};

#pragma pack(pop)