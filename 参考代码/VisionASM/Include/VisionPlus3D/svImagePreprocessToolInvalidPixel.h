#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像无效像素控制
class SV_VISION_3D_API scImagePreprocessToolInvalidPixel : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolInvalidPixel();
	virtual ~scImagePreprocessToolInvalidPixel();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessInvalidPixel;}  // 获取预处理类型

	// 设置/获取模板大小
	void SetStructSize(int nStructSize);
	unsigned int GetStructSize()const;


	// 执行预处理
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	int m_nStructSize;		// 平均化模板大小
};

#pragma pack(pop)