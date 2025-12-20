#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svTetrahedron.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiTetrahedron : public scGuiGraphic3D
{
public:
	scGuiTetrahedron();
	virtual ~scGuiTetrahedron();

	virtual GuiGraphicType3D GetType()const { return eGuiTetrahedron;}

	enum GuiTetrahedronDOF
	{
		eGuiNone = 0,
		eGuiCorner = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitCorner = 1,
		eHitTetrahedron = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Tetrahedron;}

	void SetTetrahedron(const scTetrahedron& tetrahedron);
	scTetrahedron GetTetrahedron()const {return m_Tetrahedron;}

	void SetTetrahedronP1(const scPoint3D& p1);
	scPoint3D GetTetrahedronP1()const {return m_Tetrahedron.GetP1();}

	void SetTetrahedronP2(const scPoint3D& p2);
	scPoint3D GetTetrahedronP2()const {return m_Tetrahedron.GetP2();}

	void SetTetrahedronP3(const scPoint3D& p3);
	scPoint3D GetTetrahedronP3()const {return m_Tetrahedron.GetP3();}

	void SetTetrahedronP4(const scPoint3D& p4);
	scPoint3D GetTetrahedronP4()const {return m_Tetrahedron.GetP4();}

	void SetFillType(GuiGraphicFillType fillType);
	GuiGraphicFillType GetFillType()const {return m_DrawType;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scTetrahedron m_Tetrahedron;
	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)