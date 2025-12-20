#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svPolyhedron.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiPolyhedron : public scGuiGraphic3D
{
public:
	scGuiPolyhedron();
	virtual ~scGuiPolyhedron();

	virtual GuiGraphicType3D GetType()const { return eGuiPolyhedron;}

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Polyhedron;}

	void SetPolyhedron(const scPolyhedron& polyhedron);
	scPolyhedron GetPolyhedron()const {return m_Polyhedron;}

	void SetPolyhedronVectices(const std::vector<float>& vectices);
	std::vector<float> GetPolyhedronVectices()const {return m_Polyhedron.GetFloatVectices();}

	void SetPolyhedronIndices(const std::vector<unsigned int>& indices);
	std::vector<unsigned int> GetPolyhedronIndices()const {return m_Polyhedron.GetIndices();}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scPolyhedron m_Polyhedron;
};

#pragma pack(pop)