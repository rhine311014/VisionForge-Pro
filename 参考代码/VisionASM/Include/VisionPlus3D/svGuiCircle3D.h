#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svCircle3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiCircle3D: public scGuiGraphic3D
{
public:
	scGuiCircle3D();
	virtual ~scGuiCircle3D();

	virtual GuiGraphicType3D GetType()const { return eGuiCircle3D;}

	enum GuiCircle3DDOF
	{
		eGuiNone = 0,
		eGuiRadius = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitRadius = 1,
		eHitCircle = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Circle;}

	void SetCircle3D(const scCircle3D& circle);
	scCircle3D GetCircle3D()const {return m_Circle;}

	void SetCircle3DParam(const scPoint3D& p,const scVector3D& normal,float radius);
	void GetCircle3DParam(scPoint3D& p,scVector3D& normal,float& radius)const;

	void SetCircle3DCenter(const scPoint3D& p);
	scPoint3D GetCircle3DCenter()const {return m_Circle.GetCenter();}

	void SetCircle3DNormal(const scVector3D& v);
	scVector3D GetCircle3DNormal()const {return m_Circle.GetNormal();}

	void SetCircle3DRadius(float radius);
	float GetCircle3DRadius()const {return m_Circle.GetRadius();}

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
	scCircle3D m_Circle;
	GuiGraphicFillType m_DrawType;
	int m_nSampleNum;

	scPoint3D m_ptOld;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)