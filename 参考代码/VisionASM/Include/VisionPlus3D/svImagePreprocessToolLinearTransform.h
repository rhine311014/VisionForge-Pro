#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像线性变换
class SV_VISION_3D_API scImagePreprocessToolLinearTransform : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolLinearTransform();
	virtual ~scImagePreprocessToolLinearTransform();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessLinearTransform;}  // 获取预处理类型

	// 设置/获取比例系数
	void SetCoefficient(int nCoefficient);
	int GetCoefficient()const;

	// 设置/获取偏移
	void SetOffset(int nOffset);
	int GetOffset()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	int m_nCoefficient;		// 比例系数
	int m_nOffset;			// 偏移
};

#pragma pack(pop)