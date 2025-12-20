#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svConvexMesh.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiConvexMesh: public scGuiGraphic3D
{
public:
	scGuiConvexMesh();
	virtual ~scGuiConvexMesh();

	virtual GuiGraphicType3D GetType()const { return eGuiConvexMesh;}

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_ConvexMesh;}

	void SetConvexMesh(const scConvexMesh& convexMesh);
	scConvexMesh GetConvexMesh()const {return m_ConvexMesh;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scConvexMesh m_ConvexMesh;
};

#pragma pack(pop)