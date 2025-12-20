#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svCylinder.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiCylinder:public scGuiGraphic3D
{
public:
	scGuiCylinder();
	virtual ~scGuiCylinder();

	virtual GuiGraphicType3D GetType()const { return eGuiCylinder;}

	enum GuiCylinderDOF
	{
		eGuiNone = 0,
		eGuiRadiusHeight = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitRadiusHeight = 1,
		eHitCylinder = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Cylinder;}

	void SetCylinder(const scCylinder& cylinder);
	scCylinder GetCylinder()const {return m_Cylinder;}

	void SetCylinderAxisLine(const scLine3D& line);
	scLine3D GetCylinderAxisLine()const {return m_Cylinder.GetAxisLine();}

	void SetCylinderHeight(float height);
	float GetCylinderHeight()const {return m_Cylinder.GetHeight();}

	void SetCylinderRadius(float radius);
	float GetCylinderRadius()const {return m_Cylinder.GetRadius();}

	void SetSampleNum(int nSampleNum);
	int  GetSampleNum()const {return m_nSampleNum;}

	// 是否封闭,默认封闭
	void SetClosed(bool bclosed);
	bool IsClosed()const {return m_bClosed;}

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
	scCylinder m_Cylinder;
	int m_nSampleNum;
	bool   m_bClosed;

	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	scPoint3D m_ptOld;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)