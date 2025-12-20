#pragma once

#include "svGuiGraphic3D.h"
#include "svSegment3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiSegment3D: public scGuiGraphic3D
{
public:
	scGuiSegment3D();
	virtual ~scGuiSegment3D();

	virtual GuiGraphicType3D GetType()const { return eGuiSegment3D;}

	enum GuiSegment3DDOF
	{
		eGuiNone = 0,
		eGuiSeg  = 1,
		eGuiEndPoint = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitSeg  = 1,
		eHitStartPoint = 2,
		eHitEndPoint = 3
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_seg;}

	void SetSegment3D(const scSegment3D seg);
	scSegment3D GetSegment3D() const;

	void SetSegment3DParam(const scPoint3D& P1,const scPoint3D& P2);
	void GetSegment3DParam(scPoint3D& P1,scPoint3D& P2)const;

	void SetSegment3DP1(const scPoint3D& P1);
	scPoint3D GetSegment3DP1()const;

	void SetSegment3DP2(const scPoint3D& P2);
	scPoint3D GetSegment3DP2()const;

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scSegment3D m_seg;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)