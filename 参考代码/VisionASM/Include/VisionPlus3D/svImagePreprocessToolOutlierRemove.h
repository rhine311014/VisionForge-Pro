#pragma once
#include "svImagePreprocessToolBase.h"
#include "svShape.h"

#pragma pack(push,8)



enum RemoveMethod
{
	eRemoveMethodUnkown = 0,
	eRemoveMethodAverage,
	eRemoveMethodMedian
	
};



// 图像离群点去除
class SV_VISION_3D_API scImagePreprocessToolOutlierRemove : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolOutlierRemove();
	virtual ~scImagePreprocessToolOutlierRemove();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessOutlierRemove;}  // 获取预处理类型

	// 设置/获取离群点去除方向
	void SetRemoveDirection(int nDirection);
	int GetRemoveDirection() const ;

	// 设置/获取离群点去除大小
	void SetRemoveSize(int nSize);
	int GetRemoveSize() const ;

	// 设置/获取离群点去除对象
	void SetRemoveTarget(int nTarget) ;
	int GetRemoveTarget() const;

	// 设置/获取去除阈值
	void SetRemoveThreshold(double dThreshold) ;
	double GetRemoveThreshold() const ;

	// 设置/获取去除方法
	void SetRemoveMethod(int nMethod) ;
	int GetRemoveMethod() const;

	// 执行预处理
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);

private:
	int		m_nRemoveDirection;		// 去除方向
	int		m_nRemoveSize;			// 去除大小
	int		m_nRemoveTarget;		// 去除对象
	double	m_dRemoveThreshold;		// 去除阈值(像素值)
	int		m_nRemoveMethod;		// 去除方法
};

#pragma pack(pop)