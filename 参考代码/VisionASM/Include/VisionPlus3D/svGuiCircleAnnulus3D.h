#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svCircleAnnulus3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiCircleAnnulus3D : public scGuiGraphic3D
{
public:
	scGuiCircleAnnulus3D();
	virtual ~scGuiCircleAnnulus3D();

	virtual GuiGraphicType3D GetType()const { return eGuiCircleAnnulus3D;}
	enum GuiCircleAnnulus3DDOF
	{
		eGuiNone = 0,
		eGuiRadius = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitInnerRadius = 1,
		eHitOuterRadius = 2,
		eHitCircleAnnulus = 3
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_CircleAnnulus;}

	void SetCircleAnnulus3D(const scCircleAnnulus3D& annulus);
	scCircleAnnulus3D GetCircleAnnulus3D()const {return m_CircleAnnulus;}

	void SetCircleAnnulus3DCenter(const scPoint3D& center);
	scPoint3D GetCircleAnnulus3DCenter()const {return m_CircleAnnulus.GetCenter();}

	void SetCircleAnnulus3DNormal(const scVector3D& v);
	scVector3D GetCircleAnnulus3DNormal()const {return m_CircleAnnulus.GetNormal();}

	void SetCircleAnnulus3DRadius(float inner,float outer);
	void GetCircleAnnulus3DRadius(float& inner,float& outer);

	float GetCircleAnnulus3DRadiusInner()const {return m_CircleAnnulus.GetRadiusInner();}
	float GetCircleAnnulus3DRadiusOuter()const {return m_CircleAnnulus.GetRadiusOuter();}

	void SetSampleNum(int nSampleNum);
	int  GetSampleNum()const {return m_nSampleNum;}

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
	scCircleAnnulus3D m_CircleAnnulus;
	GuiGraphicFillType m_DrawType;
	int m_nSampleNum;

	scPoint3D m_ptOld;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)