#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svConvexPolyhedron.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiConvexPolyhedron : public scGuiGraphic3D
{
public:
	scGuiConvexPolyhedron();
	virtual ~scGuiConvexPolyhedron();

	virtual GuiGraphicType3D GetType()const { return eGuiConvexPolyhedron;}

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_ConvexPolyhedron;}

	void SetConvexPolyhedron(const scConvexPolyhedron& convex);
	scConvexPolyhedron GetConvexPolyhedron()const {return m_ConvexPolyhedron;}

	void SetConvexPolyhedronVectices(const std::vector<float>& vectices);
	std::vector<float> GetConvexPolyhedronVectices()const {return m_ConvexPolyhedron.GetFloatVectices();}

	void SetConvexPolyhedronIndices(const std::vector<unsigned int>& indices);
	std::vector<unsigned int> GetConvexPolyhedronIndices()const {return m_ConvexPolyhedron.GetIndices();}

	void SetFillType(GuiGraphicFillType fillType) {m_DrawType = fillType;}
	GuiGraphicFillType GetFillType()const {return m_DrawType;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scConvexPolyhedron m_ConvexPolyhedron;
	GuiGraphicFillType m_DrawType;
};

#pragma pack(pop)
