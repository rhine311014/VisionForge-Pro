#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svFrustum.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiFrustum : public scGuiGraphic3D
{
public:
	scGuiFrustum();
	virtual ~scGuiFrustum();

	virtual GuiGraphicType3D GetType()const { return eGuiFrustum;}

	enum GuiFrustumDOF
	{
		eGuiNone = 0,
		eGuiLength = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitLength = 1,
		eHitFrustum = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Frustum;}

	void SetFrustum(const scFrustum& frustum);
	scFrustum GetFrustum()const {return m_Frustum;}

	void SetFrustumOrigion(const scPoint3D& origion);
	scPoint3D GetFrustumOrigion()const {return m_Frustum.GetOrigion();}

	void SetFrustumViewDirection(const scVector3D& vec);
	scVector3D GetFrustumViewDirection()const {return m_Frustum.GetViewDirection();}

	void SetFrustumAxisU(const scVector3D& uAxis);
	scVector3D GetFrustumAxisU()const {return m_Frustum.GetAxisU();}

	void SetFrustumAxisV(const scVector3D& vAxis);
	scVector3D GetFrustumAxisV()const {return m_Frustum.GetAxisV();}

	void SetFrustumNearPlaneDistance(float dis);
	float GetFrustumNearPlaneDistance()const {return m_Frustum.GetNearPlaneDistance();}

	void SetFrustumFarPlaneDistance(float dis);
	float GetFrustumFarPlaneDistance()const {return m_Frustum.GetFarPlaneDistance();}

	void SetFrustumBoundU(float uBound);
	float GetFrustumBoundU()const {return m_Frustum.GetBoundU();}

	void SetFrustumBoundV(float vBound);
	float GetFrustumBoundV()const {return m_Frustum.GetBoundV();}

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
	scFrustum m_Frustum;
	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)