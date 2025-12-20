#pragma once

#include "svZeroPlaneToolBase.h"
#include <vector>

class scShape;
class scPlaneFitTool;
#pragma pack(push,8)

// 平面基准(平均)计算起算面工具
class SV_VISION_3D_API scZeroPlaneRegionAveBestFitTool : public scZeroPlaneToolBase
{
public:
	scZeroPlaneRegionAveBestFitTool();
	virtual ~scZeroPlaneRegionAveBestFitTool();

	// 获取起算面计算类型
	virtual ZeroPlaneMethod GetZeroPlaneMethod() const {return eRegionAveBestFit;}

	// 添加、删除、获取起算面范围
	void AddZeroPlaneRegion(scShape* shape);
	void SetZeroPlaneRegion(scShape* shape,int nIndex);
	void RemoveZeroPlaneRegion(int nIndex);
	void RemoveZeroPlaneRegionAll();
	scShape* GetZeroPlaneRegion(int nIndex)const;

	// 添加、删除、获取屏蔽范围
	void AddZeroPlaneMaskRegion(scShape* shape);
	void SetZeroPlaneMaskRegion(scShape* shape,int nIndex);
	void RemoveZeroPlaneMaskRegion(int nIndex);
	void RemoveZeroPlaneMaskRegionAll();
	scShape* GetZeroPlaneMaskRegion(int nIndex)const;

	// 计算起算面
	virtual bool Excute(cpImage& image);
private:
	std::vector<scShape*> m_vpZeroPlaneRegion;     // 起算面范围
	std::vector<scShape*> m_vpMaskRegion;          // 屏蔽范围
};

#pragma pack(pop)