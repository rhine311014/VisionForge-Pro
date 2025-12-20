#pragma once

#include "svZeroPlaneToolBase.h"
#include <vector>

class scShape;
class scPlaneFitTool;
#pragma pack(push,8)

// 平面基准计算起算面工具
class SV_VISION_3D_API scZeroPlaneRegionBestFitTool : public scZeroPlaneToolBase
{
public:
	scZeroPlaneRegionBestFitTool();
	virtual ~scZeroPlaneRegionBestFitTool();

	// 获取起算面计算类型
	virtual ZeroPlaneMethod GetZeroPlaneMethod() const {return eRegionBestFit;}

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

	void SetPlaneFitTool(scPlaneFitTool* pTool);
	scPlaneFitTool* GetPlaneFitTool()const;

	// 计算起算面
	virtual bool Excute(cpImage& image);
private:
	std::vector<scShape*> m_vpZeroPlaneRegion;     // 起算面范围
	std::vector<scShape*> m_vpMaskRegion;          // 屏蔽范围
	scPlaneFitTool *	  m_pPlaneFitTool;		   // 平面拟合工具
};

#pragma pack(pop)