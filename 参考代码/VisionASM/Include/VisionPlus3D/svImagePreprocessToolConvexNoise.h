#pragma once
#include "svImagePreprocessToolBase.h"
#include "svShape.h"

#pragma pack(push,8)

//enum RemoveTarget
//{
//	eRemoveTargetUnkown = 0,
//	eRemoveTargetHigh,
//	eRemoveTargetLow,
//	eRemoveTargetBoth
//};

// 图像凸起状干扰
class SV_VISION_3D_API scImagePreprocessToolConvexNoise : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolConvexNoise();
	virtual ~scImagePreprocessToolConvexNoise();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessConvexNoise;}  // 获取预处理类型

	// 设置/获取去除方向
	void SetRemoveDirection(int nDirection);
	int GetRemoveDirection() const;
	
	// 设置/获取X方向去除大小
	void SetRemoveSizeX(int nSize);
	int GetRemoveSizeX() const;

	// 设置/获取Y方向去除大小
	void SetRemoveSizeY(int nSize);
	int GetRemoveSizeY() const;

	// 设置/获取去除对象
	void SetRemoveTarget(int nTarget);
	int GetRemoveTarget() const;

	// 设置/获取去除阈值
	void SetRemoveThreshold(double dThreshold);
	double GetRemoveThreshold() const;

	// 执行预处理
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst,scShape* roi,bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	int		m_nRemoveDirection;		// 去除方向
	int		m_nRemoveSizeX;			// X方向去除大小
	int		m_nRemoveSizeY;			// Y方向去除大小
	int		m_nRemoveTarget;		// 去除对象
	double	m_dRemoveThreshold;		// 去除阈值(像素值)
};

#pragma pack(pop)