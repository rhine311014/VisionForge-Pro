#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svCapsule.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiCapsule: public scGuiGraphic3D
{
public:
	scGuiCapsule();
	virtual ~scGuiCapsule();

	virtual GuiGraphicType3D GetType()const { return eGuiCapsule;}

	enum GuiCapsuleDOF
	{
		eGuiNone   = 0,
		eGuiRadiusLength = 1,
		eGuiMove   = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone    = 0,
		eHitRadiusLength  = 1,
		eHitCapsule = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Capsule;}

	void SetCapsule(const scCapsule& capsule);
	scCapsule GetCapsule()const {return m_Capsule;}

	void SetCapsuleSegment3D(const scSegment3D& seg);
	scSegment3D GetCapsuleSegment3D()const {return m_Capsule.GetSegment3D();}

	void SetCapsuleRadius(float radius);
	float GetCapsuleRadius()const {return m_Capsule.GetRadius();}

	void SetFillType(GuiGraphicFillType fillType) {m_DrawType = fillType;}
	GuiGraphicFillType GetFillType()const {return m_DrawType;}

	void SetRadiusSanmpleNum(int num);
	int GetRadiusSanmpleNum()const {return m_nRadiusSampleNum;}

	void SetCircleSampleNum(int num);
	int GetCircleSampleNum()const {return m_nCircleSampleNum;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scCapsule m_Capsule;
	int m_nRadiusSampleNum;
	int m_nCircleSampleNum;
	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)