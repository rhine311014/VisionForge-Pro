#pragma once

#include "svZeroPlaneToolBase.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

class scPlaneFitTool;
// 实时多点指定计算起算面
class SV_VISION_3D_API scZeroPlaneRealTimeMulPointsTool : public scZeroPlaneToolBase
{
public:
	scZeroPlaneRealTimeMulPointsTool();
	virtual ~scZeroPlaneRealTimeMulPointsTool();

	virtual ZeroPlaneMethod GetZeroPlaneMethod() const {return eRealTimeMulPoints;}

	// 设置、获取点坐标
	void SetVertices(const std::vector<sc2Vector>& vPoints);
	std::vector<sc2Vector>& GetVertices();

	// 添加、删除、设置、获取点坐标
	void AddVertex(sc2Vector point);
	void RemoveVertex(int nIndex);

	void SetVertex(sc2Vector point,int nIndex);
	sc2Vector GetVertex(int nIndex)const;

	// 获取点个数
	int GetVerticesCount()const;
	
	// 设置、获取均值邻域
	void SetAveArea(int nAveArea);
	int GetAveArea()const;

	//void SetPlaneFitTool(scPlaneFitTool* pTool);
	//scPlaneFitTool* GetPlaneFitTool()const;

	// 计算起算面
	virtual bool Excute(cpImage& image);

private:
	std::vector<sc2Vector> m_vPoints; // 点坐标(像素)
	int       m_nAveArea;    // 均值邻域(奇数)
	//scPlaneFitTool *	  m_pPlaneFitTool;		   // 平面拟合工具
};

#pragma pack(pop)