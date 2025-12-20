#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像二值化
class SV_VISION_3D_API scImagePreprocessToolBinary : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolBinary();
	virtual ~scImagePreprocessToolBinary();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessBinary;}  // 获取预处理类型

	// 设置/获取二值化上限
	void SetHighLimit(int nHighLimit);
	int GetHighLimit()const;

	// 设置/获取二值化下限
	void SetLowLimit(int nLowLimit);
	int GetLowLimit()const;

	// 设置/获取二值化是否反色
	void EnableInvert(bool bEnable);
	bool IsEnableInvert()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	int m_nHighLimit;	// 阈值上限
	int m_nLowLimit;	// 阈值下限
	bool m_bInvert;		// 是否反色
};

#pragma pack(pop)