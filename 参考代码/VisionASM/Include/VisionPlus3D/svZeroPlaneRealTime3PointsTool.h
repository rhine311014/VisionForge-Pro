#pragma once

#include "svZeroPlaneToolBase.h"
#include "svVector.h"

#pragma pack(push,8)

// 实时3点指定计算起算面
class SV_VISION_3D_API scZeroPlaneRealTime3PointsTool : public scZeroPlaneToolBase
{
public:
	scZeroPlaneRealTime3PointsTool();
	virtual ~scZeroPlaneRealTime3PointsTool();

	virtual ZeroPlaneMethod GetZeroPlaneMethod() const {return eRealTime3Points;}

	// 设置、获取3点坐标
	void SetPoints(sc2Vector p1,sc2Vector p2,sc2Vector p3);
	void GetPoints(sc2Vector& p1,sc2Vector& p2,sc2Vector& p3)const;

	// 设置、获取均值邻域
	void SetAveArea(int nAveArea);
	int GetAveArea()const;

	// 计算起算面
	virtual bool Excute(cpImage& image);

private:
	sc2Vector m_vPoints[3];  // 点坐标(像素)
	int       m_nAveArea;    // 均值邻域(奇数)
};

#pragma pack(pop)