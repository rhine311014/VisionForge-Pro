#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svLozenge.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiLozenge: public scGuiGraphic3D
{
public:
	scGuiLozenge();
	virtual ~scGuiLozenge();

	virtual GuiGraphicType3D GetType()const { return eGuiLozenge;}

	enum GuiLozengeDOF
	{
		eGuiNone = 0,
		eGuiRectRadius = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitRectRadius = 1,
		eHitLozenge = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Lozenge;}

	void SetLozenge(const scLozenge& lozenge);
	scLozenge GetLozenge()const {return m_Lozenge;}

	void SetLozengeRectangle3D(const scRectangle3D& rect);
	scRectangle3D GetLozengeRectangle3D()const {return m_Lozenge.GetRectangle3D();}

	void SetLozengeRadius(float radius);
	float GetLozengeRadius()const {return m_Lozenge.GetRadius();}

	void SetRectSampleNum(int num);
	int GetRectSampleNum()const {return m_nRectSampleNum;}

	void SetCircleSampleNum(int num);
	int GetCircleSampleNum()const {return m_nCircleSampleNum;}

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
	scLozenge m_Lozenge;
	GuiGraphicFillType m_DrawType;

	int m_nRectSampleNum;
	int m_nCircleSampleNum;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)