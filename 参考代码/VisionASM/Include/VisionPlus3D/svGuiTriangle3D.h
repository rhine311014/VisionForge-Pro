#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svTriangle3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiTriangle3D: public scGuiGraphic3D
{
public:
	scGuiTriangle3D();
	virtual ~scGuiTriangle3D();

	virtual GuiGraphicType3D GetType()const { return eGuiTriangle3D;}

	enum GuiTriangle3DDOF
	{
		eGuiNone = 0,
		eGuiEndPoint = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitEndPoint = 1,
		eHitTriangle = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Triangle;}

	void SetTriangle3D(const scTriangle3D& triangle);
	scTriangle3D GetTriangle3D() const {return m_Triangle;}

	void SetTriangle3DParam(const scPoint3D& p1,const scPoint3D& p2,const scPoint3D& p3);
	void GetTriangle3DParam(scPoint3D& p1,scPoint3D& p2,scPoint3D& p3)const;

	void SetTriangle3DP1(const scPoint3D& p1);
	void SetTriangle3DP2(const scPoint3D& p2);
	void SetTriangle3DP3(const scPoint3D& p3);

	scPoint3D GetTriangle3DP1()const {return m_Triangle[0];}
	scPoint3D GetTriangle3DP2()const {return m_Triangle[1];}
	scPoint3D GetTriangle3DP3()const {return m_Triangle[2];}

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
	scTriangle3D m_Triangle;
	GuiGraphicFillType m_DrawType;
	int m_nEndPointDragIndex;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)