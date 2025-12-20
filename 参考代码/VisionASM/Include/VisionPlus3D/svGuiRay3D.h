#pragma once

#include "svGuiGraphic3D.h"
#include "svRay3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiRay3D: public scGuiGraphic3D
{
public:
	scGuiRay3D();
	virtual ~scGuiRay3D();

	virtual GuiGraphicType3D GetType()const { return eGuiRay3D;}

	enum eGuiRay3DDOF
	{
		eGuiNone = 0,
		eGuiMove = 1,
		eGuiRotate = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Ray;}

	virtual void SetViewFrustumMaxLength(float length);

	void SetRay3D(const scRay3D& ray);
	scRay3D GetRay3D() const;

	void SetRay3DParam(const scPoint3D& P,const scVector3D& vec);
	void GetRay3DParam(scPoint3D& P,scVector3D& vec) const;

	void SetRay3DOrigion(const scPoint3D& P);
	scPoint3D GetRay3DOrigion()const;

	void SetRay3DDirection(const scVector3D& vec);
	scVector3D GetRay3DDirection()const;

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scRay3D m_Ray;
	float m_fDrawMaxLength;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)